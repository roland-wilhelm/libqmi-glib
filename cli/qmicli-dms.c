/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * qmicli -- Command line interface to control QMI devices
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2012 Aleksander Morgado <aleksander@gnu.org>
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#include <glib.h>
#include <gio/gio.h>

#include <libqmi-glib.h>

#include "qmicli.h"

/* Context */
typedef struct {
    QmiDevice *device;
    QmiClientDms *client;
    GCancellable *cancellable;
} Context;
static Context *ctx;

/* Options */
static gboolean get_ids_flag;

static GOptionEntry entries[] = {
    { "dms-get-ids", 0, 0, G_OPTION_ARG_NONE, &get_ids_flag,
      "Get IDs",
      NULL
    },
    { NULL }
};

GOptionGroup *
qmicli_dms_get_option_group (void)
{
	GOptionGroup *group;

	group = g_option_group_new ("dms",
	                            "DMS options",
	                            "Show Device Management Service options",
	                            NULL,
	                            NULL);
	g_option_group_add_entries (group, entries);

	return group;
}

gboolean
qmicli_dms_options_enabled (void)
{
    static guint n_actions = 0;
    static gboolean checked = FALSE;

    if (checked)
        return !!n_actions;

    n_actions = (get_ids_flag);

    if (n_actions > 1) {
        g_printerr ("error: too many DMS actions requested\n");
        exit (EXIT_FAILURE);
    }

    checked = TRUE;
    return !!n_actions;
}

static void
context_free (Context *ctx)
{
    if (!ctx)
        return;

    if (ctx->cancellable)
        g_object_unref (ctx->cancellable);
    if (ctx->device)
        g_object_unref (ctx->device);
    if (ctx->client)
        g_object_unref (ctx->client);
    g_slice_free (Context, ctx);
}

static void
client_release_ready (QmiClient *client,
                      GAsyncResult *res)
{
    GError *error = NULL;

    if (!qmi_client_release_finish (client, res, &error)) {
        g_printerr ("error: couldn't release client CID: %s", error->message);
        exit (EXIT_FAILURE);
    }

    g_debug ("Client CID released");

    /* Cleanup context and finish async operation */
    context_free (ctx);
    qmicli_async_operation_done ();
}

static void
shutdown (void)
{
    qmi_client_release (QMI_CLIENT (ctx->client),
                        10,
                        (GAsyncReadyCallback)client_release_ready,
                        NULL);
}

static void
get_ids_ready (QmiClientDms *client,
               GAsyncResult *result)
{
    GError *error = NULL;
    gchar *esn = NULL;
    gchar *imei = NULL;
    gchar *meid = NULL;

    if (!qmi_client_dms_get_ids_finish (client,
                                        result,
                                        &esn,
                                        &imei,
                                        &meid,
                                        &error)) {
        g_printerr ("error: couldn't get IDs: %s\n",
                    error->message);
        exit (EXIT_FAILURE);
    }

    g_print ("[%s] Device IDs retrieved:\n"
             "\t ESN: '%s'\n"
             "\tIMEI: '%s'\n"
             "\tMEID: '%s'\n",
             qmi_device_get_path_display (ctx->device),
             esn ? esn : "not available",
             imei ? imei : "not available",
             meid ? meid : "not available");

    g_free (esn);
    g_free (imei);
    g_free (meid);

    shutdown ();
}

static void
client_dms_new_ready (GObject *unused,
                      GAsyncResult *res)
{
    GError *error = NULL;

    ctx->client = qmi_client_dms_new_finish (res, &error);
    if (!ctx->client) {
        g_printerr ("error: couldn't create DMS client: %s\n",
                    error->message);
        exit (EXIT_FAILURE);
    }

    /* Request to get IDs? */
    if (get_ids_flag) {
        g_debug ("Asynchronously getting IDs...");
        qmi_client_dms_get_ids (ctx->client,
                                10,
                                ctx->cancellable,
                                (GAsyncReadyCallback)get_ids_ready,
                                NULL);
        return;
    }

    g_warn_if_reached ();
}

void
qmicli_dms_run (QmiDevice *device,
                GCancellable *cancellable)
{
    /* Initialize context */
    ctx = g_slice_new (Context);
    ctx->device = g_object_ref (device);
    if (cancellable)
        ctx->cancellable = g_object_ref (cancellable);

    /* Create a new DMS client */
    qmi_client_dms_new (device,
                        cancellable,
                        (GAsyncReadyCallback)client_dms_new_ready,
                        NULL);
}
