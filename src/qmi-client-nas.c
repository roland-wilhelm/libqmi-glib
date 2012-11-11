/*
 * qmi-client-nas.c
 *
 *  Created on: 24.05.2012
 *      Author: roland
 */

#include "qmi-client-nas.h"
#include "qmi-enum-types.h"
#include "qmi-message-nas.h"
#include "qmi-device.h"
#include <gio/gio.h>


G_DEFINE_TYPE (QmiClientNas, qmi_client_nas, QMI_TYPE_CLIENT);

/*****************************************************************************/
/* Get Sig Info */

/**
 *
 */
QmiNasGetSigInfoOutput*
qmi_client_nas_get_sig_info_finish(QmiClientNas *self, GAsyncResult *res, GError **error)
{

    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(res), error))
        return NULL;

    return qmi_nas_get_sig_info_output_ref(g_simple_async_result_get_op_res_gpointer(G_SIMPLE_ASYNC_RESULT(res)));
}

/**
 *
 *
 */
static void
get_sig_info_ready(QmiDevice *device,
               	   GAsyncResult *res,
               	   GSimpleAsyncResult *simple)
{
	QmiNasGetSigInfoOutput *output;
    GError *error = NULL;
    QmiMessage *reply;

    reply = qmi_device_command_finish(device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Getting Signal Info failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_nas_get_sig_info_reply_parse(reply, &error);
    if (!output) {
        g_prefix_error (&error, "Getting Signal Info reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    }
    else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_nas_get_sig_info_output_unref);

    g_simple_async_result_complete(simple);
    g_object_unref(simple);
    qmi_message_unref (reply);

}

/**
 *
 *
 */
void
qmi_client_nas_get_sig_info(QmiClientNas *self,
							guint timeout,
							GCancellable *cancellable,
							GAsyncReadyCallback callback,
							gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;


    result = g_simple_async_result_new(G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_nas_get_sig_info);

    request = qmi_message_nas_get_sig_info_new(qmi_client_get_next_transaction_id(QMI_CLIENT (self)),
                                           	   qmi_client_get_cid (QMI_CLIENT(self)));

    qmi_device_command((QmiDevice *)qmi_client_peek_device(QMI_CLIENT (self)),
                        request,
                        timeout,
                        cancellable,
                        (GAsyncReadyCallback)get_sig_info_ready,
                        result);

    qmi_message_unref (request);

}

/*****************************************************************************/
/* Get Cell Location Info */

/**
 *
 */
QmiNasGetCellLocInfoOutput*
qmi_client_nas_get_cell_loc_info_finish(QmiClientNas *self, GAsyncResult *res, GError **error)
{

    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(res), error))
        return NULL;

    return qmi_nas_get_cell_loc_info_output_ref(g_simple_async_result_get_op_res_gpointer(G_SIMPLE_ASYNC_RESULT(res)));
}

/**
 *
 *
 */
static void
get_cell_loc_info_ready(QmiDevice *device,
               	   GAsyncResult *res,
               	   GSimpleAsyncResult *simple)
{
	QmiNasGetCellLocInfoOutput *output;
    GError *error = NULL;
    QmiMessage *reply;

    reply = qmi_device_command_finish(device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Getting Cell Location Info failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_nas_get_cell_loc_info_reply_parse(reply, &error);
    if (!output) {
        g_prefix_error (&error, "Getting Cell Location Info reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    }
    else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_nas_get_cell_loc_info_output_unref);

    g_simple_async_result_complete(simple);
    g_object_unref(simple);
    qmi_message_unref (reply);

}

/**
 *
 *
 */
void
qmi_client_nas_get_cell_loc_info(QmiClientNas *self,
							guint timeout,
							GCancellable *cancellable,
							GAsyncReadyCallback callback,
							gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;

    result = g_simple_async_result_new(G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_nas_get_cell_loc_info);

    request = qmi_message_nas_get_cell_loc_info_new(qmi_client_get_next_transaction_id(QMI_CLIENT (self)),
                                           	   qmi_client_get_cid (QMI_CLIENT(self)));

    qmi_device_command((QmiDevice *)qmi_client_peek_device(QMI_CLIENT (self)),
                        request,
                        timeout,
                        cancellable,
                        (GAsyncReadyCallback)get_cell_loc_info_ready,
                        result);

    qmi_message_unref (request);

}

/*****************************************************************************/
/* Get RF Band Info */

/**
 *
 */
QmiNasGetRfBandOutput*
qmi_client_nas_get_rf_band_finish(QmiClientNas *self, GAsyncResult *res, GError **error)
{

    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(res), error))
        return NULL;

    return qmi_nas_get_rf_band_output_ref(g_simple_async_result_get_op_res_gpointer(G_SIMPLE_ASYNC_RESULT(res)));
}

