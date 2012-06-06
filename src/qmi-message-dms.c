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

#include "qmi-message-dms.h"
#include "qmi-enums.h"
#include "qmi-error-types.h"

/*****************************************************************************/
/* Get IDs */

enum {
  QMI_DMS_TLV_GET_IDS_ESN = 0x10,
  QMI_DMS_TLV_GET_IDS_IMEI,
  QMI_DMS_TLV_GET_IDS_MEID
};

/**
 * QmiDmsGetIdsOutput:
 *
 * An opaque type handling the output of the Get IDs operation.
 */
struct _QmiDmsGetIdsOutput {
    volatile gint ref_count;
    GError *error;
    gchar *esn;
    gchar *imei;
    gchar *meid;
};

/**
 * qmi_dms_get_ids_output_get_result:
 * @output: a #QmiDmsGetIdsOutput.
 * @error: a #GError.
 *
 * Get the result of the Get IDs operation.
 *
 * Returns: #TRUE if the operation succeeded, and #FALSE if @error is set.
 */
gboolean
qmi_dms_get_ids_output_get_result (QmiDmsGetIdsOutput *output,
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
 * qmi_dms_get_ids_output_get_esn:
 * @output: a #QmiDmsGetIdsOutput.
 *
 * Get the ESN.
 *
 * Returns: the ESN.
 */
const gchar *
qmi_dms_get_ids_output_get_esn (QmiDmsGetIdsOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    return output->esn;
}

/**
 * qmi_dms_get_ids_output_get_imei:
 * @output: a #QmiDmsGetIdsOutput.
 *
 * Get the IMEI.
 *
 * Returns: the IMEI.
 */
const gchar *
qmi_dms_get_ids_output_get_imei (QmiDmsGetIdsOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    return output->imei;
}

/**
 * qmi_dms_get_ids_output_get_meid:
 * @output: a #QmiDmsGetIdsOutput.
 *
 * Get the MEID.
 *
 * Returns: the MEID.
 */
const gchar *
qmi_dms_get_ids_output_get_meid (QmiDmsGetIdsOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    return output->meid;
}

/**
 * qmi_dms_get_ids_output_ref:
 * @output: a #QmiDmsGetIdsOutput.
 *
 * Atomically increments the reference count of @output by one.
 *
 * Returns: the new reference to @output.
 */
QmiDmsGetIdsOutput *
qmi_dms_get_ids_output_ref (QmiDmsGetIdsOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}

/**
 * qmi_dms_get_ids_output_unref:
 * @output: a #QmiDmsGetIdsOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_dms_get_ids_output_unref (QmiDmsGetIdsOutput *output)
{
    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {
        g_free (output->esn);
        g_free (output->imei);
        g_free (output->meid);
        if (output->error)
            g_error_free (output->error);
        g_slice_free (QmiDmsGetIdsOutput, output);
    }
}

QmiMessage *
qmi_message_dms_get_ids_new (guint8 transaction_id,
                             guint8 client_id)
{
    return qmi_message_new (QMI_SERVICE_DMS,
                            client_id,
                            transaction_id,
                            QMI_DMS_MESSAGE_GET_IDS);
}

QmiDmsGetIdsOutput *
qmi_message_dms_get_ids_reply_parse (QmiMessage *self,
                                     GError **error)
{
    QmiDmsGetIdsOutput *output;
    GError *inner_error = NULL;

    g_assert (qmi_message_get_message_id (self) == QMI_DMS_MESSAGE_GET_IDS);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    output = g_slice_new0 (QmiDmsGetIdsOutput);
    output->ref_count = 1;
    output->error = inner_error;

    /* Note: all ESN/IMEI/MEID are OPTIONAL; so it's ok if none of them appear */
    output->esn = qmi_message_tlv_get_string (self,
                                              QMI_DMS_TLV_GET_IDS_ESN,
                                              NULL);
    output->imei = qmi_message_tlv_get_string (self,
                                               QMI_DMS_TLV_GET_IDS_IMEI,
                                               NULL);
    output->meid = qmi_message_tlv_get_string (self,
                                               QMI_DMS_TLV_GET_IDS_MEID,
                                               NULL);

    return output;
}


