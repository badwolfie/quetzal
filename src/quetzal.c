/* quetzal.c
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

#include "config.h"
#include <glib/gi18n.h>
// #include "qt-application.h"

#include "quetzal.h"

int 
main (int argc, char * argv[])
{
  bindtextdomain (GETTEXT_PACKAGE, QTLOCALEDIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
	
  gtk_window_set_default_icon_name ("quetzal");
  g_set_application_name(_ (QUETZAL_NAME));
  g_set_prgname(QUETZAL_NAME);
  
  // return g_application_run (G_APPLICATION (qt_application_new()), argc, argv);
  return 0;
}