/**
 *
 *
 */
static void
get_rf_band_ready(QmiDevice *device,
               	   GAsyncResult *res,
               	   GSimpleAsyncResult *simple)
{
	QmiNasGetRfBandOutput *output;
    GError *error = NULL;
    QmiMessage *reply;

    reply = qmi_device_command_finish(device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Getting RF Band Info failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_nas_get_rf_band_reply_parse(reply, &error);
    if (!output) {
        g_prefix_error (&error, "Getting RF Band Info reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    }
    else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_nas_get_rf_band_output_unref);

    g_simple_async_result_complete(simple);
    g_object_unref(simple);
    qmi_message_unref (reply);

}

/**
 *
 *
 */
void
qmi_client_nas_get_rf_band(QmiClientNas *self,
							guint timeout,
							GCancellable *cancellable,
							GAsyncReadyCallback callback,
							gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;


    result = g_simple_async_result_new(G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_nas_get_rf_band);

    request = qmi_message_nas_get_rf_band_new(qmi_client_get_next_transaction_id(QMI_CLIENT (self)),
                                           	   qmi_client_get_cid (QMI_CLIENT(self)));

    qmi_device_command((QmiDevice *)qmi_client_peek_device(QMI_CLIENT (self)),
                        request,
                        timeout,
                        cancellable,
                        (GAsyncReadyCallback)get_rf_band_ready,
                        result);

    qmi_message_unref (request);

}

/*****************************************************************************/
/* Get System Info */

/**
 *
 */
QmiNasGetSysInfoOutput*
qmi_client_nas_get_sys_info_finish(QmiClientNas *self, GAsyncResult *res, GError **error)
{

    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(res), error))
        return NULL;

    return qmi_nas_get_sys_info_output_ref(g_simple_async_result_get_op_res_gpointer(G_SIMPLE_ASYNC_RESULT(res)));
}

/**
 *
 *
 */
static void
get_sys_info_ready(QmiDevice *device,
               	   GAsyncResult *res,
               	   GSimpleAsyncResult *simple)
{
	QmiNasGetSysInfoOutput *output;
    GError *error = NULL;
    QmiMessage *reply;

    reply = qmi_device_command_finish(device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Getting System Info failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_nas_get_sys_info_reply_parse(reply, &error);
    if (!output) {
        g_prefix_error (&error, "Getting System Info reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    }
    else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_nas_get_sys_info_output_unref);

    g_simple_async_result_complete(simple);
    g_object_unref(simple);
    qmi_message_unref (reply);

}

/**
 *
 *
 */
void
qmi_client_nas_get_sys_info(QmiClientNas *self,
							guint timeout,
							GCancellable *cancellable,
							GAsyncReadyCallback callback,
							gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;


    result = g_simple_async_result_new(G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_nas_get_sys_info);

    request = qmi_message_nas_get_sys_info_new(qmi_client_get_next_transaction_id(QMI_CLIENT (self)),
                                           	   qmi_client_get_cid (QMI_CLIENT(self)));

    qmi_device_command((QmiDevice *)qmi_client_peek_device(QMI_CLIENT (self)),
                        request,
                        timeout,
                        cancellable,
                        (GAsyncReadyCallback)get_sys_info_ready,
                        result);

    qmi_message_unref (request);

}

/*****************************************************************************/
/* Get Signal Strength */

/**
 *
 */
QmiNasGetSigStrengthOutput*
qmi_client_nas_get_sig_strength_finish(QmiClientNas *self, GAsyncResult *res, GError **error)
{

    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(res), error))
        return NULL;

    return qmi_nas_get_sig_strength_output_ref(g_simple_async_result_get_op_res_gpointer(G_SIMPLE_ASYNC_RESULT(res)));
}

/**
 *
 *
 */
