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

#include <string.h>

#include "qmi-message-wds.h"
#include "qmi-enums.h"
#include "qmi-error-types.h"

/*****************************************************************************/
/* Start network */

enum {
  QMI_WDS_TLV_START_NETWORK_APN      = 0x14,
  QMI_WDS_TLV_START_NETWORK_USERNAME = 0x17,
  QMI_WDS_TLV_START_NETWORK_PASSWORD = 0x18
};

/**
 * QmiWdsStartNetworkInput:
 *
 * An opaque type handling the input arguments that may be passed to the Start
 * Network operation in the WDS service.
 */
struct _QmiWdsStartNetworkInput {
    volatile gint ref_count;
    gchar *apn;
    gchar *username;
    gchar *password;
};

/**
 * qmi_wds_start_network_input_set_apn:
 * @input: a #QmiWdsStartNetworkInput.
 * @str: a string with the APN.
 *
 * Set the APN to use.
 */
void
qmi_wds_start_network_input_set_apn (QmiWdsStartNetworkInput *input,
                                     const gchar *str)
{
    g_return_if_fail (input != NULL);

    g_free (input->apn);
    input->apn = (str ? g_strdup (str) : NULL);
}

/**
 * qmi_wds_start_network_input_get_apn:
 * @input: a #QmiWdsStartNetworkInput.
 *
 * Get the configured APN to use.
 *
 * Returns: a constant string with the APN, or #NULL if none set.
 */
const gchar *
qmi_wds_start_network_input_get_apn (QmiWdsStartNetworkInput *input)
{
    g_return_val_if_fail (input != NULL, NULL);

    return input->apn;
}

/**
 * qmi_wds_start_network_input_set_username:
 * @input: a #QmiWdsStartNetworkInput.
 * @str: a string with the username.
 *
 * Set the username to use when authenticating with the network.
 */
void
qmi_wds_start_network_input_set_username (QmiWdsStartNetworkInput *input,
                                          const gchar *str)
{
    g_return_if_fail (input != NULL);

    g_free (input->username);
    input->username = (str ? g_strdup (str) : NULL);
}

/**
 * qmi_wds_start_network_input_get_username:
 * @input: a #QmiWdsStartNetworkInput.
 *
 * Get the configured username to use when authenticating with the network.
 *
 * Returns: a constant string with the username, or #NULL if none set.
 */
const gchar *
qmi_wds_start_network_input_get_username (QmiWdsStartNetworkInput *input)
{
    g_return_val_if_fail (input != NULL, NULL);

    return input->username;
}

/**
 * qmi_wds_start_network_input_set_password:
 * @input: a #QmiWdsStartNetworkInput.
 * @str: a string with the password.
 *
 * Set the password to use when authenticating with the network.
 */
void
qmi_wds_start_network_input_set_password (QmiWdsStartNetworkInput *input,
                                          const gchar *str)
{
    g_return_if_fail (input != NULL);

    g_free (input->password);
    input->password = (str ? g_strdup (str) : NULL);
}

/**
 * qmi_wds_start_network_input_get_password:
 * @input: a #QmiWdsStartNetworkInput.
 *
 * Get the configured password to use when authenticating with the network.
 *
 * Returns: a constant string with the password, or #NULL if none set.
 */
const gchar *
qmi_wds_start_network_input_get_password (QmiWdsStartNetworkInput *input)
{
    g_return_val_if_fail (input != NULL, NULL);

    return input->password;
}

/**
 * qmi_wds_start_network_input_new:
 *
 * Allocates a new #QmiWdsStartNetworkInput.
 *
 * Returns: the newly created #QmiWdsStartNetworkInput.
 */
QmiWdsStartNetworkInput *
qmi_wds_start_network_input_new (void)
{
    QmiWdsStartNetworkInput *input;

    input = g_slice_new0 (QmiWdsStartNetworkInput);
    input->ref_count = 1;
    return input;
}

/**
 * qmi_wds_start_network_input_ref:
 * @input: a #QmiWdsStartNetworkInput.
 *
 * Atomically increments the reference count of @input by one.
 *
 * Returns: the new reference to @input.
 */
QmiWdsStartNetworkInput *
qmi_wds_start_network_input_ref (QmiWdsStartNetworkInput *input)
{
    g_return_val_if_fail (input != NULL, NULL);

    g_atomic_int_inc (&input->ref_count);
    return input;
}

/**
 * qmi_wds_start_network_input_unref:
 * @input: a #QmiWdsStartNetworkInput.
 *
 * Atomically decrements the reference count of @input by one.
 * If the reference count drops to 0, @input is completely disposed.
 */
void
qmi_wds_start_network_input_unref (QmiWdsStartNetworkInput *input)
{
    g_return_if_fail (input != NULL);

    if (g_atomic_int_dec_and_test (&input->ref_count)) {
        g_free (input->apn);
        g_free (input->username);
        g_free (input->password);
        g_slice_free (QmiWdsStartNetworkInput, input);
    }
}

QmiMessage *
qmi_message_wds_start_network_new (guint8 transaction_id,
                                   guint8 client_id,
                                   QmiWdsStartNetworkInput *input,
                                   GError **error)
{
    QmiMessage *message;

    message = qmi_message_new (QMI_SERVICE_WDS,
                               client_id,
                               transaction_id,
                               QMI_WDS_MESSAGE_START_NETWORK);

    /* If input arguments given, use them */
    if (input) {
        /* Add APN if any */
        if (input->apn &&
            !qmi_message_tlv_add (message,
                                  QMI_WDS_TLV_START_NETWORK_APN,
                                  strlen (input->apn) + 1,
                                  input->apn,
                                  error)) {
            g_prefix_error (error, "Failed to add APN to message: ");
            qmi_message_unref (message);
            return NULL;
        }

        /* Add username if any */
        if (input->username &&
            !qmi_message_tlv_add (message,
                                  QMI_WDS_TLV_START_NETWORK_USERNAME,
                                  strlen (input->username) + 1,
                                  input->username,
                                  error)) {
            g_prefix_error (error, "Failed to add username to message: ");
            qmi_message_unref (message);
            return NULL;
        }

        /* Add password if any */
        if (input->password &&
            !qmi_message_tlv_add (message,
                                  QMI_WDS_TLV_START_NETWORK_PASSWORD,
                                  strlen (input->password) + 1,
                                  input->password,
                                  error)) {
            g_prefix_error (error, "Failed to add password to message: ");
            qmi_message_unref (message);
            return NULL;
        }
    }

    return message;
}

enum {
    START_NETWORK_OUTPUT_TLV_PACKET_DATA_HANDLE      = 0x01,
    START_NETWORK_OUTPUT_TLV_CALL_END_REASON         = 0x10,
    START_NETWORK_OUTPUT_TLV_VERBOSE_CALL_END_REASON = 0x11
};

/**
 * QmiWdsStartNetworkOutput:
 *
 * An opaque type handling the output of the Start Network operation.
 */
struct _QmiWdsStartNetworkOutput {
    volatile gint ref_count;
    GError *error;

    gboolean packet_data_handle_set;
    guint32 packet_data_handle;

    gboolean call_end_reason_set;
    guint16 call_end_reason;

    gboolean verbose_call_end_reason_set;
    guint16 verbose_call_end_reason_domain;
    guint16 verbose_call_end_reason_value;
};

/**
 * qmi_wds_start_network_output_get_result:
 * @output: a #QmiWdsStartNetworkOutput.
 * @error: a #GError.
 *
 * Get the result of the Start Network operation.
 *
 * Returns: #TRUE if the operation succeeded, and #FALSE if @error is set.
 */
