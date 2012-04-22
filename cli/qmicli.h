/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * qmicli -- Command line interface to control QMI devices
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2012 Aleksander Morgado <aleksander@gnu.org>
 */

#include <glib.h>

#ifndef __QMICLI_H__
#define __QMICLI_H__

/* Common */
void          qmicli_async_operation_done  (void);

/* CTL group */
GOptionGroup *qmicli_ctl_get_option_group (void);
gboolean      qmicli_ctl_options_enabled  (void);
void          qmicli_ctl_run              (QmiDevice *device,
                                           GCancellable *cancellable);

/* DMS group */
GOptionGroup *qmicli_dms_get_option_group (void);
gboolean      qmicli_dms_options_enabled  (void);
void          qmicli_dms_run              (QmiDevice *device,
                                           GCancellable *cancellable);

#endif /* __QMICLI_H__ */
