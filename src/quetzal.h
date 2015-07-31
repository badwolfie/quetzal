/* quetzal.h
 *
 * Copyright (C) 2015 Ian Hernandez <ihernandezs@openmailbox.org>
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

#ifndef QUETZAL_H
#define QUETZAL_H

#include <glib.h>

#define QUETZAL_NAME "Quetzal Text Editor"
#define QUETZAL_SHORTDESC "A simple and lightweight text and code editor." 

#define QUETZAL_MAJOR_VERSION (0)
#define QUETZAL_MINOR_VERSION (11) 
#define QUETZAL_MICRO_VERSION (1)

#define QUETZAL_VERSION (g_strdup_printf("%d.%d.%d", QUETZAL_MAJOR_VERSION, QUETZAL_MINOR_VERSION, QUETZAL_MICRO_VERSION))
#define QUETZAL_CHECK_VERSION (major, minor, micro) (\
  QUETZAL_MAJOR_VERSION > (major) || \
  QUETZAL_MAJOR_VERSION == (major) && QUETZAL_MINOR_VERSION > (minor) || \
  QUETZAL_MAJOR_VERSION == (major) && QUETZAL_MINOR_VERSION == (minor) && QUETZAL_MICRO_VERSION >= (micro) \
)

static gint 
array_length (gpointer array) 
{
	gint length = 0;
	if (array) {
		while (((gpointer *) array)[length]) {
			length++;
		}
	}
	
	return length;
}

#endif /* QUETZAL_H */

