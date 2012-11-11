/*
 * qmi-message-nas.c
 *
 *  Created on: 23.05.2012
 *      Author: roland
 *
 * NOTE: this is a private non-installable header
 *
 */

#include "qmi-message-nas.h"
#include "qmi-enums.h"
#include "qmi-error-types.h"
#include "qmi-wds.h"			/* enums of the bearer technology */
#include "qmi-utils.h"
#include "qmi-band-types.h"
#include "qmi-mobile-network-code.h"
#include <string.h>


/*****************************************************************************/
/* Get Sig Info */

/* Type for TLV´s */
enum {
  QMI_NAS_TLV_GET_SIGN_INFO_CDMA	=	0x10,
  QMI_NAS_TLV_GET_SIG_INFO_HDR		=	0x11,
  QMI_NAS_TLV_GET_SIG_INFO_GSM		=	0x12,
  QMI_NAS_TLV_GET_SIG_INFO_WCDMA	=	0x13,
  QMI_NAS_TLV_GET_SIG_INFO_LTE		=	0x14,
  QMI_NAS_TLV_GET_SIG_INFO_TDSCDMA	=	0x15,
};

/**
 * QmiNasGetSigInfoOutput:
 *
 * An opaque type handling the output of the Get SigInfo operation.
 */

struct _LteSignalStrength {
	gint8 rssi;
	gint8 rsrq;
	gint16 rsrp;
	gint16 snr;

}__attribute__((__packed__));

struct _QmiNasGetSigInfoOutput {
    volatile gint ref_count;
    GError *error;
    struct _LteSignalStrength lteSignal;

};



/**
 * qmi_nas_get_sig_info_output_get_result:
 *
 * @output: a #QmiNasGetSigInfoOutput.
 * @error: a #GError.
 *
 * Get the result of the Get SigInfo operation.
 *
 * Returns: #TRUE if the operation succeeded, and #FALSE if @error is set.
 */
gboolean
qmi_nas_get_sig_info_output_get_result(QmiNasGetSigInfoOutput *output, GError **error)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->error)
    {
        if (error)
            *error = g_error_copy (output->error);

        return FALSE;
    }

    return TRUE;
}

/**
 * qmi_nas_get_sig_info_output_get_rssi:
 * @output: a #QmiNasGetSigInfoOutput.
 *
 * Get the RSSI.
 *
 * Returns: the RSSI.
 */
const gint8
qmi_nas_get_sig_info_output_get_rssi(QmiNasGetSigInfoOutput *output)
{

    g_return_val_if_fail (output != NULL, 0);

    return output->lteSignal.rssi;

}

/**
 * qmi_nas_get_sig_info_output_get_rsrq:
 * @output: a #QmiNasGetSigInfoOutput.
 *
 * Get the RSSI.
 *
 * Returns: the RSSI.
 */
const gint8
qmi_nas_get_sig_info_output_get_rsrq(QmiNasGetSigInfoOutput *output)
{

    g_return_val_if_fail (output != NULL, 0);


    return output->lteSignal.rsrq;

}

/**
 * qmi_nas_get_sig_info_output_get_rsrp:
 * @output: a #QmiNasGetSigInfoOutput.
 *
 * Get the RSSI.
 *
 * Returns: the RSSI.
 */
const gint16
qmi_nas_get_sig_info_output_get_rsrp(QmiNasGetSigInfoOutput *output)
{

    g_return_val_if_fail (output != NULL, 0);

    return le16toh(output->lteSignal.rsrp);

}

/**
 * qmi_nas_get_sig_info_output_get_rsrp:
 * @output: a #QmiNasGetSigInfoOutput.
 *
 * Get the RSSI.
 *
 * Returns: the RSSI.
 */
const gint16
qmi_nas_get_sig_info_output_get_snr(QmiNasGetSigInfoOutput *output)
{

    g_return_val_if_fail (output != NULL, 0);


    return le16toh(output->lteSignal.snr);

}


/**
 * qmi_message_nas_get_sig_info_output_ref:
 * @output: a #QmiNasGetSigInfoOutput.
 *
 * Atomically increments the reference count of @output by one.
 *
 * Returns: the new reference to @output.
 */
QmiNasGetSigInfoOutput*
qmi_nas_get_sig_info_output_ref(QmiNasGetSigInfoOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}

/**
 * qmi_message_nas_get_sig_info_output_unref:
 * @output: a #QmiNasGetSigInfoOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_nas_get_sig_info_output_unref(QmiNasGetSigInfoOutput *output)
{
    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {

        if (output->error)
            g_error_free (output->error);
        g_slice_free (QmiNasGetSigInfoOutput, output);
    }
}

QmiMessage*
qmi_message_nas_get_sig_info_new(guint8 transaction_id, guint8 client_id)
{
    return qmi_message_new (QMI_SERVICE_NAS,
                            client_id,
                            transaction_id,
                            QMI_NAS_MESSAGE_GET_SIG_INFO);
}

QmiNasGetSigInfoOutput*
qmi_message_nas_get_sig_info_reply_parse(QmiMessage *self, GError **error)
{
	QmiNasGetSigInfoOutput *output;
    GError *inner_error = NULL;

    g_assert(qmi_message_get_message_id(self) == QMI_NAS_MESSAGE_GET_SIG_INFO);

    if (!qmi_message_get_response_result(self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    output = g_slice_new0 (QmiNasGetSigInfoOutput);
    output->ref_count = 1;
    output->error = inner_error;

    if(!qmi_message_tlv_get(self,
    						QMI_NAS_TLV_GET_SIG_INFO_LTE,
							sizeof(output->lteSignal),
							&output->lteSignal,
							error)) {
		g_prefix_error (error, "Couldn't get the LTE Signal Strength TLV: ");
		qmi_nas_get_sig_info_output_unref(output);
		return NULL;
	}

    return output;
}

/*****************************************************************************/


/*****************************************************************************/
/* Get Cell Location Info */

/* Type for TLV´s */
enum {
  QMI_NAS_TLV_GET_LTE_INFO_INTRA		=	0x13
};

/**
 * QmiNasGetSigInfoOutput:
 *
 * An opaque type handling the output of the Get SigInfo operation.
 */

struct _LteSignal {
	guint16		pci;
	gint16		rsrq;
	gint16		rsrp;
	gint16		rssi;
	gint16		srxlev;
}__attribute__((__packed__));

struct _LteInfoIntra {
	gint8 			ue_in_idle;
	gchar	 		plmn[3];
	guint16			tac;
	guint32			global_cell_id;
	guint16			earfcn;
	guint16			serving_cell_id;
	guint8			cell_resel_priority;
	guint8			s_non_intra_search;
	guint8			thresh_serving_low;
	guint8			s_intra_search;
	guint8			cells_len;

}__attribute__((__packed__));

struct _QmiNasGetCellLocInfoOutput {
    volatile gint ref_count;
    GError *error;
    struct _LteInfoIntra lteInfo;
    struct _LteSignal **lteSignal;
};



