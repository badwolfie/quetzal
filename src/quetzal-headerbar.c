/* quetzal-headerbar.c
 *
 * Copyright (C) 2016 Ian Hernandez <ihernandezs@openmailbox.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <glib/gi18n.h>

#include "quetzal-headerbar.h"

#include "quetzal.h"
#include "config.h"

struct _QuetzalHeaderbar
{
  GtkHeaderBar parent;
  QuetzalHeaderbarPrivate * priv;
};

struct _QuetzalHeaderbarClass
{
  GtkHeaderBarClass parent_class;
};

struct _QuetzalHeaderbarPrivate
{
  QuetzalAppwindow * window;
  GtkButton * open_button;
  GtkButton * new_tab_button;
  GtkButton * save_button;
  GtkButton * exit_fullscreen_button;
  GtkMenuButton * menu_button;
};

G_DEFINE_TYPE_WITH_PRIVATE (QuetzalHeaderbar, quetzal_headerbar, GTK_TYPE_HEADER_BAR);

static void quetzal_headerbar_init (QuetzalHeaderbar * self);
static void quetzal_headerbar_class_init (QuetzalHeaderbarClass * class);

static void quetzal_headerbar_create_widgets (QuetzalHeaderbar * self);
static void quetzal_headerbar_connect_signals (QuetzalHeaderbar * self);

QuetzalHeaderbar *
quetzal_headerbar_new (QuetzalAppwindow * window)
{
  QuetzalHeaderbar * self =
      g_object_new (QUETZAL_TYPE_HEADERBAR,
                    "show-close-button", TRUE,
                    "title", _ (QUETZAL_NAME),
                    "has-subtitle", TRUE,
                    NULL);
  self->priv = quetzal_headerbar_get_instance_private (self);
  self->priv->window = window;

  quetzal_headerbar_create_widgets (self);
  quetzal_headerbar_connect_signals (self);
  return self;
}

static void
quetzal_headerbar_init (QuetzalHeaderbar * self)
{}

static void
quetzal_headerbar_class_init (QuetzalHeaderbarClass * class)
{}

static void
quetzal_headerbar_create_widgets (QuetzalHeaderbar * self)
{
  GtkStyleContext * style_context;

  self->priv->open_button = GTK_BUTTON (gtk_button_new_with_label (_ ("Open")));
  const gchar * open_button_tooltip = g_strconcat (_ ("Open file"),
                                                   " (Ctrl + O)",
                                                   NULL);
  gtk_widget_set_tooltip_text (GTK_WIDGET (self->priv->open_button),
                               open_button_tooltip);
  gtk_widget_show (GTK_WIDGET (self->priv->open_button));


  self->priv->new_tab_button =
      GTK_BUTTON (gtk_button_new_from_icon_name ("tab-new-symbolic",
                                                 GTK_ICON_SIZE_MENU));
  const gchar * new_tab_button_tooltip = g_strconcat (_ ("New file"),
                                                      " (Ctrl + N)",
                                                      NULL);
  gtk_widget_set_tooltip_text (GTK_WIDGET (self->priv->new_tab_button),
                               new_tab_button_tooltip);
  gtk_widget_show (GTK_WIDGET (self->priv->new_tab_button));

  style_context = gtk_widget_get_style_context (
      GTK_WIDGET (self->priv->new_tab_button));
  gtk_style_context_add_class (style_context, "image-button");


  self->priv->save_button = GTK_BUTTON (gtk_button_new_with_label (_ ("Save")));
  const gchar * save_button_tooltip = g_strconcat (_ ("Save file"),
                                                   " (Ctrl + S)",
                                                   NULL);
  gtk_widget_set_tooltip_text (GTK_WIDGET (self->priv->save_button),
                               save_button_tooltip);
  gtk_widget_show (GTK_WIDGET (self->priv->save_button));


  self->priv->exit_fullscreen_button =
      GTK_BUTTON (gtk_button_new_from_icon_name ("view-restore-symbolic",
                                                 GTK_ICON_SIZE_MENU));
  const gchar * exit_fullscreen_button_tooltip =
      g_strconcat (_ ("Leave fullscreen mode"), " (F11)", NULL);
  gtk_widget_set_tooltip_text (GTK_WIDGET (self->priv->exit_fullscreen_button),
                               exit_fullscreen_button_tooltip);

  style_context = gtk_widget_get_style_context (
      GTK_WIDGET (self->priv->exit_fullscreen_button));
  gtk_style_context_add_class (style_context, "image-button");


  self->priv->menu_button = GTK_MENU_BUTTON (gtk_menu_button_new ());
  gtk_menu_button_set_direction (self->priv->menu_button, GTK_ARROW_NONE);

  style_context = gtk_widget_get_style_context (
      GTK_WIDGET (self->priv->menu_button));
  gtk_style_context_add_class (style_context, "image-button");

  GtkBuilder * builder = gtk_builder_new_from_resource (
      "/io/github/badwolfie/quetzal/quetzal-menu.ui");
  GMenuModel * menu_model =
      G_MENU_MODEL (gtk_builder_get_object (builder, "window-menu"));
  gtk_menu_button_set_menu_model (self->priv->menu_button, menu_model);
  gtk_menu_button_set_use_popover (self->priv->menu_button, TRUE);

  GtkPopover * menu_popover =
      gtk_menu_button_get_popover (self->priv->menu_button);
  g_object_set (GTK_WIDGET (menu_popover), "width-request", 275, NULL);
  gtk_widget_show_all (GTK_WIDGET (self->priv->menu_button));


  gtk_header_bar_pack_start (GTK_HEADER_BAR (self),
                             GTK_WIDGET (self->priv->open_button));

  gtk_header_bar_pack_start (GTK_HEADER_BAR (self),
                             GTK_WIDGET (self->priv->new_tab_button));

  gtk_header_bar_pack_end (GTK_HEADER_BAR (self),
                           GTK_WIDGET (self->priv->exit_fullscreen_button));

  gtk_header_bar_pack_end (GTK_HEADER_BAR (self),
                           GTK_WIDGET (self->priv->menu_button));

  gtk_header_bar_pack_end (GTK_HEADER_BAR (self),
                           GTK_WIDGET (self->priv->save_button));
}

static void
quetzal_headerbar_connect_signals (QuetzalHeaderbar * self)
{
  GtkAccelGroup * accels = gtk_accel_group_new ();
  gtk_window_add_accel_group (GTK_WINDOW (self->priv->window), accels);

  gtk_widget_add_accelerator (GTK_WIDGET (self->priv->open_button),
                              "activate", accels, GDK_KEY_O,
                              GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  gtk_widget_add_accelerator (GTK_WIDGET (self->priv->new_tab_button),
                              "activate", accels, GDK_KEY_N,
                              GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  gtk_widget_add_accelerator (GTK_WIDGET (self->priv->save_button),
                              "activate", accels, GDK_KEY_S,
                              GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  gtk_widget_add_accelerator (GTK_WIDGET (self->priv->menu_button),
                              "activate", accels, GDK_KEY_F10,
                              GDK_META_MASK,
                              GTK_ACCEL_VISIBLE);
}