/*****************************************************************************/
/* Get Operation Mode Version */

enum {
  QMI_DMS_TLV_GET_OP_MODE = 0x01
};

typedef enum {
	ONLINE	=	0x00,
	LOW_POWER,
	FACTORY_TEST_MODE,
	OFFLINE,
	RESETTING,
	SHUTTING_DOWN,
	PERSISTENT_LOW_POWER
}OperatingMode;

/**
 * QmiDmsGetOpModeOutput:
 *
 * An opaque type handling the output of the Get Op Mode operation.
 */
struct _QmiDmsGetOpModeOutput {
    volatile gint ref_count;
    GError *error;
    gint8 opMode;
    gchar *opModeString;
};


gboolean
qmi_dms_get_op_mode_output_get_result(QmiDmsGetOpModeOutput *output,
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


const gint8
qmi_dms_get_op_mode_output_get_version(QmiDmsGetOpModeOutput *output)
{
    g_return_val_if_fail (output != NULL, -1);

    return output->opMode;
}

const gchar*
qmi_dms_get_op_mode_output_get_version_string(QmiDmsGetOpModeOutput *output)
{

    g_return_val_if_fail (output != NULL, NULL);


    switch(output->opMode) {
    case ONLINE:
    	output->opModeString = g_strdup("Online");
    	break;
    case LOW_POWER:
    	output->opModeString = g_strdup("Low power");
    	break;
    case FACTORY_TEST_MODE:
    	output->opModeString = g_strdup("Factory Test mode");
    	break;
    case OFFLINE:
    	output->opModeString = g_strdup("Offline");
    	break;
    case RESETTING:
    	output->opModeString = g_strdup("Resetting");
    	break;
    case SHUTTING_DOWN:
    	output->opModeString = g_strdup("Shutting down");
    	break;
    case PERSISTENT_LOW_POWER:
    	output->opModeString = g_strdup("Persistent low power");
    	break;
    default:
    	output->opModeString = g_strdup("unknown");
    	break;
    }

    g_return_val_if_fail (output->opModeString != NULL, "OP_MODE allocation error");

    return output->opModeString;

}



QmiDmsGetOpModeOutput*
qmi_dms_get_op_mode_output_ref(QmiDmsGetOpModeOutput *output)
{
    g_return_val_if_fail(output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}

/**
 * qmi_dms_get_op_mode_output_unref:
 * @output: a #QmiDmsGetOpModeOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_dms_get_op_mode_output_unref(QmiDmsGetOpModeOutput *output)
{
    g_return_if_fail(output != NULL);

    if(g_atomic_int_dec_and_test(&output->ref_count)) {
    	g_free(output->opModeString);

        if(output->error)
            g_error_free(output->error);
        g_slice_free(QmiDmsGetOpModeOutput, output);
    }
}

QmiMessage*
qmi_message_dms_get_op_mode_new(guint8 transaction_id,
                             	   guint8 client_id)
{
    return qmi_message_new(QMI_SERVICE_DMS,
                            client_id,
                            transaction_id,
                            QMI_DMS_MESSAGE_GET_OPERATING_MODE);
}

QmiDmsGetOpModeOutput*
qmi_message_dms_get_op_mode_reply_parse(QmiMessage *self,
                                     	   GError **error)
{
	QmiDmsGetOpModeOutput *output;
    GError *inner_error = NULL;

    g_assert(qmi_message_get_message_id(self) == QMI_DMS_MESSAGE_GET_OPERATING_MODE);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    output = g_slice_new0 (QmiDmsGetOpModeOutput);
    output->ref_count = 1;
    output->error = inner_error;
    output->opMode = -1;
    output->opModeString = NULL;

    if(!qmi_message_tlv_get(self,
    						QMI_DMS_TLV_GET_OP_MODE,
    						sizeof(output->opMode),
    						&output->opMode,
    						error)) {

    		g_prefix_error (error, "Couldn't get the Operation Mode TLV: ");
    		qmi_dms_get_op_mode_output_unref(output);
    		return NULL;
    	}



    return output;
}


/*****************************************************************************/
/* Get Device Properties
 *
 * + Device Manufacturer
 * + Device Model ID
 * + Device Revision ID
 * + Device Hardware Revision
 */


/*****************************************************************************/
/* Get Device Manufacturer */

enum {
  QMI_DMS_TLV_GET_DEV_MFR 		= 	0x01

};

/**
 * QmiDmsGetOpModeOutput:
 *
 * An opaque type handling the output of the Get Op Mode operation.
 */
struct _QmiDmsGetDevMfrOutput {
    volatile gint ref_count;
    GError 	*error;
    gchar	*device_mfr;

};


gboolean
qmi_dms_get_dev_mfr_output_get_result(QmiDmsGetDevMfrOutput *output,
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


const gchar*
qmi_dms_get_dev_mfr_output_get_dev_mfr(QmiDmsGetDevMfrOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    return output->device_mfr;
}


QmiDmsGetDevMfrOutput*
qmi_dms_get_dev_mfr_output_ref(QmiDmsGetDevMfrOutput *output)
{
    g_return_val_if_fail(output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}

/**
 * qmi_dms_get_dev_mfr_output_unref:
 * @output: a #QmiDmsGetDevMfrOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_dms_get_dev_mfr_output_unref(QmiDmsGetDevMfrOutput *output)
{
    g_return_if_fail(output != NULL);

    if(g_atomic_int_dec_and_test(&output->ref_count)) {
    	g_free(output->device_mfr);

        if(output->error)
            g_error_free(output->error);
        g_slice_free(QmiDmsGetDevMfrOutput, output);
    }
}

QmiMessage*
qmi_message_dms_get_dev_mfr_new(guint8 transaction_id,
                             	   guint8 client_id)
{
    return qmi_message_new(QMI_SERVICE_DMS,
                            client_id,
                            transaction_id,
                            QMI_DMS_MESSAGE_GET_DEVICE_MFR);
}

QmiDmsGetDevMfrOutput*
qmi_message_dms_get_dev_mfr_reply_parse(QmiMessage *self,
                                     	   GError **error)
{
	QmiDmsGetDevMfrOutput *output;
    GError *inner_error = NULL;
    gchar *string = NULL;


    g_assert(qmi_message_get_message_id(self) == QMI_DMS_MESSAGE_GET_DEVICE_MFR);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    output = g_slice_new0 (QmiDmsGetDevMfrOutput);
    output->ref_count = 1;
    output->error = inner_error;
    string = qmi_message_tlv_get_string(self, QMI_DMS_TLV_GET_DEV_MFR, error);

    if(!string) {

    		g_prefix_error (error, "Couldn't get the Device Manufacturer TLV: ");

    	}

    output->device_mfr = (string ? g_strdup (string) : "unknown");

    return output;
}

/*****************************************************************************/
/* Get Device Model ID */

enum {
  QMI_DMS_TLV_GET_DEV_MODEL_ID	=	0x01

};

/**
 * QmiDmsGetDevModelIdOutput:
 *
 * An opaque type handling the output of the Get Op Mode operation.
 */
struct _QmiDmsGetDevModelIdOutput {
    volatile gint ref_count;
    GError 	*error;
   	gchar 	*device_model_id;

};


gboolean
qmi_dms_get_dev_model_id_output_get_result(QmiDmsGetDevModelIdOutput *output,
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


const gchar*
qmi_dms_get_dev_model_id_output_get_dev_model_id(QmiDmsGetDevModelIdOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    return output->device_model_id;
}


QmiDmsGetDevModelIdOutput*
qmi_dms_get_dev_model_id_output_ref(QmiDmsGetDevModelIdOutput *output)
{
    g_return_val_if_fail(output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}

/**
 * qmi_dms_get_dev_mfr_output_unref:
 * @output: a #QmiDmsGetDevModelIdOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_dms_get_dev_model_id_output_unref(QmiDmsGetDevModelIdOutput *output)
{
    g_return_if_fail(output != NULL);

    if(g_atomic_int_dec_and_test(&output->ref_count)) {
    	g_free(output->device_model_id);

        if(output->error)
            g_error_free(output->error);
        g_slice_free(QmiDmsGetDevModelIdOutput, output);
    }
}

QmiMessage*
qmi_message_dms_get_dev_model_id_new(guint8 transaction_id,
                             	   guint8 client_id)
{
    return qmi_message_new(QMI_SERVICE_DMS,
                            client_id,
                            transaction_id,
                            QMI_DMS_MESSAGE_GET_DEVICE_MODEL_ID);
}

QmiDmsGetDevModelIdOutput*
qmi_message_dms_get_dev_model_id_reply_parse(QmiMessage *self,
                                     	   GError **error)
{
	QmiDmsGetDevModelIdOutput *output;
    GError *inner_error = NULL;
    gchar *string = NULL;


    g_assert(qmi_message_get_message_id(self) == QMI_DMS_MESSAGE_GET_DEVICE_MODEL_ID);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    output = g_slice_new0 (QmiDmsGetDevModelIdOutput);
    output->ref_count = 1;
    output->error = inner_error;
    string = qmi_message_tlv_get_string(self, QMI_DMS_TLV_GET_DEV_MODEL_ID, error);

    if(!string) {

    		g_prefix_error (error, "Couldn't get the Device Model ID TLV: ");

    	}

    output->device_model_id = (string ? g_strdup (string) : "unknown");

    return output;
}

/*****************************************************************************/
/* Get Device Revision ID */

enum {
  QMI_DMS_TLV_GET_DEV_REVISION_ID	=	0x01

};

/**
 * QmiDmsGetDevRevIdOutput:
 *
 * An opaque type handling the output of the Get Op Mode operation.
 */
struct _QmiDmsGetDevRevIdOutput {
	volatile gint ref_count;
    GError 	*error;
   	gchar 	*device_revision_id;

};


gboolean
qmi_dms_get_dev_rev_id_output_get_result(QmiDmsGetDevRevIdOutput *output,
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


const gchar*
qmi_dms_get_dev_rev_id_output_get_dev_rev_id(QmiDmsGetDevRevIdOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    return output->device_revision_id;
}


QmiDmsGetDevRevIdOutput*
qmi_dms_get_dev_rev_id_output_ref(QmiDmsGetDevRevIdOutput *output)
{
    g_return_val_if_fail(output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}

/**
 * qmi_dms_get_dev_rev_id_output_unref:
 * @output: a #QmiDmsGetDevRevIdOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_dms_get_dev_rev_id_output_unref(QmiDmsGetDevRevIdOutput *output)
{
    g_return_if_fail(output != NULL);

    if(g_atomic_int_dec_and_test(&output->ref_count)) {
    	g_free(output->device_revision_id);

        if(output->error)
            g_error_free(output->error);
        g_slice_free(QmiDmsGetDevRevIdOutput, output);
    }
}

QmiMessage*
qmi_message_dms_get_dev_rev_id_new(guint8 transaction_id,
                             	   guint8 client_id)
{
    return qmi_message_new(QMI_SERVICE_DMS,
                            client_id,
                            transaction_id,
                            QMI_DMS_MESSAGE_GET_DEVICE_REV_ID);
}

QmiDmsGetDevRevIdOutput*
qmi_message_dms_get_dev_rev_id_reply_parse(QmiMessage *self,
                                     	   GError **error)
{
	QmiDmsGetDevRevIdOutput *output;
    GError *inner_error = NULL;
    gchar *string = NULL;


    g_assert(qmi_message_get_message_id(self) == QMI_DMS_MESSAGE_GET_DEVICE_REV_ID);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    output = g_slice_new0 (QmiDmsGetDevRevIdOutput);
    output->ref_count = 1;
    output->error = inner_error;
    string = qmi_message_tlv_get_string(self, QMI_DMS_TLV_GET_DEV_REVISION_ID, error);

    if(!string) {

    		g_prefix_error (error, "Couldn't get the Device Revision ID TLV: ");

    	}

    output->device_revision_id = (string ? g_strdup (string) : "unknown");

    return output;
}

/*****************************************************************************/
/* Get Device Hardware Revision ID */

enum {
  QMI_DMS_TLV_GET_DEV_HARDWARE_REVISION_ID	=	0x01

};

/**
 * QmiDmsGetDevHardwareRevIdOutput:
 *
 * An opaque type handling the output of the Get Op Mode operation.
 */
struct _QmiDmsGetDevHardwareRevIdOutput {
    volatile gint ref_count;
    GError 	*error;
   	gchar 	*hardware_rev;

};


gboolean
qmi_dms_get_hardware_rev_id_output_get_result(QmiDmsGetDevHardwareRevIdOutput *output,
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


const gchar*
qmi_dms_get_hardware_rev_output_get_hardware_rev(QmiDmsGetDevHardwareRevIdOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    return output->hardware_rev;
}


QmiDmsGetDevHardwareRevIdOutput*
qmi_dms_get_hardware_rev_id_output_ref(QmiDmsGetDevHardwareRevIdOutput *output)
{
    g_return_val_if_fail(output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}

/**
 * qmi_dms_get_dev_rev_id_output_unref:
 * @output: a #QmiDmsGetDevHardwareRevIdOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_dms_get_hardware_rev_id_output_unref(QmiDmsGetDevHardwareRevIdOutput *output)
{
    g_return_if_fail(output != NULL);

    if(g_atomic_int_dec_and_test(&output->ref_count)) {
    	g_free(output->hardware_rev);

        if(output->error)
            g_error_free(output->error);
        g_slice_free(QmiDmsGetDevHardwareRevIdOutput, output);
    }
}

QmiMessage*
qmi_message_dms_get_hardware_rev_id_new(guint8 transaction_id,
                             	   guint8 client_id)
{
    return qmi_message_new(QMI_SERVICE_DMS,
                            client_id,
                            transaction_id,
                            QMI_DMS_MESSAGE_GET_DEVICE_HARDWARE_REV);
}

QmiDmsGetDevHardwareRevIdOutput*
qmi_message_dms_get_hardware_rev_id_reply_parse(QmiMessage *self,
                                     	   GError **error)
{
	QmiDmsGetDevHardwareRevIdOutput *output;
    GError *inner_error = NULL;
    gchar *string = NULL;


    g_assert(qmi_message_get_message_id(self) == QMI_DMS_MESSAGE_GET_DEVICE_HARDWARE_REV);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    output = g_slice_new0 (QmiDmsGetDevHardwareRevIdOutput);
    output->ref_count = 1;
    output->error = inner_error;
    string = qmi_message_tlv_get_string(self, QMI_DMS_TLV_GET_DEV_HARDWARE_REVISION_ID, error);

    if(!string) {

    		g_prefix_error (error, "Couldn't get the Device Revision ID TLV: ");

    	}

    output->hardware_rev = (string ? g_strdup (string) : "unknown");

    return output;
}

/*****************************************************************************/
/* Get MSISDN */

enum {
  QMI_DMS_TLV_GET_VOICE_NUMBER	=	0x01,
  QMI_DMS_TLV_GET_MOBILE_ID		=	0x10,
  QMI_DMS_TLV_GET_IMSI			=	0x11

};

/**
 * QmiDmsGetMsisdnOutput:
 *
 * An opaque type handling the output of the Get Op Mode operation.
 */
struct _QmiDmsGetMsisdnOutput {
    volatile gint ref_count;
    GError 	*error;
   	gchar 	*voice_number;
   	gchar 	*mobile_id_number;
   	gchar 	*imsi;

};


gboolean
qmi_dms_get_msisdn_output_get_result(QmiDmsGetMsisdnOutput *output,
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


const gchar*
qmi_dms_get_msisdn_output_get_voice_number(QmiDmsGetMsisdnOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    return output->voice_number;
}

const gchar*
qmi_dms_get_msisdn_output_get_mobile_id(QmiDmsGetMsisdnOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    return output->mobile_id_number;
}

const gchar*
qmi_dms_get_msisdn_output_get_imsi(QmiDmsGetMsisdnOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    return output->imsi;
}

QmiDmsGetMsisdnOutput*
qmi_dms_get_msisdn_output_ref(QmiDmsGetMsisdnOutput *output)
{
    g_return_val_if_fail(output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}

/**
 * qmi_dms_get_voice_number_output_unref:
 * @output: a #QmiDmsGetVoiceNumberOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_dms_get_msisdn_output_unref(QmiDmsGetMsisdnOutput *output)
{
    g_return_if_fail(output != NULL);

    if(g_atomic_int_dec_and_test(&output->ref_count)) {
    	g_free(output->voice_number);
    	g_free(output->imsi);
    	g_free(output->mobile_id_number);
        if(output->error)
            g_error_free(output->error);
        g_slice_free(QmiDmsGetMsisdnOutput, output);
    }
}

QmiMessage*
qmi_message_dms_get_msisdn_new(guint8 transaction_id,
                             	   guint8 client_id)
{
    return qmi_message_new(QMI_SERVICE_DMS,
                            client_id,
                            transaction_id,
                            QMI_DMS_MESSAGE_GET_DEVICE_MSISDN);
}

QmiDmsGetMsisdnOutput*
qmi_message_dms_get_msisdn_reply_parse(QmiMessage *self,
                                     	   GError **error)
{
	QmiDmsGetMsisdnOutput *output;
    GError *inner_error = NULL;



    g_assert(qmi_message_get_message_id(self) == QMI_DMS_MESSAGE_GET_DEVICE_MSISDN);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    output = g_slice_new0 (QmiDmsGetMsisdnOutput);
    output->ref_count = 1;
    output->error = inner_error;

    output->voice_number = qmi_message_tlv_get_string(self, QMI_DMS_TLV_GET_VOICE_NUMBER, NULL);
    output->mobile_id_number = qmi_message_tlv_get_string(self, QMI_DMS_TLV_GET_MOBILE_ID, NULL);
    output->imsi = qmi_message_tlv_get_string(self, QMI_DMS_TLV_GET_IMSI, NULL);


    return output;
}

/*****************************************************************************/
/* Get SW Version */

enum {
  QMI_DMS_TLV_GET_SW_VERSION	=	0x01

};

/**
 * QmiDmsGetSwVersion:
 *
 * An opaque type handling the output of the Get Op Mode operation.
 */
struct _QmiDmsGetSwVersionOutput {
    volatile gint ref_count;
    GError 	*error;
   	gchar 	*sw_version;

};


gboolean
qmi_dms_get_sw_version_output_get_result(QmiDmsGetSwVersionOutput *output,
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


const gchar*
qmi_dms_get_sw_version_output_get_sw_version(QmiDmsGetSwVersionOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    return output->sw_version;
}


QmiDmsGetSwVersionOutput*
qmi_dms_get_sw_version_output_ref(QmiDmsGetSwVersionOutput *output)
{
    g_return_val_if_fail(output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}

/**
 * qmi_dms_get_voice_number_output_unref:
 * @output: a #QmiDmsGetSwVersion.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_dms_get_sw_version_output_unref(QmiDmsGetSwVersionOutput *output)
{
    g_return_if_fail(output != NULL);

    if(g_atomic_int_dec_and_test(&output->ref_count)) {
    	g_free(output->sw_version);

        if(output->error)
            g_error_free(output->error);
        g_slice_free(QmiDmsGetSwVersionOutput, output);
    }
}

QmiMessage*
qmi_message_dms_get_sw_version_new(guint8 transaction_id,
                             	   guint8 client_id)
{
    return qmi_message_new(QMI_SERVICE_DMS,
                            client_id,
                            transaction_id,
                            QMI_DMS_MESSAGE_GET_SW_VERSION);
}

QmiDmsGetSwVersionOutput*
qmi_message_dms_get_sw_version_reply_parse(QmiMessage *self,
                                     	   GError **error)
{
	QmiDmsGetSwVersionOutput *output;
    GError *inner_error = NULL;



    g_assert(qmi_message_get_message_id(self) == QMI_DMS_MESSAGE_GET_SW_VERSION);

    if (!qmi_message_get_response_result (self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    output = g_slice_new0 (QmiDmsGetSwVersionOutput);
    output->ref_count = 1;
    output->error = inner_error;

    output->sw_version = qmi_message_tlv_get_string(self, QMI_DMS_TLV_GET_SW_VERSION, NULL);

    return output;
}
