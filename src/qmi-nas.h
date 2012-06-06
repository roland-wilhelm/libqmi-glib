/*
 * qmi_nas.h
 *
 *  Created on: 23.05.2012
 *      Author: roland
 */

#ifndef _LIBQMI_GLIB_QMI_NAS_H_
#define _LIBQMI_GLIB_QMI_NAS_H_

#include <glib.h>


G_BEGIN_DECLS

/*****************************************************************************/
/* Supported/known messages */
typedef enum {
	QMI_NAS_MESSAGE_GET_SIGNAL_STRENGTH				=	0x0020, /* v1.0 needed - deprecated - */
    QMI_NAS_MESSAGE_PERFORM_NETWORK_SCAN			=	0x0021, /* v1.0 needed, unused currently */
    QMI_NAS_MESSAGE_GET_RF_BAND_INFO				=	0x0031, /* v1.1 needed */
    QMI_NAS_MESSAGE_SET_SYSTEM_SELECTION_PREFERENCE	= 	0x0033, /* v1.1 needed, unused currently */
    QMI_NAS_MESSAGE_GET_CELL_LOCATION_INFO			=	0x0043, /* v1.4 needed */
    QMI_NAS_MAESSAGE_GET_SYS_INFO					=	0x004D, /* v1.8 needed */
    QMI_NAS_MESSAGE_GET_SIG_INFO					=	0x004F, /* v1.8 needed */
    QMI_NAS_MESSAGE_GET_ERR_RATE					=	0x0052, /* v1.8 needed, unused currently NO LTE */
    QMI_NAS_MESSAGE_GET_TX_RX_INFO					=	0x005A, /* v1.9 needed, unused currently */

} QmiNasMessage;


/*****************************************************************************/
/* Get Sig Info */
typedef struct _QmiNasGetSigInfoOutput QmiNasGetSigInfoOutput;

QmiNasGetSigInfoOutput*			qmi_nas_get_sig_info_output_ref				(QmiNasGetSigInfoOutput *output);
void                			qmi_nas_get_sig_info_output_unref			(QmiNasGetSigInfoOutput *output);
gboolean            			qmi_nas_get_sig_info_output_get_result		(QmiNasGetSigInfoOutput *output, GError **error);
const gint8						qmi_nas_get_sig_info_output_get_rssi		(QmiNasGetSigInfoOutput *output);
const gint16					qmi_nas_get_sig_info_output_get_rsrp  		(QmiNasGetSigInfoOutput *output);
const gint8						qmi_nas_get_sig_info_output_get_rsrq  		(QmiNasGetSigInfoOutput *output);
const gfloat					qmi_nas_get_sig_info_output_get_snr   		(QmiNasGetSigInfoOutput *output);


/*****************************************************************************/
/* Get Cell Location Info */
typedef struct _QmiNasGetCellLocInfoOutput QmiNasGetCellLocInfoOutput;

QmiNasGetCellLocInfoOutput*		qmi_nas_get_cell_loc_info_output_ref				(QmiNasGetCellLocInfoOutput *output);
void							qmi_nas_get_cell_loc_info_output_unref				(QmiNasGetCellLocInfoOutput *output);
gboolean						qmi_nas_get_cell_loc_info_output_get_result			(QmiNasGetCellLocInfoOutput *output, GError **error);
QmiNasGetCellLocInfoOutput*		qmi_nas_get_cell_loc_info_output_ref				(QmiNasGetCellLocInfoOutput *output);
const gint8						qmi_nas_get_cell_loc_info_output_get_ue_in_idle		(QmiNasGetCellLocInfoOutput *output);
const gchar*					qmi_nas_get_cell_loc_info_output_get_plmn			(QmiNasGetCellLocInfoOutput *output);
const guint16					qmi_nas_get_cell_loc_info_output_get_tac			(QmiNasGetCellLocInfoOutput *output);
const guint32					qmi_nas_get_cell_loc_info_output_get_global_cell_id	(QmiNasGetCellLocInfoOutput *output);
const guint16					qmi_nas_get_cell_loc_info_output_get_earfcn			(QmiNasGetCellLocInfoOutput *output);
const guint16					qmi_nas_get_cell_loc_info_output_get_serving_cell_id(QmiNasGetCellLocInfoOutput *output);
const guint8					qmi_nas_get_cell_loc_info_output_get_cell_resel_priority(QmiNasGetCellLocInfoOutput *output);
const guint8					qmi_nas_get_cell_loc_info_output_get_s_non_intra_search(QmiNasGetCellLocInfoOutput *output);
const guint8					qmi_nas_get_cell_loc_info_output_get_thresh_serving_low(QmiNasGetCellLocInfoOutput *output);
const guint8					qmi_nas_get_cell_loc_info_output_get_s_intra_search(QmiNasGetCellLocInfoOutput *output);
const guint8					qmi_nas_get_cell_loc_info_output_get_sets			(QmiNasGetCellLocInfoOutput *output);
const guint16					qmi_nas_get_cell_loc_info_output_get_pci			(QmiNasGetCellLocInfoOutput *output, guint8 index);
const gfloat					qmi_nas_get_cell_loc_info_output_get_rsrq			(QmiNasGetCellLocInfoOutput *output, guint8 index);
const gfloat					qmi_nas_get_cell_loc_info_output_get_rsrp			(QmiNasGetCellLocInfoOutput *output, guint8 index);
const gfloat					qmi_nas_get_cell_loc_info_output_get_rssi			(QmiNasGetCellLocInfoOutput *output, guint8 index);
const gint16					qmi_nas_get_cell_loc_info_output_get_srxlev			(QmiNasGetCellLocInfoOutput *output, guint8 index);

