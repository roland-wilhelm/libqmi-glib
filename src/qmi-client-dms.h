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

#ifndef _LIBQMI_GLIB_QMI_CLIENT_DMS_H_
#define _LIBQMI_GLIB_QMI_CLIENT_DMS_H_

#include <glib-object.h>

#include "qmi-client.h"
#include "qmi-dms.h"

G_BEGIN_DECLS

#define QMI_TYPE_CLIENT_DMS            (qmi_client_dms_get_type ())
#define QMI_CLIENT_DMS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), QMI_TYPE_CLIENT_DMS, QmiClientDms))
#define QMI_CLIENT_DMS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  QMI_TYPE_CLIENT_DMS, QmiClientDmsClass))
#define QMI_IS_CLIENT_DMS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), QMI_TYPE_CLIENT_DMS))
#define QMI_IS_CLIENT_DMS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  QMI_TYPE_CLIENT_DMS))
#define QMI_CLIENT_DMS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  QMI_TYPE_CLIENT_DMS, QmiClientDmsClass))

typedef struct _QmiClientDms QmiClientDms;
typedef struct _QmiClientDmsClass QmiClientDmsClass;

struct _QmiClientDms {
    QmiClient parent;
    gpointer priv_unused;
};

struct _QmiClientDmsClass {
    QmiClientClass parent;
};

GType qmi_client_dms_get_type (void);

/*****************************************************************************/
/* Get IDs */
void                qmi_client_dms_get_ids        (QmiClientDms *self,
                                                   guint timeout,
                                                   GCancellable *cancellable,
                                                   GAsyncReadyCallback callback,
                                                   gpointer user_data);

QmiDmsGetIdsOutput *qmi_client_dms_get_ids_finish (QmiClientDms *self,
                                                   GAsyncResult *res,
                                                   GError **error);

/*****************************************************************************/
/* Get Operation Mode */
void                qmi_client_dms_get_op_mode (QmiClientDms *self,
                                                   guint timeout,
                                                   GCancellable *cancellable,
                                                   GAsyncReadyCallback callback,
                                                   gpointer user_data);

QmiDmsGetOpModeOutput* qmi_client_dms_get_op_mode_finish (QmiClientDms *self,
                                                   GAsyncResult *res,
                                                   GError **error);

/*****************************************************************************/
/* Get Device Mfr */

QmiDmsGetDevMfrOutput*
qmi_client_dms_get_dev_mfr_finish (QmiClientDms *self,
                               GAsyncResult *res,
                               GError **error);

void
qmi_client_dms_get_dev_mfr (QmiClientDms *self,
                        guint timeout,
                        GCancellable *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer user_data);

/*****************************************************************************/
/* Get Device Model ID */

QmiDmsGetDevModelIdOutput*
qmi_client_dms_get_dev_model_id_finish (QmiClientDms *self,
                               GAsyncResult *res,
                               GError **error);

void
qmi_client_dms_get_dev_model_id (QmiClientDms *self,
                        guint timeout,
                        GCancellable *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer user_data);

/*****************************************************************************/
/* Get Device Revision ID */

QmiDmsGetDevRevIdOutput*
qmi_client_dms_get_dev_rev_id_finish (QmiClientDms *self,
                               GAsyncResult *res,
                               GError **error);


void
qmi_client_dms_get_dev_rev_id (QmiClientDms *self,
                        guint timeout,
                        GCancellable *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer user_data);

/*****************************************************************************/
/* Get Device Hardware Revision ID */

QmiDmsGetDevHardwareRevIdOutput*
qmi_client_dms_get_hardware_id_finish (QmiClientDms *self,
                               GAsyncResult *res,
                               GError **error);

void
qmi_client_dms_get_hardware_id (QmiClientDms *self,
                        guint timeout,
                        GCancellable *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer user_data);

/*****************************************************************************/
/* Get MSISDN/ Voice number */

QmiDmsGetMsisdnOutput*
qmi_client_dms_get_msisdn_finish (QmiClientDms *self,
                               GAsyncResult *res,
                               GError **error);

void
qmi_client_dms_get_msisdn (QmiClientDms *self,
                        guint timeout,
                        GCancellable *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer user_data);

/*****************************************************************************/
/* Get SW Version */

void
qmi_client_dms_get_sw_version (QmiClientDms *self,
                        guint timeout,
                        GCancellable *cancellable,
                        GAsyncReadyCallback callback,
                        gpointer user_data);

QmiDmsGetSwVersionOutput*
qmi_client_dms_get_sw_version_finish (QmiClientDms *self,
                               GAsyncResult *res,
                               GError **error);





G_END_DECLS

#endif /* _LIBQMI_GLIB_QMI_CLIENT_DMS_H_ */
