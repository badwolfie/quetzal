/* qt-document.h
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

#ifndef QT_DOCUMENT_H
#define QT_DOCUMENT_H

#include <gtk/gtk.h>

#include "qt-text-editor.h"
#include "qt-sourceview.h"

#define QT_TYPE_DOCUMENT (qt_document_get_type ())
#define QT_DOCUMENT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), QT_TYPE_DOCUMENT, QtDocument))
#define QT_IS_DOCUMENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), QT_TYPE_DOCUMENT))

typedef struct _QtDocument QtDocument;
typedef struct _QtDocumentClass QtDocumentClass;
typedef struct _QtDocumentPrivate QtDocumentPrivate;

GType qt_document_get_type (void);
QtDocument * qt_document_new (QtTextEditor * editor, GFile * file);

const gchar * qt_document_get_doc_path (QtDocument * self);
void qt_document_set_doc_path (QtDocument * self, const gchar * value);

const gchar * qt_document_get_doc_title (QtDocument * self);
void qt_document_set_doc_title (QtDocument * self, const gchar * value);

gboolean qt_document_get_doc_is_modified (QtDocument * self);
void qt_document_set_doc_is_modified (QtDocument * self, gboolean value);

QtSourceView * qt_document_get_doc_view (QtDocument * self);
GtkScrolledWindow * qt_document_get_doc_scroll (QtDocument * self);

void qt_document_mark_title (QtDocument * self);
void qt_document_refresh_title (QtDocument * self);

#endif /* QT_DOCUMENT_H */