/*****************************************************************************/
/* Get RF Band Information List */
typedef struct _QmiNasGetRfBandOutput QmiNasGetRfBandOutput;

gboolean						qmi_nas_get_rf_band_output_get_result(QmiNasGetRfBandOutput *output, GError **error);
QmiNasGetRfBandOutput*			qmi_nas_get_rf_band_output_ref(QmiNasGetRfBandOutput *output);
void							qmi_nas_get_rf_band_output_unref(QmiNasGetRfBandOutput *output);
const guint8					qmi_nas_get_rf_band_output_get_instances(QmiNasGetRfBandOutput *output);
const gint8						qmi_nas_get_rf_band_output_get_radio_if(QmiNasGetRfBandOutput *output, guint8 index);
const gint16					qmi_nas_get_rf_band_output_get_active_band(QmiNasGetRfBandOutput *output, guint8 index);
const gint16					qmi_nas_get_rf_band_output_get_active_channel(QmiNasGetRfBandOutput *output, guint8 index);

/*****************************************************************************/
/* Get System Info */
typedef struct _QmiNasGetSysInfoOutput QmiNasGetSysInfoOutput;

gboolean						qmi_nas_get_sys_info_output_get_result(QmiNasGetSysInfoOutput *output, GError **error);
QmiNasGetSysInfoOutput*			qmi_nas_get_sys_info_output_ref(QmiNasGetSysInfoOutput *output);
void							qmi_nas_get_sys_info_output_unref(QmiNasGetSysInfoOutput *output);
gboolean						qmi_nas_get_sys_info_output_get_roam_status_valid(QmiNasGetSysInfoOutput *output);
const guint8					qmi_nas_get_sys_info_output_get_roam_status(QmiNasGetSysInfoOutput *output);
gboolean						qmi_nas_get_sys_info_output_get_lac_valid(QmiNasGetSysInfoOutput *output);
const guint16					qmi_nas_get_sys_info_output_get_lac(QmiNasGetSysInfoOutput *output);
gboolean						qmi_nas_get_sys_info_output_get_cell_id_valid(QmiNasGetSysInfoOutput *output);
const guint32					qmi_nas_get_sys_info_output_get_cell_id(QmiNasGetSysInfoOutput *output);
gboolean						qmi_nas_get_sys_info_output_get_network_id_valid(QmiNasGetSysInfoOutput *output);
const guint32					qmi_nas_get_sys_info_output_get_mcc(QmiNasGetSysInfoOutput *output);
const guint32					qmi_nas_get_sys_info_output_get_mnc(QmiNasGetSysInfoOutput *output);
gboolean						qmi_nas_get_sys_info_output_get_tac_valid(QmiNasGetSysInfoOutput *output);
const guint16					qmi_nas_get_sys_info_output_get_tac(QmiNasGetSysInfoOutput *output);



G_END_DECLS

#endif /* _LIBQMI_GLIB_QMI_NAS_H_ */