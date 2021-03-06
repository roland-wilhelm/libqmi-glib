/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * qmicli -- Command line interface to control QMI devices
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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
 * Copyright (C) 2012 Aleksander Morgado <aleksander@lanedo.com>
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#include <glib.h>
#include <glib/gprintf.h>
#include <gio/gio.h>

#include <libqmi-glib.h>

#include "qmicli.h"

#define PROGRAM_NAME    "qmicli"
#define PROGRAM_VERSION PACKAGE_VERSION

/* Globals */
static GMainLoop *loop;
static GCancellable *cancellable;
static QmiDevice *device;
static QmiClient *client;
static QmiService service;
static gboolean operation_status;

/* Main options */
static gchar *device_str;
static gboolean device_open_version_info_flag;
static gboolean device_open_sync_flag;
static gchar *client_cid_str;
static gboolean client_no_release_cid_flag;
static gboolean verbose_flag;
static gboolean silent_flag;
static gboolean version_flag;

static GOptionEntry main_entries[] = {
    { "device", 'd', 0, G_OPTION_ARG_STRING, &device_str,
      "Specify device path",
      "[PATH]"
    },
    { "device-open-version-info", 0, 0, G_OPTION_ARG_NONE, &device_open_version_info_flag,
      "Run version info check when opening device",
      NULL
    },
    { "device-open-sync", 0, 0, G_OPTION_ARG_NONE, &device_open_sync_flag,
      "Run sync operation when opening device",
      NULL
    },
    { "client-cid", 0, 0, G_OPTION_ARG_STRING, &client_cid_str,
      "Use the given CID, don't allocate a new one",
      "[CID]"
    },
    { "client-no-release-cid", 0, 0, G_OPTION_ARG_NONE, &client_no_release_cid_flag,
      "Do not release the CID when exiting",
      NULL
    },
    { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose_flag,
      "Run action with verbose logs, including the debug ones",
      NULL
    },
    { "silent", 0, 0, G_OPTION_ARG_NONE, &silent_flag,
      "Run action with no logs; not even the error/warning ones",
      NULL
    },
    { "version", 'V', 0, G_OPTION_ARG_NONE, &version_flag,
      "Print version",
      NULL
    },
    { NULL }
};

static void
signals_handler (int signum)
{
    if (cancellable) {
        /* Ignore consecutive requests of cancellation */
        if (!g_cancellable_is_cancelled (cancellable)) {
            g_printerr ("%s\n",
                        "cancelling the operation...\n");
            g_cancellable_cancel (cancellable);
        }
        return;
    }

    if (loop &&
        g_main_loop_is_running (loop)) {
        g_printerr ("%s\n",
                    "cancelling the main loop...\n");
        g_main_loop_quit (loop);
    }
}

static void
log_handler (const gchar *log_domain,
             GLogLevelFlags log_level,
             const gchar *message,
             gpointer user_data)
{
    const gchar *log_level_str;
    time_t now;
    gchar time_str[64];
    struct tm *local_time;
    gboolean err;

    /* Nothing to do if we're silent */
    if (silent_flag)
        return;

    now = time ((time_t *) NULL);
    local_time = localtime (&now);
    strftime (time_str, 64, "%d %b %Y, %H:%M:%S", local_time);
    err = FALSE;

    switch (log_level) {
    case G_LOG_LEVEL_WARNING:
        log_level_str = "-Warning **";
        err = TRUE;
        break;

    case G_LOG_LEVEL_CRITICAL:
    case G_LOG_FLAG_FATAL:
    case G_LOG_LEVEL_ERROR:
        log_level_str = "-Error **";
        err = TRUE;
        break;

    case G_LOG_LEVEL_DEBUG:
        log_level_str = "[Debug]";
        break;

    default:
        log_level_str = "";
        break;
    }

    if (!verbose_flag && !err)
        return;

    g_fprintf (err ? stderr : stdout,
               "[%s] %s %s\n",
               time_str,
               log_level_str,
               message);
}

static void
print_version_and_exit (void)
{
    g_print ("\n"
             PROGRAM_NAME " " PROGRAM_VERSION "\n"
             "Copyright (2012) Aleksander Morgado\n"
             "License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl-2.0.html>\n"
             "This is free software: you are free to change and redistribute it.\n"
             "There is NO WARRANTY, to the extent permitted by law.\n"
             "\n");
    exit (EXIT_SUCCESS);
}

/*****************************************************************************/
/* Running asynchronously */

static void
release_client_ready (QmiDevice *device,
                      GAsyncResult *res)
{
    GError *error = NULL;

    if (!qmi_device_release_client_finish (device, res, &error)) {
        g_printerr ("error: couldn't release client: %s", error->message);
        g_error_free (error);
    } else
        g_debug ("Client released");

    g_main_loop_quit (loop);
}

void
qmicli_async_operation_done (gboolean reported_operation_status)
{
    QmiDeviceReleaseClientFlags flags = QMI_DEVICE_RELEASE_CLIENT_FLAGS_NONE;

    /* Keep the result of the operation */
    operation_status = reported_operation_status;

    if (cancellable) {
        g_object_unref (cancellable);
        cancellable = NULL;
    }

    if (!client_no_release_cid_flag)
        flags |= QMI_DEVICE_RELEASE_CLIENT_FLAGS_RELEASE_CID;
    else
        g_print ("[%s] Client ID not released:\n"
                 "\tService: '%s'\n"
                 "\t    CID: '%u'\n",
                 qmi_device_get_path_display (device),
                 qmi_service_get_string (service),
                 qmi_client_get_cid (client));

    qmi_device_release_client (device,
                               client,
                               flags,
                               10,
                               NULL,
                               (GAsyncReadyCallback)release_client_ready,
                               NULL);
}