static void
get_sig_strength_ready(QmiDevice *device,
               	   	   GAsyncResult *res,
               	   	   GSimpleAsyncResult *simple)
{
	QmiNasGetSigStrengthOutput *output;
    GError *error = NULL;
    QmiMessage *reply;

    reply = qmi_device_command_finish(device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Getting Signal Strength failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_nas_get_sig_strength_reply_parse(reply, &error);
    if (!output) {
        g_prefix_error (&error, "Getting Signal Strength reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    }
    else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_nas_get_sig_strength_output_unref);

    g_simple_async_result_complete(simple);
    g_object_unref(simple);
    qmi_message_unref (reply);

}

/**
 *
 *
 */
void
qmi_client_nas_get_sig_strength(QmiClientNas *self,
							guint timeout,
							GCancellable *cancellable,
							GAsyncReadyCallback callback,
							gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;
    GError *error = NULL;

    result = g_simple_async_result_new(G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_nas_get_sig_strength);

    request = qmi_message_nas_get_sig_strength_new(qmi_client_get_next_transaction_id(QMI_CLIENT (self)),
                                           	   qmi_client_get_cid (QMI_CLIENT(self)), &error);

    qmi_device_command((QmiDevice *)qmi_client_peek_device(QMI_CLIENT (self)),
                        request,
                        timeout,
                        cancellable,
                        (GAsyncReadyCallback)get_sig_strength_ready,
                        result);

    qmi_message_unref (request);

}

/*****************************************************************************/
/* Get System Selection Preference */

/**
 *
 */
QmiNasGetSystemSelectionPrefOutput*
qmi_client_nas_get_system_selection_pref_finish(QmiClientNas *self, GAsyncResult *res, GError **error)
{

    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(res), error))
        return NULL;

    return qmi_nas_get_system_selection_pref_output_ref(g_simple_async_result_get_op_res_gpointer(G_SIMPLE_ASYNC_RESULT(res)));
}

/**
 *
 *
 */
static void
get_system_selection_pref_ready(QmiDevice *device,
               	   	   GAsyncResult *res,
               	   	   GSimpleAsyncResult *simple)
{
	QmiNasGetSystemSelectionPrefOutput *output;
    GError *error = NULL;
    QmiMessage *reply;

    reply = qmi_device_command_finish(device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Getting System Selection Preference failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_nas_get_system_selection_pref_reply_parse(reply, &error);
    if (!output) {
        g_prefix_error (&error, "Getting System Selection Preference reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    }
    else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_nas_get_system_selection_pref_output_unref);

    g_simple_async_result_complete(simple);
    g_object_unref(simple);
    qmi_message_unref (reply);

}

/**
 *
 *
 */
void
qmi_client_nas_get_system_selection_pref(QmiClientNas *self,
										guint timeout,
										GCancellable *cancellable,
										GAsyncReadyCallback callback,
										gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;
    GError *error = NULL;

    result = g_simple_async_result_new(G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_nas_get_system_selection_pref);

    request = qmi_message_nas_get_system_selection_pref_new(   qmi_client_get_next_transaction_id(QMI_CLIENT (self)),
                                           	   	   	   	   	   qmi_client_get_cid (QMI_CLIENT(self)), &error);

    qmi_device_command((QmiDevice *)qmi_client_peek_device(QMI_CLIENT (self)),
                        request,
                        timeout,
                        cancellable,
                        (GAsyncReadyCallback)get_system_selection_pref_ready,
                        result);

    qmi_message_unref (request);

}

/*****************************************************************************/
/* Set System Selection Preference */

/**
 *
 */
QmiNasSetSystemSelectionPrefOutput*
qmi_client_nas_set_system_selection_pref_finish(QmiClientNas *self, GAsyncResult *res, GError **error)
{

    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(res), error))
        return NULL;

    return qmi_nas_set_system_selection_pref_output_ref(g_simple_async_result_get_op_res_gpointer(G_SIMPLE_ASYNC_RESULT(res)));
}

/**
 *
 *
 */
