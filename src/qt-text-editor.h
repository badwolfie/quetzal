#ifndef QT_TEXT_EDITOR_H
#define QT_TEXT_EDITOR_H

#include <glib-object.h>

#define QT_TEXT_EDITOR_TYPE (qt_text_editor_get_type())
#define QT_TEXT_EDITOR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), QT_TEXT_EDITOR_TYPE, QtTextEditor))

typedef struct _QtTextEditor QtTextEditor;
typedef struct _QtTextEditorClass QtTextEditorClass;
typedef struct _QtTextEditorPrivate QtTextEditorPrivate; 

GType qt_text_editor_get_type (void);
QtTextEditor * qt_text_editor_new (void);

gboolean qt_text_editor_get_show_line_numbers (QtTextEditor * self);
void qt_text_editor_set_show_line_numbers (QtTextEditor * self, gboolean value);

gboolean qt_text_editor_get_show_right_margin (QtTextEditor * self);
void qt_text_editor_set_show_right_margin (QtTextEditor * self, gboolean value);

gint qt_text_editor_get_right_margin_at (QtTextEditor * self);
void qt_text_editor_set_right_margin_at (QtTextEditor * self, gint value);

gboolean qt_text_editor_get_show_grid_pattern (QtTextEditor * self);
void qt_text_editor_set_show_grid_pattern (QtTextEditor * self, gboolean value);

gboolean qt_text_editor_get_highlight_current_line (QtTextEditor * self);
void qt_text_editor_set_highlight_current_line (QtTextEditor * self, gboolean value);

gboolean qt_text_editor_get_highlight_brackets (QtTextEditor * self);
void qt_text_editor_set_highlight_brackets (QtTextEditor * self, gboolean value);

gboolean qt_text_editor_get_use_text_wrap (QtTextEditor * self);
void qt_text_editor_set_use_text_wrap (QtTextEditor * self, gboolean value);

gint qt_text_editor_get_tab_width (QtTextEditor * self);
void qt_text_editor_set_tab_width (QtTextEditor * self, gint value);

gboolean qt_text_editor_get_insert_spaces (QtTextEditor * self);
void qt_text_editor_set_insert_spaces (QtTextEditor * self, gboolean value);

gboolean qt_text_editor_get_auto_indent (QtTextEditor * self);
void qt_text_editor_set_auto_indent (QtTextEditor * self, gboolean value);

gboolean qt_text_editor_get_insert_braces (QtTextEditor * self);
void qt_text_editor_set_insert_braces (QtTextEditor * self, gboolean value);

gboolean qt_text_editor_get_save_workspace (QtTextEditor * self);
void qt_text_editor_set_save_workspace (QtTextEditor * self, gboolean value);

gboolean qt_text_editor_get_use_default_typo (QtTextEditor * self);
void qt_text_editor_set_use_default_typo (QtTextEditor * self, gboolean value);

const gchar * qt_text_editor_get_editor_font (QtTextEditor * self);
void qt_text_editor_set_editor_font (QtTextEditor * self, const gchar * value);

const gchar * qt_text_editor_get_color_scheme (QtTextEditor * self);
void qt_text_editor_set_color_scheme (QtTextEditor * self, const gchar * value);

gboolean qt_text_editor_get_prefer_dark (QtTextEditor * self);
void qt_text_editor_set_prefer_dark (QtTextEditor * self, gboolean value);


#endif /* QT_TEXT_EDITOR_H */
