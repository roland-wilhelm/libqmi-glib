/*
 * qmicli-nas.c
 *
 *  Created on: 24.05.2012
 *      Author: roland
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#include <glib.h>
#include <gio/gio.h>

#include "libqmi-glib.h"
#include "qmicli.h"
#include "qmi-nas.h"



/* Context */
typedef struct {
    QmiDevice *device;
    QmiClientNas *client;
    GCancellable *cancellable;
} Context;

static Context *ctx;

/* Options */
static gboolean get_sig_info_flag;
static gboolean get_cell_info_flag;
static gboolean get_rf_band_flag;
static gboolean noop_flag;
static gboolean get_sys_info_flag;


static GOptionEntry entries[] = {
    { "nas-get-sig-info", 0, 0, G_OPTION_ARG_NONE, &get_sig_info_flag,
      "Get Signal Information",
      NULL
    },
    { "nas-get-cell-info", 0, 0, G_OPTION_ARG_NONE, &get_cell_info_flag,
	  "Get Cell Information",
	  NULL
	},
	{ "nas-get-rf-band", 0, 0, G_OPTION_ARG_NONE, &get_rf_band_flag,
	  "Get RF Band Information",
	  NULL
	},
	{ "nas-get-sys-info", 0, 0, G_OPTION_ARG_NONE, &get_sys_info_flag,
	  "",
	  NULL
	},
    { NULL }
};

GOptionGroup*
qmicli_nas_get_option_group (void)
{
	GOptionGroup *group;

	group = g_option_group_new ("nas",
	                            "NAS options",
	                            "Show Network Access Service options",
	                            NULL,
	                            NULL);
	g_option_group_add_entries (group, entries);

	return group;
}