static void
set_system_selection_pref_ready(QmiDevice *device,
               	   	   GAsyncResult *res,
               	   	   GSimpleAsyncResult *simple)
{
	GError *error = NULL;
    QmiMessage *reply;
    QmiNasSetSystemSelectionPrefOutput *output;

    reply = qmi_device_command_finish(device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Setting System Selection Preference failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_nas_set_system_selection_pref_reply_parse(reply, &error);
    if (!output) {
        g_prefix_error (&error, "Setting System Selection Preference reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    }
    else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_nas_set_system_selection_pref_output_unref);

    g_simple_async_result_complete(simple);
    g_object_unref(simple);
    qmi_message_unref (reply);

}

/**
 *
 *
 */
void
qmi_client_nas_set_system_selection_pref(QmiClientNas *self,
										QmiNasSetSystemSelectionPrefInput *input,
										guint timeout,
										GCancellable *cancellable,
										GAsyncReadyCallback callback,
										gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;
    GError *error = NULL;

    result = g_simple_async_result_new(G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_nas_set_system_selection_pref);

    request = qmi_message_nas_set_system_selection_pref_new(qmi_client_get_next_transaction_id(QMI_CLIENT (self)),
                                           	   qmi_client_get_cid (QMI_CLIENT(self)),
												input,
												&error);


	if (!request) {
		g_prefix_error (&error, "Couldn't create request message: ");
		g_simple_async_result_take_error (result, error);
		g_simple_async_result_complete_in_idle (result);
		g_object_unref (result);
		return;
	}

    qmi_device_command((QmiDevice *)qmi_client_peek_device(QMI_CLIENT (self)),
                        request,
                        timeout,
                        cancellable,
                       (GAsyncReadyCallback)set_system_selection_pref_ready,
                        result);

    qmi_message_unref (request);

}


/**
 *
 *
 */
static void
get_technology_pref_ready(	QmiDevice *device,
               	   	   	   	GAsyncResult *res,
               	   	   	    GSimpleAsyncResult *simple)
{
	GError *error = NULL;
    QmiMessage *reply;
    QmiNasGetTechnologyPrefOutput *output;

    reply = qmi_device_command_finish(device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Getting Technology Preference failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_nas_get_technology_pref_reply_parse(reply, &error);
    if (!output) {
        g_prefix_error (&error, "Getting Technology Preference reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    }
    else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_nas_get_technology_pref_output_unref);

    g_simple_async_result_complete(simple);
    g_object_unref(simple);
    qmi_message_unref (reply);

}

/*****************************************************************************/
/* Get Technology Preference */

void
qmi_client_nas_get_technology_pref(		QmiClientNas *self,
										guint timeout,
										GCancellable *cancellable,
										GAsyncReadyCallback callback,
										gpointer user_data) {

	GSimpleAsyncResult *result;
	QmiMessage *request;
	GError *error = NULL;

	result = g_simple_async_result_new(G_OBJECT (self),
										callback,
										user_data,
										qmi_client_nas_get_technology_pref);

	request = qmi_message_nas_get_technology_pref_new(  qmi_client_get_next_transaction_id(QMI_CLIENT (self)),
														qmi_client_get_cid (QMI_CLIENT(self)), &error);

	qmi_device_command((QmiDevice *)qmi_client_peek_device(QMI_CLIENT (self)),
						request,
						timeout,
						cancellable,
						(GAsyncReadyCallback)get_technology_pref_ready,
						result);

	qmi_message_unref (request);
}

QmiNasGetTechnologyPrefOutput*
qmi_client_nas_get_technology_pref_finish(QmiClientNas *self, GAsyncResult *res, GError **error) {


    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(res), error))
        return NULL;

    return qmi_nas_get_technology_pref_output_ref(g_simple_async_result_get_op_res_gpointer(G_SIMPLE_ASYNC_RESULT(res)));

}

/**
 *
 *
 */
static void
set_technology_pref_ready(	QmiDevice *device,
               	   	   	   	GAsyncResult *res,
               	   	   	    GSimpleAsyncResult *simple)
{
	GError *error = NULL;
    QmiMessage *reply;
    QmiNasSetTechnologyPrefOutput *output;

    reply = qmi_device_command_finish(device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Setting Technology Preference failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_nas_set_technology_pref_reply_parse(reply, &error);
    if (!output) {
        g_prefix_error (&error, "Setting Technology Preference reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    }
    else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_nas_set_technology_pref_output_unref);

    g_simple_async_result_complete(simple);
    g_object_unref(simple);
    qmi_message_unref (reply);

}

/*****************************************************************************/
/* Set Technology Preference */

void
qmi_client_nas_set_technology_pref(		QmiClientNas *self,
										QmiNasSetTechnologyPrefInput *input,
										guint timeout,
										GCancellable *cancellable,
										GAsyncReadyCallback callback,
										gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;
    GError *error = NULL;

    result = g_simple_async_result_new(G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_nas_set_technology_pref);

    request = qmi_message_nas_set_technology_pref_new(	qmi_client_get_next_transaction_id(QMI_CLIENT (self)),
                                           	   	   	   	qmi_client_get_cid (QMI_CLIENT(self)),
                                           	   	   	   	input,
                                           	   	   	   	&error);


	if (!request) {
		g_prefix_error (&error, "Couldn't create request message: ");
		g_simple_async_result_take_error (result, error);
		g_simple_async_result_complete_in_idle (result);
		g_object_unref (result);
		return;
	}

    qmi_device_command((QmiDevice *)qmi_client_peek_device(QMI_CLIENT (self)),
                        request,
                        timeout,
                        cancellable,
                       (GAsyncReadyCallback)set_technology_pref_ready,
                        result);

    qmi_message_unref (request);

}


QmiNasSetTechnologyPrefOutput*
qmi_client_nas_set_technology_pref_finish(QmiClientNas *self, GAsyncResult *res, GError **error){


    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(res), error))
        return NULL;

    return qmi_nas_set_technology_pref_output_ref(g_simple_async_result_get_op_res_gpointer(G_SIMPLE_ASYNC_RESULT(res)));

}



static void
qmi_client_nas_init (QmiClientNas *self)
{
}

static void
qmi_client_nas_class_init (QmiClientNasClass *klass)
{
}
