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

#ifndef _LIBQMI_GLIB_QMI_DEVICE_H_
#define _LIBQMI_GLIB_QMI_DEVICE_H_

#include <glib-object.h>
#include <gio/gio.h>
#include "qmi-enums.h"
#include "qmi-message.h"
#include "qmi-client.h"

G_BEGIN_DECLS

#define QMI_TYPE_DEVICE            (qmi_device_get_type ())
#define QMI_DEVICE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), QMI_TYPE_DEVICE, QmiDevice))
#define QMI_DEVICE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  QMI_TYPE_DEVICE, QmiDeviceClass))
#define QMI_IS_DEVICE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), QMI_TYPE_DEVICE))
#define QMI_IS_DEVICE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  QMI_TYPE_DEVICE))
#define QMI_DEVICE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  QMI_TYPE_DEVICE, QmiDeviceClass))

typedef struct _QmiDevice QmiDevice;
typedef struct _QmiDeviceClass QmiDeviceClass;
typedef struct _QmiDevicePrivate QmiDevicePrivate;

#ifndef _QMICLIENT
#define _QMICLIENT
typedef struct _QmiClient QmiClient;
#endif


#define QMI_DEVICE_FILE       "device-file"
#define QMI_DEVICE_CLIENT_CTL "device-client-ctl"

struct _QmiDevice {
    GObject parent;
    QmiDevicePrivate *priv;
};

struct _QmiDeviceClass {
    GObjectClass parent;
};

GType qmi_device_get_type (void);

void       qmi_device_new       (GFile *file,
                                 GCancellable *cancellable,
                                 GAsyncReadyCallback callback,
                                 gpointer user_data);
QmiDevice *qmi_device_new_finish (GAsyncResult *res,
                                  GError **error);

GFile        *qmi_device_get_file         (QmiDevice *self);
GFile        *qmi_device_peek_file        (QmiDevice *self);
const gchar  *qmi_device_get_path         (QmiDevice *self);
const gchar  *qmi_device_get_path_display (QmiDevice *self);
gboolean      qmi_device_is_open          (QmiDevice *self);

/**
 * QmiDeviceOpenFlags:
 * @QMI_DEVICE_OPEN_FLAGS_NONE: No flags.
 * @QMI_DEVICE_OPEN_FLAGS_VERSION_INFO: Run version info check when opening.
 * @QMI_DEVICE_OPEN_FLAGS_SYNC: Synchronize with endpoint once the device is open. Will release any previously allocated client ID.
 *
 * Flags to specify which actions to be performed when the device is open.
 */
typedef enum {
    QMI_DEVICE_OPEN_FLAGS_NONE         = 0,
    QMI_DEVICE_OPEN_FLAGS_VERSION_INFO = 1 << 0,
    QMI_DEVICE_OPEN_FLAGS_SYNC         = 1 << 1
} QmiDeviceOpenFlags;

void         qmi_device_open        (QmiDevice *self,
                                     QmiDeviceOpenFlags flags,
                                     guint timeout,
                                     GCancellable *cancellable,
                                     GAsyncReadyCallback callback,
                                     gpointer user_data);
gboolean     qmi_device_open_finish (QmiDevice *self,
                                     GAsyncResult *res,
                                     GError **error);

gboolean     qmi_device_close (QmiDevice *self,
                               GError **error);

void          qmi_device_allocate_client        (QmiDevice *self,
                                                 QmiService service,
                                                 guint8 cid,
                                                 guint timeout,
                                                 GCancellable *cancellable,
                                                 GAsyncReadyCallback callback,
                                                 gpointer user_data);

QmiClient    *qmi_device_allocate_client_finish (QmiDevice *self,
                                                 GAsyncResult *res,
                                                 GError **error);

/**
 * QmiDeviceReleaseClientFlags:
 * @QMI_DEVICE_RELEASE_CLIENT_FLAGS_NONE: No flags.
 * @QMI_DEVICE_RELEASE_CLIENT_FLAGS_RELEASE_CID: Release the CID when releasing the client.
 *
 * Flags to specify which actions to be performed when releasing the client.
 */
typedef enum {
    QMI_DEVICE_RELEASE_CLIENT_FLAGS_NONE        = 0,
    QMI_DEVICE_RELEASE_CLIENT_FLAGS_RELEASE_CID = 1 << 0
} QmiDeviceReleaseClientFlags;

void          qmi_device_release_client        (QmiDevice *self,
                                                QmiClient *client,
                                                QmiDeviceReleaseClientFlags flags,
                                                guint timeout,
                                                GCancellable *cancellable,
                                                GAsyncReadyCallback callback,
                                                gpointer user_data);
gboolean      qmi_device_release_client_finish (QmiDevice *self,
                                                GAsyncResult *res,
                                                GError **error);

void         qmi_device_command        (QmiDevice *self,
                                        QmiMessage *message,
                                        guint timeout,
                                        GCancellable *cancellable,
                                        GAsyncReadyCallback callback,
                                        gpointer user_data);
QmiMessage  *qmi_device_command_finish (QmiDevice *self,
                                        GAsyncResult *res,
                                        GError **error);

G_END_DECLS

#endif /* _LIBQMI_GLIB_QMI_DEVICE_H_ */
