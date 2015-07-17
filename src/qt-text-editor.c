#include "qt-text-editor.h"

struct _QtTextEditor 
{
	GObject parent;
	QtTextEditorPrivate * priv;
};

struct _QtTextEditorClass
{
	GObjectClass parent_class;
};

struct _QtTextEditorPrivate 
{
	gboolean show_line_numbers;
	gboolean show_right_margin;
	gint right_margin_at;
	gboolean show_grid_pattern;
	gboolean highlight_current_line;
	gboolean highlight_brackets;
	gboolean use_text_wrap;
	
	gint tab_width;
	gboolean insert_spaces;
	gboolean auto_indent;
	gboolean insert_braces;
	gboolean save_workspace;
	
	gboolean use_default_typo;
	const gchar * editor_font;
	const gchar * color_scheme;
	gboolean prefer_dark;
};

G_DEFINE_TYPE_WITH_PRIVATE (QtTextEditor, qt_text_editor, G_TYPE_OBJECT);

enum {
	QT_TEXT_EDITOR_DUMMY_PROPERTY,
	QT_TEXT_EDITOR_SHOW_LINE_NUMBERS_PROPERTY, 
	QT_TEXT_EDITOR_SHOW_RIGHT_MARGIN, 
	QT_TEXT_EDITOR_RIGHT_MARGIN_AT, 
	QT_TEXT_EDITOR_SHOW_GRID_PATTERN, 
	QT_TEXT_EDITOR_HIGHLIGHT_CURRENT_LINE, 
	QT_TEXT_EDITOR_HIGHLIGHT_BRACKETS, 
	QT_TEXT_EDITOR_USE_TEXT_WRAP, 
	QT_TEXT_EDITOR_TAB_WIDTH, 
	QT_TEXT_EDITOR_INSERT_SPACES, 
	QT_TEXT_EDITOR_AUTO_INDENT, 
	QT_TEXT_EDITOR_INSERT_BRACES, 
	QT_TEXT_EDITOR_SAVE_WORKSPACE, 
	QT_TEXT_EDITOR_USE_DEFAULT_TYPO, 
	QT_TEXT_EDITOR_EDITOR_FONT, 
	QT_TEXT_EDITOR_COLOR_SCHEME, 
	QT_TEXT_EDITOR_PREFER_DARK, 
	N_PROPERTIES
};

static GParamSpec * obj_properties[N_PROPERTIES] = { NULL, };