gboolean
qmi_wds_start_network_output_get_result (QmiWdsStartNetworkOutput *output,
                                         GError **error)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->error) {
        if (error)
            *error = g_error_copy (output->error);

        return FALSE;
    }

    return TRUE;
}

/**
 * qmi_wds_start_network_output_get_packet_data_handle:
 * @output: a #QmiWdsStartNetworkOutput.
 * @packet_data_handle: location for the output packet data handle.
 *
 * Get the packet data handle on a successful Start Network operation.
 *
 * Returns: #TRUE if @packet_data_handle is set, #FALSE otherwise.
 */
gboolean
qmi_wds_start_network_output_get_packet_data_handle (QmiWdsStartNetworkOutput *output,
                                                     guint32 *packet_data_handle)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->packet_data_handle_set)
        *packet_data_handle = output->packet_data_handle;
    return output->packet_data_handle_set;
}

/**
 * qmi_wds_start_network_output_get_call_end_reason:
 * @output: a #QmiWdsStartNetworkOutput.
 * @call_end_reason: location for the output call end reason.
 *
 * Get the call end reason, if the operation failed with a CALL_FAILED error.
 * This field is optional.
 *
 * Returns: #TRUE if @call_end_reason is set, #FALSE otherwise.
 */
gboolean
qmi_wds_start_network_output_get_call_end_reason (QmiWdsStartNetworkOutput *output,
                                                  guint16 *call_end_reason)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->call_end_reason_set)
        *call_end_reason = output->call_end_reason;
    return output->call_end_reason_set;
}

/**
 * qmi_wds_start_network_output_get_call_end_reason:
 * @output: a #QmiWdsStartNetworkOutput.
 * @verbose_call_end_reason_domain: location for the output call end reason domain.
 * @verbose_call_end_reason_value: location for the output call end reason value.
 *
 * Get the verbose call end reason, if the operation failed with a CALL_FAILED error.
 * This field is optional.
 *
 * Returns: #TRUE if @call_end_reason is set, #FALSE otherwise.
 */
gboolean
qmi_wds_start_network_output_get_verbose_call_end_reason (QmiWdsStartNetworkOutput *output,
                                                          guint16 *verbose_call_end_reason_domain,
                                                          guint16 *verbose_call_end_reason_value)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->verbose_call_end_reason_set) {
        *verbose_call_end_reason_domain = output->verbose_call_end_reason_domain;
        *verbose_call_end_reason_value = output->verbose_call_end_reason_value;
    }
    return output->verbose_call_end_reason_set;
}

/**
 * qmi_wds_start_network_output_ref:
 * @output: a #QmiWdsStartNetworkOutput.
 *
 * Atomically increments the reference count of @output by one.
 *
 * Returns: the new reference to @output.
 */
QmiWdsStartNetworkOutput *
qmi_wds_start_network_output_ref (QmiWdsStartNetworkOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}

/**
 * qmi_wds_start_network_output_unref:
 * @output: a #QmiWdsStartNetworkOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_wds_start_network_output_unref (QmiWdsStartNetworkOutput *output)
{
    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {
        if (output->error)
            g_error_free (output->error);
        g_slice_free (QmiWdsStartNetworkOutput, output);
    }
}

QmiWdsStartNetworkOutput *
qmi_message_wds_start_network_reply_parse (QmiMessage *self,
                                           GError **error)
{
    QmiWdsStartNetworkOutput *output;
    GError *inner_error = NULL;
    guint32 packet_data_handle_le;

    g_assert (qmi_message_get_message_id (self) == QMI_WDS_MESSAGE_START_NETWORK);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    output = g_slice_new0 (QmiWdsStartNetworkOutput);
    output->ref_count = 1;
    output->error = inner_error;

    /* On CALL_FAILED errors, we can try to get more info on the reason */
    if (output->error) {
        if (g_error_matches (output->error,
                             QMI_PROTOCOL_ERROR,
                             QMI_PROTOCOL_ERROR_CALL_FAILED)) {
            guint16 cer = 0;
            struct verbose_call_end_reason {
                guint16 call_end_reason_domain;
                guint16 call_end_reason_value;
            } __attribute__((__packed__)) verbose_cer = { 0, 0 };

            /* TODO: Prepare an enum with all the possible call end reasons,
             * in order to do this nicely */

            if (qmi_message_tlv_get (self,
                                     START_NETWORK_OUTPUT_TLV_VERBOSE_CALL_END_REASON,
                                     sizeof (verbose_cer),
                                     &verbose_cer,
                                     NULL)) {
                output->verbose_call_end_reason_set = TRUE;
                output->verbose_call_end_reason_domain = verbose_cer.call_end_reason_domain;
                output->verbose_call_end_reason_value = verbose_cer.call_end_reason_value;
            }

            if (qmi_message_tlv_get (self,
                                     START_NETWORK_OUTPUT_TLV_CALL_END_REASON,
                                     sizeof (cer),
                                     &cer,
                                     NULL)) {
                output->call_end_reason_set = TRUE;
                output->call_end_reason = cer;
            }
        }

        return output;
    }

    /* success */

    packet_data_handle_le = 0;
    if (!qmi_message_tlv_get (self,
                              START_NETWORK_OUTPUT_TLV_PACKET_DATA_HANDLE,
                              sizeof (packet_data_handle_le),
                              &packet_data_handle_le,
                              error)) {
        g_prefix_error (error, "Couldn't get the packet data handle TLV: ");
        qmi_wds_start_network_output_unref (output);
        return NULL;
    } else {
        output->packet_data_handle = GUINT32_FROM_LE (packet_data_handle_le);
        output->packet_data_handle_set = TRUE;
    }

    return output;
}

/*****************************************************************************/
/* Stop network */

enum {
    STOP_NETWORK_INPUT_TLV_PACKET_DATA_HANDLE = 0x01
};

/**
 * QmiWdsStopNetworkInput:
 *
 * An opaque type handling the input arguments that may be passed to the Stop
 * Network operation in the WDS service.
 */
struct _QmiWdsStopNetworkInput {
    volatile gint ref_count;

    gboolean packet_data_handle_set;
    guint32 packet_data_handle;
};

/**
 * qmi_wds_stop_network_input_set_packet_data_handle:
 * @input: a #QmiWdsStopNetworkInput.
 * @packet_data_handle: the packet data handle.
 *
 * Set the packet data handle of the connection.
 */
void
qmi_wds_stop_network_input_set_packet_data_handle (QmiWdsStopNetworkInput *input,
                                                   guint32 packet_data_handle)
{
    g_return_if_fail (input != NULL);

    input->packet_data_handle_set = TRUE;
    input->packet_data_handle = packet_data_handle;
}

/**
 * qmi_wds_stop_network_input_get_packet_data_handle:
 * @input: a #QmiWdsStopNetworkInput.
 * @packet_data_handle: output location for the packet data handle.
 *
 * Get the packet data handle of the connection.
 *
 * Returns: #TRUE if @packet_data_handle is set, #FALSE otherwise.
 */
gboolean
qmi_wds_stop_network_input_get_packet_data_handle (QmiWdsStopNetworkInput *input,
                                                   guint32 *packet_data_handle)
{
    g_return_val_if_fail (input != NULL, FALSE);

    if (input->packet_data_handle_set)
        *packet_data_handle = input->packet_data_handle;
    return input->packet_data_handle_set;
}

/**
 * qmi_wds_stop_network_input_new:
 *
 * Allocates a new #QmiWdsStopNetworkInput.
 *
 * Returns: the newly created #QmiWdsStopNetworkInput.
 */
