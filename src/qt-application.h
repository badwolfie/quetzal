#ifndef QTAPPLICATION_H
#define QTAPPLICATION_H

#include <gtk/gtk.h>

#define QT_APPLICATION_TYPE (qt_application_get_type ())
#define QT_APPLICATION(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), QT_APPLICATION_TYPE, QtApplication))

#define APP_NAME "Quetzal"
#define APP_VERSION "0.11.1"
#define APP_SHORTDESC "A simple and lightweight text and code editor."

typedef struct _QtApplication QtApplication;
typedef struct _QtApplicationClass QtApplicationClass;
typedef struct _QtApplicationPrivate QtApplicationPrivate;

GType qt_application_get_type (void);
QtApplication * qt_application_new (void);

static void qt_application_init (QtApplication * self);
static void qt_application_class_init (QtApplicationClass * class);

static void qt_application_startup (GApplication * self);
static void qt_application_activate (GApplication * self);

static void qt_about_activated (GSimpleAction * action, GVariant * parameter, gpointer app);
static void qt_quit_activated (GSimpleAction * action, GVariant * parameter, gpointer app);

#endif /* STAPPLICATION_H */