static void 
qt_text_editor_set_property (GObject * object, 
														 guint property_id, 
														 const GValue * value, 
														 GParamSpec * pspec) 
{
	QtTextEditor * self = QT_TEXT_EDITOR (object);
	
	switch (property_id) {
		QT_TEXT_EDITOR_SHOW_LINE_NUMBERS_PROPERTY:
			self->priv->show_line_numbers = g_value_get_boolean(value);
			break;
		QT_TEXT_EDITOR_SHOW_RIGHT_MARGIN:
			self->priv->show_right_margin = g_value_get_boolean(value);
			break;
		QT_TEXT_EDITOR_RIGHT_MARGIN_AT:
			self->priv->right_margin_at = g_value_get_int(value);
			break;
		QT_TEXT_EDITOR_SHOW_GRID_PATTERN:
			self->priv->show_grid_pattern = g_value_get_boolean(value);
			break;
		QT_TEXT_EDITOR_HIGHLIGHT_CURRENT_LINE:
			self->priv->highlight_current_line = g_value_get_boolean(value);
			break;
		QT_TEXT_EDITOR_HIGHLIGHT_BRACKETS:
			self->priv->highlight_brackets = g_value_get_boolean(value);
			break;
		QT_TEXT_EDITOR_USE_TEXT_WRAP:
			self->priv->use_text_wrap = g_value_get_boolean(value);
			break;
		QT_TEXT_EDITOR_TAB_WIDTH:
			self->priv->tab_width = g_value_get_int(value);
			break;
		QT_TEXT_EDITOR_INSERT_SPACES:
			self->priv->insert_spaces = g_value_get_boolean(value);
			break;
		QT_TEXT_EDITOR_AUTO_INDENT:
			self->priv->auto_indent = g_value_get_boolean(value);
			break;
		QT_TEXT_EDITOR_INSERT_BRACES:
			self->priv->insert_braces = g_value_get_boolean(value);
			break;
		QT_TEXT_EDITOR_SAVE_WORKSPACE:
			self->priv->save_workspace = g_value_get_boolean(value);
			break;
		QT_TEXT_EDITOR_USE_DEFAULT_TYPO:
			self->priv->use_default_typo = g_value_get_boolean(value);
			break;
		QT_TEXT_EDITOR_EDITOR_FONT:
			self->priv->editor_font = g_value_get_string(value);
			break;
		QT_TEXT_EDITOR_COLOR_SCHEME:
			self->priv->color_scheme = g_value_get_string(value);
			break;
		QT_TEXT_EDITOR_PREFER_DARK:
			self->priv->prefer_dark = g_value_get_boolean(value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	 		break;
	}
}

static void 
qt_text_editor_get_property (GObject * object, 
														 guint property_id, 
														 GValue * value, 
														 GParamSpec * pspec) 
{
	QtTextEditor * self = QT_TEXT_EDITOR (object);
	
	switch (property_id) {
		QT_TEXT_EDITOR_SHOW_LINE_NUMBERS_PROPERTY:
			g_value_set_boolean(value, self->priv->show_line_numbers);
			break;
		QT_TEXT_EDITOR_SHOW_RIGHT_MARGIN:
			g_value_set_boolean(value, self->priv->show_right_margin);
			break;
		QT_TEXT_EDITOR_RIGHT_MARGIN_AT:
			g_value_set_int(value, self->priv->right_margin_at);
			break;
		QT_TEXT_EDITOR_SHOW_GRID_PATTERN:
			g_value_set_boolean(value, self->priv->show_grid_pattern);
			break;
		QT_TEXT_EDITOR_HIGHLIGHT_CURRENT_LINE:
			g_value_set_boolean(value, self->priv->highlight_current_line);
			break;
		QT_TEXT_EDITOR_HIGHLIGHT_BRACKETS:
			g_value_set_boolean(value, self->priv->highlight_brackets);
			break;
		QT_TEXT_EDITOR_USE_TEXT_WRAP:
			g_value_set_boolean(value, self->priv->use_text_wrap);
			break;
		QT_TEXT_EDITOR_TAB_WIDTH:
			g_value_set_int(value, self->priv->tab_width);
			break;
		QT_TEXT_EDITOR_INSERT_SPACES:
			g_value_set_boolean(value, self->priv->insert_spaces);
			break;
		QT_TEXT_EDITOR_AUTO_INDENT:
			g_value_set_boolean(value, self->priv->auto_indent);
			break;
		QT_TEXT_EDITOR_INSERT_BRACES:
			g_value_set_boolean(value, self->priv->insert_braces);
			break;
		QT_TEXT_EDITOR_SAVE_WORKSPACE:
			g_value_set_boolean(value, self->priv->save_workspace);
			break;
		QT_TEXT_EDITOR_USE_DEFAULT_TYPO:
			g_value_set_boolean(value, self->priv->use_default_typo);
			break;
		QT_TEXT_EDITOR_EDITOR_FONT:
			g_value_set_string(value, self->priv->editor_font);
			break;
		QT_TEXT_EDITOR_COLOR_SCHEME:
			g_value_set_string(value, self->priv->color_scheme);
			break;
		QT_TEXT_EDITOR_PREFER_DARK:
			g_value_set_boolean(value, self->priv->prefer_dark);
			break;
		default:
	 		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	 		break;
	}
}

static void 
qt_text_editor_init (QtTextEditor * self)
{}

static void 
qt_text_editor_class_init (QtTextEditorClass * class)
{
	GObjectClass * g_class = G_OBJECT_CLASS (class);
	g_class->set_property = qt_text_editor_set_property;
	g_class->get_property = qt_text_editor_get_property;
	
	obj_properties[QT_TEXT_EDITOR_SHOW_LINE_NUMBERS_PROPERTY] = 
		g_param_spec_boolean("show-line-numbers", 
											 	 "show-line-numbers", 
											   "show-line-numbers",  
											   FALSE, 
											   G_PARAM_READWRITE);

	obj_properties[QT_TEXT_EDITOR_SHOW_RIGHT_MARGIN] = 
		g_param_spec_boolean("show-right-margin", 
											   "show-right-margin", 
											   "show-right-margin", 
											   FALSE, 
											   G_PARAM_READWRITE);

	obj_properties[QT_TEXT_EDITOR_RIGHT_MARGIN_AT] = 
		g_param_spec_int("right-margin-at", 
									   "right-margin-at", 
									   "right-margin-at", 
									   80, 120, 80, 
									   G_PARAM_READWRITE);

	obj_properties[QT_TEXT_EDITOR_SHOW_GRID_PATTERN] = 
		g_param_spec_boolean("show-grid-pattern", 
											   "show-grid-pattern", 
											 	 "show-grid-pattern", 
											   FALSE, 
											   G_PARAM_READWRITE);

	obj_properties[QT_TEXT_EDITOR_HIGHLIGHT_CURRENT_LINE] = 
		g_param_spec_boolean("highlight-current-line", 
											   "highlight-current-line", 
											   "highlight-current-line", 
											   FALSE, 
											   G_PARAM_READWRITE);

	obj_properties[QT_TEXT_EDITOR_HIGHLIGHT_BRACKETS] = 
		g_param_spec_boolean("highlight-brackets", 
											 	 "highlight-brackets", 
											   "highlight-brackets", 
											   FALSE, 
											   G_PARAM_READWRITE);

	obj_properties[QT_TEXT_EDITOR_USE_TEXT_WRAP] = 
		g_param_spec_boolean("use-text-wrap", 
											   "use-text-wrap", 
											   "use-text-wrap", 
											   FALSE, 
											   G_PARAM_READWRITE);

	obj_properties[QT_TEXT_EDITOR_TAB_WIDTH] = 
		g_param_spec_int("tab-width", 
									   "tab-width", 
									   "tab-width", 
									   2, 8, 4, 
									   G_PARAM_READWRITE);

	obj_properties[QT_TEXT_EDITOR_INSERT_SPACES] = 
		g_param_spec_boolean("insert-spaces", 
											   "insert-spaces", 
											   "insert-spaces", 
											   FALSE, 
											   G_PARAM_READWRITE);

	obj_properties[QT_TEXT_EDITOR_AUTO_INDENT] = 
		g_param_spec_boolean("auto-indent", 
											   "auto-indent", 
											   "auto-indent", 
											   FALSE, 
											   G_PARAM_READWRITE);

	obj_properties[QT_TEXT_EDITOR_INSERT_BRACES] = 
		g_param_spec_boolean("insert-braces", 
											   "insert-braces", 
											   "insert-braces", 
											   FALSE, 
											   G_PARAM_READWRITE);

	obj_properties[QT_TEXT_EDITOR_SAVE_WORKSPACE] = 
		g_param_spec_boolean("save-workspace", 
											   "save-workspace", 
											   "save-workspace", 
											   FALSE, 
											   G_PARAM_READWRITE);

	obj_properties[QT_TEXT_EDITOR_USE_DEFAULT_TYPO] = 
		g_param_spec_boolean("use-default-typo", 
											   "use-default-typo", 
											   "use-default-typo", 
											   TRUE, 
											   G_PARAM_READWRITE);

	obj_properties[QT_TEXT_EDITOR_EDITOR_FONT] = 
		g_param_spec_string("editor-font", 
											  "editor-font", 
											  "editor-font", 
											  "Monospace 10", 
											  G_PARAM_READWRITE);

	obj_properties[QT_TEXT_EDITOR_COLOR_SCHEME] = 
		g_param_spec_string("color-scheme", 
											  "color-scheme", 
											  "color-scheme", 
											  "classic", 
											  G_PARAM_READWRITE);

	obj_properties[QT_TEXT_EDITOR_PREFER_DARK] = 
		g_param_spec_boolean("prefer-dark", 
											   "prefer-dark", 
											   "prefer-dark", 
											   FALSE, 
											   G_PARAM_READWRITE);
	
	g_object_class_install_properties(
		g_class, 
		N_PROPERTIES, 
		obj_properties
	);
}

gboolean 
qt_text_editor_get_show_line_numbers (QtTextEditor * self) 
{
	return self->priv->show_line_numbers;
}

void 
qt_text_editor_set_show_line_numbers (QtTextEditor * self, gboolean value) 
{
	if (self->priv->show_line_numbers != value) {
		self->priv->show_line_numbers = value;
	}

	g_object_notify(G_OBJECT (self), "show-line-numbers");
}

gboolean 
qt_text_editor_get_show_right_margin (QtTextEditor * self) 
{
	return self->priv->show_right_margin;
}

void 
qt_text_editor_set_show_right_margin (QtTextEditor * self, gboolean value) 
{
	if (self->priv->show_right_margin != value) {
		self->priv->show_right_margin = value;
	}

	g_object_notify(G_OBJECT (self), "show-right-margin");
}

gint 
qt_text_editor_get_right_margin_at (QtTextEditor * self) 
{
	return self->priv->right_margin_at;
}

void 
qt_text_editor_set_right_margin_at (QtTextEditor * self, gint value) 
{
	if (self->priv->right_margin_at != value) {
		self->priv->right_margin_at = value;
	}

	g_object_notify(G_OBJECT (self), "right-margin-at");
}

gboolean 
qt_text_editor_get_show_grid_pattern (QtTextEditor * self) 
{
	return self->priv->show_grid_pattern;
}

void 
qt_text_editor_set_show_grid_pattern (QtTextEditor * self, gboolean value) 
{
	if (self->priv->show_grid_pattern != value) {
		self->priv->show_grid_pattern = value;
	}

	g_object_notify(G_OBJECT (self), "show-grid-pattern");
}

gboolean 
qt_text_editor_get_highlight_current_line (QtTextEditor * self) 
{
	return self->priv->highlight_current_line;
}

void 
qt_text_editor_set_highlight_current_line (QtTextEditor * self, gboolean value) 
{
	if (self->priv->highlight_current_line != value) {
		self->priv->highlight_current_line = value;
	}

	g_object_notify(G_OBJECT (self), "highlight-current-line");
}

gboolean 
qt_text_editor_get_highlight_brackets (QtTextEditor * self) 
{
	return self->priv->highlight_brackets;
}

void 
qt_text_editor_set_highlight_brackets (QtTextEditor * self, gboolean value) 
{
	if (self->priv->highlight_brackets != value) {
		self->priv->highlight_brackets = value;
	}

	g_object_notify(G_OBJECT (self), "highlight-brackets");
}

gboolean 
qt_text_editor_get_use_text_wrap (QtTextEditor * self) 
{
	return self->priv->use_text_wrap;
}

void 
qt_text_editor_set_use_text_wrap (QtTextEditor * self, gboolean value) 
{
	if (self->priv->use_text_wrap != value) {
		self->priv->use_text_wrap = value;
	}

	g_object_notify(G_OBJECT (self), "use-text-wrap");
}

gint 
qt_text_editor_get_tab_width (QtTextEditor * self) 
{
	return self->priv->tab_width;
}

void 
qt_text_editor_set_tab_width (QtTextEditor * self, gint value) 
{
	if (self->priv->tab_width != value) {
		self->priv->tab_width = value;
	}

	g_object_notify(G_OBJECT (self), "tab-width");
}

gboolean 
qt_text_editor_get_insert_spaces (QtTextEditor * self) 
{
	return self->priv->insert_spaces;
}

void 
qt_text_editor_set_insert_spaces (QtTextEditor * self, gboolean value) 
{
	if (self->priv->insert_spaces != value) {
		self->priv->insert_spaces = value;
	}

	g_object_notify(G_OBJECT (self), "insert-spaces");
}

gboolean 
qt_text_editor_get_auto_indent (QtTextEditor * self) 
{
	return self->priv->auto_indent;
}

void 
qt_text_editor_set_auto_indent (QtTextEditor * self, gboolean value) 
{
	if (self->priv->auto_indent != value) {
		self->priv->auto_indent = value;
	}

	g_object_notify(G_OBJECT (self), "auto-indent");
}

gboolean 
qt_text_editor_get_insert_braces (QtTextEditor * self) 
{
	return self->priv->insert_braces;
}

void 
qt_text_editor_set_insert_braces (QtTextEditor * self, gboolean value) 
{
	if (self->priv->insert_braces != value) {
		self->priv->insert_braces = value;
	}

	g_object_notify(G_OBJECT (self), "insert-braces");
}

gboolean 
qt_text_editor_get_save_workspace (QtTextEditor * self) 
{
	return self->priv->save_workspace;
}

void 
qt_text_editor_set_save_workspace (QtTextEditor * self, gboolean value) 
{
	if (self->priv->save_workspace != value) {
		self->priv->save_workspace = value;
	}

	g_object_notify(G_OBJECT (self), "save-workspace");
}

gboolean 
qt_text_editor_get_use_default_typo (QtTextEditor * self) 
{
	return self->priv->use_default_typo;
}

void 
qt_text_editor_set_use_default_typo (QtTextEditor * self, gboolean value) 
{
	if (self->priv->use_default_typo != value) {
		self->priv->use_default_typo = value;
	}

	g_object_notify(G_OBJECT (self), "use-default-typo");
}

const gchar * 
qt_text_editor_get_editor_font (QtTextEditor * self) 
{
	return self->priv->editor_font;
}

void 
qt_text_editor_set_editor_font (QtTextEditor * self, const gchar * value) 
{
	if (g_strcmp0(self->priv->editor_font, value) != 0) {
		self->priv->editor_font = g_strdup(value);
	}

	g_object_notify(G_OBJECT (self), "editor-font");
}

const gchar * 
qt_text_editor_get_color_scheme (QtTextEditor * self) 
{
	return self->priv->color_scheme;
}

void 
qt_text_editor_set_color_scheme (QtTextEditor * self, const gchar * value) 
{
	if (g_strcmp0(self->priv->color_scheme, value) != 0) {
		self->priv->color_scheme = g_strdup(value);
	}

	g_object_notify(G_OBJECT (self), "color-scheme");
}

gboolean 
qt_text_editor_get_prefer_dark (QtTextEditor * self) 
{
	return self->priv->prefer_dark;
}

void 
qt_text_editor_set_prefer_dark (QtTextEditor * self, gboolean value) 
{
	if (self->priv->prefer_dark != value) {
		self->priv->prefer_dark = value;
	}

	g_object_notify(G_OBJECT (self), "prefer-dark");
}

QtTextEditor * 
qt_text_editor_new (void) 
{
	QtTextEditor * new_text_editor;
	 
	new_text_editor = g_object_new (QT_TEXT_EDITOR_TYPE, NULL);
	new_text_editor->priv = qt_text_editor_get_instance_private(new_text_editor);
	
	return new_text_editor;
}