/**
 * qmi_nas_get_cell_loc_info_output_get_result:
 *
 * @output: a #QmiNasGetCellLocInfoOutput.
 * @error: a #GError.
 *
 * Get the result of the Get Cell Location Info operation.
 *
 * Returns: #TRUE if the operation succeeded, and #FALSE if @error is set.
 */
gboolean
qmi_nas_get_cell_loc_info_output_get_result(QmiNasGetCellLocInfoOutput *output, GError **error)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->error)
    {
        if (error)
            *error = g_error_copy (output->error);

        return FALSE;
    }

    return TRUE;
}

/**
 * qmi_nas_get_cell_loc_info_output_get_rssi:
 * @output: a #QmiNasGetCellLocInfoOutput.
 *
 * Get the RSSI.
 *
 * Returns: the RSSI.
 */


const gint8
qmi_nas_get_cell_loc_info_output_get_ue_in_idle(QmiNasGetCellLocInfoOutput *output)
{

    g_return_val_if_fail (output != NULL, -1);

    return output->lteInfo.ue_in_idle;

}

const gchar*
qmi_nas_get_cell_loc_info_output_get_plmn(QmiNasGetCellLocInfoOutput *output)
{

    g_return_val_if_fail (output != NULL, 0);

    return qmi_utils_str_hex(output->lteInfo.plmn, 3, ':');

}

const guint16
qmi_nas_get_cell_loc_info_output_get_tac(QmiNasGetCellLocInfoOutput *output)
{

    g_return_val_if_fail (output != NULL, 0);

    return le16toh(output->lteInfo.tac);

}

const guint32
qmi_nas_get_cell_loc_info_output_get_global_cell_id(QmiNasGetCellLocInfoOutput *output)
{

    g_return_val_if_fail (output != NULL, 0);

    return le32toh(output->lteInfo.global_cell_id);

}

const guint16
qmi_nas_get_cell_loc_info_output_get_earfcn(QmiNasGetCellLocInfoOutput *output)
{

    g_return_val_if_fail (output != NULL, 0);

    return le16toh(output->lteInfo.earfcn);

}

const guint16
qmi_nas_get_cell_loc_info_output_get_serving_cell_id(QmiNasGetCellLocInfoOutput *output)
{

    g_return_val_if_fail (output != NULL, -1);

    return le16toh(output->lteInfo.serving_cell_id);

}

const guint8
qmi_nas_get_cell_loc_info_output_get_cell_resel_priority(QmiNasGetCellLocInfoOutput *output)
{

    g_return_val_if_fail (output != NULL, 0);

    return output->lteInfo.cell_resel_priority;

}

const guint8
qmi_nas_get_cell_loc_info_output_get_s_non_intra_search(QmiNasGetCellLocInfoOutput *output)
{

    g_return_val_if_fail (output != NULL, 0);

    return output->lteInfo.s_non_intra_search;

}

const guint8
qmi_nas_get_cell_loc_info_output_get_thresh_serving_low(QmiNasGetCellLocInfoOutput *output)
{

    g_return_val_if_fail (output != NULL, 0);

    return output->lteInfo.thresh_serving_low;

}

const guint8
qmi_nas_get_cell_loc_info_output_get_s_intra_search(QmiNasGetCellLocInfoOutput *output)
{

    g_return_val_if_fail (output != NULL, 0);

    return output->lteInfo.s_intra_search;

}

const guint8
qmi_nas_get_cell_loc_info_output_get_sets(QmiNasGetCellLocInfoOutput *output) {

	g_return_val_if_fail (output != NULL, 0);

	return output->lteInfo.cells_len;
}

const guint16
qmi_nas_get_cell_loc_info_output_get_pci(QmiNasGetCellLocInfoOutput *output, guint8 index)
{

    g_return_val_if_fail(output, 0);


    if((index >= 0) && (index < output->lteInfo.cells_len)) {
    	/* FIXME: index = 0 --> adresse = 0 */
    	return le16toh(output->lteSignal[index]->pci);
    }
    else {

    	g_printerr("qmi_nas_get_cell_loc_info_output_get_pci out of index");
    	return 0;
    }

}

const gfloat
qmi_nas_get_cell_loc_info_output_get_rsrq(QmiNasGetCellLocInfoOutput *output, guint8 index)
{

    g_return_val_if_fail (output != NULL, 0);


	if((index >= 0) && (index < output->lteInfo.cells_len)) {

		return (gfloat)le16toh(output->lteSignal[index]->rsrq)/10;
	}
	else {

		g_printerr("qmi_nas_get_cell_loc_info_output_get_rsrq out of index");
		return 0;
	}


}

const gfloat
qmi_nas_get_cell_loc_info_output_get_rsrp(QmiNasGetCellLocInfoOutput *output, guint8 index)
{

    g_return_val_if_fail (output != NULL, 0.0);


	if((index >= 0) && (index < output->lteInfo.cells_len)) {

		return (gfloat)le16toh(output->lteSignal[index]->rsrp)/10;
	}
	else {

		g_printerr("qmi_nas_get_cell_loc_info_output_get_rsrp out of index");
		return 0.0;
	}


}

const gfloat
qmi_nas_get_cell_loc_info_output_get_rssi(QmiNasGetCellLocInfoOutput *output, guint8 index)
{

    g_return_val_if_fail (output != NULL, 0);


    if((index >= 0) && (index < output->lteInfo.cells_len)) {

		return (gfloat)le16toh(output->lteSignal[index]->rssi)/10;
	}
	else {

		g_printerr("qmi_nas_get_cell_loc_info_output_get_rssi out of index");
		return 0;
	}


}

const gint16
qmi_nas_get_cell_loc_info_output_get_srxlev(QmiNasGetCellLocInfoOutput *output, guint8 index)
{

    g_return_val_if_fail (output != NULL, 0);


    if((index >= 0) && (index < output->lteInfo.cells_len)) {

   		return le16toh(output->lteSignal[index]->srxlev);
   	}
   	else {

   		g_printerr("qmi_nas_get_cell_loc_info_output_get_srxlev out of index");
   		return 0;
   	}


}

/**
 * qmi_nas_get_cell_loc_info_output_ref:
 * @output: a #QmiNasGetCellLocInfoOutput.
 *
 * Atomically increments the reference count of @output by one.
 *
 * Returns: the new reference to @output.
 */
QmiNasGetCellLocInfoOutput*
qmi_nas_get_cell_loc_info_output_ref(QmiNasGetCellLocInfoOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);
    return output;
}

