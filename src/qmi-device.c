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

#include <gio/gio.h>

#include "qmi-device.h"
#include "qmi-error-types.h"

static void async_initable_iface_init (GAsyncInitableIface *iface);

G_DEFINE_TYPE_EXTENDED (QmiDevice, qmi_device, G_TYPE_OBJECT, 0,
                        G_IMPLEMENT_INTERFACE (G_TYPE_ASYNC_INITABLE, async_initable_iface_init));

enum {
    PROP_0,
    PROP_FILE,
    PROP_LAST
};

static GParamSpec *properties[PROP_LAST];

struct _QmiDevicePrivate {
    /* File */
    GFile *file;
    gchar *path;
    gchar *path_display;
};

/*****************************************************************************/

/**
 * qmi_device_get_file:
 * @self: a #QmiDevice.
 *
 * Get the #GFile associated with this #QmiDevice.
 *
 * Returns: a #GFile that must be freed with g_object_unref().
 */
GFile *
qmi_device_get_file (QmiDevice *self)
{
    GFile *file = NULL;

    g_return_val_if_fail (QMI_IS_DEVICE (self), NULL);

    g_object_get (G_OBJECT (self),
                  QMI_DEVICE_FILE, &file,
                  NULL);
    return file;
}

/**
 * qmi_device_peek_file:
 * @self: a #QmiDevice.
 *
 * Get the #GFile associated with this #QmiDevice, without increasing the reference count
 * on the returned object.
 *
 * Returns: a #GFile. Do not free the returned object, it is owned by @self.
 */
GFile *
qmi_device_peek_file (QmiDevice *self)
{
    g_return_val_if_fail (QMI_IS_DEVICE (self), NULL);

    return self->priv->file;
}

/**
 * qmi_device_get_path:
 * @self: a #QmiDevice.
 *
 * Get the system path of the underlying QMI device.
 *
 * Returns: the system path of the device.
 */
const gchar *
qmi_device_get_path (QmiDevice *self)
{
    g_return_val_if_fail (QMI_IS_DEVICE (self), NULL);

    return self->priv->path;
}

/**
 * qmi_device_get_path_display:
 * @self: a #QmiDevice.
 *
 * Get the system path of the underlying QMI device in UTF-8.
 *
 * Returns: UTF-8 encoded system path of the device.
 */
const gchar *
qmi_device_get_path_display (QmiDevice *self)
{
    g_return_val_if_fail (QMI_IS_DEVICE (self), NULL);

    return self->priv->path_display;
}

/*****************************************************************************/
/* New QMI device */

/**
 * qmi_device_new_finish:
 * @res: a #GAsyncResult.
 * @error: a #GError.
 *
 * Finishes an operation started with qmi_device_new().
 *
 * Returns: A newly created #QmiDevice, or #NULL if @error is set.
 */
QmiDevice *
qmi_device_new_finish (GAsyncResult *res,
                       GError **error)
{
  GObject *ret;
  GObject *source_object;

  source_object = g_async_result_get_source_object (res);
  ret = g_async_initable_new_finish (G_ASYNC_INITABLE (source_object), res, error);
  g_object_unref (source_object);

  return (ret ? QMI_DEVICE (ret) : NULL);
}

/**
 * qmi_device_new:
 * @file: a #GFile.
 * @cancellable: optional #GCancellable object, #NULL to ignore.
 * @callback: a #GAsyncReadyCallback to call when the initialization is finished.
 * @user_data: the data to pass to callback function.
 *
 * Asynchronously creates a #QmiDevice object to manage @file.
 * When the operation is finished, @callback will be invoked. You can then call
 * qmi_device_new_finish() to get the result of the operation.
 */
void
qmi_device_new (GFile *file,
                GCancellable *cancellable,
                GAsyncReadyCallback callback,
                gpointer user_data)
{
    g_async_initable_new_async (QMI_TYPE_DEVICE,
                                G_PRIORITY_DEFAULT,
                                cancellable,
                                callback,
                                user_data,
                                QMI_DEVICE_FILE, file,
                                NULL);
}

/*****************************************************************************/
/* Async init */

typedef struct {
    QmiDevice *self;
    GSimpleAsyncResult *result;
    GCancellable *cancellable;
} InitContext;

static void
init_context_complete_and_free (InitContext *ctx)
{
    g_simple_async_result_complete_in_idle (ctx->result);
    if (ctx->cancellable)
        g_object_unref (ctx->cancellable);
    g_object_unref (ctx->result);
    g_object_unref (ctx->self);
    g_slice_free (InitContext, ctx);
}

static gboolean
initable_init_finish (GAsyncInitable  *initable,
                      GAsyncResult    *result,
                      GError         **error)
{
    return !g_simple_async_result_propagate_error (G_SIMPLE_ASYNC_RESULT (result), error);
}

