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

/* NOTE: this is a private non-installable header */

#ifndef _LIBQMI_GLIB_QMI_MESSAGE_DMS_H_
#define _LIBQMI_GLIB_QMI_MESSAGE_DMS_H_

#include <glib.h>

#include "qmi-dms.h"
#include "qmi-message.h"

G_BEGIN_DECLS

/*****************************************************************************/
/* Get IDs */
QmiMessage         *qmi_message_dms_get_ids_new         (guint8 transaction_id,
                                                         guint8 client_id);
QmiDmsGetIdsOutput *qmi_message_dms_get_ids_reply_parse (QmiMessage *self,
                                                         GError **error);

/*****************************************************************************/
/* Get Operation Mode */
QmiMessage         *qmi_message_dms_get_op_mode_new         (guint8 transaction_id,
                                                         guint8 client_id);
QmiDmsGetOpModeOutput *qmi_message_dms_get_op_mode_reply_parse (QmiMessage *self,
                                                         GError **error);

/*****************************************************************************/
/* Get Operation Mode */
QmiMessage         *qmi_message_dms_get_op_mode_new         (guint8 transaction_id,
                                                         guint8 client_id);
QmiDmsGetOpModeOutput *qmi_message_dms_get_op_mode_reply_parse (QmiMessage *self,
                                                         GError **error);

/*****************************************************************************/
/* Get Device Manufacturer */
QmiMessage         *qmi_message_dms_get_dev_mfr_new         (guint8 transaction_id,
                                                         guint8 client_id);
QmiDmsGetDevMfrOutput *qmi_message_dms_get_dev_mfr_reply_parse (QmiMessage *self,
                                                         GError **error);
/*****************************************************************************/
/* Get Device Model ID */
QmiMessage         *qmi_message_dms_get_dev_model_id_new         (guint8 transaction_id,
                                                         guint8 client_id);
QmiDmsGetDevModelIdOutput *qmi_message_dms_get_dev_model_id_reply_parse (QmiMessage *self,
                                                         GError **error);

/*****************************************************************************/
/* Get Device Revision ID */
QmiMessage         *qmi_message_dms_get_dev_rev_id_new         (guint8 transaction_id,
                                                         guint8 client_id);
QmiDmsGetDevRevIdOutput *qmi_message_dms_get_dev_rev_id_reply_parse (QmiMessage *self,
                                                         GError **error);

/*****************************************************************************/
/* Get Device Hardware Revision ID */
QmiMessage         *qmi_message_dms_get_hardware_rev_id_new         (guint8 transaction_id,
                                                         guint8 client_id);
QmiDmsGetDevHardwareRevIdOutput *qmi_message_dms_get_hardware_rev_id_reply_parse (QmiMessage *self,
                                                         GError **error);


/*****************************************************************************/
/* Get MSISDN/ Voice number */
QmiMessage         *qmi_message_dms_get_msisdn_new         (guint8 transaction_id,
                                                         guint8 client_id);
QmiDmsGetMsisdnOutput *qmi_message_dms_get_msisdn_reply_parse (QmiMessage *self,
                                                         GError **error);

/*****************************************************************************/
/* Get SW Version */
QmiMessage*
qmi_message_dms_get_sw_version_new(guint8 transaction_id,
                             	   guint8 client_id);
QmiDmsGetSwVersionOutput*
qmi_message_dms_get_sw_version_reply_parse(QmiMessage *self,
                                     	   GError **error);

G_END_DECLS

#endif /* _LIBQMI_GLIB_QMI_MESSAGE_DMS_H_ */