QmiWdsStopNetworkInput *
qmi_wds_stop_network_input_new (void)
{
    QmiWdsStopNetworkInput *input;

    input = g_slice_new0 (QmiWdsStopNetworkInput);
    input->ref_count = 1;
    return input;
}

/**
 * qmi_wds_stop_network_input_ref:
 * @input: a #QmiWdsStopNetworkInput.
 *
 * Atomically increments the reference count of @input by one.
 *
 * Returns: the new reference to @input.
 */
QmiWdsStopNetworkInput *
qmi_wds_stop_network_input_ref (QmiWdsStopNetworkInput *input)
{
    g_return_val_if_fail (input != NULL, NULL);

    g_atomic_int_inc (&input->ref_count);
    return input;
}

/**
 * qmi_wds_stop_network_input_unref:
 * @input: a #QmiWdsStopNetworkInput.
 *
 * Atomically decrements the reference count of @input by one.
 * If the reference count drops to 0, @input is completely disposed.
 */
void
qmi_wds_stop_network_input_unref (QmiWdsStopNetworkInput *input)
{
    g_return_if_fail (input != NULL);

    if (g_atomic_int_dec_and_test (&input->ref_count)) {
        g_slice_free (QmiWdsStopNetworkInput, input);
    }
}

QmiMessage *
qmi_message_wds_stop_network_new (guint8 transaction_id,
                                  guint8 client_id,
                                  QmiWdsStopNetworkInput *input,
                                  GError **error)
{
    QmiMessage *message;
    guint32 handle;

    /* Check mandatory input arguments */
    if (!input ||
        !input->packet_data_handle_set) {
        g_set_error (error,
                     QMI_CORE_ERROR,
                     QMI_CORE_ERROR_INVALID_ARGS,
                     "Missing mandatory argument 'packet data handle'");
        return NULL;
    }

    if (!input->packet_data_handle) {
        g_set_error (error,
                     QMI_CORE_ERROR,
                     QMI_CORE_ERROR_INVALID_ARGS,
                     "Invalid 'packet data handle': %u",
                     (guint)input->packet_data_handle);
        return NULL;
    }

    /* We want `handle' in LE; FROM_LE() can also be used as TO_LE() */
    handle = GUINT32_FROM_LE (input->packet_data_handle);

    message = qmi_message_new (QMI_SERVICE_WDS,
                               client_id,
                               transaction_id,
                               QMI_WDS_MESSAGE_STOP_NETWORK);

    if (!qmi_message_tlv_add (message,
                              STOP_NETWORK_INPUT_TLV_PACKET_DATA_HANDLE,
                              sizeof (handle),
                              &handle,
                              error)) {
        g_prefix_error (error, "Failed to add packet data handle to message: ");
        qmi_message_unref (message);
        return NULL;
    }

    return message;
}

/**
 * QmiWdsStopNetworkOutput:
 *
 * An opaque type handling the output of the Stop Network operation.
 */
struct _QmiWdsStopNetworkOutput {
    volatile gint ref_count;
    GError *error;
};

/**
 * qmi_wds_stop_network_output_get_result:
 * @output: a #QmiWdsStopNetworkOutput.
 * @error: a #GError.
 *
 * Get the result of the Stop Network operation.
 *
 * Returns: #TRUE if the operation succeeded, and #FALSE if @error is set.
 */
gboolean
qmi_wds_stop_network_output_get_result (QmiWdsStopNetworkOutput *output,
                                         GError **error)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->error) {
        if (error)
            *error = g_error_copy (output->error);

        return FALSE;
    }

    return TRUE;
}

/**
 * qmi_wds_stop_network_output_ref:
 * @output: a #QmiWdsStopNetworkOutput.
 *
 * Atomically increments the reference count of @output by one.
 *
 * Returns: the new reference to @output.
 */
QmiWdsStopNetworkOutput *
qmi_wds_stop_network_output_ref (QmiWdsStopNetworkOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}

/**
 * qmi_wds_stop_network_output_unref:
 * @output: a #QmiWdsStopNetworkOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_wds_stop_network_output_unref (QmiWdsStopNetworkOutput *output)
{
    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {
        if (output->error)
            g_error_free (output->error);
        g_slice_free (QmiWdsStopNetworkOutput, output);
    }
}

QmiWdsStopNetworkOutput *
qmi_message_wds_stop_network_reply_parse (QmiMessage *self,
                                           GError **error)
{
    QmiWdsStopNetworkOutput *output;
    GError *inner_error = NULL;

    g_assert (qmi_message_get_message_id (self) == QMI_WDS_MESSAGE_STOP_NETWORK);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    output = g_slice_new0 (QmiWdsStopNetworkOutput);
    output->ref_count = 1;
    output->error = inner_error;

    return output;
}

/*****************************************************************************/
/* Get packet service status */

QmiMessage *
qmi_message_wds_get_packet_service_status_new (guint8 transaction_id,
                                               guint8 client_id)
{
    return qmi_message_new (QMI_SERVICE_WDS,
                            client_id,
                            transaction_id,
                            QMI_WDS_MESSAGE_GET_PACKET_SERVICE_STATUS);
}

enum {
    GET_PACKET_SERVICE_STATUS_OUTPUT_TLV_CONNECTION_STATUS = 0x01
};

/**
 * QmiWdsGetPacketServiceStatusOutput:
 *
 * An opaque type handling the output of the Stop Network operation.
 */
struct _QmiWdsGetPacketServiceStatusOutput {
    volatile gint ref_count;
    GError *error;
    guint8 connection_status;
};

/**
 * qmi_wds_get_packet_service_status_output_get_result:
 * @output: a #QmiWdsGetPacketServiceStatusOutput.
 * @error: a #GError.
 *
 * Get the result of the Get Packet Status operation.
 *
 * Returns: #TRUE if the operation succeeded, and #FALSE if @error is set.
 */
gboolean
qmi_wds_get_packet_service_status_output_get_result (QmiWdsGetPacketServiceStatusOutput *output,
                                                     GError **error)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->error) {
        if (error)
            *error = g_error_copy (output->error);

        return FALSE;
    }

    return TRUE;
}

/**
 * qmi_wds_start_network_output_get_connection_status:
 * @output: a #QmiWdsGetPacketServiceStatusOutput.
 *
 * Get the connection status.
 *
 * Returns: a #QmiWdsConnectionStatus.
 */
QmiWdsConnectionStatus
qmi_wds_get_packet_service_status_output_get_connection_status (QmiWdsGetPacketServiceStatusOutput *output)
{
    g_return_val_if_fail (output != NULL, QMI_WDS_CONNECTION_STATUS_UNKNOWN);

    return (QmiWdsConnectionStatus)output->connection_status;
}

/**
 * qmi_wds_get_packet_service_status_output_ref:
 * @output: a #QmiWdsGetPacketServiceStatusOutput.
 *
 * Atomically increments the reference count of @output by one.
 *
 * Returns: the new reference to @output.
 */
QmiWdsGetPacketServiceStatusOutput *
qmi_wds_get_packet_service_status_output_ref (QmiWdsGetPacketServiceStatusOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}

