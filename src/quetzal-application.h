/* quetzal-application.h
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
#ifndef QUETZAL_APPLICATION__H
#define QUETZAL_APPLICATION__H

#include <gtk/gtk.h>

#define QUETZAL_TYPE_APPLICATION (quetzal_application_get_type ())
#define QUETZAL_APPLICATION(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), QUETZAL_TYPE_APPLICATION, QuetzalApplication))

typedef struct _QuetzalApplication QuetzalApplication;
typedef struct _QuetzalApplicationClass QuetzalApplicationClass;
typedef struct _QuetzalApplicationPrivate QuetzalApplicationPrivate;

GType quetzal_application_get_type (void);
QuetzalApplication * quetzal_application_new (void);

#endif /* QUETZAL_APPLICATION__H */