static void
query_info_async_ready (GFile *file,
                        GAsyncResult *res,
                        InitContext *ctx)
{
    GError *error = NULL;
    GFileInfo *info;

    info = g_file_query_info_finish (file, res, &error);
    if (!info) {
        g_prefix_error (&error,
                        "Couldn't query file info: ");
        g_simple_async_result_take_error (ctx->result, error);
        init_context_complete_and_free (ctx);
        return;
    }

    /* Our QMI device must be of SPECIAL type */
    if (g_file_info_get_file_type (info) != G_FILE_TYPE_SPECIAL) {
        g_simple_async_result_set_error (ctx->result,
                                         QMI_CORE_ERROR,
                                         QMI_CORE_ERROR_FAILED,
                                         "Wrong file type");
        init_context_complete_and_free (ctx);
        return;
    }

    g_object_unref (info);

    /* Done we are */
    g_simple_async_result_set_op_res_gboolean (ctx->result, TRUE);
    init_context_complete_and_free (ctx);
}

static void
initable_init_async (GAsyncInitable *initable,
                     int io_priority,
                     GCancellable *cancellable,
                     GAsyncReadyCallback callback,
                     gpointer user_data)
{
    InitContext *ctx;

    ctx = g_slice_new0 (InitContext);
    ctx->self = g_object_ref (initable);
    if (cancellable)
        ctx->cancellable = g_object_ref (cancellable);
    ctx->result = g_simple_async_result_new (G_OBJECT (initable),
                                             callback,
                                             user_data,
                                             initable_init_async);

    /* We need a proper file to initialize */
    if (!ctx->self->priv->file) {
        g_simple_async_result_set_error (ctx->result,
                                         QMI_CORE_ERROR,
                                         QMI_CORE_ERROR_INVALID_ARGS,
                                         "Cannot initialize QMI device: No file given");
        init_context_complete_and_free (ctx);
        return;
    }

    /* Check the file type. Note that this is just a quick check to avoid
     * creating QmiDevices pointing to a location already known not to be a QMI
     * device. */
    g_file_query_info_async (ctx->self->priv->file,
                             G_FILE_ATTRIBUTE_STANDARD_TYPE,
                             G_FILE_QUERY_INFO_NONE,
                             G_PRIORITY_DEFAULT,
                             ctx->cancellable,
                             (GAsyncReadyCallback)query_info_async_ready,
                             ctx);
}

/*****************************************************************************/

static void
set_property (GObject *object,
              guint prop_id,
              const GValue *value,
              GParamSpec *pspec)
{
    QmiDevice *self = QMI_DEVICE (object);

    switch (prop_id) {
    case PROP_FILE:
        g_assert (self->priv->file == NULL);
        self->priv->file = g_value_dup_object (value);
        self->priv->path = g_file_get_path (self->priv->file);
        self->priv->path_display = g_filename_display_name (self->priv->path);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
get_property (GObject *object,
              guint prop_id,
              GValue *value,
              GParamSpec *pspec)
{
    QmiDevice *self = QMI_DEVICE (object);

    switch (prop_id) {
    case PROP_FILE:
        g_value_set_object (value, self->priv->file);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
qmi_device_init (QmiDevice *self)
{
    self->priv = G_TYPE_INSTANCE_GET_PRIVATE ((self),
                                              QMI_TYPE_DEVICE,
                                              QmiDevicePrivate);
}

static void
dispose (GObject *object)
{
    QmiDevice *self = QMI_DEVICE (object);

    g_clear_object (&self->priv->file);

    G_OBJECT_CLASS (qmi_device_parent_class)->dispose (object);
}

static void
finalize (GObject *object)
{
    QmiDevice *self = QMI_DEVICE (object);

    g_free (self->priv->path);
    g_free (self->priv->path_display);

    G_OBJECT_CLASS (qmi_device_parent_class)->finalize (object);
}

static void
async_initable_iface_init (GAsyncInitableIface *iface)
{
    iface->init_async = initable_init_async;
    iface->init_finish = initable_init_finish;
}

static void
qmi_device_class_init (QmiDeviceClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (object_class, sizeof (QmiDevicePrivate));

    object_class->get_property = get_property;
    object_class->set_property = set_property;
    object_class->finalize = finalize;
    object_class->dispose = dispose;

    properties[PROP_FILE] =
        g_param_spec_object (QMI_DEVICE_FILE,
                             "Device file",
                             "File to the underlying QMI device",
                             G_TYPE_FILE,
                             G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
    g_object_class_install_property (object_class, PROP_FILE, properties[PROP_FILE]);
}
