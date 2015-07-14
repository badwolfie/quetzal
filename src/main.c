#include <locale.h>
#include "qt-application.h"

int 
main (int argc, char * argv[])
{	
	setlocale (LC_ALL, "");
	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
	
	// gtk_window_set_default_icon_name ("quetzal");
	gint result = g_application_run (G_APPLICATION (qt_application_new), argc, argv);
	return result;
}