/**
 * qmi_nas_get_cell_loc_info_output_unref:
 * @output: a #QmiNasGetCellLocInfoOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_nas_get_cell_loc_info_output_unref(QmiNasGetCellLocInfoOutput *output)
{
	gint i;
    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {

    	for(i = 0; i < output->lteInfo.cells_len; i++) {

    		g_free(output->lteSignal[i]);
    	}

        if (output->error)
            g_error_free (output->error);

        g_free(output->lteSignal);
		g_free(output);
    }
}

QmiMessage*
qmi_message_nas_get_cell_loc_info_new(guint8 transaction_id, guint8 client_id)
{
    return qmi_message_new (QMI_SERVICE_NAS,
                            client_id,
                            transaction_id,
                            QMI_NAS_MESSAGE_GET_CELL_LOCATION_INFO);
}

QmiNasGetCellLocInfoOutput*
qmi_message_nas_get_cell_loc_info_reply_parse(QmiMessage *self, GError **error)
{
	QmiNasGetCellLocInfoOutput *output;
    GError *inner_error = NULL;
    gchar *pResponse = NULL;
    guint8 i;
    gint index=0;

    g_assert(qmi_message_get_message_id(self) == QMI_NAS_MESSAGE_GET_CELL_LOCATION_INFO);

    if (!qmi_message_get_response_result(self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }


    if(!(pResponse = qmi_message_tlv_get_string(self,
    						QMI_NAS_TLV_GET_LTE_INFO_INTRA,
							error))) {
		g_prefix_error (error, "Couldn't get the Cell Location Info TLV: ");
		return NULL;
	}


    output = g_new0(QmiNasGetCellLocInfoOutput, 1);
	output->ref_count = 1;
	output->error = inner_error;

    memcpy(&output->lteInfo, pResponse, sizeof(output->lteInfo));
    index = sizeof(output->lteInfo);

    output->lteSignal = g_new0(struct _LteSignal *, output->lteInfo.cells_len);


    for(i = 0; i < output->lteInfo.cells_len; i++) {


    	output->lteSignal[i] = g_new0(struct _LteSignal, 1);
    	if(!output->lteSignal[i]) {

			g_printerr("lteSignal[%d] could not be allocated.\n", i);
			qmi_nas_get_cell_loc_info_output_unref(output);
			return NULL;
		}

    	memcpy(output->lteSignal[i], &pResponse[index], sizeof(struct _LteSignal));
    	index += sizeof(struct _LteSignal);
    }

    if(pResponse)
    	g_free(pResponse);

    return output;
}

/*****************************************************************************/
/*****************************************************************************/
/* Get RF Band Information List */

/* Type for TLV´s */
enum {
  QMI_NAS_TLV_GET_RF_BAND_INFO	=	0x01,

};

/**
 * _QmiNasGetRfBandOutput:
 *
 * An opaque type handling the output of the Get SigInfo operation.
 */

struct _RfBand {
	gint8 radio_if;
	gint16 active_band;
	gint16 actice_channel;


}__attribute__((__packed__));

struct _QmiNasGetRfBandOutput {
    volatile gint ref_count;
    GError *error;
    guint8 instances;
    struct _RfBand **rfBand;

};

const guint8
qmi_nas_get_rf_band_output_get_instances(QmiNasGetRfBandOutput *output) {

	g_return_val_if_fail (output != NULL, 0);

	return output->instances;

}

gboolean
qmi_nas_get_rf_band_output_get_result(QmiNasGetRfBandOutput *output, GError **error)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->error)
    {
        if (error)
            *error = g_error_copy (output->error);

        return FALSE;
    }

    return TRUE;
}


const gint8
qmi_nas_get_rf_band_output_get_radio_if(QmiNasGetRfBandOutput *output, guint8 index)
{

    g_return_val_if_fail (output != NULL, 0);


	if((index >= 0) && (index < output->instances)) {

		return output->rfBand[index]->radio_if;
	}
	else {

		g_printerr("qmi_nas_get_rf_band_output_get_radio_if out of index");
		return 0;
	}


}


const gchar*
qmi_nas_get_rf_band_output_get_active_band(QmiNasGetRfBandOutput *output, guint8 index)
{

    g_return_val_if_fail (output != NULL, NULL);


    if((index >= 0) && (index < output->instances)) {

		return qmi_band_enum_get_string(le16toh(output->rfBand[index]->active_band));
	}
	else {

		g_printerr("qmi_nas_get_rf_band_output_get_active_band out of index");
		return NULL;
	}

}


const gint16
qmi_nas_get_rf_band_output_get_active_channel(QmiNasGetRfBandOutput *output, guint8 index)
{

    g_return_val_if_fail (output != NULL, 0);


	if((index >= 0) && (index < output->instances)) {

		return le16toh(output->rfBand[index]->actice_channel);
	}
	else {

		g_printerr("qmi_nas_get_rf_band_output_get_active_channel out of index");
		return 0;
	}

}



/**
 * qmi_message_nas_get_sig_info_output_ref:
 * @output: a #QmiNasGetSigInfoOutput.
 *
 * Atomically increments the reference count of @output by one.
 *
 * Returns: the new reference to @output.
 */
QmiNasGetRfBandOutput*
qmi_nas_get_rf_band_output_ref(QmiNasGetRfBandOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);

    return output;
}

/**
 * qmi_message_nas_get_sig_info_output_unref:
 * @output: a #QmiNasGetSigInfoOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_nas_get_rf_band_output_unref(QmiNasGetRfBandOutput *output)
{
	gint i;

    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {

    	for(i = 0; i < output->instances; i++) {
    		//g_slice_free(struct _RfBand, output->rfBand[i]);
    		g_free(output->rfBand[i]);
    	}

        if (output->error)
            g_error_free (output->error);

        //g_slice_free(struct _RfBand *, output->rfBand);
        g_free(output->rfBand);
        //g_slice_free(QmiNasGetRfBandOutput, output);
        g_free(output);
    }
}

QmiMessage*
qmi_message_nas_get_rf_band_new(guint8 transaction_id, guint8 client_id)
{
    return qmi_message_new (QMI_SERVICE_NAS,
                            client_id,
                            transaction_id,
                            QMI_NAS_MESSAGE_GET_RF_BAND_INFO);
}

QmiNasGetRfBandOutput*
qmi_message_nas_get_rf_band_reply_parse(QmiMessage *self, GError **error)
{
	QmiNasGetRfBandOutput *output;
    GError *inner_error = NULL;
    gchar *pResponse = NULL;
    gint8 i;
    gint index=0;

    g_assert(qmi_message_get_message_id(self) == QMI_NAS_MESSAGE_GET_RF_BAND_INFO);

    if (!qmi_message_get_response_result(self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    //g_debug("Size of Msg: %d  --> Message: %s\n", qmi_message_get_length(self), qmi_message_get_printable(self, ""));

    if(!(pResponse = qmi_message_tlv_get_string(self,
    											QMI_NAS_TLV_GET_RF_BAND_INFO,
    											error))) {
		g_prefix_error (error, "Couldn't get the RF Band Info TLV: ");
		return NULL;
	}

    //output = g_slice_new0 (QmiNasGetRfBandOutput);
    output = g_new0(QmiNasGetRfBandOutput, 1);
	output->ref_count = 1;
	output->error = inner_error;

    index = 0;
    memcpy(&output->instances, &pResponse[index], sizeof(output->instances));
    index = sizeof(output->instances);

    //output->rfBand = g_slice_alloc0(output->instances * sizeof(struct _RfBand *));
    output->rfBand = g_new0(struct _RfBand *, output->instances);

	for(i = 0; i < output->instances; i++) {

//		output->rfBand[i] = g_slice_new0(struct _RfBand);
		output->rfBand[i] = g_new0(struct _RfBand, 1);
		if(!output->rfBand[i]) {

			g_printerr("rfBand[%i] could not be allocated.\n", i);
			qmi_nas_get_rf_band_output_unref(output);
			return NULL;
		}
		memcpy(output->rfBand[i], &pResponse[index], sizeof(struct _RfBand));
		index += sizeof(struct _RfBand);

	}

    if(pResponse)
		g_free(pResponse);

    return output;
}

/*****************************************************************************/
/*****************************************************************************/
/* Get System Information */

