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

#endif /* QT_TAB_BAR_H */