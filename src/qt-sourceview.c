#include <gtksourceview/completion-providers/words/gtksourcecompletionwords.h>
#include "qt-sourceview.h"
#include <stdlib.h>
#include "config.h"
#include <glib/gi18n.h>
#include <string.h>

#define TARGET_TYPE_URI_LIST 80

struct _QtSourceView 
{
	GtkSourceView parent;
	QtSourceViewPrivate * priv;
};

struct _QtSourceViewClass 
{
	GtkSourceViewClass parent;
};

struct _QtSourceViewPrivate 
{
	GtkSourceCompletionWords * word_completion;
	GtkSourceSearchSettings * search_settings;
	GtkSourceFileLoader * file_loader;
	GtkSourceFileSaver * file_saver;
	GtkSourceFile * source_file;
	QtTextEditor * editor;
	gboolean insert_matching_braces;
};

G_DEFINE_TYPE_WITH_PRIVATE (QtSourceView, qt_source_view, GTK_SOURCE_TYPE_VIEW);

static void 
qt_source_view_init (QtSourceView * self) 
{}

static void 
qt_source_view_class_init (QtSourceViewClass * class) 
{
	g_signal_new("buffer_modified", 
							 QT_SOURCE_VIEW_TYPE, 
							 G_SIGNAL_RUN_LAST, 0, 
							 NULL, NULL, 
							 g_cclosure_marshal_VOID__BOOLEAN,
							 G_TYPE_NONE, 1, 
							 G_TYPE_BOOLEAN);
	
	g_signal_new("drag_n_drop", 
							 QT_SOURCE_VIEW_TYPE, 
							 G_SIGNAL_RUN_LAST, 0, 
							 NULL, NULL, 
							 g_cclosure_marshal_VOID__STRING,
							 G_TYPE_NONE, 1, 
							 G_TYPE_STRING);
	
	g_signal_new("file_saved", 
							 QT_SOURCE_VIEW_TYPE, 
							 G_SIGNAL_RUN_LAST, 0, 
							 NULL, NULL, 
							 g_cclosure_marshal_VOID__VOID,
							 G_TYPE_NONE, 0);
	
	g_signal_new("file_loaded", 
							 QT_SOURCE_VIEW_TYPE, 
							 G_SIGNAL_RUN_LAST, 0, 
							 NULL, NULL, 
							 g_cclosure_marshal_VOID__VOID,
							 G_TYPE_NONE, 0);
}

GtkSourceFile * 
qt_source_view_get_source_file (QtSourceView * self) 
{
	return self->priv->source_file;
}

GtkSourceBuffer * 
qt_source_view_get_source_buffer (QtSourceView * self) 
{
	GtkSourceBuffer * buffer;
	buffer = GTK_SOURCE_BUFFER (gtk_text_view_get_buffer(GTK_TEXT_VIEW (self)));
	return buffer;
}

GtkSourceSearchSettings * 
qt_source_view_get_search_settings (QtSourceView * self) 
{
	return self->priv->search_settings;
}

static const GtkTargetEntry target_list[1] = {
	{ "text/uri-list", 0, TARGET_TYPE_URI_LIST }
};

static void 
qt_source_view_load_file_ready (GObject * sender, 
																GAsyncResult * result, 
																gpointer data) 
{
	QtSourceView * self = QT_SOURCE_VIEW (data);
	GFile * file = gtk_source_file_get_location(self->priv->source_file);
	GtkSourceBuffer * buffer = 
		GTK_SOURCE_BUFFER (gtk_text_view_get_buffer(GTK_TEXT_VIEW (self)));
	
	GError * inner_error = NULL;
	gboolean file_loaded = 
		gtk_source_file_loader_load_finish(self->priv->file_loader,
																		   result, &inner_error);
	if (inner_error != NULL) {
		g_error("gtk_source_file_loader_load_finish error: %s", 
				    inner_error->message);
		g_error_free(inner_error);
	}
	
	if (file_loaded) {
		gboolean result_uncertain;
		gchar * filename = g_file_get_path(file);
	
		const gchar * file_text;
		g_object_get(G_OBJECT (buffer), 
							   "text", &file_text, 
							   NULL);
	
		gchar * content_type = g_content_type_guess(
			filename, (const guchar *) file_text, 0, &result_uncertain
		);
	
		
		if (result_uncertain) content_type = NULL;
	
		GtkSourceLanguageManager * language_manager = 
			gtk_source_language_manager_get_default();
		GtkSourceLanguage * language = gtk_source_language_manager_guess_language(
			language_manager, filename, content_type
		);
	
		gtk_source_buffer_set_language(buffer, language);
		gtk_text_buffer_set_modified(GTK_TEXT_BUFFER (buffer), FALSE);
		g_signal_emit_by_name(self, "file-loaded");
	}
}