/* Type for TLV´s */
enum {
  QMI_NAS_TLV_GET_LTE_SYSTEM_INFO	=	0x19,

};

/**
 * _QmiNasGetRfBandOutput:
 *
 * An opaque type handling the output of the Get SigInfo operation.
 */

struct _LteSysInfo {
	gint8 srv_domain_valid;
	gint8 srv_domain;
	gint8 srv_capability_valid;
	gint8 srv_capability;
	gint8 roam_status_valid;
	gint8 roam_status;
	gint8 is_sys_forbidden_valid;
	gint8 is_sys_forbidden;
	gint8 lac_valid;
	gint16 lac;
	gint8 cell_id_valid;
	guint32 cell_id;
	gint8 reg_reject_info_valid;
	gint8 reject_srv_domain;
	gint8 reject_cause;
	gint8 network_id_valid;
	gchar mcc[3];
	gchar mnc[3];
	gint8 tac_valid;
	guint16 tac;


}__attribute__((__packed__));

struct _QmiNasGetSysInfoOutput {
    volatile gint ref_count;
    GError *error;
    struct _LteSysInfo lteSysInfo;

};


gboolean qmi_nas_get_sys_info_output_get_roam_status_valid(QmiNasGetSysInfoOutput *output) {

    g_return_val_if_fail (output != NULL, 0);

	return !!(output->lteSysInfo.roam_status_valid);

}

const guint8 qmi_nas_get_sys_info_output_get_roam_status(QmiNasGetSysInfoOutput *output) {

	g_return_val_if_fail(output != NULL, 0);

	return output->lteSysInfo.roam_status;
}

gboolean qmi_nas_get_sys_info_output_get_lac_valid(QmiNasGetSysInfoOutput *output) {

	g_return_val_if_fail (output != NULL, 0);

	return !!(output->lteSysInfo.lac_valid);

}

const guint16 qmi_nas_get_sys_info_output_get_lac(QmiNasGetSysInfoOutput *output) {

	g_return_val_if_fail(output != NULL, 0);

	return le16toh(output->lteSysInfo.lac);
}

gboolean qmi_nas_get_sys_info_output_get_cell_id_valid(QmiNasGetSysInfoOutput *output) {

	g_return_val_if_fail (output != NULL, 0);

	return !!(output->lteSysInfo.cell_id_valid);
}


const guint32 qmi_nas_get_sys_info_output_get_cell_id(QmiNasGetSysInfoOutput *output) {

	g_return_val_if_fail(output != NULL, 0);

	return le32toh(output->lteSysInfo.cell_id);

}

gboolean qmi_nas_get_sys_info_output_get_network_id_valid(QmiNasGetSysInfoOutput *output) {

	g_return_val_if_fail (output != NULL, 0);

	return !!(output->lteSysInfo.network_id_valid);

}

const gchar* qmi_nas_get_sys_info_output_get_mcc(QmiNasGetSysInfoOutput *output) {

	guint value = 0;
	gchar pString[4];

	g_return_val_if_fail(output != NULL, NULL);

	memcpy(pString, output->lteSysInfo.mcc, 3);
	pString[3] = '\0';

	value = (guint32)g_ascii_strtoull(pString, NULL, 0);

	return qmi_mcc_get_string(value);

}

const gchar* qmi_nas_get_sys_info_output_get_mnc(QmiNasGetSysInfoOutput *output) {

	guint value = 0;
	gchar pString[4];

	g_return_val_if_fail(output != NULL, NULL);

	memcpy(pString, output->lteSysInfo.mnc, 3);
	pString[3] = '\0';

	if(pString[2] == 0xFF) {
		pString[2] = '\0';
	}

	value = (guint32)g_ascii_strtoull(pString, NULL, 0);

	return qmi_mnc_get_string(value);

}

gboolean qmi_nas_get_sys_info_output_get_tac_valid(QmiNasGetSysInfoOutput *output) {

	g_return_val_if_fail (output != NULL, 0);

	return !!(output->lteSysInfo.tac_valid);

}

const guint16 qmi_nas_get_sys_info_output_get_tac(QmiNasGetSysInfoOutput *output) {

	g_return_val_if_fail(output != NULL, 0);

	return le16toh(output->lteSysInfo.tac);

}



gboolean
qmi_nas_get_sys_info_output_get_result(QmiNasGetSysInfoOutput *output, GError **error)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->error)
    {
        if (error)
            *error = g_error_copy (output->error);

        return FALSE;
    }

    return TRUE;
}



/**
 * qmi_message_nas_get_sig_info_output_ref:
 * @output: a #QmiNasGetSysInfoOutput.
 *
 * Atomically increments the reference count of @output by one.
 *
 * Returns: the new reference to @output.
 */
QmiNasGetSysInfoOutput*
qmi_nas_get_sys_info_output_ref(QmiNasGetSysInfoOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);

    return output;
}

/**
 * qmi_message_nas_get_sig_info_output_unref:
 * @output: a #QmiNasGetSysInfoOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_nas_get_sys_info_output_unref(QmiNasGetSysInfoOutput *output)
{

    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {

        if (output->error)
            g_error_free (output->error);
        g_slice_free (QmiNasGetSysInfoOutput, output);
    }
}

QmiMessage*
qmi_message_nas_get_sys_info_new(guint8 transaction_id, guint8 client_id)
{
    return qmi_message_new (QMI_SERVICE_NAS,
                            client_id,
                            transaction_id,
                            QMI_NAS_MAESSAGE_GET_SYS_INFO);
}

QmiNasGetSysInfoOutput*
qmi_message_nas_get_sys_info_reply_parse(QmiMessage *self, GError **error)
{
	QmiNasGetSysInfoOutput *output;
    GError *inner_error = NULL;

    g_assert(qmi_message_get_message_id(self) == QMI_NAS_MAESSAGE_GET_SYS_INFO);

    if (!qmi_message_get_response_result(self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    //g_debug("Size of Msg: %d  --> Message: %s\n", qmi_message_get_length(self), qmi_message_get_printable(self, ""));

    output = g_slice_new0 (QmiNasGetSysInfoOutput);
    output->ref_count = 1;
    output->error = inner_error;

    if(!qmi_message_tlv_get(self,
							QMI_NAS_TLV_GET_LTE_SYSTEM_INFO,
							sizeof(output->lteSysInfo),
							&output->lteSysInfo,
							error)) {
    	g_prefix_error (error, "Couldn't get the LTE System Info TLV: ");
		qmi_nas_get_sys_info_output_unref(output);
		return NULL;
	}

    return output;
}

/*****************************************************************************/

/*****************************************************************************/
/* Get Signal Strength */

