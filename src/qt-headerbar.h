/* qt-headerbar.h
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

#ifndef QT_HEADERBAR_H
#define QT_HEADERBAR_H

#include <gtk/gtk.h>
#include "qt-appwindow.h"

#define QT_HEADER_BAR_TYPE (qt_header_bar_get_type ())
#define QT_HEADER_BAR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), QT_HEADER_BAR_TYPE, QtHeaderBar))

typedef struct _QtHeaderBar QtHeaderBar;
typedef struct _QtHeaderBarClass QtHeaderBarClass;
typedef struct _QtHeaderBarPrivate QtHeaderBarPrivate;

GType qt_header_bar_get_type (void);
QtHeaderBar * qt_header_bar_new (QtAppWindow * window);

void qt_header_bar_toggle_fullscreen (QtHeaderBar * self);

#endif /* QT_HEADERBAR_H */
