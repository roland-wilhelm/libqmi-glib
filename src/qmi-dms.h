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
 * Copyright (C) 2012 Aleksander Morgado <aleksander@gnu.org>
 */

#ifndef _LIBQMI_GLIB_QMI_DMS_H_
#define _LIBQMI_GLIB_QMI_DMS_H_

#include <glib.h>

#include "qmi-enums.h"

G_BEGIN_DECLS

/*****************************************************************************/
/* Supported/known messages */
typedef enum {
    QMI_DMS_MESSAGE_EVENT              				= 	0x0001, /* v1.0 needed, unused currently */
    QMI_DMS_MESSAGE_GET_GET_DEVICE_CAP				=	0x0020, /* v1.0 needed, unused currently */
    QMI_DMS_MESSAGE_GET_DEVICE_MFR	  				= 	0x0021, /* v1.0 needed */
    QMI_DMS_MESSAGE_GET_DEVICE_MODEL_ID				=	0x0022, /* v1.0 needed */
    QMI_DMS_MESSAGE_GET_DEVICE_REV_ID				=	0x0023, /* v1.0 needed */
    QMI_DMS_MESSAGE_GET_DEVICE_MSISDN				=	0x0024,	/* v1.0 needed */
    QMI_DMS_MESSAGE_GET_IDS            				= 	0x0025, /* v1.0 needed */
    QMI_DMS_MESSAGE_GET_DEVICE_HARDWARE_REV			=	0x002C, /* v1.2 needed */
    QMI_DMS_MESSAGE_GET_OPERATING_MODE 				= 	0x002D, /* v1.2 needed */
    QMI_DMS_MESSAGE_SET_OPERATING_MODE 				= 	0x002E, /* v1.2 needed, unused currently */
    QMI_DMS_MESSAGE_GET_BAND_CAPABILITY				=	0x0045, /* v1.3 needed, unused currently */
    QMI_DMS_MESSAGE_GET_SW_VERSION					=	0x0051  /* v1.5 needed, unused currently */
} QmiDmsMessage;

/*****************************************************************************/
/* Get IDs */
typedef struct _QmiDmsGetIdsOutput QmiDmsGetIdsOutput;
QmiDmsGetIdsOutput *qmi_dms_get_ids_output_ref        (QmiDmsGetIdsOutput *output);
void                qmi_dms_get_ids_output_unref      (QmiDmsGetIdsOutput *output);
gboolean            qmi_dms_get_ids_output_get_result (QmiDmsGetIdsOutput *output,
                                                       GError **error);
const gchar        *qmi_dms_get_ids_output_get_esn    (QmiDmsGetIdsOutput *output);
const gchar        *qmi_dms_get_ids_output_get_imei   (QmiDmsGetIdsOutput *output);
const gchar        *qmi_dms_get_ids_output_get_meid   (QmiDmsGetIdsOutput *output);

/*****************************************************************************/
/* Get Operation Mode */

typedef struct _QmiDmsGetOpModeOutput QmiDmsGetOpModeOutput;
QmiDmsGetOpModeOutput*		qmi_dms_get_op_mode_output_ref        			(QmiDmsGetOpModeOutput *output);
void                		qmi_dms_get_op_mode_output_unref      			(QmiDmsGetOpModeOutput *output);
gboolean            		qmi_dms_get_op_mode_output_get_result 			(QmiDmsGetOpModeOutput *output,
                                                       	   	   	   	   	   	 GError **error);
const gint8					qmi_dms_get_op_mode_output_get_version  		(QmiDmsGetOpModeOutput *output);
const gchar*				qmi_dms_get_op_mode_output_get_version_string	(QmiDmsGetOpModeOutput *output);

/*****************************************************************************/
/* Get Device Properties
 *
 * + Device Manufacturer
 * + Device Model ID
 * + Device Revision ID
 * + Device Hardware Revision
 * + MSISDN/ Voice Number
 */

/*****************************************************************************/
/* Get Device Manufacturer */

typedef struct _QmiDmsGetDevMfrOutput QmiDmsGetDevMfrOutput;