static void 
qt_source_view_load_file (QtSourceView * self) 
{
	GFile * file = gtk_source_file_get_location(self->priv->source_file);
	
	if (file != NULL) {
		GtkSourceBuffer * buffer = 
			GTK_SOURCE_BUFFER (gtk_text_view_get_buffer(GTK_TEXT_VIEW (self)));
	 
		self->priv->file_loader = 
			gtk_source_file_loader_new(buffer, self->priv->source_file);
		
		gtk_source_file_loader_load_async(
			self->priv->file_loader,
			G_PRIORITY_HIGH, NULL, 
			NULL, NULL, NULL, 
			qt_source_view_load_file_ready, self
		);
	}
}

static void 
qt_source_view_save_file_ready (GObject * sender, 
																GAsyncResult * result, 
																gpointer data) 
{
	QtSourceView * self = QT_SOURCE_VIEW (data);
	GFile * file = gtk_source_file_get_location(self->priv->source_file);
	GtkSourceBuffer * buffer = 
		GTK_SOURCE_BUFFER (gtk_text_view_get_buffer(GTK_TEXT_VIEW (self)));
	
	GError * inner_error;
	gboolean file_saved = 
		gtk_source_file_saver_save_finish(self->priv->file_saver, 
																		  NULL, &inner_error);
	if (inner_error != NULL) {
		g_error("gtk_source_file_saver_save_finish error: %s", 
				    inner_error->message);
		g_error_free(inner_error);
	}
	
	if (file_saved) {
		gboolean result_uncertain;
		gchar * filename = g_file_get_path(file);
	
		const gchar * file_text;
		g_object_get(G_OBJECT (buffer), 
							   "text", &file_text, 
							   NULL);
	
		gchar * content_type = g_content_type_guess(
			filename, (const guchar *) file_text, 0, &result_uncertain
		);
	
		if (result_uncertain) content_type = NULL;
	
		GtkSourceLanguageManager * language_manager = 
			gtk_source_language_manager_get_default();
		GtkSourceLanguage * language = gtk_source_language_manager_guess_language(
			language_manager, filename, content_type
		);
	
		gtk_source_buffer_set_language(buffer, language);
		gtk_text_buffer_set_modified(GTK_TEXT_BUFFER (buffer), FALSE);
		g_signal_emit_by_name(self, "file-saved");
	}
}

static void 
qt_source_view_save_file (QtSourceView * self, GFile * target_file) 
{
	if (target_file != NULL) {
		gtk_source_file_set_location(self->priv->source_file, target_file);
		
		GError * inner_error;
		if (!g_file_query_exists(target_file, NULL)) {
			g_file_create(target_file, G_FILE_CREATE_NONE, NULL, &inner_error);
			if (inner_error != NULL) {
				g_error("g_file_create: I/O error: %s", inner_error->message);
				g_error_free(inner_error);
			}
		}
	}
	
	GtkSourceBuffer * buffer = 
			GTK_SOURCE_BUFFER (gtk_text_view_get_buffer(GTK_TEXT_VIEW (self)));
	
	self->priv->file_saver = 
		gtk_source_file_saver_new(buffer, self->priv->source_file);
	
	gtk_source_file_saver_save_async(
		self->priv->file_saver, 
		G_PRIORITY_HIGH, NULL, 
		NULL, NULL, NULL, 
		qt_source_view_save_file_ready, self
	);
}

