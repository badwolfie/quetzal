#include "qt-application.h"
#include "qt-appwindow.h"

#include "config.h"
#include <glib/gi18n.h>
#include <stdlib.h>

struct _QtApplication
{
	GtkApplication parent;
	QtApplicationPrivate * priv;
};

struct _QtApplicationClass
{
	GtkApplicationClass parent_class;
};

struct _QtApplicationPrivate
{
	QtAppWindow * window;
	GSettings * settings;
};

G_DEFINE_TYPE_WITH_PRIVATE (QtApplication, qt_application, GTK_TYPE_APPLICATION);

static void 
qt_application_init (QtApplication * self) 
{}

static void 
qt_prefs_activated (GSimpleAction * action, 
										GVariant * parameter, 
										gpointer app) 
{
	
}

static void 
qt_about_activated (GSimpleAction * action, 
										GVariant * parameter, 
										gpointer app) 
{
	const gchar * author[2] = {
		"Ian Hernandez <ihernandezs@openmailbox.org>",
		NULL 
	};

	const gchar * contrib[2] = {
		"Carlos Lopez <clopezr_1205@openmailbox.org>",
		NULL 
	};

	gchar * translator_credits = _ ("translator-credits");

	gchar * license = "Pendiente...";

	GtkAboutDialog * about_dialog;
	about_dialog = GTK_ABOUT_DIALOG (gtk_about_dialog_new());
	gtk_window_set_transient_for(GTK_WINDOW (about_dialog), 
															 GTK_WINDOW (QT_APPLICATION (app)->priv->window));
	gtk_about_dialog_set_program_name(about_dialog, APP_NAME);
	gtk_about_dialog_set_comments (about_dialog, APP_SHORTDESC);
	gtk_about_dialog_set_authors (about_dialog, author);
	gtk_about_dialog_add_credit_section (about_dialog, _ ("Contributors"), contrib);
	// gtk_about_dialog_set_logo_icon_name (about_dialog, "quetzal");
	gtk_about_dialog_set_license (about_dialog, license);
	gtk_about_dialog_set_version (about_dialog, APP_VERSION);
	gtk_about_dialog_set_website (about_dialog, 
		"https://github.com/badwolfie/quetzal");
	gtk_about_dialog_set_website_label (about_dialog, _ ("Web page"));
	gtk_about_dialog_set_copyright (about_dialog, 
		"Copyright \xc2\xa9 2015 Ian Hernández");
	
	gtk_dialog_run ((GtkDialog *) about_dialog);
	gtk_widget_destroy ((GtkWidget *) about_dialog);
}

static void 
qt_quit_activated (GSimpleAction * action, 
									 GVariant * parameter, 
									 gpointer app) 
{
	g_application_quit (G_APPLICATION (app));
}

static GActionEntry quetzal_app_entries[] = {
	{ "prefs", qt_prefs_activated, NULL, NULL, NULL }, 
	{ "about", qt_about_activated, NULL, NULL, NULL }, 
	{ "quit", qt_quit_activated, NULL, NULL, NULL }
};

