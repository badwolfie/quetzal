/* qt-sourceview.h
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

#ifndef QT_SOURCE_VIEW_H
#define QT_SOURCE_VIEW_H

#include <gtksourceview/gtksource.h>
#include <gtk/gtk.h>

#include "qt-text-editor.h"

#define QT_SOURCE_VIEW_TYPE (qt_source_view_get_type ())
#define QT_SOURCE_VIEW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), QT_SOURCE_VIEW_TYPE, QtSourceView))

typedef struct _QtSourceView QtSourceView;
typedef struct _QtSourceViewClass QtSourceViewClass;
typedef struct _QtSourceViewPrivate QtSourceViewPrivate;

GType qt_source_view_get_type (void);
QtSourceView * qt_source_view_new (QtTextEditor * editor, GFile * file);

const gchar * qt_source_view_get_language_name (QtSourceView * self);
GtkSourceFile * qt_source_view_get_source_file (QtSourceView * self);

GtkSourceBuffer * qt_source_view_get_source_buffer (QtSourceView * self);
GtkSourceSearchSettings * qt_source_view_get_search_settings (QtSourceView * self);

void qt_source_view_save_file (QtSourceView * self, GFile * target_file);
void qt_source_view_change_language (QtSourceView * self, const gchar * language);

#endif /* QT_SOURCE_VIEW_H */
