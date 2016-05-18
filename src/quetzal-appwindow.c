/* quetzal-appwindow.c
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

#include "quetzal-appwindow.h"

#include "config.h"

struct _QuetzalAppwindow
{
  GtkApplicationWindow parent;
  QuetzalAppwindowPrivate * priv;
};

struct _QuetzalAppwindowClass
{
  GtkApplicationWindowClass parent_class;
};

struct _QuetzalAppwindowPrivate
{
  int id;
};

G_DEFINE_TYPE_WITH_PRIVATE (QuetzalAppwindow, quetzal_appwindow, GTK_TYPE_APPLICATION_WINDOW);

static void quetzal_appwindow_init (QuetzalAppwindow * self);
static void quetzal_appwindow_class_init (QuetzalAppwindowClass * class);

static void quetzal_appwindow_create_widgets (QuetzalAppwindow * self);
static void quetzal_appwindow_connect_signals (QuetzalAppwindow * self);

QuetzalAppwindow *
quetzal_appwindow_new (QuetzalApplication * application)
{
  QuetzalAppwindow * self = g_object_new (QUETZAL_TYPE_APPWINDOW,
                                          "application", application,
                                          NULL);
  self->priv = quetzal_appwindow_get_instance_private (self);

  g_object_set (GTK_WINDOW (self), "window-position", GTK_WIN_POS_CENTER, NULL);
  gtk_window_set_default_size (GTK_WINDOW (self), 800, 600);
  gtk_container_set_border_width (GTK_CONTAINER (self), 0);
  // gtk_window_maximize (GTK_WINDOW (self));

  quetzal_appwindow_create_widgets (self);
  quetzal_appwindow_connect_signals (self);
  return self;
}

static void
quetzal_appwindow_init (QuetzalAppwindow * self)
{}

static void
quetzal_appwindow_class_init (QuetzalAppwindowClass * class)
{}

static void
quetzal_appwindow_create_widgets (QuetzalAppwindow * self)
{}

static void
quetzal_appwindow_connect_signals (QuetzalAppwindow * self)
{}

