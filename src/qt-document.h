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

#endif /* QT_DOCUMENT_H */
