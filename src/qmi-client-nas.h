/*
 * qmi-client-nas.h
 *
 *  Created on: 24.05.2012
 *      Author: roland
 */


#ifndef _LIBQMI_GLIB_QMI_CLIENT_NAS_H_
#define _LIBQMI_GLIB_QMI_CLIENT_NAS_H_

#include <glib-object.h>
#include <gio/gio.h>
#include "qmi-client.h"
#include "qmi-nas.h"

G_BEGIN_DECLS

#define QMI_TYPE_CLIENT_NAS            (qmi_client_nas_get_type ())
#define QMI_CLIENT_NAS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), QMI_TYPE_CLIENT_NAS, QmiClientNas))
#define QMI_CLIENT_NAS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  QMI_TYPE_CLIENT_NAS, QmiClientNasClass))
#define QMI_IS_CLIENT_NAS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), QMI_TYPE_CLIENT_NAS))
#define QMI_IS_CLIENT_NAS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  QMI_TYPE_CLIENT_NAS))
#define QMI_CLIENT_NAS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  QMI_TYPE_CLIENT_NAS, QmiClientNasClass))

typedef struct _QmiClientNas QmiClientNas;
typedef struct _QmiClientNasClass QmiClientNasClass;

struct _QmiClientNas {
    QmiClient parent;
    gpointer priv_unused;
};

struct _QmiClientNasClass {
    QmiClientClass parent;
};

GType qmi_client_nas_get_type (void);

/*****************************************************************************/
/* Get Sig Info */
void                	qmi_client_nas_get_sig_info			(QmiClientNas *self,
                                                   	     	 guint timeout,
                                                   	     	 GCancellable *cancellable,
                                                   	     	 GAsyncReadyCallback callback,
                                                   	     	 gpointer user_data);

QmiNasGetSigInfoOutput*	qmi_client_nas_get_sig_info_finish	(QmiClientNas *self,
                                                   	   	 	 GAsyncResult *res,
                                                   	   	 	 GError **error);

/*****************************************************************************/
/* Get Cell Location Info */
void                	qmi_client_nas_get_cell_loc_info	(QmiClientNas *self,
                                                   	     	 guint timeout,
                                                   	     	 GCancellable *cancellable,
                                                   	     	 GAsyncReadyCallback callback,
                                                   	     	 gpointer user_data);

QmiNasGetCellLocInfoOutput*	qmi_client_nas_get_cell_loc_info_finish	(QmiClientNas *self,
                                                   	   	 	 GAsyncResult *res,
                                                   	   	 	 GError **error);

/*****************************************************************************/
/* Get RF Band Info */
void						qmi_client_nas_get_rf_band(QmiClientNas *self,
														guint timeout,
														GCancellable *cancellable,
														GAsyncReadyCallback callback,
														gpointer user_data);

QmiNasGetRfBandOutput*		qmi_client_nas_get_rf_band_finish(QmiClientNas *self, GAsyncResult *res, GError **error);

/*****************************************************************************/
/* Get System Info */

QmiNasGetSysInfoOutput*
qmi_client_nas_get_sys_info_finish(QmiClientNas *self, GAsyncResult *res, GError **error);

void
qmi_client_nas_get_sys_info(QmiClientNas *self,
							guint timeout,
							GCancellable *cancellable,
							GAsyncReadyCallback callback,
							gpointer user_data);



G_END_DECLS



#endif /* _LIBQMI_GLIB_QMI_CLIENT_NAS_H_ */
