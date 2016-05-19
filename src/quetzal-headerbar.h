/* quetzal-headerbar.h
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
#ifndef QUETZAL_HEADERBAR__H
#define QUETZAL_HEADERBAR__H

#include <gtk/gtk.h>

#include "quetzal-appwindow.h"

#define QUETZAL_TYPE_HEADERBAR (quetzal_headerbar_get_type ())
#define QUETZAL_HEADERBAR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), QUETZAL_TYPE_HEADERBAR, QuetzalHeaderbar))

typedef struct _QuetzalHeaderbar QuetzalHeaderbar;
typedef struct _QuetzalHeaderbarClass QuetzalHeaderbarClass;
typedef struct _QuetzalHeaderbarPrivate QuetzalHeaderbarPrivate;

GType quetzal_headerbar_get_type (void);
QuetzalHeaderbar * quetzal_headerbar_new (QuetzalAppwindow * window);

#endif /* QUETZAL_HEADERBAR__H */
