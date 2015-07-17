#include <gtksourceview/completion-providers/words/gtksourcecompletionwords.h>
#include "qt-sourceview.h"

#include "config.h"
#include <glib/gi18n.h>

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
{}

static void 
qt_source_view_create_components (QtSourceView * self, const gchar * filename) 
{
	self->priv->source_file = gtk_source_file_new();
	if (filename != NULL) {
		GFile * file = g_file_new_for_path(filename);
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
	
	/* override_font */
	
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
	
	/* drag_dest_set */
	
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
}

static void 
qt_source_view_connect_signals (QtSourceView * self) 
{
	
}

QtSourceView * 
qt_source_view_new (QtTextEditor * editor, const gchar * filename) 
{
	QtSourceView * new_source_view;
	new_source_view = g_object_new (
		QT_SOURCE_VIEW_TYPE, 
		NULL
	);
	
	new_source_view->priv = qt_source_view_get_instance_private(new_source_view);
	qt_source_view_create_components(new_source_view, filename);
	qt_source_view_connect_signals(new_source_view);
	qt_source_view_set_properties(new_source_view);
	return new_source_view;
}