/**
 * qmi_wds_get_packet_service_status_output_unref:
 * @output: a #QmiWdsGetPacketServiceStatusOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_wds_get_packet_service_status_output_unref (QmiWdsGetPacketServiceStatusOutput *output)
{
    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {
        if (output->error)
            g_error_free (output->error);
        g_slice_free (QmiWdsGetPacketServiceStatusOutput, output);
    }
}

QmiWdsGetPacketServiceStatusOutput *
qmi_message_wds_get_packet_service_status_reply_parse (QmiMessage *self,
                                                       GError **error)
{
    QmiWdsGetPacketServiceStatusOutput *output;
    GError *inner_error = NULL;

    g_assert (qmi_message_get_message_id (self) == QMI_WDS_MESSAGE_GET_PACKET_SERVICE_STATUS);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    /* success */

    output = g_slice_new0 (QmiWdsGetPacketServiceStatusOutput);
    output->ref_count = 1;
    output->error = inner_error;

    /* If we got a QMI protocol error, no need to look for any TLV */
    if (output->error)
        return output;

    if (!qmi_message_tlv_get (self,
                              GET_PACKET_SERVICE_STATUS_OUTPUT_TLV_CONNECTION_STATUS,
                              sizeof (output->connection_status),
                              &output->connection_status,
                              error)) {
        g_prefix_error (error, "Couldn't get the connection status TLV: ");
        qmi_wds_get_packet_service_status_output_unref (output);
        return NULL;
    }

    return output;
}

/*****************************************************************************/
/* Get data bearer technology */

QmiMessage *
qmi_message_wds_get_data_bearer_technology_new (guint8 transaction_id,
                                                guint8 client_id)
{
    return qmi_message_new (QMI_SERVICE_WDS,
                            client_id,
                            transaction_id,
                            QMI_WDS_MESSAGE_GET_DATA_BEARER_TECHNOLOGY);
}

enum {
    GET_DATA_BEARER_TECHNOLOGY_OUTPUT_TLV_CURRENT = 0x01,
    GET_DATA_BEARER_TECHNOLOGY_OUTPUT_TLV_LAST    = 0x10
};

/**
 * QmiWdsGetDataBearerTechnologyOutput:
 *
 * An opaque type handling the output of the Stop Network operation.
 */
struct _QmiWdsGetDataBearerTechnologyOutput {
    volatile gint ref_count;
    GError *error;
    gint8 current;
    gint8 last;
};

/**
 * qmi_wds_get_data_bearer_technology_output_get_result:
 * @output: a #QmiWdsGetDataBearerTechnologyOutput.
 * @error: a #GError.
 *
 * Get the result of the Get Data Bearer Technology operation.
 *
 * Returns: #TRUE if the operation succeeded, and #FALSE if @error is set.
 */
gboolean
qmi_wds_get_data_bearer_technology_output_get_result (QmiWdsGetDataBearerTechnologyOutput *output,
                                                      GError **error)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->error) {
        if (error)
            *error = g_error_copy (output->error);

        return FALSE;
    }

    return TRUE;
}

/**
 * qmi_wds_get_data_bearer_technology_output_get_current:
 * @output: a #QmiWdsGetDataBearerTechnologyOutput.
 *
 * Get the current data bearer technology.
 *
 * Returns: a #QmiWdsDataBearerTechnology.
 */
QmiWdsDataBearerTechnology
qmi_wds_get_data_bearer_technology_output_get_current (QmiWdsGetDataBearerTechnologyOutput *output)
{
    g_return_val_if_fail (output != NULL, QMI_WDS_DATA_BEARER_TECHNOLOGY_UNKNOWN);

    return (QmiWdsDataBearerTechnology)output->current;
}

/**
 * qmi_wds_get_data_bearer_technology_output_output_get_last:
 * @output: a #QmiWdsGetDataBearerTechnologyOutput.
 *
 * Get the data bearer technology of the last connection.
 * This field is optional, and it may appear only when the operation failed with #QMI_PROTOCOL_ERROR_OUT_OF_CALL.
 *
 * Returns: a #QmiWdsDataBearerTechnology.
 */
QmiWdsDataBearerTechnology
qmi_wds_get_data_bearer_technology_output_get_last (QmiWdsGetDataBearerTechnologyOutput *output)
{
    g_return_val_if_fail (output != NULL, QMI_WDS_DATA_BEARER_TECHNOLOGY_UNKNOWN);

    return (QmiWdsDataBearerTechnology)output->last;
}

/**
 * qmi_wds_get_data_bearer_technology_output_ref:
 * @output: a #QmiWdsGetDataBearerTechnologyOutput.
 *
 * Atomically increments the reference count of @output by one.
 *
 * Returns: the new reference to @output.
 */
QmiWdsGetDataBearerTechnologyOutput *
qmi_wds_get_data_bearer_technology_output_ref (QmiWdsGetDataBearerTechnologyOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}

/**
 * qmi_wds_get_data_bearer_technology_output_unref:
 * @output: a #QmiWdsGetDataBearerTechnologyOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_wds_get_data_bearer_technology_output_unref (QmiWdsGetDataBearerTechnologyOutput *output)
{
    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {
        if (output->error)
            g_error_free (output->error);
        g_slice_free (QmiWdsGetDataBearerTechnologyOutput, output);
    }
}

QmiWdsGetDataBearerTechnologyOutput *
qmi_message_wds_get_data_bearer_technology_reply_parse (QmiMessage *self,
                                                        GError **error)
{
    QmiWdsGetDataBearerTechnologyOutput *output;
    GError *inner_error = NULL;

    g_assert (qmi_message_get_message_id (self) == QMI_WDS_MESSAGE_GET_DATA_BEARER_TECHNOLOGY);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    /* success */

    output = g_slice_new0 (QmiWdsGetDataBearerTechnologyOutput);
    output->ref_count = 1;
    output->error = inner_error;
    output->current = QMI_WDS_DATA_BEARER_TECHNOLOGY_UNKNOWN;
    output->last = QMI_WDS_DATA_BEARER_TECHNOLOGY_UNKNOWN;

    /* Handle QMI protocol errors */
    if (output->error) {
        if (g_error_matches (output->error,
                             QMI_PROTOCOL_ERROR,
                             QMI_PROTOCOL_ERROR_OUT_OF_CALL)) {
            /* last will only appear if we get out-of-call errors */
            qmi_message_tlv_get (self,
                                 GET_DATA_BEARER_TECHNOLOGY_OUTPUT_TLV_LAST,
                                 sizeof (output->last),
                                 &output->last,
                                 NULL);
        }

        return output;
    }

    if (!qmi_message_tlv_get (self,
                              GET_DATA_BEARER_TECHNOLOGY_OUTPUT_TLV_CURRENT,
                              sizeof (output->current),
                              &output->current,
                              error)) {
        g_prefix_error (error, "Couldn't get the current technology TLV: ");
        qmi_wds_get_data_bearer_technology_output_unref (output);
        return NULL;
    }

    return output;
}

/*****************************************************************************/
/* Get current data bearer technology */

QmiMessage *
qmi_message_wds_get_current_data_bearer_technology_new (guint8 transaction_id,
                                                        guint8 client_id)
{
    return qmi_message_new (QMI_SERVICE_WDS,
                            client_id,
                            transaction_id,
                            QMI_WDS_MESSAGE_GET_CURRENT_DATA_BEARER_TECHNOLOGY);
}

enum {
    GET_CURRENT_DATA_BEARER_TECHNOLOGY_OUTPUT_TLV_CURRENT = 0x01,
    GET_CURRENT_DATA_BEARER_TECHNOLOGY_OUTPUT_TLV_LAST    = 0x10
};

struct current_data_bearer_technology {
    guint8 nw;
    guint32 rat_mask;
    guint32 so_mask;
} __attribute__((__packed__));

/**
 * QmiWdsGetCurrentDataBearerTechnologyOutput:
 *
 * An opaque type handling the output of the Get Current Data Bearer Technology operation.
 */
struct _QmiWdsGetCurrentDataBearerTechnologyOutput {
    volatile gint ref_count;
    GError *error;
    struct current_data_bearer_technology current;
    struct current_data_bearer_technology last;
};