static void
allocate_client_ready (QmiDevice *device,
                       GAsyncResult *res)
{
    GError *error = NULL;

    client = qmi_device_allocate_client_finish (device, res, &error);
    if (!client) {
        g_printerr ("error: couldn't create client for the '%s' service: %s\n",
                    qmi_service_get_string (service),
                    error->message);
        exit (EXIT_FAILURE);
    }

    /* Run the service-specific action */
    switch (service) {
    case QMI_SERVICE_DMS:
        qmicli_dms_run (device, QMI_CLIENT_DMS (client), cancellable);
        return;
    case QMI_SERVICE_WDS:
        qmicli_wds_run (device, QMI_CLIENT_WDS (client), cancellable);
        return;
    default:
        g_assert_not_reached ();
    }
}

static void
device_open_ready (QmiDevice *device,
                   GAsyncResult *res)
{
    GError *error = NULL;
    guint8 cid = QMI_CID_NONE;

    if (!qmi_device_open_finish (device, res, &error)) {
        g_printerr ("error: couldn't open the QmiDevice: %s\n",
                    error->message);
        exit (EXIT_FAILURE);
    }

    g_debug ("QMI Device at '%s' ready",
             qmi_device_get_path_display (device));

    if (client_cid_str) {
        guint32 cid32;

        cid32 = atoi (client_cid_str);
        if (!cid32 || cid32 > G_MAXUINT8) {
            g_printerr ("error: invalid CID given '%s'\n",
                        client_cid_str);
            exit (EXIT_FAILURE);
        }

        cid = (guint8)cid32;
        g_debug ("Reusing CID '%u'", cid);
    }

    /* As soon as we get the QmiDevice, create a client for the requested
     * service */
    qmi_device_allocate_client (device,
                                service,
                                cid,
                                10,
                                cancellable,
                                (GAsyncReadyCallback)allocate_client_ready,
                                NULL);
}

static void
device_new_ready (GObject *unused,
                  GAsyncResult *res)
{
    QmiDeviceOpenFlags open_flags = QMI_DEVICE_OPEN_FLAGS_NONE;
    GError *error = NULL;

    device = qmi_device_new_finish (res, &error);
    if (!device) {
        g_printerr ("error: couldn't create QmiDevice: %s\n",
                    error->message);
        exit (EXIT_FAILURE);
    }

    /* Setup device open flags */
    if (device_open_version_info_flag)
        open_flags |= QMI_DEVICE_OPEN_FLAGS_VERSION_INFO;
    if (device_open_sync_flag)
        open_flags |= QMI_DEVICE_OPEN_FLAGS_SYNC;

    /* Open the device */
    qmi_device_open (device,
                     open_flags,
                     5,
                     cancellable,
                     (GAsyncReadyCallback)device_open_ready,
                     NULL);
}

/*****************************************************************************/

int main (int argc, char **argv)
{
    GFile *file;
    GOptionContext *context;

    setlocale (LC_ALL, "");

    g_type_init ();

    /* Setup option context, process it and destroy it */
    context = g_option_context_new ("- Control QMI devices");
	g_option_context_add_group (context,
	                            qmicli_dms_get_option_group ());
	g_option_context_add_group (context,
	                            qmicli_wds_get_option_group ());
    g_option_context_add_main_entries (context, main_entries, NULL);
    g_option_context_parse (context, &argc, &argv, NULL);
	g_option_context_free (context);

    if (version_flag)
        print_version_and_exit ();

    g_log_set_handler (G_LOG_DOMAIN, G_LOG_LEVEL_MASK, log_handler, NULL);

    /* No device path given? */
    if (!device_str) {
        g_printerr ("error: no device path specified\n");
        exit (EXIT_FAILURE);
    }

    /* Build new GFile from the commandline arg */
    file = g_file_new_for_commandline_arg (device_str);

    /* Setup signals */
    signal (SIGINT, signals_handler);
    signal (SIGHUP, signals_handler);
    signal (SIGTERM, signals_handler);

    /* DMS options? */
    if (qmicli_dms_options_enabled ()) {
        service = QMI_SERVICE_DMS;
    }
    /* WDS options? */
    else if (qmicli_wds_options_enabled ()) {
        service = QMI_SERVICE_WDS;
    }
    /* No options? */
    else {
        g_printerr ("error: no actions specified\n");
        exit (EXIT_FAILURE);
    }

    /* Create requirements for async options */
    cancellable = g_cancellable_new ();
    loop = g_main_loop_new (NULL, FALSE);

    /* Launch QmiDevice creation */
    qmi_device_new (file,
                    cancellable,
                    (GAsyncReadyCallback)device_new_ready,
                    GUINT_TO_POINTER (service));
    g_main_loop_run (loop);

    if (cancellable)
        g_object_unref (cancellable);
    if (client)
        g_object_unref (client);
    if (device)
        g_object_unref (device);
    g_main_loop_unref (loop);
    g_object_unref (file);

    return (operation_status ? EXIT_SUCCESS : EXIT_FAILURE);
}
