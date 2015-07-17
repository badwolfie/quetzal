#include "qt-tabbar.h"

struct _QtTabBar 
{
	GtkBox parent;
	QtTabBarPrivate * priv;
};

struct _QtTabBarClass 
{
	GtkBoxClass parent_class;
};

struct _QtTabBarPrivate 
{
	GtkStack * stack;
};
