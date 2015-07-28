#include "qt-headerbar.h"
#include "quetzal.h"

#include "config.h"
#include <glib/gi18n.h>

struct _QtHeaderBar 
{
	GtkHeaderBar parent;
	QtHeaderBarPrivate * priv;
};

struct _QtHeaderBarClass 
{
	GtkHeaderBarClass parent_class;
};

struct _QtHeaderBarPrivate 
{
	QtAppWindow * window;
	GtkMenuButton * menu_button;
	GtkButton * open_button;
	GtkButton * save_button;
	GtkButton * new_tab_button;
	GtkButton * exit_fullscreen;
};

G_DEFINE_TYPE_WITH_PRIVATE (QtHeaderBar, qt_header_bar, GTK_TYPE_HEADER_BAR);

static void 
qt_header_bar_init (QtHeaderBar * self) 
{}

static void 
qt_header_bar_class_init (QtHeaderBarClass * class) 
{}

static void 
qt_header_bar_create_widgets (QtHeaderBar * self) 
{
	GtkBuilder * builder = gtk_builder_new_from_resource(
		"/com/github/badwolfie/quetzal/quetzal-menu.ui"
	);
	
	self->priv->menu_button = GTK_MENU_BUTTON (gtk_menu_button_new());
	gtk_menu_button_set_direction(self->priv->menu_button, GTK_ARROW_NONE);
	
	GtkStyleContext * menu_button_context = 
		gtk_widget_get_style_context(GTK_WIDGET (self->priv->menu_button));
	gtk_style_context_add_class(menu_button_context, "image-button");
	
	GMenuModel * menu_model = G_MENU_MODEL (
		gtk_builder_get_object(builder, "window-menu")
	);
	
	gtk_menu_button_set_menu_model(self->priv->menu_button, menu_model);
	gtk_menu_button_set_use_popover(self->priv->menu_button, TRUE);
	GtkPopover * popover = gtk_menu_button_get_popover(self->priv->menu_button);
	g_object_set(GTK_WIDGET (popover), "width-request", 275, NULL);
	gtk_widget_show_all(GTK_WIDGET (self->priv->menu_button));
	
	self->priv->open_button = GTK_BUTTON (gtk_button_new_with_label(_ ("Open")));
	const gchar * open_button_tooltip = 
		g_strconcat(_ ("Open file"), " (Ctrl+O)", NULL);
	gtk_widget_set_tooltip_text(GTK_WIDGET (self->priv->open_button), 
														  open_button_tooltip);
	gtk_widget_show(GTK_WIDGET (self->priv->open_button));
	
	self->priv->save_button = GTK_BUTTON (gtk_button_new_with_label(_ ("Save")));
	const gchar * save_button_tooltip = 
		g_strconcat(_ ("Save file"), " (Ctrl+S)", NULL);
	gtk_widget_set_tooltip_text(GTK_WIDGET (self->priv->save_button), 
														  save_button_tooltip);
	gtk_widget_show(GTK_WIDGET (self->priv->save_button));
	
	self->priv->new_tab_button = GTK_BUTTON (
		gtk_button_new_from_icon_name("tab-new-symbolic", GTK_ICON_SIZE_MENU));
	const gchar * new_tab_button_tooltip = 
		g_strconcat(_ ("New file"), " (Ctrl+N)", NULL);
	gtk_widget_set_tooltip_text(GTK_WIDGET (self->priv->new_tab_button), 
														  new_tab_button_tooltip);
	gtk_widget_show(GTK_WIDGET (self->priv->new_tab_button));
	
	self->priv->exit_fullscreen = GTK_BUTTON (
		gtk_button_new_from_icon_name("view-restore-symbolic", GTK_ICON_SIZE_MENU));
	gtk_widget_set_tooltip_text(GTK_WIDGET (self->priv->exit_fullscreen), 
														  "Leave fullscreen mode");
	
	gtk_header_bar_pack_start(GTK_HEADER_BAR (self), 
													  GTK_WIDGET(self->priv->open_button));
	
	gtk_header_bar_pack_start(GTK_HEADER_BAR (self), 
													  GTK_WIDGET(self->priv->new_tab_button));
	
	gtk_header_bar_pack_end(GTK_HEADER_BAR (self), 
													  GTK_WIDGET(self->priv->exit_fullscreen));
	
	gtk_header_bar_pack_end(GTK_HEADER_BAR (self), 
													  GTK_WIDGET(self->priv->menu_button));
	
	gtk_header_bar_pack_end(GTK_HEADER_BAR (self), 
													  GTK_WIDGET(self->priv->save_button));
	
	gtk_header_bar_set_title(GTK_HEADER_BAR (self), _ (QUETZAL_NAME));
}

void 
qt_header_bar_toggle_fullscreen (QtHeaderBar * self) 
{
	GdkWindow * window = gtk_widget_get_window(GTK_WIDGET (self->priv->window));
	GdkWindowState window_state = gdk_window_get_state(window);
	
	if ((window_state & GDK_WINDOW_STATE_FULLSCREEN) != 0) {
		gtk_header_bar_set_show_close_button(GTK_HEADER_BAR (self), TRUE);
		gtk_widget_hide(GTK_WIDGET (self->priv->exit_fullscreen));
	} else {
		gtk_header_bar_set_show_close_button(GTK_HEADER_BAR (self), FALSE);
		gtk_widget_show_all(GTK_WIDGET (self->priv->exit_fullscreen));
	}
}

static void 
qt_header_bar_connect_signals (QtHeaderBar * self) 
{
	g_signal_connect (G_OBJECT (self->priv->new_tab_button), 
									  "clicked", G_CALLBACK (qt_app_window_create_new_doc), 
									  self->priv->window);
  
  g_signal_connect (G_OBJECT (self->priv->open_button), 
									  "clicked", G_CALLBACK (qt_app_window_open_file), 
									  self->priv->window);
  
  GtkAccelGroup * accels = gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW (self->priv->window), accels);
  
  gtk_widget_add_accelerator(GTK_WIDGET (self->priv->new_tab_button), 
                             "activate", accels, GDK_KEY_N, 
                             GDK_CONTROL_MASK, 
                             GTK_ACCEL_VISIBLE);
  
  gtk_widget_add_accelerator(GTK_WIDGET (self->priv->open_button), 
                             "activate", accels, GDK_KEY_O, 
                             GDK_CONTROL_MASK, 
                             GTK_ACCEL_VISIBLE);
  
  gtk_widget_add_accelerator(GTK_WIDGET (self->priv->menu_button), 
                             "activate", accels, GDK_KEY_F10, 
                             GDK_META_MASK, 
                             GTK_ACCEL_VISIBLE);
}

QtHeaderBar * 
qt_header_bar_new (QtAppWindow * window) 
{
	QtHeaderBar * new_header_bar = g_object_new (QT_HEADER_BAR_TYPE, NULL);
	
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR (new_header_bar), TRUE);
	new_header_bar->priv = qt_header_bar_get_instance_private(new_header_bar);
	
	new_header_bar->priv->window = window;
	qt_header_bar_create_widgets(new_header_bar);
	qt_header_bar_connect_signals(new_header_bar);
	return new_header_bar;
}