/**
 * qmi_wds_get_current_data_bearer_technology_output_get_result:
 * @output: a #QmiWdsGetCurrentDataBearerTechnologyOutput.
 * @error: a #GError.
 *
 * Get the result of the Get Current Data Bearer Technology operation.
 *
 * Returns: #TRUE if the operation succeeded, and #FALSE if @error is set.
 */
gboolean
qmi_wds_get_current_data_bearer_technology_output_get_result (QmiWdsGetCurrentDataBearerTechnologyOutput *output,
                                                              GError **error)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->error) {
        if (error)
            *error = g_error_copy (output->error);

        return FALSE;
    }

    return TRUE;
}

/**
 * qmi_wds_get_current_data_bearer_technology_output_get_current_network_type:
 * @output: a #QmiWdsGetCurrentDataBearerTechnologyOutput.
 *
 * Get the current network type.
 *
 * Returns: a #QmiWdsNetworkType.
 */
QmiWdsNetworkType
qmi_wds_get_current_data_bearer_technology_output_get_current_network_type (QmiWdsGetCurrentDataBearerTechnologyOutput *output)
{
    g_return_val_if_fail (output != NULL, QMI_WDS_NETWORK_TYPE_UNKNOWN);

    return (QmiWdsNetworkType)output->current.nw;
}

/**
 * qmi_wds_get_current_data_bearer_technology_output_get_current_rat_3gpp2:
 * @output: a #QmiWdsGetCurrentDataBearerTechnologyOutput.
 *
 * Get the current 3GPP2 specific Radio Access Technology, if the current network type is #QMI_NETWORK_TYPE_3GPP2.
 *
 * Returns: a #QmiWdsRat3gpp2.
 */
QmiWdsRat3gpp2
qmi_wds_get_current_data_bearer_technology_output_get_current_rat_3gpp2 (QmiWdsGetCurrentDataBearerTechnologyOutput *output)
{
    g_return_val_if_fail (output != NULL, QMI_WDS_RAT_3GPP2_NONE);
    g_return_val_if_fail (output->current.nw == QMI_WDS_NETWORK_TYPE_3GPP2, QMI_WDS_RAT_3GPP2_NONE);

    return (QmiWdsRat3gpp2)output->current.rat_mask;
}

/**
 * qmi_wds_get_current_data_bearer_technology_output_get_current_rat_3gpp:
 * @output: a #QmiWdsGetCurrentDataBearerTechnologyOutput.
 *
 * Get the current 3GPP specific Radio Access Technology, if the current network type is #QMI_NETWORK_TYPE_3GPP.
 *
 * Returns: a #QmiWdsRat3gpp.
 */
QmiWdsRat3gpp
qmi_wds_get_current_data_bearer_technology_output_get_current_rat_3gpp (QmiWdsGetCurrentDataBearerTechnologyOutput *output)
{
    g_return_val_if_fail (output != NULL, QMI_WDS_RAT_3GPP_NONE);
    g_return_val_if_fail (output->current.nw == QMI_WDS_NETWORK_TYPE_3GPP, QMI_WDS_RAT_3GPP_NONE);

    return (QmiWdsRat3gpp)output->current.rat_mask;
}

/**
 * qmi_wds_get_current_data_bearer_technology_output_get_current_so_cdma1x:
 * @output: a #QmiWdsGetCurrentDataBearerTechnologyOutput.
 *
 * Get the current Service Option, if the current network type is #QMI_NETWORK_TYPE_3GPP2
 * and the Radio Access Technology mask contains #QMI_WDS_RAT_3GPP2_CDMA1X.
 *
 * Returns: a #QmiWdsSoCdma1x.
 */
QmiWdsSoCdma1x
qmi_wds_get_current_data_bearer_technology_output_get_current_so_cdma1x (QmiWdsGetCurrentDataBearerTechnologyOutput *output)
{
    g_return_val_if_fail (output != NULL, QMI_WDS_SO_CDMA1X_NONE);
    g_return_val_if_fail (output->current.nw == QMI_WDS_NETWORK_TYPE_3GPP2, QMI_WDS_SO_CDMA1X_NONE);
    g_return_val_if_fail (output->current.rat_mask & QMI_WDS_RAT_3GPP2_CDMA1X, QMI_WDS_SO_CDMA1X_NONE);

    return (QmiWdsSoCdma1x)output->current.so_mask;
}

/**
 * qmi_wds_get_current_data_bearer_technology_output_get_current_so_evdo_reva:
 * @output: a #QmiWdsGetCurrentDataBearerTechnologyOutput.
 *
 * Get the current Service Option, if the current network type is #QMI_NETWORK_TYPE_3GPP2
 * and the Radio Access Technology mask contains #QMI_WDS_RAT_3GPP2_EVDO_REVA.
 *
 * Returns: a #QmiWdsSoEvdoRevA.
 */
QmiWdsSoEvdoRevA
qmi_wds_get_current_data_bearer_technology_output_get_current_so_evdo_reva (QmiWdsGetCurrentDataBearerTechnologyOutput *output)
{
    g_return_val_if_fail (output != NULL, QMI_WDS_SO_EVDO_REVA_NONE);
    g_return_val_if_fail (output->current.nw == QMI_WDS_NETWORK_TYPE_3GPP2, QMI_WDS_SO_EVDO_REVA_NONE);
    g_return_val_if_fail (output->current.rat_mask & QMI_WDS_RAT_3GPP2_EVDO_REVA, QMI_WDS_SO_EVDO_REVA_NONE);

    return (QmiWdsSoEvdoRevA)output->current.so_mask;
}

/**
 * qmi_wds_get_current_data_bearer_technology_output_get_last_network_type:
 * @output: a #QmiWdsGetCurrentDataBearerTechnologyOutput.
 *
 * Get the network type of the last connection.
 * This field is optional, and it may appear only when the operation failed with #QMI_PROTOCOL_ERROR_OUT_OF_CALL.
 *
 * Returns: a #QmiWdsNetworkType.
 */
QmiWdsNetworkType
qmi_wds_get_current_data_bearer_technology_output_get_last_network_type (QmiWdsGetCurrentDataBearerTechnologyOutput *output)
{
    g_return_val_if_fail (output != NULL, QMI_WDS_NETWORK_TYPE_UNKNOWN);

    return (QmiWdsConnectionStatus)output->last.nw;
}

/**
 * qmi_wds_get_current_data_bearer_technology_output_get_last_rat_3gpp2:
 * @output: a #QmiWdsGetCurrentDataBearerTechnologyOutput.
 *
 * Get the 3GPP2 specific Radio Access Technology of the last connection, if the last network type is #QMI_NETWORK_TYPE_3GPP2.
 * This field is optional, and it may appear only when the operation failed with #QMI_PROTOCOL_ERROR_OUT_OF_CALL.
 *
 * Returns: a #QmiWdsRat3gpp2.
 */
QmiWdsRat3gpp2
qmi_wds_get_current_data_bearer_technology_output_get_last_rat_3gpp2 (QmiWdsGetCurrentDataBearerTechnologyOutput *output)
{
    g_return_val_if_fail (output != NULL, QMI_WDS_RAT_3GPP2_NONE);
    g_return_val_if_fail (output->last.nw == QMI_WDS_NETWORK_TYPE_3GPP2, QMI_WDS_RAT_3GPP2_NONE);

    return (QmiWdsRat3gpp2)output->last.rat_mask;
}

/**
 * qmi_wds_get_current_data_bearer_technology_output_get_last_rat_3gpp:
 * @output: a #QmiWdsGetCurrentDataBearerTechnologyOutput.
 *
 * Get the 3GPP specific Radio Access Technology of the last connection, if the last network type is #QMI_NETWORK_TYPE_3GPP.
 * This field is optional, and it may appear only when the operation failed with #QMI_PROTOCOL_ERROR_OUT_OF_CALL.
 *
 * Returns: a #QmiWdsRat3gpp.
 */