/* Type for TLV´s */
enum {
	QMI_NAS_TLV_GET_LTE_SIG_STRENGTH_RSSI	=	0x11,
	QMI_NAS_TLV_GET_LTE_SIG_STRENGTH_RSRQ	=	0x16,
	QMI_NAS_TLV_GET_LTE_SIG_STRENGTH_SNR  	=	0x17,
	QMI_NAS_TLV_GET_LTE_SIG_STRENGTH_RSRP 	= 	0x18

};

/**
 * _QmiNasGetRfBandOutput:
 *
 * An opaque type handling the output of the Get SigInfo operation.
 */

struct _LteSigStrengthRSRQ {
	gint8 rsrq;
	gint8 radio_if;


}__attribute__((__packed__));

struct _LteSigStrengthRSSI {
	gint16 num_instances;
	guint8 rssi;
	guint8 radio_if;


}__attribute__((__packed__));

struct _QmiNasGetSigStrengthOutput{
    volatile gint ref_count;
    GError *error;
    struct _LteSigStrengthRSRQ lteSigStrengthRSRQ;
    struct _LteSigStrengthRSSI lteSigStrengthRSSI;
    gint16 snr;
    gint16 rsrp;

};


const gint8
qmi_nas_get_sig_strength_output_get_rsrq(QmiNasGetSigStrengthOutput *output) {

    g_return_val_if_fail (output != NULL, 0);

	return output->lteSigStrengthRSRQ.rsrq;

}

const gint8
qmi_nas_get_sig_strength_output_get_radio_if(QmiNasGetSigStrengthOutput *output) {

	g_return_val_if_fail(output != NULL, 0);

	return output->lteSigStrengthRSRQ.radio_if;
}

const float
qmi_nas_get_sig_strength_output_get_snr(QmiNasGetSigStrengthOutput *output) {

	g_return_val_if_fail (output != NULL, 0);

	return (float)(le16toh(output->snr)) / 10;

}

const gint16
qmi_nas_get_sig_strength_output_get_rsrp(QmiNasGetSigStrengthOutput *output) {

	g_return_val_if_fail(output != NULL, 0);

	return le16toh(output->rsrp);
}

const gint16
qmi_nas_get_sig_strength_output_get_rssi(QmiNasGetSigStrengthOutput *output) {

    g_return_val_if_fail (output != NULL, 0);

	return -(output->lteSigStrengthRSSI.rssi);

}


gboolean
qmi_nas_get_sig_strength_output_get_result(QmiNasGetSigStrengthOutput *output, GError **error)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->error)
    {
        if (error)
            *error = g_error_copy (output->error);

        return FALSE;
    }

    return TRUE;
}



/**
 * qmi_message_nas_get_sig_info_output_ref:
 * @output: a #QmiNasGetSysInfoOutput.
 *
 * Atomically increments the reference count of @output by one.
 *
 * Returns: the new reference to @output.
 */
QmiNasGetSigStrengthOutput*
qmi_nas_get_sig_strength_output_ref(QmiNasGetSigStrengthOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);

    return output;
}

/**
 * qmi_message_nas_get_sig_info_output_unref:
 * @output: a #QmiNasGetSysInfoOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_nas_get_sig_strength_output_unref(QmiNasGetSigStrengthOutput *output)
{

    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {

        if (output->error)
            g_error_free (output->error);
        g_slice_free (QmiNasGetSigStrengthOutput, output);
    }
}

QmiMessage*
qmi_message_nas_get_sig_strength_new(guint8 transaction_id, guint8 client_id, GError **error)
{
	QmiMessage *message;
	guint16 mask = 0;
	mask |= (1<<0);	/* RSSI */
	mask |= (1<<5); /* RSRQ */
	mask |= (1<<6); /* SNR */
	mask |= (1<<7); /* RSRP */

	message = qmi_message_new (QMI_SERVICE_NAS,
                            client_id,
                            transaction_id,
                            QMI_NAS_MESSAGE_GET_SIGNAL_STRENGTH);

    if(!qmi_message_tlv_add(message,
							0x10,
							sizeof(mask),
							&mask,
							error)) {
		g_prefix_error(error, "Failed to add Request Info to message: ");
		g_error_free (*error);
		qmi_message_unref (message);
		return NULL;
	}

    return message;

}

QmiNasGetSigStrengthOutput*
qmi_message_nas_get_sig_strength_reply_parse(QmiMessage *self, GError **error)
{
	QmiNasGetSigStrengthOutput *output;
    GError *inner_error = NULL;

    g_assert(qmi_message_get_message_id(self) == QMI_NAS_MESSAGE_GET_SIGNAL_STRENGTH);

    if (!qmi_message_get_response_result(self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    //g_debug("Size of Msg: %d  --> Message: %s\n", qmi_message_get_length(self), qmi_message_get_printable(self, ""));

    output = g_slice_new0 (QmiNasGetSigStrengthOutput);
    output->ref_count = 1;
    output->error = inner_error;

    if(!qmi_message_tlv_get(self,
    						QMI_NAS_TLV_GET_LTE_SIG_STRENGTH_RSRQ,
							sizeof(output->lteSigStrengthRSRQ),
							&output->lteSigStrengthRSRQ,
							&output->error)) {
    	g_printerr("Couldn't get the LTE RSRQ Signal Strength TLV: %s\n", output->error->message);
    	g_clear_error(&output->error);

	}

    if(!qmi_message_tlv_get(self,
							QMI_NAS_TLV_GET_LTE_SIG_STRENGTH_SNR,
							sizeof(output->snr),
							&output->snr,
							&output->error)) {
    	g_printerr("Couldn't get the LTE SNR Signal Strength TLV: %s\n", output->error->message);
		g_clear_error(&output->error);

	}


    if(!qmi_message_tlv_get(self,
							QMI_NAS_TLV_GET_LTE_SIG_STRENGTH_RSRP,
							sizeof(output->rsrp),
							&output->rsrp,
							&output->error)) {
    	g_printerr("Couldn't get the LTE RSRP Signal Strength TLV: %s\n", output->error->message);
		g_clear_error(&output->error);

	}

    if(!qmi_message_tlv_get(self,
							QMI_NAS_TLV_GET_LTE_SIG_STRENGTH_RSSI,
							sizeof(output->lteSigStrengthRSSI),
							&output->lteSigStrengthRSSI,
							&output->error)) {
    	g_printerr("Couldn't get the LTE RSSI Signal Strength TLV: %s\n", output->error->message);
		g_clear_error(&output->error);

	}

    return output;
}

/*****************************************************************************/

/*****************************************************************************/
/* Get System Selection Preference */

/* Type for TLV´s */
enum {

	QMI_NAS_TLV_GET_MODE_PREF				=	0x11,
	QMI_NAS_TLV_GET_LTE_BAND_PREF		  	=	0x15

};



struct _QmiNasGetSystemSelectionPrefOutput{

    volatile gint ref_count;
    GError *error;
    guint16 mode_pref;
    guint64 lte_band_pref;

};


const guint16
qmi_nas_get_system_selection_pref_output_get_mode_pref(QmiNasGetSystemSelectionPrefOutput *output) {

	g_return_val_if_fail(output != NULL, 0);

	return le16toh(output->mode_pref);
}

const guint64
qmi_nas_get_system_selection_pref_output_get_lte_band_pref(QmiNasGetSystemSelectionPrefOutput *output) {

    g_return_val_if_fail (output != NULL, 0);

	return le64toh(output->lte_band_pref);

}


gboolean
qmi_nas_get_system_selection_pref_output_get_result(QmiNasGetSystemSelectionPrefOutput *output, GError **error)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->error)
    {
        if (error)
            *error = g_error_copy (output->error);

        return FALSE;
    }

    return TRUE;
}