static void 
qt_source_view_create_components (QtSourceView * self, GFile * file) 
{
	self->priv->source_file = gtk_source_file_new();
	if (file != NULL) {
		gtk_source_file_set_location(self->priv->source_file, file);
		self->priv->word_completion = gtk_source_completion_words_new(
			_ ("Document words"), NULL
		);
		
		g_object_set(G_OBJECT (self->priv->word_completion),
			"activation", 
			GTK_SOURCE_COMPLETION_ACTIVATION_USER_REQUESTED | 
				GTK_SOURCE_COMPLETION_ACTIVATION_INTERACTIVE, 
			"interactive-delay", 25, 
			NULL
		);
		
		GError * inner_error;
		gboolean provider_added = FALSE;
		GtkSourceCompletion * completion;
		
		completion = gtk_source_view_get_completion(GTK_SOURCE_VIEW (self));
		provider_added = gtk_source_completion_add_provider(completion, 
			GTK_SOURCE_COMPLETION_PROVIDER (self->priv->word_completion), 
			&inner_error
		);
		
		if (!provider_added && (inner_error != NULL)) {
			g_error("gtk_source_completion_add_provider error: %s", 
					    inner_error->message);
			g_error_free(inner_error);
		}
	}
	
	GtkSourceStyleSchemeManager * scheme_manager = 
		gtk_source_style_scheme_manager_get_default();
	
	const gchar * color_scheme = 
		qt_text_editor_get_color_scheme(self->priv->editor);
	
	GtkSourceStyleScheme * source_scheme = 
		gtk_source_style_scheme_manager_get_scheme(scheme_manager, color_scheme);
	
	GtkSourceBuffer * buffer = GTK_SOURCE_BUFFER (
		gtk_text_view_get_buffer(GTK_TEXT_VIEW (self))
	);

	gtk_source_buffer_set_style_scheme(buffer, source_scheme);
	gtk_source_buffer_set_highlight_syntax(buffer, TRUE);

	gtk_source_buffer_set_highlight_matching_brackets(buffer,
		qt_text_editor_get_highlight_brackets(self->priv->editor)
	);
	
	qt_source_view_load_file(self);
}

static gint 
array_length (gpointer array) 
{
	int length = 0;
	if (array) {
		while (((gpointer *) array)[length]) {
			length++;
		}
	}
	
	return length;
}

static gchar * 
qt_source_view_get_file_path_from_uri (const gchar * uri) 
{
	gchar * result = NULL;
	gchar * path = NULL;
	gint offset = 0;
	
	if (g_str_has_prefix(uri, "file://")) {
		offset = strlen("file://");
		path = g_strndup(uri + offset, (gsize) -1);
	} else if (g_str_has_prefix(uri, "file:")) {
		offset = strlen("file:");
		path = g_strndup(uri + offset, (gsize) -1);
	}
	
	result = g_strstrip (path);
	return result;
}

static void 
qt_source_view_on_drag_data_received (GtkWidget * sender, 
																			GdkDragContext * context, 
																			gint x, gint y, 
																			GtkSelectionData * selection_data, 
																			guint target_type, 
																			guint _time,
																			gpointer data) 
{
	QtSourceView * self = QT_SOURCE_VIEW (sender);
	
	if (target_type == TARGET_TYPE_URI_LIST) {
		const guchar * data = gtk_selection_data_get_data(selection_data);
		gchar * uris = g_strstrip ((gchar *) data);
		gchar ** uri_list = g_strsplit(uris, "\n", 0);
		
		gint i, uri_list_length = array_length(uri_list);
		for (i = 0; i < uri_list_length; i++) {
			gchar * path = qt_source_view_get_file_path_from_uri(uri_list[i]);
			g_signal_emit_by_name(self, "drag-n-drop", path);
			g_free(path);
		}
	}
}

static void 
qt_source_view_set_font (QtSourceView * self, const gchar * font_description) 
{
	GError * inner_error;
	GtkCssProvider * css_provider = gtk_css_provider_new();
	const gchar * css_data = g_strconcat(
		"GtkSourceView { font: ", 
		font_description, 
		"; }", 
		NULL
	);
	
	gtk_css_provider_load_from_data(css_provider, css_data, -1, &inner_error);
	
	if (inner_error != NULL) {
		g_error("gtk_css_provider_load_from_data error: %s\n", 
					  inner_error->message);
		g_error_free(inner_error);
	}
	
	GtkStyleContext * style_context = 
		gtk_widget_get_style_context(GTK_WIDGET (self));
	
	gtk_style_context_add_provider(
		style_context, 
		GTK_STYLE_PROVIDER (css_provider), 
		GTK_STYLE_PROVIDER_PRIORITY_SETTINGS
	);
}

