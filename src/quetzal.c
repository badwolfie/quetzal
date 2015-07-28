#include "config.h"
#include <glib/gi18n.h>
#include "qt-application.h"

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
  
  return g_application_run (G_APPLICATION (qt_application_new()), argc, argv);
}
