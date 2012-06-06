/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * libqmi-glib -- GLib/GIO based library to control QMI devices
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 * Copyright (C) 2012 Aleksander Morgado <aleksander@lanedo.com>
 */

#include <gio/gio.h>

#include "qmi-enum-types.h"
#include "qmi-device.h"
#include "qmi-client-dms.h"
#include "qmi-message-dms.h"

G_DEFINE_TYPE (QmiClientDms, qmi_client_dms, QMI_TYPE_CLIENT)

/*****************************************************************************/
/* Get IDs */

QmiDmsGetIdsOutput *
qmi_client_dms_get_ids_finish (QmiClientDms *self,
                               GAsyncResult *res,
                               GError **error)
{
    if (g_simple_async_result_propagate_error (G_SIMPLE_ASYNC_RESULT (res), error))
        return NULL;

    return qmi_dms_get_ids_output_ref (g_simple_async_result_get_op_res_gpointer (G_SIMPLE_ASYNC_RESULT (res)));
}

static void
get_ids_ready (QmiDevice *device,
               GAsyncResult *res,
               GSimpleAsyncResult *simple)
{
    QmiDmsGetIdsOutput *output;
    GError *error = NULL;
    QmiMessage *reply;

    reply = qmi_device_command_finish (device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Getting IDs failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_dms_get_ids_reply_parse (reply, &error);
    if (!output) {
        g_prefix_error (&error, "Getting IDs reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    } else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_dms_get_ids_output_unref);
    g_simple_async_result_complete (simple);
    g_object_unref (simple);
    qmi_message_unref (reply);
}

void
qmi_client_dms_get_ids (QmiClientDms *self,
                        guint timeout,
                        GCancellable *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;

    result = g_simple_async_result_new (G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_dms_get_ids);

    request = qmi_message_dms_get_ids_new (qmi_client_get_next_transaction_id (QMI_CLIENT (self)),
                                           qmi_client_get_cid (QMI_CLIENT (self)));
    qmi_device_command (QMI_DEVICE (qmi_client_peek_device (QMI_CLIENT (self))),
                        request,
                        timeout,
                        cancellable,
                        (GAsyncReadyCallback)get_ids_ready,
                        result);
    qmi_message_unref (request);
}

/*****************************************************************************/

/*****************************************************************************/
/* Get Operation Mode */

QmiDmsGetOpModeOutput*
qmi_client_dms_get_op_mode_finish (QmiClientDms *self,
                               GAsyncResult *res,
                               GError **error)
{
    if (g_simple_async_result_propagate_error (G_SIMPLE_ASYNC_RESULT (res), error))
        return NULL;

    return qmi_dms_get_op_mode_output_ref (g_simple_async_result_get_op_res_gpointer (G_SIMPLE_ASYNC_RESULT (res)));
}

static void
get_op_mode_ready (QmiDevice *device,
               GAsyncResult *res,
               GSimpleAsyncResult *simple)
{
    QmiDmsGetOpModeOutput *output;
    GError *error = NULL;
    QmiMessage *reply;

    reply = qmi_device_command_finish (device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Getting Operation Mode failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_dms_get_op_mode_reply_parse (reply, &error);
    if (!output) {
        g_prefix_error (&error, "Getting Operation Mode reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    } else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_dms_get_op_mode_output_unref);
    g_simple_async_result_complete (simple);
    g_object_unref (simple);
    qmi_message_unref (reply);
}

void
qmi_client_dms_get_op_mode (QmiClientDms *self,
                        guint timeout,
                        GCancellable *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;

    result = g_simple_async_result_new (G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_dms_get_op_mode);

    request = qmi_message_dms_get_op_mode_new (qmi_client_get_next_transaction_id (QMI_CLIENT (self)),
                                           qmi_client_get_cid (QMI_CLIENT (self)));

    qmi_device_command (QMI_DEVICE (qmi_client_peek_device (QMI_CLIENT (self))),
                        request,
                        timeout,
                        cancellable,
                        (GAsyncReadyCallback)get_op_mode_ready,
                        result);
    qmi_message_unref (request);
}

/*****************************************************************************/
/*****************************************************************************/
/* Get Device Manufacturer */

QmiDmsGetDevMfrOutput*
qmi_client_dms_get_dev_mfr_finish (QmiClientDms *self,
                               GAsyncResult *res,
                               GError **error)
{
    if (g_simple_async_result_propagate_error (G_SIMPLE_ASYNC_RESULT (res), error))
        return NULL;

    return qmi_dms_get_dev_mfr_output_ref (g_simple_async_result_get_op_res_gpointer (G_SIMPLE_ASYNC_RESULT (res)));
}

static void
get_dev_mfr_mode_ready (QmiDevice *device,
               GAsyncResult *res,
               GSimpleAsyncResult *simple)
{
	QmiDmsGetDevMfrOutput *output;
    GError *error = NULL;
    QmiMessage *reply;

    reply = qmi_device_command_finish (device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Getting Device Manufacturer failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_dms_get_dev_mfr_reply_parse (reply, &error);
    if (!output) {
        g_prefix_error (&error, "Getting Device Manufacturer reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    } else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_dms_get_dev_mfr_output_unref);
    g_simple_async_result_complete (simple);
    g_object_unref (simple);
    qmi_message_unref (reply);
}

void
qmi_client_dms_get_dev_mfr (QmiClientDms *self,
                        guint timeout,
                        GCancellable *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;

    result = g_simple_async_result_new (G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_dms_get_dev_mfr);

    request = qmi_message_dms_get_dev_mfr_new (qmi_client_get_next_transaction_id (QMI_CLIENT (self)),
                                           qmi_client_get_cid (QMI_CLIENT (self)));

    qmi_device_command (QMI_DEVICE (qmi_client_peek_device (QMI_CLIENT (self))),
                        request,
                        timeout,
                        cancellable,
                        (GAsyncReadyCallback)get_dev_mfr_mode_ready,
                        result);
    qmi_message_unref (request);
}

/*****************************************************************************/
/*****************************************************************************/
/* Get Device Model ID */
QmiDmsGetDevModelIdOutput*
qmi_client_dms_get_dev_model_id_finish (QmiClientDms *self,
                               GAsyncResult *res,
                               GError **error)
{
    if (g_simple_async_result_propagate_error (G_SIMPLE_ASYNC_RESULT (res), error))
        return NULL;

    return qmi_dms_get_dev_model_id_output_ref (g_simple_async_result_get_op_res_gpointer (G_SIMPLE_ASYNC_RESULT (res)));
}

static void
get_dev_model_id_ready (QmiDevice *device,
               GAsyncResult *res,
               GSimpleAsyncResult *simple)
{
	QmiDmsGetDevModelIdOutput *output;
    GError *error = NULL;
    QmiMessage *reply;

    reply = qmi_device_command_finish (device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Getting Device Model ID failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_dms_get_dev_model_id_reply_parse (reply, &error);
    if (!output) {
        g_prefix_error (&error, "Getting Device Model ID reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    } else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_dms_get_dev_model_id_output_unref);
    g_simple_async_result_complete (simple);
    g_object_unref (simple);
    qmi_message_unref (reply);
}

void
qmi_client_dms_get_dev_model_id (QmiClientDms *self,
                        guint timeout,
                        GCancellable *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;

    result = g_simple_async_result_new (G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_dms_get_dev_model_id);

    request = qmi_message_dms_get_dev_model_id_new (qmi_client_get_next_transaction_id (QMI_CLIENT (self)),
                                           qmi_client_get_cid (QMI_CLIENT (self)));

    qmi_device_command (QMI_DEVICE (qmi_client_peek_device (QMI_CLIENT (self))),
                        request,
                        timeout,
                        cancellable,
                        (GAsyncReadyCallback)get_dev_model_id_ready,
                        result);
    qmi_message_unref (request);

}


/*****************************************************************************/
/*****************************************************************************/
/* Get Device Revision ID */
QmiDmsGetDevRevIdOutput*
qmi_client_dms_get_dev_rev_id_finish (QmiClientDms *self,
                               GAsyncResult *res,
                               GError **error)
{
    if (g_simple_async_result_propagate_error (G_SIMPLE_ASYNC_RESULT (res), error))
        return NULL;

    return qmi_dms_get_dev_rev_id_output_ref (g_simple_async_result_get_op_res_gpointer (G_SIMPLE_ASYNC_RESULT (res)));
}

static void
get_dev_rev_id_ready (QmiDevice *device,
               GAsyncResult *res,
               GSimpleAsyncResult *simple)
{
	QmiDmsGetDevRevIdOutput *output;
    GError *error = NULL;
    QmiMessage *reply;

    reply = qmi_device_command_finish (device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Getting Device Revision ID failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_dms_get_dev_rev_id_reply_parse (reply, &error);
    if (!output) {
        g_prefix_error (&error, "Getting Device Revision ID reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    } else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_dms_get_dev_rev_id_output_unref);
    g_simple_async_result_complete (simple);
    g_object_unref (simple);
    qmi_message_unref (reply);
}

void
qmi_client_dms_get_dev_rev_id (QmiClientDms *self,
                        guint timeout,
                        GCancellable *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;

    result = g_simple_async_result_new (G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_dms_get_dev_rev_id);

    request = qmi_message_dms_get_dev_rev_id_new (qmi_client_get_next_transaction_id (QMI_CLIENT (self)),
                                           qmi_client_get_cid (QMI_CLIENT (self)));

    qmi_device_command (QMI_DEVICE (qmi_client_peek_device (QMI_CLIENT (self))),
                        request,
                        timeout,
                        cancellable,
                        (GAsyncReadyCallback)get_dev_rev_id_ready,
                        result);
    qmi_message_unref (request);

}

/*****************************************************************************/
/* Get Device Hardware Revision ID */
QmiDmsGetDevHardwareRevIdOutput*
qmi_client_dms_get_hardware_id_finish (QmiClientDms *self,
                               GAsyncResult *res,
                               GError **error)
{
    if (g_simple_async_result_propagate_error (G_SIMPLE_ASYNC_RESULT (res), error))
        return NULL;

    return qmi_dms_get_hardware_rev_id_output_ref (g_simple_async_result_get_op_res_gpointer (G_SIMPLE_ASYNC_RESULT (res)));
}

static void
get_hardware_id_ready (QmiDevice *device,
               GAsyncResult *res,
               GSimpleAsyncResult *simple)
{
	QmiDmsGetDevHardwareRevIdOutput *output;
    GError *error = NULL;
    QmiMessage *reply;

    reply = qmi_device_command_finish (device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Getting Hardware Revision ID failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_dms_get_hardware_rev_id_reply_parse (reply, &error);
    if (!output) {
        g_prefix_error (&error, "Getting Hardware Revision ID reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    } else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_dms_get_hardware_rev_id_output_unref);
    g_simple_async_result_complete (simple);
    g_object_unref (simple);
    qmi_message_unref (reply);
}

void
qmi_client_dms_get_hardware_id (QmiClientDms *self,
                        guint timeout,
                        GCancellable *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;

    result = g_simple_async_result_new (G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_dms_get_hardware_id);

    request = qmi_message_dms_get_hardware_rev_id_new (qmi_client_get_next_transaction_id (QMI_CLIENT (self)),
                                           qmi_client_get_cid (QMI_CLIENT (self)));

    qmi_device_command (QMI_DEVICE (qmi_client_peek_device (QMI_CLIENT (self))),
                        request,
                        timeout,
                        cancellable,
                        (GAsyncReadyCallback)get_hardware_id_ready,
                        result);
    qmi_message_unref (request);

}

/*****************************************************************************/
/* Get MSISDN/ Voice number */

QmiDmsGetMsisdnOutput*
qmi_client_dms_get_msisdn_finish (QmiClientDms *self,
                               GAsyncResult *res,
                               GError **error)
{
    if (g_simple_async_result_propagate_error (G_SIMPLE_ASYNC_RESULT (res), error))
        return NULL;

    return qmi_dms_get_msisdn_output_ref (g_simple_async_result_get_op_res_gpointer (G_SIMPLE_ASYNC_RESULT (res)));
}

static void
get_msisdn_ready (QmiDevice *device,
               GAsyncResult *res,
               GSimpleAsyncResult *simple)
{
	QmiDmsGetMsisdnOutput *output;
    GError *error = NULL;
    QmiMessage *reply;

    reply = qmi_device_command_finish (device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Getting MSISDN failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_dms_get_msisdn_reply_parse (reply, &error);
    if (!output) {
        g_prefix_error (&error, "Getting MSISDN reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    } else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_dms_get_msisdn_output_unref);
    g_simple_async_result_complete (simple);
    g_object_unref (simple);
    qmi_message_unref (reply);
}

void
qmi_client_dms_get_msisdn(QmiClientDms *self,
                        guint timeout,
                        GCancellable *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;

    result = g_simple_async_result_new (G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_dms_get_msisdn);

    request = qmi_message_dms_get_msisdn_new (qmi_client_get_next_transaction_id (QMI_CLIENT (self)),
                                           qmi_client_get_cid (QMI_CLIENT (self)));

    qmi_device_command (QMI_DEVICE (qmi_client_peek_device (QMI_CLIENT (self))),
                        request,
                        timeout,
                        cancellable,
                        (GAsyncReadyCallback)get_msisdn_ready,
                        result);
    qmi_message_unref (request);

}

/*****************************************************************************/
/* Get SW Version */

QmiDmsGetSwVersionOutput*
qmi_client_dms_get_sw_version_finish (QmiClientDms *self,
                               GAsyncResult *res,
                               GError **error)
{
    if (g_simple_async_result_propagate_error (G_SIMPLE_ASYNC_RESULT (res), error))
        return NULL;

    return qmi_dms_get_sw_version_output_ref (g_simple_async_result_get_op_res_gpointer (G_SIMPLE_ASYNC_RESULT (res)));
}

static void
get_sw_version_ready (QmiDevice *device,
               GAsyncResult *res,
               GSimpleAsyncResult *simple)
{
	QmiDmsGetSwVersionOutput *output;
    GError *error = NULL;
    QmiMessage *reply;

    reply = qmi_device_command_finish (device, res, &error);
    if (!reply) {
        g_prefix_error (&error, "Getting SW Version failed: ");
        g_simple_async_result_take_error (simple, error);
        g_simple_async_result_complete (simple);
        g_object_unref (simple);
        return;
    }

    /* Parse reply */
    output = qmi_message_dms_get_sw_version_reply_parse (reply, &error);
    if (!output) {
        g_prefix_error (&error, "Getting SW Version reply parsing failed: ");
        g_simple_async_result_take_error (simple, error);
    } else
        g_simple_async_result_set_op_res_gpointer (simple,
                                                   output,
                                                   (GDestroyNotify)qmi_dms_get_sw_version_output_unref);
    g_simple_async_result_complete (simple);
    g_object_unref (simple);
    qmi_message_unref (reply);
}

void
qmi_client_dms_get_sw_version (QmiClientDms *self,
                        guint timeout,
                        GCancellable *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer user_data)
{
    GSimpleAsyncResult *result;
    QmiMessage *request;

    result = g_simple_async_result_new (G_OBJECT (self),
                                        callback,
                                        user_data,
                                        qmi_client_dms_get_sw_version);

    request = qmi_message_dms_get_sw_version_new (qmi_client_get_next_transaction_id (QMI_CLIENT (self)),
                                           qmi_client_get_cid (QMI_CLIENT (self)));

    qmi_device_command (QMI_DEVICE (qmi_client_peek_device (QMI_CLIENT (self))),
                        request,
                        timeout,
                        cancellable,
                        (GAsyncReadyCallback)get_sw_version_ready,
                        result);
    qmi_message_unref (request);

}


static void
qmi_client_dms_init (QmiClientDms *self)
{
}

static void
qmi_client_dms_class_init (QmiClientDmsClass *klass)
{
}
