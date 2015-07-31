/* qt-application.c
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

#include "qt-text-editor.h"
#include "qt-application.h"
#include "qt-appwindow.h"
#include "quetzal.h"

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

const gchar * 
qt_application_get_license_text (void) 
{
	const gchar * license[] = {
    _ ("Quetzal is free software: you can redistribute it and/or modify "
       "it under the terms of the GNU General Public License as published by "
       "the Free Software Foundation, either version 3 of the License, or "
       "(at your option) any later version."),
    _ ("Quetzal is distributed in the hope that it will be useful, "
       "but WITHOUT ANY WARRANTY; without even the implied warranty of "
       "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
       "GNU General Public License for more details."),
    _ ("You should have received a copy of the GNU General Public License "
       "along with Quetzal.  If not, see <http://www.gnu.org/licenses/>.")
	};

  const gchar * full_license = 
    g_strjoin("\n\n", _ (license[0]), _ (license[1]), _ (license[2]), NULL);
  return full_license;
}

static void 
qt_about_activated (GSimpleAction * action, 
										GVariant * parameter, 
										gpointer app) 
{
	const gchar * author[2] = {
		"Ian Hernández <ihernandezs@openmailbox.org>",
		NULL 
	};
  
  const gchar * documenters[2] = {
		"Ian Hernández",
		NULL 
	};
  
  const gchar * artists[2] = {
		"Ian Hernández",
		NULL 
	};

	const gchar * contribs[2] = {
		"Carlos López <clopezr_1205@openmailbox.org>",
		NULL 
	};

	GtkAboutDialog * about_dialog;
	about_dialog = GTK_ABOUT_DIALOG (gtk_about_dialog_new());
	gtk_window_set_transient_for(GTK_WINDOW (about_dialog), 
															 GTK_WINDOW (QT_APPLICATION (app)->priv->window));
  g_object_set(G_OBJECT (about_dialog),
               "artists", artists, 
               "authors", author, 
               "comments", _ (QUETZAL_SHORTDESC),
               "copyright", "Copyright \xc2\xa9 2015 Ian Hernández",
               // "documenters", documenters, 
               "license", qt_application_get_license_text(), 
               "license-type", GTK_LICENSE_CUSTOM, 
               "logo-icon-name", "quetzal",
               "program-name", _ (QUETZAL_NAME),
               "translator-credits", _ ("translator-credits"), 
               "version", QUETZAL_VERSION,
               "website", "https://badwolfie.github.io/quetzal",
               "website-label", _ ("Official web page"),
               "wrap-license", TRUE, 
               NULL);
  gtk_about_dialog_add_credit_section(about_dialog, 
                                      _ ("Contributors"), 
                                      contribs);
	
	gtk_dialog_run ((GtkDialog *) about_dialog);
	gtk_widget_destroy ((GtkWidget *) about_dialog);
}

static void 
qt_quit_activated (GSimpleAction * action, 
									 GVariant * parameter, 
									 gpointer app) 
{
  QtApplication * self = QT_APPLICATION (app);
  gtk_widget_destroy(GTK_WIDGET (self->priv->window));
	g_application_quit (G_APPLICATION (self));
}

static GActionEntry quetzal_app_entries[] = {
	{ "prefs", qt_prefs_activated, NULL, NULL, NULL }, 
	{ "about", qt_about_activated, NULL, NULL, NULL }, 
	{ "quit", qt_quit_activated, NULL, NULL, NULL }
};

static void 
qt_application_startup (GApplication * self) 
{
	G_APPLICATION_CLASS (qt_application_parent_class)->startup(self);
	
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
		g_settings_new("com.github.badwolfie.quetzal");
	
	QtTextEditor * editor = qt_text_editor_new();
	
	qt_text_editor_set_show_line_numbers(editor, 
		g_settings_get_boolean(
			QT_APPLICATION (self)->priv->settings, 
			"show-line-numbers"
		)
	);

	qt_text_editor_set_show_right_margin(editor, 
		g_settings_get_boolean(
			QT_APPLICATION (self)->priv->settings, 
			"show-right-margin"
		)
	);

	qt_text_editor_set_right_margin_at(editor, 
		g_settings_get_int(
			QT_APPLICATION (self)->priv->settings, 
			"right-margin-at"
		)
	);

	qt_text_editor_set_show_grid_pattern(editor, 
		g_settings_get_boolean(
			QT_APPLICATION (self)->priv->settings, 
			"show-grid-pattern"
		)
	);

	qt_text_editor_set_highlight_current_line(editor, 
		g_settings_get_boolean(
			QT_APPLICATION (self)->priv->settings, 
			"highlight-current-line"
		)
	);

	qt_text_editor_set_highlight_brackets(editor, 
		g_settings_get_boolean(
			QT_APPLICATION (self)->priv->settings, 
			"highlight-brackets"
		)
	);

	qt_text_editor_set_use_text_wrap(editor, 
		g_settings_get_boolean(
			QT_APPLICATION (self)->priv->settings, 
			"use-text-wrap"
		)
	);

	qt_text_editor_set_tab_width(editor, 
		g_settings_get_int(
			QT_APPLICATION (self)->priv->settings, 
			"tab-width"
		)
	);

	qt_text_editor_set_insert_spaces(editor, 
		g_settings_get_boolean(
			QT_APPLICATION (self)->priv->settings, 
			"insert-spaces"
		)
	);

	qt_text_editor_set_auto_indent(editor, 
		g_settings_get_boolean(
			QT_APPLICATION (self)->priv->settings, 
			"auto-indent"
		)
	);

	qt_text_editor_set_insert_braces(editor, 
		g_settings_get_boolean(
			QT_APPLICATION (self)->priv->settings, 
			"insert-braces"
		)
	);

	qt_text_editor_set_save_workspace(editor, 
		g_settings_get_boolean(
			QT_APPLICATION (self)->priv->settings, 
			"save-workspace"
		)
	);

	qt_text_editor_set_use_default_typo(editor, 
		g_settings_get_boolean(
			QT_APPLICATION (self)->priv->settings, 
			"use-default-typo"
		)
	);

	qt_text_editor_set_editor_font(editor, 
		g_settings_get_string(
			QT_APPLICATION (self)->priv->settings, 
			"editor-font"
		)
	);

	qt_text_editor_set_color_scheme(editor, 
		g_settings_get_string(
			QT_APPLICATION (self)->priv->settings, 
			"color-scheme"
		)
	);

	qt_text_editor_set_prefer_dark(editor, 
		g_settings_get_boolean(
			QT_APPLICATION (self)->priv->settings, 
			"prefer-dark"
		)
	);
	
	QT_APPLICATION (self)->priv->window = qt_app_window_new(
		QT_APPLICATION (self), editor
	);
	
	GtkBuilder * builder = gtk_builder_new_from_resource(
		"/com/github/badwolfie/quetzal/quetzal-menu.ui");
	GMenuModel * app_menu = G_MENU_MODEL (
		gtk_builder_get_object(builder, "appmenu"));
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
																				"win.search_mode", 
																				accels_search);
	system("clear");
}

static void 
qt_application_activate (GApplication * self) 
{
	QtApplication * application;
	
	application = QT_APPLICATION (self);
  qt_app_window_set_arg_files(application->priv->window, NULL, 0);
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
  qt_app_window_set_arg_files(application->priv->window, files, n_files);
	gtk_window_present(GTK_WINDOW (application->priv->window));
}

static void 
qt_application_shutdown (GApplication * self) 
{
	G_APPLICATION_CLASS (qt_application_parent_class)->shutdown(self);
	
	QtTextEditor * editor = 
		qt_app_window_get_editor(QT_APPLICATION (self)->priv->window);

	g_settings_set_boolean(
		QT_APPLICATION (self)->priv->settings, 
		"show-line-numbers", 
		qt_text_editor_get_show_line_numbers(editor)
	);

	g_settings_set_boolean(
		QT_APPLICATION (self)->priv->settings, 
		"show-right-margin", 
		qt_text_editor_get_show_right_margin(editor)
	);

	g_settings_set_int(
		QT_APPLICATION (self)->priv->settings, 
		"right-margin-at", 
		qt_text_editor_get_right_margin_at(editor)
	);

	g_settings_set_boolean(
		QT_APPLICATION (self)->priv->settings, 
		"show-grid-pattern", 
		qt_text_editor_get_show_grid_pattern(editor)
	);

	g_settings_set_boolean(
		QT_APPLICATION (self)->priv->settings, 
		"highlight-current-line", 
		qt_text_editor_get_highlight_current_line(editor)
	);

	g_settings_set_boolean(
		QT_APPLICATION (self)->priv->settings, 
		"highlight-brackets", 
		qt_text_editor_get_highlight_brackets(editor)
	);

	g_settings_set_boolean(
		QT_APPLICATION (self)->priv->settings, 
		"use-text-wrap", 
		qt_text_editor_get_use_text_wrap(editor)
	);

	g_settings_set_int(
		QT_APPLICATION (self)->priv->settings, 
		"tab-width", 
		qt_text_editor_get_tab_width(editor)
	);

	g_settings_set_boolean(
		QT_APPLICATION (self)->priv->settings, 
		"insert-spaces", 
		qt_text_editor_get_insert_spaces(editor)
	);

	g_settings_set_boolean(
		QT_APPLICATION (self)->priv->settings, 
		"auto-indent", 
		qt_text_editor_get_auto_indent(editor)
	);

	g_settings_set_boolean(
		QT_APPLICATION (self)->priv->settings, 
		"insert-braces", 
		qt_text_editor_get_insert_braces(editor)
	);

	g_settings_set_boolean(
		QT_APPLICATION (self)->priv->settings, 
		"save-workspace", 
		qt_text_editor_get_save_workspace(editor)
	);

	g_settings_set_boolean(
		QT_APPLICATION (self)->priv->settings, 
		"use-default-typo", 
		qt_text_editor_get_use_default_typo(editor)
	);
	
	if (qt_text_editor_get_use_default_typo(editor)) {
		g_settings_reset(
			QT_APPLICATION (self)->priv->settings,
			"editor-font"
		);
	} else {
		g_settings_set_string(
			QT_APPLICATION (self)->priv->settings, 
			"editor-font", 
			qt_text_editor_get_editor_font(editor)
		);
	}
		
	g_settings_set_string(
		QT_APPLICATION (self)->priv->settings, 
		"color-scheme", 
		qt_text_editor_get_color_scheme(editor)
	);

	g_settings_set_boolean(
		QT_APPLICATION (self)->priv->settings, 
		"prefer-dark", 
		qt_text_editor_get_prefer_dark(editor)
	);
	
	g_object_unref(editor);
}

static void 
qt_application_class_init (QtApplicationClass * class) 
{
	G_APPLICATION_CLASS (class)->startup = qt_application_startup;
	G_APPLICATION_CLASS (class)->activate = qt_application_activate;
	G_APPLICATION_CLASS (class)->open = qt_application_open;
	G_APPLICATION_CLASS (class)->shutdown = qt_application_shutdown;
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

	new_app->priv = qt_application_get_instance_private(new_app);
	return new_app;
}

