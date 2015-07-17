#ifndef QT_TAB_BAR_H
#define QT_TAB_BAR_H

#include <gtk/gtk.h>
#include "qt-appwindow.h"

#define QT_TAB_BAR_TYPE (qt_tab_bar_get_type ())
#define QT_TAB_BAR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), QT_TAB_BAR_TYPE, QtTabBar))

typedef struct _QtTabBar QtTabBar;
typedef struct _QtTabBarClass QtTabBarClass;
typedef struct _QtTabBarPrivate QtTabBarPrivate;

GType qt_tab_bar_get_type (void);
QtTabBar * qt_tab_bar_new (void);

#endif /* QT_TAB_BAR_H */
