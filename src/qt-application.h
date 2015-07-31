/* qt-application.h
 *
 * Copyright (C) 2015 Ian Hernandez <ihernandezs@openmailbox.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QTAPPLICATION_H
#define QTAPPLICATION_H

#include <gtk/gtk.h>

#define QT_APPLICATION_TYPE (qt_application_get_type ())
#define QT_APPLICATION(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), QT_APPLICATION_TYPE, QtApplication))

typedef struct _QtApplication QtApplication;
typedef struct _QtApplicationClass QtApplicationClass;
typedef struct _QtApplicationPrivate QtApplicationPrivate;

GType qt_application_get_type (void);
QtApplication * qt_application_new (void);

#endif /* QTAPPLICATION_H */