QmiWdsRat3gpp
qmi_wds_get_current_data_bearer_technology_output_get_last_rat_3gpp (QmiWdsGetCurrentDataBearerTechnologyOutput *output)
{
    g_return_val_if_fail (output != NULL, QMI_WDS_RAT_3GPP2_NONE);
    g_return_val_if_fail (output->last.nw == QMI_WDS_NETWORK_TYPE_3GPP, QMI_WDS_RAT_3GPP_NONE);

    return (QmiWdsRat3gpp)output->last.rat_mask;
}

/**
 * qmi_wds_get_current_data_bearer_technology_output_get_last_so_cdma1x:
 * @output: a #QmiWdsGetCurrentDataBearerTechnologyOutput.
 *
 * Get the Service Option of the last connection, if the last network type is #QMI_NETWORK_TYPE_3GPP2
 * and the Radio Access Technology mask contains #QMI_WDS_RAT_3GPP2_CDMA1X.
 * This field is optional, and it may appear only when the operation failed with #QMI_PROTOCOL_ERROR_OUT_OF_CALL.
 *
 * Returns: a #QmiWdsSoCdma1x.
 */
QmiWdsSoCdma1x
qmi_wds_get_current_data_bearer_technology_output_get_last_so_cdma1x (QmiWdsGetCurrentDataBearerTechnologyOutput *output)
{
    g_return_val_if_fail (output != NULL, QMI_WDS_SO_CDMA1X_NONE);
    g_return_val_if_fail (output->last.nw == QMI_WDS_NETWORK_TYPE_3GPP2, QMI_WDS_SO_CDMA1X_NONE);
    g_return_val_if_fail (output->last.rat_mask & QMI_WDS_RAT_3GPP2_CDMA1X, QMI_WDS_SO_CDMA1X_NONE);

    return (QmiWdsSoCdma1x)output->last.so_mask;
}

/**
 * qmi_wds_get_current_data_bearer_technology_output_get_last_so_evdo_reva:
 * @output: a #QmiWdsGetCurrentDataBearerTechnologyOutput.
 *
 * Get the Service Option of the last connection, if the last network type is #QMI_NETWORK_TYPE_3GPP2
 * and the Radio Access Technology mask contains #QMI_WDS_RAT_3GPP2_EVDO_REVA.
 * This field is optional, and it may appear only when the operation failed with #QMI_PROTOCOL_ERROR_OUT_OF_CALL.
 *
 * Returns: a #QmiWdsSoEvdoRevA.
 */
QmiWdsSoEvdoRevA
qmi_wds_get_current_data_bearer_technology_output_get_last_so_evdo_reva (QmiWdsGetCurrentDataBearerTechnologyOutput *output)
{
    g_return_val_if_fail (output != NULL, QMI_WDS_SO_EVDO_REVA_NONE);
    g_return_val_if_fail (output->last.nw == QMI_WDS_NETWORK_TYPE_3GPP2, QMI_WDS_SO_EVDO_REVA_NONE);
    g_return_val_if_fail (output->last.rat_mask & QMI_WDS_RAT_3GPP2_EVDO_REVA, QMI_WDS_SO_EVDO_REVA_NONE);

    return (QmiWdsSoEvdoRevA)output->last.so_mask;
}

/**
 * qmi_wds_get_data_bearer_technology_output_ref:
 * @output: a #QmiWdsGetCurrentDataBearerTechnologyOutput.
 *
 * Atomically increments the reference count of @output by one.
 *
 * Returns: the new reference to @output.
 */
QmiWdsGetCurrentDataBearerTechnologyOutput *
qmi_wds_get_current_data_bearer_technology_output_ref (QmiWdsGetCurrentDataBearerTechnologyOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}

/**
 * qmi_wds_get_current_data_bearer_technology_output_unref:
 * @output: a #QmiWdsGetCurrentDataBearerTechnologyOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_wds_get_current_data_bearer_technology_output_unref (QmiWdsGetCurrentDataBearerTechnologyOutput *output)
{
    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {
        if (output->error)
            g_error_free (output->error);
        g_slice_free (QmiWdsGetCurrentDataBearerTechnologyOutput, output);
    }
}

QmiWdsGetCurrentDataBearerTechnologyOutput *
qmi_message_wds_get_current_data_bearer_technology_reply_parse (QmiMessage *self,
                                                                GError **error)
{
    QmiWdsGetCurrentDataBearerTechnologyOutput *output;
    GError *inner_error = NULL;

    g_assert (qmi_message_get_message_id (self) == QMI_WDS_MESSAGE_GET_CURRENT_DATA_BEARER_TECHNOLOGY);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    /* success */

    output = g_slice_new0 (QmiWdsGetCurrentDataBearerTechnologyOutput);
    output->ref_count = 1;
    output->error = inner_error;
    output->current.nw = QMI_WDS_NETWORK_TYPE_UNKNOWN;
    output->current.rat_mask = 0;
    output->current.so_mask = 0;
    output->last.nw = QMI_WDS_NETWORK_TYPE_UNKNOWN;
    output->last.rat_mask = 0;
    output->last.so_mask = 0;

    /* Handle QMI protocol errors */
    if (output->error) {
        if (g_error_matches (output->error,
                             QMI_PROTOCOL_ERROR,
                             QMI_PROTOCOL_ERROR_OUT_OF_CALL)) {
            /* last will only appear if we get out-of-call errors */
            qmi_message_tlv_get (self,
                                 GET_CURRENT_DATA_BEARER_TECHNOLOGY_OUTPUT_TLV_LAST,
                                 sizeof (output->last),
                                 &output->last,
                                 NULL);
        }

        return output;
    }

    if (!qmi_message_tlv_get (self,
                              GET_CURRENT_DATA_BEARER_TECHNOLOGY_OUTPUT_TLV_CURRENT,
                              sizeof (output->current),
                              &output->current,
                              error)) {
        g_prefix_error (error, "Couldn't get the current technology TLV: ");
        qmi_wds_get_current_data_bearer_technology_output_unref (output);
        return NULL;
    }

    return output;
}

/*****************************************************************************/
/* Get Dun Call Info */

enum {
	QMI_WDS_TLV_GET_DUN_TX_BYTES_OK		=	0x12,
	QMI_WDS_TLV_GET_DUN_RX_BYTES_OK		=	0x13,
	QMI_WDS_TLV_GET_DUN_CHANNEL_RATE	=	0x16,

};

enum {
	QMI_WDS_TLV_SET_REQUEST_INFO = 0x01
};

struct _QmiWdsBytesOk {
	guint64 tx_bytes_ok;
	guint64 rx_bytes_ok;
}__attribute__((__packed__));

struct _QmiWdsChannelRate {
	guint32 current_channel_tx_rate;
	guint32 current_channel_rx_rate;
	guint32 max_channel_tx_rate;
	guint32 max_channel_rx_rate;
}__attribute__((__packed__));

struct _QmiWdsDunCallOutput {
	volatile gint ref_count;
	GError *error;
	struct _QmiWdsBytesOk bytes_ok;
	struct _QmiWdsChannelRate channel_rate;

};

const guint64
qmi_wds_get_dun_call_output_get_tx_bytes_ok(QmiWdsDunCallOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le64toh(output->bytes_ok.tx_bytes_ok);
}

const guint64
qmi_wds_get_dun_call_output_get_rx_bytes_ok(QmiWdsDunCallOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le64toh(output->bytes_ok.rx_bytes_ok);
}