QmiDmsGetDevMfrOutput*		qmi_dms_get_dev_mfr_output_ref        			(QmiDmsGetDevMfrOutput *output);
void                		qmi_dms_get_dev_mfr_output_unref      			(QmiDmsGetDevMfrOutput *output);
gboolean            		qmi_dms_get_dev_mfr_output_get_result 			(QmiDmsGetDevMfrOutput *output, GError **error);
const gchar*				qmi_dms_get_dev_mfr_output_get_dev_mfr			(QmiDmsGetDevMfrOutput *output);


/*****************************************************************************/
/* Get Device Model ID */
typedef struct _QmiDmsGetDevModelIdOutput QmiDmsGetDevModelIdOutput;

QmiDmsGetDevModelIdOutput*	qmi_dms_get_dev_model_id_output_ref        			(QmiDmsGetDevModelIdOutput *output);
void                		qmi_dms_get_dev_model_id_output_unref      			(QmiDmsGetDevModelIdOutput *output);
gboolean            		qmi_dms_get_dev_model_id_output_get_result 			(QmiDmsGetDevModelIdOutput *output, GError **error);
const gchar*				qmi_dms_get_dev_model_id_output_get_dev_model_id	(QmiDmsGetDevModelIdOutput *output);

/*****************************************************************************/
/* Get Device Revision ID */
typedef struct _QmiDmsGetDevRevIdOutput QmiDmsGetDevRevIdOutput;

QmiDmsGetDevRevIdOutput*	qmi_dms_get_dev_rev_id_output_ref        			(QmiDmsGetDevRevIdOutput *output);
void                		qmi_dms_get_dev_rev_id_output_unref      			(QmiDmsGetDevRevIdOutput *output);
gboolean            		qmi_dms_get_dev_rev_id_output_get_result 			(QmiDmsGetDevRevIdOutput *output, GError **error);
const gchar*				qmi_dms_get_dev_rev_id_output_get_dev_rev_id		(QmiDmsGetDevRevIdOutput *output);

/*****************************************************************************/
/* Get Device Hardware Revision ID */
typedef struct _QmiDmsGetDevHardwareRevIdOutput QmiDmsGetDevHardwareRevIdOutput;

QmiDmsGetDevHardwareRevIdOutput*	qmi_dms_get_hardware_rev_id_output_ref    	(QmiDmsGetDevHardwareRevIdOutput *output);
void                		qmi_dms_get_hardware_rev_id_output_unref      		(QmiDmsGetDevHardwareRevIdOutput *output);
gboolean            		qmi_dms_get_hardware_rev_id_output_get_result 		(QmiDmsGetDevHardwareRevIdOutput *output, GError **error);
const gchar*				qmi_dms_get_hardware_rev_output_get_hardware_rev	(QmiDmsGetDevHardwareRevIdOutput *output);

/*****************************************************************************/
/* Get MSISDN/ Voice number */
typedef struct _QmiDmsGetMsisdnOutput QmiDmsGetMsisdnOutput;

QmiDmsGetMsisdnOutput*		qmi_dms_get_msisdn_output_ref        			(QmiDmsGetMsisdnOutput *output);
void                		qmi_dms_get_msisdn_output_unref      			(QmiDmsGetMsisdnOutput *output);
gboolean            		qmi_dms_get_msisdn_output_get_result 			(QmiDmsGetMsisdnOutput *output, GError **error);
const gchar*				qmi_dms_get_msisdn_output_get_voice_number	(QmiDmsGetMsisdnOutput *output);
const gchar*				qmi_dms_get_msisdn_output_get_mobile_id		(QmiDmsGetMsisdnOutput *output);
const gchar*				qmi_dms_get_msisdn_output_get_imsi			(QmiDmsGetMsisdnOutput *output);

/*****************************************************************************/
/* Get SW Version */
typedef struct _QmiDmsGetSwVersionOutput QmiDmsGetSwVersionOutput;

QmiDmsGetSwVersionOutput*	qmi_dms_get_sw_version_output_ref        			(QmiDmsGetSwVersionOutput *output);
void                		qmi_dms_get_sw_version_output_unref      			(QmiDmsGetSwVersionOutput *output);
gboolean            		qmi_dms_get_sw_version_output_get_result 			(QmiDmsGetSwVersionOutput *output, GError **error);
const gchar*				qmi_dms_get_sw_version_output_get_sw_version		(QmiDmsGetSwVersionOutput *output);

G_END_DECLS

#endif /* _LIBQMI_GLIB_QMI_DMS_H_ */
