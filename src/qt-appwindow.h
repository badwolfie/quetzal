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

#endif /* QTAPPWINDOW_H */
