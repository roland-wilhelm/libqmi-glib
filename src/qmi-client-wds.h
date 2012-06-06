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

#ifndef _LIBQMI_GLIB_QMI_CLIENT_WDS_H_
#define _LIBQMI_GLIB_QMI_CLIENT_WDS_H_

#include <glib-object.h>

#include "qmi-client.h"
#include "qmi-wds.h"

G_BEGIN_DECLS

#define QMI_TYPE_CLIENT_WDS            (qmi_client_wds_get_type ())
#define QMI_CLIENT_WDS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), QMI_TYPE_CLIENT_WDS, QmiClientWds))
#define QMI_CLIENT_WDS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  QMI_TYPE_CLIENT_WDS, QmiClientWdsClass))
#define QMI_IS_CLIENT_WDS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), QMI_TYPE_CLIENT_WDS))
#define QMI_IS_CLIENT_WDS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  QMI_TYPE_CLIENT_WDS))
#define QMI_CLIENT_WDS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  QMI_TYPE_CLIENT_WDS, QmiClientWdsClass))

typedef struct _QmiClientWds QmiClientWds;
typedef struct _QmiClientWdsClass QmiClientWdsClass;

struct _QmiClientWds {
    QmiClient parent;
    gpointer priv_unused;
};

struct _QmiClientWdsClass {
    QmiClientClass parent;
};

GType qmi_client_wds_get_type (void);

/*****************************************************************************/
/* Start network */
void                      qmi_client_wds_start_network        (QmiClientWds *self,
                                                               QmiWdsStartNetworkInput *input,
                                                               guint timeout,
                                                               GCancellable *cancellable,
                                                               GAsyncReadyCallback callback,
                                                               gpointer user_data);
QmiWdsStartNetworkOutput *qmi_client_wds_start_network_finish (QmiClientWds *self,
                                                               GAsyncResult *res,
                                                               GError **error);

/*****************************************************************************/
/* Stop network */
void                      qmi_client_wds_stop_network        (QmiClientWds *self,
                                                              QmiWdsStopNetworkInput *input,
                                                              guint timeout,
                                                              GCancellable *cancellable,
                                                              GAsyncReadyCallback callback,
                                                              gpointer user_data);
QmiWdsStopNetworkOutput *qmi_client_wds_stop_network_finish (QmiClientWds *self,
                                                             GAsyncResult *res,
                                                             GError **error);

/*****************************************************************************/
/* Get packet service status */
void                                qmi_client_wds_get_packet_service_status        (QmiClientWds *self,
                                                                                     gpointer input_unused,
                                                                                     guint timeout,
                                                                                     GCancellable *cancellable,
                                                                                     GAsyncReadyCallback callback,
                                                                                     gpointer user_data);
QmiWdsGetPacketServiceStatusOutput *qmi_client_wds_get_packet_service_status_finish (QmiClientWds *self,
                                                                                     GAsyncResult *res,
                                                                                     GError **error);

/*****************************************************************************/
/* Get data bearer technology */
void                                qmi_client_wds_get_data_bearer_technology        (QmiClientWds *self,
                                                                                      gpointer input_unused,
                                                                                      guint timeout,
                                                                                      GCancellable *cancellable,
                                                                                      GAsyncReadyCallback callback,
                                                                                      gpointer user_data);
QmiWdsGetDataBearerTechnologyOutput *qmi_client_wds_get_data_bearer_technology_finish (QmiClientWds *self,
                                                                                       GAsyncResult *res,
                                                                                       GError **error);

/*****************************************************************************/
/* Get current data bearer technology */
void                                        qmi_client_wds_get_current_data_bearer_technology        (QmiClientWds *self,
                                                                                                      gpointer input_unused,
                                                                                                      guint timeout,
                                                                                                      GCancellable *cancellable,
                                                                                                      GAsyncReadyCallback callback,
                                                                                                      gpointer user_data);
QmiWdsGetCurrentDataBearerTechnologyOutput *qmi_client_wds_get_current_data_bearer_technology_finish (QmiClientWds *self,
                                                                                                      GAsyncResult *res,
                                                                                                      GError **error);

/*****************************************************************************/
/* Get Dun Call Info */

QmiWdsDunCallOutput*
qmi_client_wds_get_dun_call_finish(QmiClientWds *self, GAsyncResult *res, GError **error);

void
qmi_client_wds_get_dun_call(QmiClientWds *self,
							guint timeout,
							GCancellable *cancellable,
							GAsyncReadyCallback callback,
							gpointer user_data);



G_END_DECLS

#endif /* _LIBQMI_GLIB_QMI_CLIENT_WDS_H_ */
