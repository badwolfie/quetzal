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
QtSourceView * qt_source_view_new (QtTextEditor * editor, const gchar * filename);

#endif /* QT_SOURCE_VIEW_H */