static void 
qt_source_view_set_properties (QtSourceView * self) 
{
	QtTextEditor * editor = self->priv->editor;
	self->priv->insert_matching_braces = qt_text_editor_get_insert_braces(editor);
	
	gtk_source_view_set_show_line_numbers(
		GTK_SOURCE_VIEW (self),
		qt_text_editor_get_show_line_numbers(editor)
	);
	
	gtk_source_view_set_show_right_margin(
		GTK_SOURCE_VIEW (self),
		qt_text_editor_get_show_right_margin(editor)
	);

	gtk_source_view_set_right_margin_position(
		GTK_SOURCE_VIEW (self),
		qt_text_editor_get_right_margin_at(editor)
	);

	gtk_source_view_set_highlight_current_line(
		GTK_SOURCE_VIEW (self),
		qt_text_editor_get_highlight_current_line(editor)
	);

	gtk_source_view_set_tab_width(
		GTK_SOURCE_VIEW (self),
		qt_text_editor_get_tab_width(editor)
	);
	
	gtk_source_view_set_insert_spaces_instead_of_tabs(
		GTK_SOURCE_VIEW (self),
		qt_text_editor_get_insert_spaces(editor)
	);
	
	gtk_source_view_set_auto_indent(
		GTK_SOURCE_VIEW (self),
		qt_text_editor_get_auto_indent(editor)
	);
	
	qt_source_view_set_font(self, qt_text_editor_get_editor_font(editor));
	
	if (qt_text_editor_get_show_grid_pattern(editor)) {
		gtk_source_view_set_background_pattern(
			GTK_SOURCE_VIEW (self), 
			GTK_SOURCE_BACKGROUND_PATTERN_TYPE_GRID
		);
	} else {
		gtk_source_view_set_background_pattern(
			GTK_SOURCE_VIEW (self), 
			GTK_SOURCE_BACKGROUND_PATTERN_TYPE_NONE
		);
	}
	
	gtk_source_view_set_smart_home_end(
		GTK_SOURCE_VIEW (self), 
		GTK_SOURCE_SMART_HOME_END_BEFORE
	);
	
	gtk_text_view_set_wrap_mode(
		GTK_TEXT_VIEW (self), 
		GTK_WRAP_NONE
	);
	
	gtk_drag_dest_set(GTK_WIDGET (self), 
									  GTK_DEST_DEFAULT_ALL, 
									  target_list, 
									  G_N_ELEMENTS (target_list), 
									  GDK_ACTION_COPY);
	
	self->priv->search_settings = gtk_source_search_settings_new();
	
	gtk_source_search_settings_set_at_word_boundaries(
		self->priv->search_settings, FALSE
	);
	
	gtk_source_search_settings_set_case_sensitive(
		self->priv->search_settings, FALSE
	);
	
	gtk_source_search_settings_set_regex_enabled(
		self->priv->search_settings, FALSE
	);
	
	gtk_source_search_settings_set_search_text(
		self->priv->search_settings, NULL
	);
	
	gtk_source_search_settings_set_wrap_around(
		self->priv->search_settings, TRUE
	);
	
	g_signal_connect (GTK_WIDGET (self), 
									  "drag-data-received", 
									  G_CALLBACK (qt_source_view_on_drag_data_received), 
									  self);
}

const gchar * 
qt_source_view_get_language_name (QtSourceView * self) 
{
	const gchar * language_name = NULL;
	GtkSourceBuffer * buffer = 
		GTK_SOURCE_BUFFER (gtk_text_view_get_buffer(GTK_TEXT_VIEW (self)));
	GtkSourceLanguage * language = gtk_source_buffer_get_language(buffer);
	
	if (language != NULL) {
		language_name = gtk_source_language_get_name(language);
	}
	
	return language_name;
}

void 
qt_source_view_change_language (QtSourceView * self, const gchar * language) 
{
	GtkSourceLanguageManager * language_manager = 
		gtk_source_language_manager_get_default();
	GtkSourceLanguage * source_lang = 
		gtk_source_language_manager_get_language(language_manager, language);
	GtkSourceBuffer * buffer = GTK_SOURCE_BUFFER (
		gtk_text_view_get_buffer(GTK_TEXT_VIEW (self))
	);
	
	gtk_source_buffer_set_language(buffer, source_lang);
	gtk_text_buffer_set_modified(GTK_TEXT_BUFFER (buffer), FALSE);
}

static void 
qt_source_view_connect_signals (QtSourceView * self) 
{
	
}

QtSourceView * 
qt_source_view_new (QtTextEditor * editor, GFile * file) 
{
	QtSourceView * new_source_view;
	new_source_view = g_object_new (
		QT_SOURCE_VIEW_TYPE, 
		NULL
	);
	
	new_source_view->priv = qt_source_view_get_instance_private(new_source_view);
	new_source_view->priv->editor = editor;
	
	qt_source_view_create_components(new_source_view, file);
	qt_source_view_connect_signals(new_source_view);
	qt_source_view_set_properties(new_source_view);
	return new_source_view;
}