/**
 * qmi_message_nas_get_sig_info_output_ref:
 * @output: a #QmiNasGetSysInfoOutput.
 *
 * Atomically increments the reference count of @output by one.
 *
 * Returns: the new reference to @output.
 */
QmiNasGetSystemSelectionPrefOutput*
qmi_nas_get_system_selection_pref_output_ref(QmiNasGetSystemSelectionPrefOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);

    return output;
}



/**
 * qmi_message_nas_get_sig_info_output_unref:
 * @output: a #QmiNasGetSysInfoOutput.
 *
 * Atomically decrements the reference count of @output by one.
 * If the reference count drops to 0, @output is completely disposed.
 */
void
qmi_nas_get_system_selection_pref_output_unref(QmiNasGetSystemSelectionPrefOutput *output)
{

    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {

        if (output->error)
            g_error_free (output->error);
        g_slice_free (QmiNasGetSystemSelectionPrefOutput, output);
    }
}

QmiMessage*
qmi_message_nas_get_system_selection_pref_new(guint8 transaction_id, guint8 client_id, GError **error)
{
	QmiMessage *message;

	message = qmi_message_new (QMI_SERVICE_NAS,
                            client_id,
                            transaction_id,
                            QMI_NAS_MESAGE_GET_SYSTEM_SELECTION_PREF);


    return message;

}


