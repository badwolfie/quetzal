#include <glib/gi18n.h>
#include "qt-application.h"

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

};

G_DEFINE_TYPE_WITH_PRIVATE (QtApplication, qt_application, GTK_TYPE_APPLICATION);

static void 
qt_prefs_activated (GSimpleAction * action, GVariant * parameter, gpointer app) 
{}

static void 
qt_about_activated (GSimpleAction * action, GVariant * parameter, gpointer app) 
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
	/* gtk_window_set_transient_for(GTK_WINDOW (about_dialog), 
		GTK_WINDOW (ECCED_APPLICATION (app)->window)); */
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
qt_quit_activated (GSimpleAction * action, GVariant * parameter, gpointer app) 
{
	g_application_quit (G_APPLICATION (app));
}

static void 
qt_application_init (QtApplication * self) 
{}

static GActionEntry quetzal_app_entries[] = {
	{ "prefs", qt_about_activated, NULL, NULL, NULL }, 
	{ "about", qt_about_activated, NULL, NULL, NULL }, 
	{ "quit", qt_quit_activated, NULL, NULL, NULL }
};

static void 
qt_application_startup (GApplication * self) 
{
	GtkBuilder * builder;
	GMenuModel * app_menu;

	const gchar * prefs_accels[2] = { "<Control>F1", NULL };
	const gchar * about_accels[2] = { "F1", NULL };
	const gchar * quit_accels[2] = { "<Control>Q", NULL };

	G_APPLICATION_CLASS (qt_application_parent_class)->startup(self);
	g_action_map_add_action_entries (
		G_ACTION_MAP (self),
		quetzal_app_entries,
		G_N_ELEMENTS (quetzal_app_entries),
		self
	);

	gtk_application_set_accels_for_action (
		GTK_APPLICATION (self), 
		"app.prefs",
		prefs_accels
	);

	gtk_application_set_accels_for_action (
		GTK_APPLICATION (self), 
		"app.about",
		about_accels
	);

	gtk_application_set_accels_for_action (
		GTK_APPLICATION (self), 
		"app.about",
		quit_accels
	);

	builder = gtk_builder_new_from_resource(
		"/com/github/badwolfie/quetzal-menu.ui");
	app_menu = G_MENU_MODEL (gtk_builder_get_object(builder, "appmenu"));
	gtk_application_set_app_menu(GTK_APPLICATION (self), app_menu);
	g_object_unref(builder);
}

static void 
qt_application_activate (GApplication * self) 
{}

static void 
qt_application_class_init (QtApplicationClass * class) 
{
	G_APPLICATION_CLASS (class)->startup = qt_application_startup;
	G_APPLICATION_CLASS (class)->activate = qt_application_activate;
}

QtApplication * 
qt_application_new (void) 
{
	QtApplication * new_app = g_object_new (
		QT_APPLICATION_TYPE, 
		"application-id", "com.github.badwolfie.quetzal.app", 
		"flags", G_APPLICATION_FLAGS_NONE, 
		NULL
	);

	new_app->priv = qt_application_get_instance_private (new_app);
	return new_app;
}

