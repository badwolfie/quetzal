/* qt-text-editor.h
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
