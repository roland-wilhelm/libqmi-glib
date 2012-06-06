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



static void
get_hardware_id_ready(QmiClientDms *client,
               GAsyncResult *res);
static void
get_dev_rev_id_ready(QmiClientDms *client,
               GAsyncResult *res);
static void
get_dev_model_id_ready(QmiClientDms *client,
               GAsyncResult *res);
static void
get_dev_mfr_ready(QmiClientDms *client,
               GAsyncResult *res);
static void
get_sw_version_ready(QmiClientDms *client,
               GAsyncResult *res);


/* Context */
typedef struct {
    QmiDevice *device;
    QmiClientDms *client;
    GCancellable *cancellable;
} Context;
static Context *ctx;

/* Options */
static gboolean get_ids_flag;
static gboolean get_op_mode_flag;
static gboolean noop_flag;
static gboolean get_hardware_prop_flag;
static gboolean get_msisdn_flag;

static GOptionEntry entries[] = {
    { "dms-get-ids", 0, 0, G_OPTION_ARG_NONE, &get_ids_flag,
      "Get IDs",
      NULL
    },

    { "dms-get-hardware-properties", 0, 0, G_OPTION_ARG_NONE, &get_hardware_prop_flag,
          "",
          NULL
    },
    { "dms-get-msisdn", 0, 0, G_OPTION_ARG_NONE, &get_msisdn_flag,
             "",
             NULL
    },

    { "dms-get-op-mode", 0, 0, G_OPTION_ARG_NONE, &get_op_mode_flag,
          "Get Operation Mode",
          NULL
    },

    { "dms-noop", 0, 0, G_OPTION_ARG_NONE, &noop_flag,
      "Just allocate or release a DMS client. Use with `--client-no-release-cid' and/or `--client-cid'",
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

    n_actions = (get_ids_flag +
                 noop_flag +
                 get_op_mode_flag+
                 get_hardware_prop_flag+
                 get_msisdn_flag);

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
shutdown (gboolean operation_status)
{
    /* Cleanup context and finish async operation */
    context_free (ctx);
    qmicli_async_operation_done (operation_status);
}

static void
get_ids_ready (QmiClientDms *client,
               GAsyncResult *res)
{
    GError *error = NULL;
    QmiDmsGetIdsOutput *output;

    output = qmi_client_dms_get_ids_finish (client, res, &error);
    if (!output) {
        g_printerr ("error: operation failed: %s\n", error->message);
        g_error_free (error);
        shutdown (FALSE);
        return;
    }

    if (!qmi_dms_get_ids_output_get_result (output, &error)) {
        g_printerr ("error: couldn't get IDs: %s\n", error->message);
        g_error_free (error);
        qmi_dms_get_ids_output_unref (output);
        shutdown (FALSE);
        return;
    }

#undef VALIDATE_UNKNOWN
#define VALIDATE_UNKNOWN(str) (str ? str : "unknown")

    g_print ("[%s] Device IDs retrieved:\n"
             "\t ESN: '%s'\n"
             "\tIMEI: '%s'\n"
             "\tMEID: '%s'\n",
             qmi_device_get_path_display (ctx->device),
             VALIDATE_UNKNOWN (qmi_dms_get_ids_output_get_esn (output)),
             VALIDATE_UNKNOWN (qmi_dms_get_ids_output_get_imei (output)),
             VALIDATE_UNKNOWN (qmi_dms_get_ids_output_get_meid (output)));

    qmi_dms_get_ids_output_unref (output);
    shutdown (TRUE);
}

static void
get_op_mode_ready (QmiClientDms *client,
               GAsyncResult *res)
{
    GError *error = NULL;
    QmiDmsGetOpModeOutput *output;

    output = qmi_client_dms_get_op_mode_finish (client, res, &error);
    if (!output) {
        g_printerr ("error: operation failed: %s\n", error->message);
        g_error_free (error);
        shutdown (FALSE);
        return;
    }

    if (!qmi_dms_get_op_mode_output_get_result (output, &error)) {
        g_printerr ("error: couldn't get Operation Mode: %s\n", error->message);
        g_error_free (error);
        qmi_dms_get_op_mode_output_unref (output);
        shutdown (FALSE);
        return;
    }

#undef VALIDATE_UNKNOWN
#define VALIDATE_UNKNOWN(str) (str ? str : "unknown")

    g_print ("[%s] Device Operation Mode retrieved:\n"
             "\t OP-Mode: '%s'\n",
             qmi_device_get_path_display (ctx->device),
             VALIDATE_UNKNOWN (qmi_dms_get_op_mode_output_get_version_string(output)));


    qmi_dms_get_op_mode_output_unref (output);
    shutdown (TRUE);
}

static gboolean
noop_cb (gpointer unused)
{
    shutdown (TRUE);
    return FALSE;
}

static void
get_dev_mfr_ready(QmiClientDms *client,
               GAsyncResult *res)
{
    GError *error = NULL;
    QmiDmsGetDevMfrOutput *output;

    output = qmi_client_dms_get_dev_mfr_finish (client, res, &error);
    if (!output) {
        g_printerr ("error: operation failed: %s\n", error->message);
        g_error_free (error);
        shutdown (FALSE);
        return;
    }

    if (!qmi_dms_get_dev_mfr_output_get_result (output, &error)) {
        g_printerr ("error: couldn't get Device Manufacturer: %s\n", error->message);
        g_error_free (error);
        qmi_dms_get_dev_mfr_output_unref (output);
        shutdown (FALSE);
        return;
    }

#undef VALIDATE_UNKNOWN
#define VALIDATE_UNKNOWN(str) (str ? str : "unknown")

    g_print ("[%s] Device Hardware Properties retrieved:\n"
             "\t Device Manufacturer: '%s'\n",
             qmi_device_get_path_display (ctx->device),
             VALIDATE_UNKNOWN (qmi_dms_get_dev_mfr_output_get_dev_mfr(output)));


    qmi_dms_get_dev_mfr_output_unref (output);


    /* Request to get Device Hardware Revision ID? */
	qmi_client_dms_get_hardware_id(ctx->client,
									10,
									ctx->cancellable,
									(GAsyncReadyCallback)get_hardware_id_ready,
									NULL);


}

static void
get_dev_model_id_ready(QmiClientDms *client,
               GAsyncResult *res)
{
    GError *error = NULL;
    QmiDmsGetDevModelIdOutput *output;

    output = qmi_client_dms_get_dev_model_id_finish (client, res, &error);
    if (!output) {
        g_printerr ("error: operation failed: %s\n", error->message);
        g_error_free (error);
        shutdown (FALSE);
        return;
    }

    if (!qmi_dms_get_dev_model_id_output_get_result (output, &error)) {
        g_printerr ("error: couldn't get Device Model ID: %s\n", error->message);
        g_error_free (error);
        qmi_dms_get_dev_model_id_output_unref (output);
        shutdown (FALSE);
        return;
    }

#undef VALIDATE_UNKNOWN
#define VALIDATE_UNKNOWN(str) (str ? str : "unknown")

    g_print ("\t Model ID:: '%s'\n",
             VALIDATE_UNKNOWN (qmi_dms_get_dev_model_id_output_get_dev_model_id(output)));


    qmi_dms_get_dev_model_id_output_unref (output);

    /* Request to get SW Version? */
	qmi_client_dms_get_sw_version(ctx->client,
								10,
								ctx->cancellable,
								(GAsyncReadyCallback)get_sw_version_ready,
								NULL);



}

static void
get_dev_rev_id_ready(QmiClientDms *client,
               GAsyncResult *res)
{
    GError *error = NULL;
    QmiDmsGetDevRevIdOutput *output;

    output = qmi_client_dms_get_dev_rev_id_finish (client, res, &error);
    if (!output) {
        g_printerr ("error: operation failed: %s\n", error->message);
        g_error_free (error);
        shutdown (FALSE);
        return;
    }

    if (!qmi_dms_get_dev_rev_id_output_get_result (output, &error)) {
        g_printerr ("error: couldn't get Device Revision ID: %s\n", error->message);
        g_error_free (error);
        qmi_dms_get_dev_rev_id_output_unref (output);
        shutdown (FALSE);
        return;
    }

#undef VALIDATE_UNKNOWN
#define VALIDATE_UNKNOWN(str) (str ? str : "unknown")

    g_print ("\t Device Revision ID:: '%s'\n",
             VALIDATE_UNKNOWN (qmi_dms_get_dev_rev_id_output_get_dev_rev_id(output)));


    qmi_dms_get_dev_rev_id_output_unref (output);

    /* Request to get Get Device Model ID? */
	qmi_client_dms_get_dev_model_id(ctx->client,
								10,
								ctx->cancellable,
								(GAsyncReadyCallback)get_dev_model_id_ready,
								NULL);


}

static void
get_hardware_id_ready(QmiClientDms *client,
               GAsyncResult *res)
{
    GError *error = NULL;
    QmiDmsGetDevHardwareRevIdOutput *output;

    output = qmi_client_dms_get_hardware_id_finish (client, res, &error);
    if (!output) {
        g_printerr ("error: operation failed: %s\n", error->message);
        g_error_free (error);
        shutdown (FALSE);
        return;
    }

    if (!qmi_dms_get_hardware_rev_id_output_get_result (output, &error)) {
        g_printerr ("error: couldn't get Hardware Revision ID: %s\n", error->message);
        g_error_free (error);
        qmi_dms_get_hardware_rev_id_output_unref (output);
        shutdown (FALSE);
        return;
    }

#undef VALIDATE_UNKNOWN
#define VALIDATE_UNKNOWN(str) (str ? str : "unknown")

    g_print ("\t Hardware ID: '%s'\n",
             VALIDATE_UNKNOWN (qmi_dms_get_hardware_rev_output_get_hardware_rev(output)));


    qmi_dms_get_hardware_rev_id_output_unref (output);

    /* Request to get Device Revision ID? */
	qmi_client_dms_get_dev_rev_id(ctx->client,
								10,
								ctx->cancellable,
								(GAsyncReadyCallback)get_dev_rev_id_ready,
								NULL);

}

static void
get_msisdn_ready(QmiClientDms *client,
               GAsyncResult *res)
{
    GError *error = NULL;
    QmiDmsGetMsisdnOutput *output;

    output = qmi_client_dms_get_msisdn_finish (client, res, &error);
    if (!output) {
        g_printerr ("error: operation failed: %s\n", error->message);
        g_error_free (error);
        shutdown (FALSE);
        return;
    }

    if (!qmi_dms_get_msisdn_output_get_result (output, &error)) {
        g_printerr ("error: couldn't get MSISDN: %s\n", error->message);
        g_error_free (error);
        qmi_dms_get_msisdn_output_unref (output);
        shutdown (FALSE);
        return;
    }

#undef VALIDATE_UNKNOWN
#define VALIDATE_UNKNOWN(str) (str ? str : "unknown")

    g_print ("\t Voice number: '%s'\n"
    		 "\t Mobile ID number: '%s'\n"
    		 "\t IMSI: '%s'\n",
    		 VALIDATE_UNKNOWN(qmi_dms_get_msisdn_output_get_voice_number(output)),
    		 VALIDATE_UNKNOWN(qmi_dms_get_msisdn_output_get_mobile_id(output)),
    		 VALIDATE_UNKNOWN(qmi_dms_get_msisdn_output_get_imsi(output)));


    qmi_dms_get_msisdn_output_unref (output);


	shutdown(TRUE);


}

static void
get_sw_version_ready(QmiClientDms *client,
               GAsyncResult *res)
{
    GError *error = NULL;
    QmiDmsGetSwVersionOutput *output;

    output = qmi_client_dms_get_sw_version_finish (client, res, &error);
    if (!output) {
        g_printerr ("error: operation failed: %s\n", error->message);
        g_error_free (error);
        shutdown (FALSE);
        return;
    }

    if (!qmi_dms_get_sw_version_output_get_result (output, &error)) {
        g_printerr ("error: couldn't get SW Version: %s\n", error->message);
        g_error_free (error);
        qmi_dms_get_sw_version_output_unref (output);
        shutdown (FALSE);
        return;
    }

#undef VALIDATE_UNKNOWN
#define VALIDATE_UNKNOWN(str) (str ? str : "unknown")

    g_print ("\t SW Version: '%s'\n",
             VALIDATE_UNKNOWN (qmi_dms_get_sw_version_output_get_sw_version(output)));


    qmi_dms_get_sw_version_output_unref (output);

    shutdown(TRUE);

}



void
qmicli_dms_run (QmiDevice *device,
                QmiClientDms *client,
                GCancellable *cancellable)
{
    /* Initialize context */
    ctx = g_slice_new (Context);
    ctx->device = g_object_ref (device);
    ctx->client = g_object_ref (client);
    if (cancellable)
        ctx->cancellable = g_object_ref (cancellable);

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

    /* Request to get Operation Mode? */
	if (get_op_mode_flag) {
		g_debug ("Asynchronously getting Operation Mode...");
		qmi_client_dms_get_op_mode (ctx->client,
								10,
								ctx->cancellable,
								(GAsyncReadyCallback)get_op_mode_ready,
								NULL);
		return;
	}

	/* Request to get Hardware Properties? */
	if (get_hardware_prop_flag) {
		g_debug ("Asynchronously getting Hardware Properties...");
		qmi_client_dms_get_dev_mfr(ctx->client,
								10,
								ctx->cancellable,
								(GAsyncReadyCallback)get_dev_mfr_ready,
								NULL);


		return;
	}


	/* Request to get MSISDN/ Voice number? */
	if (get_msisdn_flag) {
		g_debug ("Asynchronously getting MSISDN...");
		qmi_client_dms_get_msisdn(ctx->client,
							10,
							ctx->cancellable,
							(GAsyncReadyCallback)get_msisdn_ready,
							NULL);
		return;
	}



    /* Just client allocate/release? */
    if (noop_flag) {
        g_idle_add (noop_cb, NULL);
        return;
    }

    g_warn_if_reached ();
}