const guint32
qmi_wds_get_dun_call_output_get_current_channel_rx_rate(QmiWdsDunCallOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le32toh(output->channel_rate.current_channel_rx_rate);
}

const guint32
qmi_wds_get_dun_call_output_get_current_channel_tx_rate(QmiWdsDunCallOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le32toh(output->channel_rate.current_channel_tx_rate);
}

const guint32
qmi_wds_get_dun_call_output_get_max_channel_tx_rate(QmiWdsDunCallOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le32toh(output->channel_rate.max_channel_tx_rate);
}

const guint32
qmi_wds_get_dun_call_output_get_max_channel_rx_rate(QmiWdsDunCallOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le32toh(output->channel_rate.max_channel_rx_rate);
}

QmiMessage*
qmi_message_wds_get_dun_call_new(guint8 transaction_id,
                                   guint8 client_id,
                                   GError **error)
{
    QmiMessage *message;
    guint32 mask = 0;
    mask |= (1 << 2);	/* Tx/ Rx bytes ok */
    mask |= (1 << 5);	/* Channel rate */


    message = qmi_message_new(QMI_SERVICE_WDS,
    							client_id,
    							transaction_id,
    							QMI_WDS_MESSAGE_GET_DUN_CALL_INFO);


	if(!qmi_message_tlv_add(message,
							QMI_WDS_TLV_SET_REQUEST_INFO,
							sizeof(mask),
							&mask,
							error)) {
		g_prefix_error(error, "Failed to add Request Info to message: ");
		qmi_message_unref (message);
		return NULL;
	}



    return message;
}

gboolean
qmi_wds_get_dun_call_output_get_result (QmiWdsDunCallOutput *output,
                                         GError **error)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->error) {
        if (error)
            *error = g_error_copy (output->error);

        return FALSE;
    }

    return TRUE;
}


QmiWdsDunCallOutput *
qmi_wds_get_dun_call_output_ref (QmiWdsDunCallOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}


void
qmi_wds_get_dun_call_output_unref (QmiWdsDunCallOutput *output)
{
    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {
        if (output->error)
            g_error_free (output->error);
        g_slice_free (QmiWdsDunCallOutput, output);
    }
}

QmiWdsDunCallOutput *
qmi_message_wds_get_dun_call_reply_parse (QmiMessage *self,
                                           GError **error)
{
	QmiWdsDunCallOutput *output;
    GError *inner_error = NULL;

    g_assert (qmi_message_get_message_id (self) == QMI_WDS_MESSAGE_GET_DUN_CALL_INFO);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    g_debug("Size of Msg: %d  --> Message: %s\n", qmi_message_get_length(self), qmi_message_get_printable(self, ""));

    output = g_slice_new0(QmiWdsDunCallOutput);
    output->ref_count = 1;
    output->error = inner_error;

    if(!qmi_message_tlv_get(self, QMI_WDS_TLV_GET_DUN_TX_BYTES_OK,
						sizeof(guint64),
						&output->bytes_ok.tx_bytes_ok,
						error)) {
    	g_prefix_error (error, "Couldn't get the TLV Tx bytes ok: ");

    }

    if(!qmi_message_tlv_get(self, QMI_WDS_TLV_GET_DUN_RX_BYTES_OK,
						sizeof(guint64),
						&output->bytes_ok.rx_bytes_ok,
						error)) {
    	g_prefix_error (error, "Couldn't get the TLV Rx bytes ok: ");

    }

    if(!qmi_message_tlv_get(self, QMI_WDS_TLV_GET_DUN_CHANNEL_RATE,
						sizeof(struct _QmiWdsChannelRate),
						&output->channel_rate,
						error)) {
    	g_prefix_error (error, "Couldn't get the TLV channel rate: ");

    }

    return output;
}



/*****************************************************************************/
/* Get Current Channel Rate */

enum {
	QMI_WDS_TLV_GET_CHANNEL_RATE	=	0x01,

};



struct _QmiWdsCurrentChannelRateOutput {
	volatile gint ref_count;
	GError *error;
	struct _QmiWdsChannelRate channel_rate;

};


const guint32
qmi_wds_get_current_channel_rate_output_get_current_channel_rx_rate(QmiWdsCurrentChannelRateOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le32toh(output->channel_rate.current_channel_rx_rate);
}

const guint32
qmi_wds_get_current_channel_rate_output_get_current_channel_tx_rate(QmiWdsCurrentChannelRateOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le32toh(output->channel_rate.current_channel_tx_rate);
}

const guint32
qmi_wds_get_current_channel_rate_output_get_max_channel_tx_rate(QmiWdsCurrentChannelRateOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le32toh(output->channel_rate.max_channel_tx_rate);
}

const guint32
qmi_wds_get_current_channel_rate_output_get_max_channel_rx_rate(QmiWdsCurrentChannelRateOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le32toh(output->channel_rate.max_channel_rx_rate);
}

QmiMessage*
qmi_message_wds_get_current_channel_rate_new(guint8 transaction_id,
                                   guint8 client_id)
{
    QmiMessage *message;


    message = qmi_message_new(QMI_SERVICE_WDS,
    							client_id,
    							transaction_id,
    							QMI_WDS_MESSAGE_GET_CURRENT_CHANNEL_RATE);


    return message;
}

gboolean
qmi_wds_get_current_channel_rate_output_get_result (QmiWdsCurrentChannelRateOutput *output,
                                         GError **error)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->error) {
        if (error)
            *error = g_error_copy (output->error);

        return FALSE;
    }

    return TRUE;
}


QmiWdsCurrentChannelRateOutput*
qmi_wds_get_current_channel_rate_output_ref (QmiWdsCurrentChannelRateOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}


void
qmi_wds_get_current_channel_rate_output_unref (QmiWdsCurrentChannelRateOutput *output)
{
    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {
        if (output->error)
            g_error_free (output->error);
        g_slice_free (QmiWdsCurrentChannelRateOutput, output);
    }
}

QmiWdsCurrentChannelRateOutput*
qmi_message_wds_get_current_channel_rate_reply_parse (QmiMessage *self,
                                           GError **error)
{
	QmiWdsCurrentChannelRateOutput *output;
    GError *inner_error = NULL;


    g_assert (qmi_message_get_message_id (self) == QMI_WDS_MESSAGE_GET_CURRENT_CHANNEL_RATE);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    g_debug("Size of Msg: %d  --> Message: %s\n", qmi_message_get_length(self), qmi_message_get_printable(self, ""));

    output = g_slice_new0(QmiWdsCurrentChannelRateOutput);
    output->ref_count = 1;
    output->error = inner_error;

    if(!qmi_message_tlv_get(self, QMI_WDS_TLV_GET_CHANNEL_RATE,
						sizeof(struct _QmiWdsChannelRate),
						&output->channel_rate,
						error)) {
    	g_prefix_error (error, "Couldn't get the LTE Signal Strength TLV: ");
    	qmi_wds_get_current_channel_rate_output_unref(output);
    	return NULL;

    }


    return output;
}

/*****************************************************************************/
/* Get PKT Statistics */

enum {
	QMI_WDS_TLV_PKT_STATISTICS_MASK	=	0x01
};

enum {
	QMI_WDS_TLV_GET_TX_PKT_OK		=	0x10,
	QMI_WDS_TLV_GET_RX_PKT_OK		=	0x11,
	QMI_WDS_TLV_GET_TX_PKT_ERRORS	=	0x12,
	QMI_WDS_TLV_GET_RX_PKT_ERRORS	=	0x13,
	QMI_WDS_TLV_GET_TX_OVERFLOWS	=	0x14,
	QMI_WDS_TLV_GET_RX_OVERFLOWS	=	0x15,
	QMI_WDS_TLV_GET_TX_BYTES_OK		=	0x19,
	QMI_WDS_TLV_GET_RX_BYTES_OK		=	0x1A

};


