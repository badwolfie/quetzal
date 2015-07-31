/* qt-documentbar.h
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

#ifndef QT_DOCUMENT_BAR_H
#define QT_DOCUMENT_BAR_H

#include <gtk/gtk.h>
#include "qt-appwindow.h"
#include "qt-document.h"

#define QT_DOCUMENT_BAR_TYPE (qt_document_bar_get_type ())
#define QT_DOCUMENT_BAR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), QT_DOCUMENT_BAR_TYPE, QtDocumentBar))

typedef struct _QtDocumentBar QtDocumentBar;
typedef struct _QtDocumentBarClass QtDocumentBarClass;
typedef struct _QtDocumentBarPrivate QtDocumentBarPrivate;

GType qt_document_bar_get_type (void);
QtDocumentBar * qt_document_bar_new (void);

void qt_document_bar_set_stack (QtDocumentBar * self, GtkStack * stack);
void qt_document_bar_add_doc (QtDocumentBar * self, QtDocument * doc);

gboolean qt_document_bar_doc_is_opened (QtDocumentBar * self, GFile * file);
QtDocument * qt_document_bar_get_current_doc (QtDocumentBar * self);

GList * qt_document_bar_get_doc_list (QtDocumentBar * self);
GList * qt_document_bar_get_extra_doc_list (QtDocumentBar * self);

#endif /* QT_TAB_BAR_H */