QmiNasGetSystemSelectionPrefOutput*
qmi_message_nas_get_system_selection_pref_reply_parse(QmiMessage *self, GError **error)
{
	QmiNasGetSystemSelectionPrefOutput *output;
    GError *inner_error = NULL;

    g_assert(qmi_message_get_message_id(self) == QMI_NAS_MESAGE_GET_SYSTEM_SELECTION_PREF);

    if (!qmi_message_get_response_result(self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }



    output = g_slice_new0 (QmiNasGetSystemSelectionPrefOutput);
    output->ref_count = 1;
    output->error = inner_error;

    if(!qmi_message_tlv_get(self,
    						QMI_NAS_TLV_GET_MODE_PREF,
							sizeof(output->mode_pref),
							&output->mode_pref,
							&output->error)) {
    	g_printerr("Couldn't get the Mode Preference TLV: %s\n", output->error->message);
    	g_clear_error(&output->error);

	}

    if(!qmi_message_tlv_get(self,
    						QMI_NAS_TLV_GET_LTE_BAND_PREF,
							sizeof(output->lte_band_pref),
							&output->lte_band_pref,
							&output->error)) {
		g_printerr("Couldn't get the LTE Band Preference TLV: %s\n", output->error->message);
		g_clear_error(&output->error);

	}



    return output;
}

/*****************************************************************************/


/*****************************************************************************/
/* Set System Selection Preference */

/* Type for TLV´s */
enum {

	QMI_NAS_TLV_SET_MODE_PREF				=	0x11,
	QMI_NAS_TLV_SET_LTE_BAND_PREF		  	=	0x15,
	QMI_NAS_TLV_SET_CHANGE_DURATION			= 	0x17

};


struct _QmiNasSetSystemSelectionPrefOutput{

    volatile gint ref_count;
    GError *error;

};

struct _QmiNasSetSystemSelectionPrefInput {

    volatile gint ref_count;

    guint16 mode_pref_mask;
    guint64 lte_band_mask;
    guint8 change_duration;

};


void
qmi_nas_set_system_selection_pref_input_mask(	QmiNasSetSystemSelectionPrefInput *input,
												Set_System_Selection_Mode_Preference mode_pref_mask,
												Set_System_Selection_LTE_Band_Preference lte_band_mask,
												Set_System_Selection_Change_Duration change_duration)
{
    g_return_if_fail (input != NULL);

    switch(lte_band_mask) {

    	default:
		case SYSTEM_SELECTION_BAND_AUTOMATIC:
			input->lte_band_mask = htole64((guint64)(0xFFFFFFFFFFFFFFFFull)); // ALL
			break;

		case SYSTEM_SELECTION_BAND_7:
			input->lte_band_mask = htole64((guint64)(1<<6));	/* LTE 2,6 GHz Band 7 */
			break;

		case SYSTEM_SELECTION_BAND_3:
			input->lte_band_mask = htole64((guint64)(1<<2));
			break;

		case SYSTEM_SELECTION_BAND_20:
			input->lte_band_mask = htole64((guint64)(1<<19));			/* LTE 800 MHz Band 20 */
			break;

	}

    switch(mode_pref_mask) {

    	default:
		case SYSTEM_SELECTION_MODE_AUTOMATIC:
			input->mode_pref_mask = htole16((guint16)(0xFF));	// ALL
			break;

		case SYSTEM_SELECTION_GSM:
			input->mode_pref_mask = htole16((guint16)(1<<2));	// GSM
			break;

		case SYSTEM_SELECTION_UMTS:
			input->mode_pref_mask = htole16((guint16)(1<<3));	// UMTS
			break;

		case SYSTEM_SELECTION_LTE:
			input->mode_pref_mask = htole16((guint16)(1<<4));	// LTE
			break;

	}

    switch(change_duration) {

    	default:
		case SYSTEM_SELECTION_POWER_CYCLE:
			input->change_duration = (guint8)(0x00);
			break;

		case SYSTEM_SELECTION_PERMANENT:
			input->change_duration = (guint8)(0x01);
			break;


	}


}

QmiNasSetSystemSelectionPrefInput*
qmi_nas_set_system_selection_pref_input_new (void)
{
	QmiNasSetSystemSelectionPrefInput *input;

    input = g_slice_new0 (QmiNasSetSystemSelectionPrefInput);
    input->ref_count = 1;
    return input;
}


QmiNasSetSystemSelectionPrefInput*
qmi_nas_set_system_selection_pref_input_ref (QmiNasSetSystemSelectionPrefInput *input)
{
    g_return_val_if_fail (input != NULL, NULL);

    g_atomic_int_inc (&input->ref_count);
    return input;
}


void
qmi_nas_set_system_selection_pref_input_unref (QmiNasSetSystemSelectionPrefInput *input)
{
    g_return_if_fail (input != NULL);

    if (g_atomic_int_dec_and_test (&input->ref_count)) {
        g_slice_free (QmiNasSetSystemSelectionPrefInput, input);
    }
}


QmiNasSetSystemSelectionPrefOutput*
qmi_nas_set_system_selection_pref_output_ref(QmiNasSetSystemSelectionPrefOutput *output)
{
    g_return_val_if_fail (output != NULL, NULL);

    g_atomic_int_inc (&output->ref_count);

    return output;
}


void
qmi_nas_set_system_selection_pref_output_unref(QmiNasSetSystemSelectionPrefOutput *output)
{

    g_return_if_fail (output != NULL);

    if (g_atomic_int_dec_and_test (&output->ref_count)) {

        if (output->error)
            g_error_free (output->error);
        g_slice_free (QmiNasSetSystemSelectionPrefOutput, output);
    }
}

gboolean
qmi_nas_set_system_selection_pref_output_get_result(QmiNasSetSystemSelectionPrefOutput *output, GError **error)
{
    g_return_val_if_fail (output != NULL, FALSE);

    if (output->error)
    {
        if (error)
            *error = g_error_copy (output->error);

        return FALSE;
    }

    return TRUE;
}

QmiMessage*
qmi_message_nas_set_system_selection_pref_new(	guint8 transaction_id,
												guint8 client_id,
												QmiNasSetSystemSelectionPrefInput *input,
												GError **error)
{
	QmiMessage *message;


	message = qmi_message_new (QMI_SERVICE_NAS,
								client_id,
								transaction_id,
								QMI_NAS_MESSAGE_SET_SYSTEM_SELECTION_PREF);



	if(input) {

		if(input->mode_pref_mask == SYSTEM_SELECTION_LTE) {

			if(!qmi_message_tlv_add(message,
							QMI_NAS_TLV_SET_LTE_BAND_PREF,
							sizeof(input->lte_band_mask),
							&input->lte_band_mask,
							error)) {

			g_prefix_error(error, "Failed to add lte_band_mask Request Info to message: ");
			g_error_free(*error);
			qmi_message_unref(message);
			return NULL;
			}
		}

		if(!qmi_message_tlv_add(message,
								QMI_NAS_TLV_SET_MODE_PREF,
								sizeof(input->mode_pref_mask),
								&input->mode_pref_mask,
								error)) {

			g_prefix_error(error, "Failed to add mode_pref_mask Request Info to message: ");
			g_error_free(*error);
			qmi_message_unref(message);
			return NULL;
		}

		/* Change Duration is set to Power cycle - remains active until the next device power cycle */
		if(!qmi_message_tlv_add(message,
								QMI_NAS_TLV_SET_CHANGE_DURATION,
								sizeof(input->change_duration),
								&input->change_duration,
								error)) {

			g_prefix_error(error, "Failed to add set change duration Request Info to message: ");
			g_error_free(*error);
			qmi_message_unref(message);
			return NULL;
		}
	}


	return message;

}

QmiNasSetSystemSelectionPrefOutput*
qmi_message_nas_set_system_selection_pref_reply_parse(QmiMessage *self, GError **error)
{
	QmiNasSetSystemSelectionPrefOutput *output;
    GError *inner_error = NULL;

    g_assert(qmi_message_get_message_id(self) == QMI_NAS_MESSAGE_SET_SYSTEM_SELECTION_PREF);

    if (!qmi_message_get_response_result(self, &inner_error)) {
        /* Only QMI protocol errors are set in the Output result, all the
         * others (e.g. failures parsing) are directly propagated to error. */
        if (inner_error->domain != QMI_PROTOCOL_ERROR) {
            g_propagate_error (error, inner_error);
            return NULL;
        }

        /* Otherwise, build output */
    }

    output = g_slice_new0 (QmiNasSetSystemSelectionPrefOutput);
    output->ref_count = 1;
    output->error = inner_error;


    return output;
}

/*****************************************************************************/

/*****************************************************************************/
/* Get Technology Preference */

/* Type for TLV´s */
enum {

	QMI_NAS_TLV_GET_ACTIVE_TECHNOLOGY_PREF			=	0x01,
	QMI_NAS_TLV_GET_PERSISTENT_TECHNOLOGY_PREF	  	=	0x10

};

struct _QmiNasGetTechnologyActiveTechPref {

    guint16 active_technology_pref;
    guint8 active_technology_duration;

}__attribute__((__packed__));

struct _QmiNasGetTechnologyPrefOutput {

    volatile gint ref_count;
    GError *error;
    struct _QmiNasGetTechnologyActiveTechPref QmiNasGetTechnologyActiveTechPref;
    guint16 persistent_technology_pref;

};



const guint16
qmi_nas_get_technology_pref_output_get_active_technoloy_pref(QmiNasGetTechnologyPrefOutput *output) {

	g_return_val_if_fail(output != NULL, 0);

	return le16toh(output->QmiNasGetTechnologyActiveTechPref.active_technology_pref);
}

const guint8
qmi_nas_get_technology_pref_output_get_active_technology_duration_pref(QmiNasGetTechnologyPrefOutput *output) {

	g_return_val_if_fail(output != NULL, 0);

	return output->QmiNasGetTechnologyActiveTechPref.active_technology_duration;
}

const guint16
qmi_nas_get_technology_pref_output_get_persistent_technoloy_pref(QmiNasGetTechnologyPrefOutput *output) {

	g_return_val_if_fail(output != NULL, 0);

	return le16toh(output->persistent_technology_pref);
}

gboolean
qmi_nas_get_technology_pref_output_get_result(QmiNasGetTechnologyPrefOutput *output, GError **error) {

	g_return_val_if_fail (output != NULL, FALSE);

	if (output->error)
	{
		if (error)
			*error = g_error_copy (output->error);

		return FALSE;
	}

	return TRUE;
}

QmiNasGetTechnologyPrefOutput*
qmi_nas_get_technology_pref_output_ref(QmiNasGetTechnologyPrefOutput *output) {

	g_return_val_if_fail (output != NULL, NULL);

	g_atomic_int_inc (&output->ref_count);

	return output;
}

void
qmi_nas_get_technology_pref_output_unref(QmiNasGetTechnologyPrefOutput *output) {

	g_return_if_fail (output != NULL);

	if (g_atomic_int_dec_and_test (&output->ref_count)) {

		if (output->error)
			g_error_free (output->error);
		g_slice_free (QmiNasGetTechnologyPrefOutput, output);
	}
}

QmiMessage*
qmi_message_nas_get_technology_pref_new(	guint8 transaction_id,
											guint8 client_id,
											GError **error) {

	QmiMessage *message;

	message = qmi_message_new(	QMI_SERVICE_NAS,
								client_id,
								transaction_id,
								QMI_NAS_MESSAGE_GET_TECHNOLOGY_PREF);


	return message;
}

QmiNasGetTechnologyPrefOutput*
qmi_message_nas_get_technology_pref_reply_parse(QmiMessage *self, GError **error) {

	QmiNasGetTechnologyPrefOutput *output;
	GError *inner_error = NULL;

	g_assert(qmi_message_get_message_id(self) == QMI_NAS_MESSAGE_GET_TECHNOLOGY_PREF);

	if (!qmi_message_get_response_result(self, &inner_error)) {
		/* Only QMI protocol errors are set in the Output result, all the
		 * others (e.g. failures parsing) are directly propagated to error. */
		if (inner_error->domain != QMI_PROTOCOL_ERROR) {
			g_propagate_error (error, inner_error);
			return NULL;
		}

		/* Otherwise, build output */
	}



	output = g_slice_new0(QmiNasGetTechnologyPrefOutput);
	output->ref_count = 1;
	output->error = inner_error;

	if(!qmi_message_tlv_get(self,
							QMI_NAS_TLV_GET_ACTIVE_TECHNOLOGY_PREF,
							sizeof(output->QmiNasGetTechnologyActiveTechPref),
							&output->QmiNasGetTechnologyActiveTechPref,
							&output->error)) {
		g_printerr("Couldn't get the active Technology Preference TLV: %s\n", output->error->message);
		g_clear_error(&output->error);

	}

	if(!qmi_message_tlv_get(self,
							QMI_NAS_TLV_GET_PERSISTENT_TECHNOLOGY_PREF,
							sizeof(output->persistent_technology_pref),
							&output->persistent_technology_pref,
							&output->error)) {
		g_printerr("Couldn't get the persistent Technology Preference TLV: %s\n", output->error->message);
		g_clear_error(&output->error);

	}


	return output;
}


/*****************************************************************************/
/*****************************************************************************/
/* Set Technology Preference */


/* Type for TLV´s */
enum {

	QMI_NAS_TLV_SET_TECHNOLOGY_PREF		=	0x01
};


struct _SetTechnologyActiveTechPref {

	guint16 technology_pref;
	guint8 technology_duration;

}__attribute__((__packed__));


struct _QmiNasSetTechnologyPrefInput {

    volatile gint ref_count;
    struct _SetTechnologyActiveTechPref SetTechnologyActiveTechPref;

};

struct _QmiNasSetTechnologyPrefOutput {

    volatile gint ref_count;
    GError *error;
};

void
qmi_nas_set_technology_pref_input_mask(	QmiNasSetTechnologyPrefInput *input,
										Set_Technology_Preference technology_pref,
										Set_Technology_Duration_Preference duration) {

	g_return_if_fail (input != NULL);

	switch(technology_pref) {

//		AUTOMATIC	=	0x00
//		GSM			=	((1<<2)|(1<<1)) - GSM = 3GPP | Analog
//		CDMA		=	((1<<3)|(1<<0)) - CDMA = 3GGP2 | Digital
//		WCDMA		=	((1<<3)|(1<<1)) - WCDMA = 3GGP | Digital
//		HDR			=	(1<<4)
//		LTE			=	(1<<5)

		default:
		case TECHNOLOGY_AUTOMATIC:
			input->SetTechnologyActiveTechPref.technology_pref = htole16((guint16)(0x00));
			break;

		case TECHNOLOGY_GSM:
			input->SetTechnologyActiveTechPref.technology_pref = htole16((guint16)((1<<2)|(1<<1)));
			break;

		case TECHNOLOGY_CDMA:
			input->SetTechnologyActiveTechPref.technology_pref = htole16((guint16)((1<<3)|(1<<0)));
			break;

		case TECHNOLOGY_WCDMA:
			input->SetTechnologyActiveTechPref.technology_pref = htole16((guint16)((1<<3)|(1<<1)));
			break;

		case TECHNOLOGY_HDR:
			input->SetTechnologyActiveTechPref.technology_pref = htole16((guint16)(1<<4));
			break;

		case TECHNOLOGY_LTE:
			input->SetTechnologyActiveTechPref.technology_pref = htole16((guint16)(1<<5));
			break;

	}

	switch(duration) {

		default:
		case TECHNOLOGY_PERMANENT:
			input->SetTechnologyActiveTechPref.technology_duration = (guint8)(0x00);
			break;

		case TECHNOLOGY_POWER_CYCLE:
			input->SetTechnologyActiveTechPref.technology_duration = (guint8)(0x01);
			break;

	}

}

QmiNasSetTechnologyPrefInput*
qmi_nas_set_technology_pref_input_ref (QmiNasSetTechnologyPrefInput *input) {

	g_return_val_if_fail (input != NULL, NULL);

	g_atomic_int_inc (&input->ref_count);

	return input;
}

void
qmi_nas_set_technology_pref_input_unref (QmiNasSetTechnologyPrefInput *input) {

	g_return_if_fail (input != NULL);

	if (g_atomic_int_dec_and_test (&input->ref_count)) {
		g_slice_free (QmiNasSetTechnologyPrefInput, input);
	}
}

QmiNasSetTechnologyPrefInput*
qmi_nas_set_technology_pref_input_new (void) {

	QmiNasSetTechnologyPrefInput *input;

	input = g_slice_new0 (QmiNasSetTechnologyPrefInput);
	input->ref_count = 1;
	return input;
}

gboolean
qmi_nas_set_technology_pref_output_get_result(QmiNasSetTechnologyPrefOutput *output, GError **error) {

	g_return_val_if_fail (output != NULL, FALSE);

	if (output->error)
	{
		if (error)
			*error = g_error_copy (output->error);

		return FALSE;
	}

	return TRUE;
}

QmiNasSetTechnologyPrefOutput*
qmi_nas_set_technology_pref_output_ref(QmiNasSetTechnologyPrefOutput *output) {

	g_return_val_if_fail (output != NULL, NULL);

	g_atomic_int_inc (&output->ref_count);

	return output;
}

void
qmi_nas_set_technology_pref_output_unref(QmiNasSetTechnologyPrefOutput *output) {

	g_return_if_fail (output != NULL);

	if (g_atomic_int_dec_and_test (&output->ref_count)) {

		if (output->error)
			g_error_free (output->error);
		g_slice_free (QmiNasSetTechnologyPrefOutput, output);
	}
}

QmiMessage*
qmi_message_nas_set_technology_pref_new(	guint8 transaction_id,
											guint8 client_id,
											QmiNasSetTechnologyPrefInput *input,
											GError **error) {

	QmiMessage *message;

	message = qmi_message_new(	QMI_SERVICE_NAS,
								client_id,
								transaction_id,
								QMI_NAS_MESSAGE_SET_TECHNOLOGY_PREF);

	if(input) {

		if(!qmi_message_tlv_add(message,
								QMI_NAS_TLV_SET_TECHNOLOGY_PREF,
								sizeof(input->SetTechnologyActiveTechPref),
								&input->SetTechnologyActiveTechPref,
								error)) {

			g_prefix_error(error, "Failed to add SetTechnologyActiveTechPref Request Info to message: ");
			g_error_free(*error);
			qmi_message_unref(message);
			return NULL;
		}

	}


	return message;
}

QmiNasSetTechnologyPrefOutput*
qmi_message_nas_set_technology_pref_reply_parse(QmiMessage *self, GError **error) {

	QmiNasSetTechnologyPrefOutput *output;
	GError *inner_error = NULL;

	g_assert(qmi_message_get_message_id(self) == QMI_NAS_MESSAGE_SET_TECHNOLOGY_PREF);

	if (!qmi_message_get_response_result(self, &inner_error)) {
		/* Only QMI protocol errors are set in the Output result, all the
		 * others (e.g. failures parsing) are directly propagated to error. */
		if (inner_error->domain != QMI_PROTOCOL_ERROR) {
			g_propagate_error (error, inner_error);
			return NULL;
		}

		/* Otherwise, build output */
	}

	output = g_slice_new0 (QmiNasSetTechnologyPrefOutput);
	output->ref_count = 1;
	output->error = inner_error;


	return output;
}
