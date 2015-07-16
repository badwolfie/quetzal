#ifndef QT_HEADERBAR_H
#define QT_HEADERBAR_H

#include <gtk/gtk.h>
#include "qt-appwindow.h"

#define QT_HEADER_BAR_TYPE (qt_header_bar_get_type ())
#define QT_HEADER_BAR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), QT_HEADER_BAR_TYPE, QtHeaderBar))

typedef struct _QtHeaderBar QtHeaderBar;
typedef struct _QtHeaderBarClass QtHeaderBarClass;
typedef struct _QtHeaderBarPrivate QtHeaderBarPrivate;

GType qt_header_bar_get_type (void);
QtHeaderBar * qt_header_bar_new (QtAppWindow * window);

void qt_header_bar_toggle_fullscreen (QtHeaderBar * self);

#endif /* QT_HEADERBAR_H */
