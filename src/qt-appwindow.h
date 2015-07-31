/* qt-appwindow.h
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

#ifndef QTAPPWINDOW_H
#define QTAPPWINDOW_H

#include <gtk/gtk.h>
#include "qt-application.h"
#include "qt-text-editor.h"

#define QT_APP_WINDOW_TYPE (qt_app_window_get_type ())
#define QT_APP_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), QT_APP_WINDOW_TYPE, QtAppWindow))

typedef struct _QtAppWindow QtAppWindow;
typedef struct _QtAppWindowClass QtAppWindowClass;
typedef struct _QtAppWindowPrivate QtAppWindowPrivate;

GType qt_app_window_get_type (void); 
QtAppWindow * qt_app_window_new (QtApplication * app, QtTextEditor * editor);

QtTextEditor * qt_app_window_get_editor (QtAppWindow * self);
void qt_app_window_create_new_doc (GObject * sender, gpointer data);
void qt_app_window_open_file (GObject * sender, gpointer data);
void qt_app_window_save_doc_to_file (GObject * sender, gpointer data);

void qt_app_window_set_arg_files (QtAppWindow * self, GFile ** arg_files, gint n_files);

#endif /* QTAPPWINDOW_H */