static void 
qt_application_startup (GApplication * self) 
{
	gchar * conf_dir_path = g_strconcat(g_get_home_dir(), 
																		  "/.quetzal", 
																		  NULL);
	GFile * conf_dir = g_file_new_for_path(conf_dir_path);
	
	gchar * schemes_dir_path = g_strconcat(g_get_home_dir(), 
																		  "/.quetzal/styles", 
																		  NULL);
	GFile * schemes_dir = g_file_new_for_path(schemes_dir_path);
	
	gchar * saved_workspace_path = g_strconcat(g_get_home_dir(), 
																		  "/.quetzal/saved-workspace", 
																		  NULL);
	GFile * saved_workspace = g_file_new_for_path(saved_workspace_path);
	
	GError * inner_error = NULL;
	if (!g_file_query_exists(conf_dir, NULL)) {
		g_file_make_directory(conf_dir, NULL, &inner_error);
		if (inner_error != NULL) {
			g_error("conf_dir (g_file_make_directory): I/O error: %s", 
					    inner_error->message);
			g_error_free(inner_error);
		}
	}
	
	inner_error = NULL;
	if (!g_file_query_exists(schemes_dir, NULL)) {
		g_file_make_directory(schemes_dir, NULL, &inner_error);
		if (inner_error != NULL) {
			g_error("schemes_dir (g_file_make_directory): I/O error: %s", 
					 		inner_error->message);
			g_error_free(inner_error);
		}
	}
	
	inner_error = NULL;
	if (!g_file_query_exists(saved_workspace, NULL)) {
		g_file_create(saved_workspace, G_FILE_CREATE_PRIVATE, NULL, &inner_error);
		if (inner_error != NULL) {
			g_error("saved_workspace (g_file_create): I/O error: %s", 
					 		inner_error->message);
			g_error_free(inner_error);
		}
	}

	const gchar * prefs_accels[2] = { "<control>F1", NULL };
	const gchar * about_accels[2] = { "F1", NULL };
	const gchar * quit_accels[2] = { "<control>Q", NULL };

	G_APPLICATION_CLASS (qt_application_parent_class)->startup(self);
	g_action_map_add_action_entries (G_ACTION_MAP (self),
																	 quetzal_app_entries,
																	 G_N_ELEMENTS (quetzal_app_entries), 
																	 self);

	gtk_application_set_accels_for_action(GTK_APPLICATION (self), 
																				"app.prefs", 
																				prefs_accels);

	gtk_application_set_accels_for_action(GTK_APPLICATION (self), 
																				"app.about", 
																			  about_accels);

	gtk_application_set_accels_for_action(GTK_APPLICATION (self), 
																			  "app.about", 
																			 quit_accels);
	
	QT_APPLICATION (self)->priv->settings = 
		g_settings_new("com.github.badwolfie.simple-text");
	
	QT_APPLICATION (self)->priv->window = qt_appwindow_new(QT_APPLICATION (self));
	
	GtkBuilder * builder = gtk_builder_new_from_resource(
		"/com/github/badwolfie/quetzal/quetzal-menu.ui");
	GMenuModel * app_menu = G_MENU_MODEL (gtk_builder_get_object(builder, "appmenu"));
	gtk_application_set_app_menu(GTK_APPLICATION (self), app_menu);
	g_object_unref(builder);
	
	const gchar * accels_reload[2] = { "<control>R", NULL };
	gtk_application_set_accels_for_action(GTK_APPLICATION (self), 
																				"win.reload", 
																				accels_reload);
	
	const gchar * accels_build[2] = { "<control>B", NULL };
	gtk_application_set_accels_for_action(GTK_APPLICATION (self), 
																				"win.build", 
																				accels_build);
	
	const gchar * accels_fullscreen[2] = { "F11", NULL };
	gtk_application_set_accels_for_action(GTK_APPLICATION (self), 
																				"win.fullscreen", 
																				accels_fullscreen);
	
	const gchar * accels_re_open[2] = { "<control><shift>T", NULL };
	gtk_application_set_accels_for_action(GTK_APPLICATION (self), 
																				"win.re_open", 
																				accels_re_open);
	
	const gchar * accels_save_as[2] = { "<control><shift>S", NULL };
	gtk_application_set_accels_for_action(GTK_APPLICATION (self), 
																				"win.save_as", 
																				accels_save_as);
	
	const gchar * accels_set_syntax[2] = { "<control><shift>P", NULL };
	gtk_application_set_accels_for_action(GTK_APPLICATION (self), 
																				"win.set_syntax", 
																				accels_set_syntax);
	
	const gchar * accels_show_terminal[2] = { "<control><shift>C", NULL };
	gtk_application_set_accels_for_action(GTK_APPLICATION (self), 
																				"win.toggle_terminal", 
																				accels_show_terminal);
	
	const gchar * accels_next_tab[2] = { "<control>Page_Down", NULL };
	gtk_application_set_accels_for_action(GTK_APPLICATION (self), 
																				"win.next_tab", 
																				accels_next_tab);
	
	const gchar * accels_prev_tab[2] = { "<control>Page_Up", NULL };
	gtk_application_set_accels_for_action(GTK_APPLICATION (self), 
																				"win.prev_tab", 
																				accels_prev_tab);
	
	const gchar * accels_close[2] = { "<control>W", NULL };
	gtk_application_set_accels_for_action(GTK_APPLICATION (self), 
																				"win.close", 
																				accels_close);
	
	const gchar * accels_close_all[2] = { "<control><shift>W", NULL };
	gtk_application_set_accels_for_action(GTK_APPLICATION (self), 
																				"win.close_all", 
																				accels_close_all);
	
	const gchar * accels_search[2] = { "<control>F", NULL };
	gtk_application_set_accels_for_action(GTK_APPLICATION (self), 
																				"win.search", 
																				accels_search);
	system("clear");
}

static void 
qt_application_activate (GApplication * self) 
{
	QtApplication * application;
	
	application = QT_APPLICATION (self);
	gtk_window_present(GTK_WINDOW (application->priv->window));
}

static void 
qt_application_open (GApplication * self, 
										 GFile ** files, 
										 gint n_files, 
										 const gchar * hint) 
{
	QtApplication * application;
	
	application = QT_APPLICATION (self);
	gtk_window_present(GTK_WINDOW (application->priv->window));
}

static void 
qt_application_class_init (QtApplicationClass * class) 
{
	G_APPLICATION_CLASS (class)->startup = qt_application_startup;
	G_APPLICATION_CLASS (class)->activate = qt_application_activate;
	G_APPLICATION_CLASS (class)->open = qt_application_open;
}

QtApplication * 
qt_application_new (void) 
{
	QtApplication * new_app = g_object_new (
		QT_APPLICATION_TYPE, 
		"application-id", "com.github.badwolfie.quetzal.app", 
		"flags", G_APPLICATION_HANDLES_OPEN, 
		NULL
	);

	new_app->priv = qt_application_get_instance_private (new_app);
	return new_app;
}