struct _PktStatistics {
	guint32	tx_ok_pkt;
	guint32 rx_ok_pkt;
	guint32 tx_err_pkt;
	guint32 rx_err_pkt;
	guint32 tx_ofl_pkt;
	guint32 rx_ofl_pkt;
	guint64 tx_bytes_ok;
	guint64 rx_bytes_ok;

};

struct _QmiWdsPktStatisticsOutput {
	volatile gint ref_count;
	GError *error;
	struct _PktStatistics pkt_stat;

};


const guint32
qmi_wds_get_pkt_stat_output_get_tx_ok_pkt(QmiWdsPktStatisticsOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le32toh(output->pkt_stat.tx_ok_pkt);
}

const guint32
qmi_wds_get_pkt_stat_output_get_rx_ok_pkt(QmiWdsPktStatisticsOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le32toh(output->pkt_stat.rx_ok_pkt);
}

const guint32
qmi_wds_get_pkt_stat_output_get_tx_err_pkt(QmiWdsPktStatisticsOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le32toh(output->pkt_stat.tx_err_pkt);
}

const guint32
qmi_wds_get_pkt_stat_output_get_rx_err_pkt(QmiWdsPktStatisticsOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le32toh(output->pkt_stat.rx_err_pkt);
}

const guint32
qmi_wds_get_pkt_stat_output_get_tx_ofl_pkt(QmiWdsPktStatisticsOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le32toh(output->pkt_stat.tx_ofl_pkt);
}

const guint32
qmi_wds_get_pkt_stat_output_get_rx_ofl_pkt(QmiWdsPktStatisticsOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le32toh(output->pkt_stat.rx_ofl_pkt);
}

const guint64
qmi_wds_get_pkt_stat_output_get_tx_bytes_ok(QmiWdsPktStatisticsOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le64toh(output->pkt_stat.tx_bytes_ok);
}

const guint64
qmi_wds_get_pkt_stat_output_get_rx_bytes_ok(QmiWdsPktStatisticsOutput *output)
{
    g_return_val_if_fail (output != NULL, 0);

    return le64toh(output->pkt_stat.rx_bytes_ok);
}

QmiMessage*
qmi_message_wds_get_pkt_stat_new(guint8 transaction_id,
                                 guint8 client_id,
                                 GError **error)
{
    QmiMessage *message;
    guint32 mask = 0;
    mask |= (1<<0);		/* Tx packets ok */
    mask |= (1<<1);		/* Rx packet ok */
    mask |= (1<<2);		/* Tx packet error */
    mask |= (1<<3);		/* Rx packet error */
    mask |= (1<<4);		/* Tx packet overflows */
    mask |= (1<<5);		/* Rx packet overflows */
    mask |= (1<<6);		/* Tx bytes ok */
    mask |= (1<<7);		/* Rx bytes ok */

    message = qmi_message_new(QMI_SERVICE_WDS,
    							client_id,
    							transaction_id,
    							QMI_WDS_MESSAGE_GET_PKT_STATISTICS);

    if(!qmi_message_tlv_add(message,
    						QMI_WDS_TLV_PKT_STATISTICS_MASK,
							sizeof(mask),
							&mask,
							error)) {

		g_prefix_error(error, "Failed to add Request Info to message: ");
		qmi_message_unref (message);
		return NULL;
	}


    return message;
}

gboolean
qmi_wds_get_pkt_stat_output_get_result(QmiWdsPktStatisticsOutput *output,
                                         GError **error)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->error) {
        if (error)
            *error = g_error_copy (output->error);

        return FALSE;
    }

    return TRUE;
}


QmiWdsPktStatisticsOutput*
qmi_wds_get_pkt_stat_output_ref(QmiWdsPktStatisticsOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}


void
qmi_wds_get_pkt_stat_output_unref(QmiWdsPktStatisticsOutput *output)
{
    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {
        if (output->error)
            g_error_free (output->error);
        g_slice_free (QmiWdsPktStatisticsOutput, output);
    }
}

QmiWdsPktStatisticsOutput*
qmi_message_wds_get_pkt_stat_reply_parse (QmiMessage *self,
                                           GError **error)
{
	QmiWdsPktStatisticsOutput *output;
    GError *inner_error = NULL;


    g_assert (qmi_message_get_message_id (self) == QMI_WDS_MESSAGE_GET_PKT_STATISTICS);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    g_debug("Size of Msg: %d  --> Message: %s\n", qmi_message_get_length(self), qmi_message_get_printable(self, ""));

    output = g_slice_new0(QmiWdsPktStatisticsOutput);
    output->ref_count = 1;
    output->error = inner_error;

    if(!qmi_message_tlv_get(self, QMI_WDS_TLV_GET_TX_PKT_OK,
						sizeof(output->pkt_stat.tx_ok_pkt),
						&output->pkt_stat.tx_ok_pkt,
						error)) {
    	g_prefix_error (error, "Couldn't get the Tx Packet OK TLV: ");

    }

    if(!qmi_message_tlv_get(self, QMI_WDS_TLV_GET_RX_PKT_OK,
    						sizeof(output->pkt_stat.rx_ok_pkt),
    						&output->pkt_stat.rx_ok_pkt,
    						error)) {
		g_prefix_error (error, "Couldn't get the Rx Packet OK TLV: ");

	}

    if(!qmi_message_tlv_get(self, QMI_WDS_TLV_GET_TX_PKT_ERRORS,
        						sizeof(output->pkt_stat.tx_err_pkt),
        						&output->pkt_stat.tx_err_pkt,
        						error)) {
		g_prefix_error (error, "Couldn't get the Tx Packet Error TLV: ");

	}

    if(!qmi_message_tlv_get(self, QMI_WDS_TLV_GET_RX_PKT_ERRORS,
            						sizeof(output->pkt_stat.rx_err_pkt),
            						&output->pkt_stat.rx_err_pkt,
            						error)) {
		g_prefix_error (error, "Couldn't get the Rx Packet Error TLV: ");

	}

    if(!qmi_message_tlv_get(self, QMI_WDS_TLV_GET_TX_OVERFLOWS,
               						sizeof(output->pkt_stat.tx_ofl_pkt),
               						&output->pkt_stat.tx_ofl_pkt,
               						error)) {
		g_prefix_error (error, "Couldn't get the Tx Packet Overflows TLV: ");

	}

    if(!qmi_message_tlv_get(self, QMI_WDS_TLV_GET_RX_OVERFLOWS,
                   						sizeof(output->pkt_stat.rx_ofl_pkt),
                   						&output->pkt_stat.rx_ofl_pkt,
                   						error)) {
		g_prefix_error (error, "Couldn't get the Rx Packet Overflows TLV: ");

	}

    if(!qmi_message_tlv_get(self, QMI_WDS_TLV_GET_TX_BYTES_OK,
                       						sizeof(output->pkt_stat.tx_bytes_ok),
                       						&output->pkt_stat.tx_bytes_ok,
                       						error)) {
		g_prefix_error (error, "Couldn't get the Tx Bytes OK TLV: ");

	}

    if(!qmi_message_tlv_get(self, QMI_WDS_TLV_GET_RX_BYTES_OK,
                           						sizeof(output->pkt_stat.rx_bytes_ok),
                           						&output->pkt_stat.rx_bytes_ok,
                           						error)) {
		g_prefix_error (error, "Couldn't get the Rx Bytes OK TLV: ");

	}

    return output;
}

