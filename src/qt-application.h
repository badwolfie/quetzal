#ifndef QTAPPLICATION_H
#define QTAPPLICATION_H

#include <gtk/gtk.h>

#define QT_APPLICATION_TYPE (qt_application_get_type ())
#define QT_APPLICATION(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), QT_APPLICATION_TYPE, QtApplication))

typedef struct _QtApplication QtApplication;
typedef struct _QtApplicationClass QtApplicationClass;
typedef struct _QtApplicationPrivate QtApplicationPrivate;

GType qt_application_get_type (void);
QtApplication * qt_application_new (void);

#endif /* QTAPPLICATION_H */
