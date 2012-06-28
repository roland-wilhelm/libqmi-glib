/*
 * qmi-message-nas.h
 *
 *  Created on: 23.05.2012
 *      Author: roland
 *
 * NOTE: this is a private non-installable header
 */

#ifndef _LIBQMI_GLIB_QMI_MESSAGE_NAS_H_
#define _LIBQMI_GLIB_QMI_MESSAGE_NAS_H_


#include <glib.h>

#include "qmi-nas.h"
#include "qmi-message.h"


G_BEGIN_DECLS

/*****************************************************************************/
/* Get Sig Info */

QmiMessage*						qmi_message_nas_get_sig_info_new         (guint8 transaction_id, guint8 client_id);
QmiNasGetSigInfoOutput*			qmi_message_nas_get_sig_info_reply_parse (QmiMessage *self, GError **error);


/*****************************************************************************/
/* Get Cell Location Info */

QmiMessage*						qmi_message_nas_get_cell_loc_info_new         (guint8 transaction_id, guint8 client_id);
QmiNasGetCellLocInfoOutput*			qmi_message_nas_get_cell_loc_info_reply_parse (QmiMessage *self, GError **error);

/*****************************************************************************/
/* Get RF Band Information List */

QmiMessage*							qmi_message_nas_get_rf_band_new         (guint8 transaction_id, guint8 client_id);
QmiNasGetRfBandOutput*				qmi_message_nas_get_rf_band_reply_parse (QmiMessage *self, GError **error);

/*****************************************************************************/
/* Get System Info */
QmiMessage*							qmi_message_nas_get_sys_info_new         (guint8 transaction_id, guint8 client_id);
QmiNasGetSysInfoOutput*				qmi_message_nas_get_sys_info_reply_parse (QmiMessage *self, GError **error);

/*****************************************************************************/
/* Get Get Signal Strength */
QmiMessage*							qmi_message_nas_get_sig_strength_new         (guint8 transaction_id, guint8 client_id, GError **error);
QmiNasGetSigStrengthOutput*				qmi_message_nas_get_sig_strength_reply_parse (QmiMessage *self, GError **error);

G_END_DECLS


#endif /* _LIBQMI_GLIB_QMI_MESSAGE_NAS_H_ */
