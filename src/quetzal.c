/* quetzal.c
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

#include "quetzal-application.h"

#include "quetzal.h"
#include "config.h"

gint
main (gint argc, gchar * argv[])
{
  bindtextdomain (GETTEXT_PACKAGE, QUETZAL_LOCALEDIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
	
  gtk_window_set_default_icon_name ("quetzal");
  g_set_application_name (_ (QUETZAL_NAME));
  g_set_prgname (QUETZAL_NAME);

  return g_application_run (
    G_APPLICATION (quetzal_application_new ()), argc, argv
  );
}