gboolean
qmicli_nas_options_enabled (void)
{
    static guint n_actions = 0;
    static gboolean checked = FALSE;

    if (checked)
        return !!n_actions;

    n_actions = (get_sig_info_flag+
    			get_cell_info_flag+
    			get_rf_band_flag+
    			get_sys_info_flag);

    if (n_actions > 1) {
        g_printerr ("error: too many NAS actions requested\n");
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
get_sig_info_ready(QmiClientNas *client,
               	   GAsyncResult *res)
{
    GError *error = NULL;
    QmiNasGetSigInfoOutput *output;


    output = qmi_client_nas_get_sig_info_finish (client, res, &error);

    if (!output) {
        g_printerr ("error: operation failed: %s\n", error->message);
        g_error_free (error);
		shutdown (FALSE);
		return;
    }

    if (!qmi_nas_get_sig_info_output_get_result (output, &error)) {
        g_printerr ("error: couldn't get Signal Info: %s\n", error->message);
        g_error_free (error);
		qmi_nas_get_sig_info_output_unref (output);
		shutdown (FALSE);
		return;
    }


    g_print ("[%s] Signal Info retrieved:\n"
             "\tRSSI: '%d'\n"
             "\tRSRP: '%d'\n"
             "\tRSRQ: '%d'\n"
    		 "\t SNR: '%f'\n",
             qmi_device_get_path_display (ctx->device),
             qmi_nas_get_sig_info_output_get_rssi (output),
             qmi_nas_get_sig_info_output_get_rsrp (output),
             qmi_nas_get_sig_info_output_get_rsrq (output),
             qmi_nas_get_sig_info_output_get_snr (output));


    qmi_nas_get_sig_info_output_unref (output);

    shutdown (TRUE);
}

static void
get_cell_info_ready(QmiClientNas *client,
               	   GAsyncResult *res)
{
    GError *error = NULL;
    QmiNasGetCellLocInfoOutput *output;
    guint8 i;

    output = qmi_client_nas_get_cell_loc_info_finish (client, res, &error);
    if (!output) {
        g_printerr ("error: operation failed: %s\n", error->message);
        g_error_free (error);
		shutdown (FALSE);
		return;
    }

    if (!qmi_nas_get_cell_loc_info_output_get_result (output, &error)) {
        g_printerr ("error: couldn't get Cell Info: %s\n", error->message);
        g_error_free (error);
		qmi_nas_get_cell_loc_info_output_unref (output);
		shutdown (FALSE);
		return;
    }


    g_print("[%s] Cell Info retrieved:\n", qmi_device_get_path_display(ctx->device));
    g_print("\tUE in Idle: '%d'\n", qmi_nas_get_cell_loc_info_output_get_ue_in_idle(output));
    g_print("\tPLMN: '%s'\n", qmi_nas_get_cell_loc_info_output_get_plmn(output));
    g_print("\tTAC: '%d'\n", qmi_nas_get_cell_loc_info_output_get_tac(output));
    g_print("\tGlobal Cell ID: '%d'\n", qmi_nas_get_cell_loc_info_output_get_global_cell_id(output));
    g_print("\tChannel number: '%d'\n", qmi_nas_get_cell_loc_info_output_get_earfcn(output));
    g_print("\tCell ID: '%d'\n", qmi_nas_get_cell_loc_info_output_get_serving_cell_id(output));
    g_print("\tCell Priority: '%d'\n", qmi_nas_get_cell_loc_info_output_get_cell_resel_priority(output));
    g_print("\tS non-intra search threshold: '%d'\n", qmi_nas_get_cell_loc_info_output_get_s_non_intra_search(output));
    g_print("\tCell low threshold: '%d'\n", qmi_nas_get_cell_loc_info_output_get_thresh_serving_low(output));
    g_print("\tS intra search threshold: '%d'\n", qmi_nas_get_cell_loc_info_output_get_s_intra_search(output));

    for(i = 0; i < qmi_nas_get_cell_loc_info_output_get_sets(output); i++) {
		g_print("--------------------------------------------------------------------\n");
		g_print("\tPCI: '%d'\n", qmi_nas_get_cell_loc_info_output_get_pci(output, i));
		g_print("\tRSRQ: '%f'\n", qmi_nas_get_cell_loc_info_output_get_rsrq(output, i));
		g_print("\tRSRP: '%f'\n", qmi_nas_get_cell_loc_info_output_get_rsrp(output, i));
		g_print("\tRSSI: '%f'\n", qmi_nas_get_cell_loc_info_output_get_rssi(output, i));
		g_print("\tCell selection Rx level: '%d'\n", qmi_nas_get_cell_loc_info_output_get_srxlev(output, i));
		g_print("--------------------------------------------------------------------\n");

    }

    qmi_nas_get_cell_loc_info_output_unref (output);

    shutdown (TRUE);
}

static void
get_rf_band_ready(QmiClientNas *client,
               	   GAsyncResult *res)
{
    GError *error = NULL;
    QmiNasGetRfBandOutput *output;
    guint8 i;

    output = qmi_client_nas_get_rf_band_finish(client, res, &error);
    if (!output) {
        g_printerr("error: operation failed: %s\n", error->message);
        g_error_free(error);
		shutdown(FALSE);
		return;
    }

    if (!qmi_nas_get_rf_band_output_get_result (output, &error)) {
        g_printerr("error: couldn't get RF Band Info: %s\n", error->message);
        g_error_free(error);
		qmi_nas_get_rf_band_output_unref (output);
		shutdown(FALSE);
		return;
    }


    g_print("[%s] RF Band Info retrieved:\n", qmi_device_get_path_display(ctx->device));

    for(i = 0; i < qmi_nas_get_rf_band_output_get_instances(output); i++) {
		g_print("--------------------------------------------------------------------\n");
		g_print("\tRadio interface: '%d'\n", qmi_nas_get_rf_band_output_get_radio_if(output, i));
		g_print("\tActive Band: '%d'\n", qmi_nas_get_rf_band_output_get_active_band(output, i));
		g_print("\tActive channel: '%d'\n", qmi_nas_get_rf_band_output_get_active_channel(output, i));
		g_print("--------------------------------------------------------------------\n");

    }

    qmi_nas_get_rf_band_output_unref(output);

    shutdown(TRUE);
}

static void
get_sys_info_ready(QmiClientNas *client,
               	   GAsyncResult *res)
{
    GError *error = NULL;
    QmiNasGetSysInfoOutput *output;


    output = qmi_client_nas_get_sys_info_finish(client, res, &error);
    if (!output) {
        g_printerr("error: operation failed: %s\n", error->message);
        g_error_free(error);
		shutdown(FALSE);
		return;
    }

    if (!qmi_nas_get_sys_info_output_get_result (output, &error)) {
        g_printerr("error: couldn't get System Info: %s\n", error->message);
        g_error_free(error);
		qmi_nas_get_sys_info_output_unref (output);
		shutdown(FALSE);
		return;
    }


    g_print("[%s] LTE System Info retrieved:\n", qmi_device_get_path_display(ctx->device));
	g_print("--------------------------------------------------------------------\n");
	g_print("\tRoam status valid: '%d'\n", qmi_nas_get_sys_info_output_get_roam_status_valid(output));
	g_print("\tRoam status: '%d'\n", qmi_nas_get_sys_info_output_get_roam_status(output));
	g_print("\tLocation Area Code valid: '%d'\n", qmi_nas_get_sys_info_output_get_lac_valid(output));
	g_print("\tLocation Area Code: '%d'\n", qmi_nas_get_sys_info_output_get_lac(output));
	g_print("\tCell ID valid: '%d'\n", qmi_nas_get_sys_info_output_get_cell_id_valid(output));
	g_print("\tCell ID: '%d'\n", qmi_nas_get_sys_info_output_get_cell_id(output));
	g_print("\tNetwork ID valid: '%d'\n", qmi_nas_get_sys_info_output_get_network_id_valid(output));
	g_print("\tMobile Country Code: '%d'\n", qmi_nas_get_sys_info_output_get_mcc(output));
	g_print("\tMobile Network Code: '%d'\n", qmi_nas_get_sys_info_output_get_mnc(output));
	g_print("--------------------------------------------------------------------\n");



    qmi_nas_get_sys_info_output_unref(output);

    shutdown(TRUE);
}




static gboolean
noop_cb (gpointer unused)
{
    shutdown (TRUE);
    return FALSE;
}

void
qmicli_nas_run(QmiDevice *device, QmiClientNas *client, GCancellable *cancellable)
{
    /* Initialize context */
    ctx = g_slice_new (Context);
    ctx->device = g_object_ref (device);
    ctx->client = g_object_ref (client);
    if (cancellable)
        ctx->cancellable = g_object_ref (cancellable);

    /* Request to get Sig Info? */
	if (get_sig_info_flag) {
		g_debug ("Asynchronously getting Signal Info...");
		qmi_client_nas_get_sig_info(ctx->client,
									10,
									ctx->cancellable,
									(GAsyncReadyCallback)get_sig_info_ready,
									NULL);
		return;
	}
	/* Request to get Cell Information? */
	if (get_cell_info_flag) {
		g_debug ("Asynchronously getting Cell Info...");
		qmi_client_nas_get_cell_loc_info(ctx->client,
									10,
									ctx->cancellable,
									(GAsyncReadyCallback)get_cell_info_ready,
									NULL);
		return;
	}
	/* Request to get RF Band Information? */
	if (get_rf_band_flag) {
		g_debug ("Asynchronously getting RF Band Info...");
		qmi_client_nas_get_rf_band(ctx->client,
									10,
									ctx->cancellable,
									(GAsyncReadyCallback)get_rf_band_ready,
									NULL);
		return;
	}
	/* Request to get System Information? */
	if (get_sys_info_flag) {
		g_debug ("Asynchronously getting System Info...");
		qmi_client_nas_get_sys_info(ctx->client,
									10,
									ctx->cancellable,
									(GAsyncReadyCallback)get_sys_info_ready,
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





