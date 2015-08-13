/* qt-appwindow.c
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

#include "qt-appwindow.h"
#include "qt-headerbar.h"
#include "qt-documentbar.h"
#include "qt-sourceview.h"
#include "qt-document.h"
#include "quetzal.h"

#include "config.h"
#include <glib/gi18n.h>
#include <vte/vte.h>
#include <stdlib.h>

typedef struct _GListForeachParams GListForeachParams;

struct _GListForeachParams 
{
  gchar * workspace;
  gchar * untitled;
};

struct _QtAppWindow 
{
	GtkApplicationWindow parent;
	QtAppWindowPrivate * priv;
};

struct _QtAppWindowClass 
{
	GtkApplicationWindowClass parent_class;
};

struct _QtAppWindowPrivate 
{
	QtTextEditor * editor;
	QtHeaderBar * header_bar;
	QtHeaderBar * fs_header_bar;
	
	GtkPaned * content;
	GtkSearchBar * search_bar;
	GtkSearchEntry * search_entry;
	GtkButton * search_next;
	GtkButton * search_prev;
	
	QtDocumentBar * doc_bar;
	GtkStack * documents;
	VteTerminal * terminal;	
	
	GtkSourceSearchContext * search_context;
	const gchar * untitled;
	gint counter;
};

G_DEFINE_TYPE_WITH_PRIVATE (QtAppWindow, qt_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void 
qt_app_window_init (QtAppWindow * self) 
{}

static void 
qt_app_window_class_init (QtAppWindowClass * class) 
{}

static void 
qt_app_window_check_doc_count (QtAppWindow * self) 
{
  GList * open_docs = qt_document_bar_get_doc_list(self->priv->doc_bar);
  GList * open_extra_docs = 
    qt_document_bar_get_extra_doc_list(self->priv->doc_bar);
  gint opened_docs_count = 
    g_list_length(open_docs) + g_list_length(open_extra_docs);
  
  if (opened_docs_count == 0) {
    gtk_header_bar_set_subtitle(GTK_HEADER_BAR (self->priv->header_bar), "");
    gtk_header_bar_set_subtitle(GTK_HEADER_BAR (self->priv->fs_header_bar), "");
  }
}

QtTextEditor * 
qt_app_window_get_editor (QtAppWindow * self) 
{
	return self->priv->editor;
}

static void qt_app_window_add_doc_from_file (QtAppWindow * self, GFile * file);

static void 
qt_app_window_load_workspace (QtAppWindow * self) 
{
  gchar * workspace = NULL;
  GError * inner_error = NULL;
  gboolean loaded = g_file_get_contents(
    g_strconcat(g_get_home_dir(), "/.quetzal/saved-workspace", NULL), 
    &workspace, NULL, 
    &inner_error
  );
  
  if (!loaded || (inner_error != NULL)) {
    g_error("g_file_get_contents: I/O error: %s", inner_error->message);
	  g_error_free(inner_error);
  }
  
  if (workspace != NULL) {
    if (g_strcmp0(workspace, "") == 0) { 
      qt_app_window_create_new_doc(NULL, self);
    } else {
      gchar ** workspace_files = g_strsplit(workspace, "\n", 0);
      gint workspace_len = array_length(workspace_files);

      gint i, counter = 0;
      for (i = 0; i < workspace_len; i++) {
        if (g_strcmp0(workspace_files[i], "") != 0) {
          GFile * file = g_file_new_for_path(workspace_files[i]);
          if (g_file_query_exists(file, NULL)) {
            qt_app_window_add_doc_from_file(self, file);
            counter++;
          }
        }
      }
      
      if (counter == 0) 
        qt_app_window_create_new_doc(NULL, self);
    }
  }
}

static void 
qt_app_window_concat_to_saved_workspace (gpointer data, gpointer user_data) 
{
  if (!QT_IS_DOCUMENT (data)) return ; 
  
  QtDocument * entry = QT_DOCUMENT (data);
  GListForeachParams * params = (GListForeachParams *) user_data;
  
  const gchar * doc_path = qt_document_get_doc_path(entry);
  if (g_strcmp0(doc_path, params->untitled) == 0) return ;

  params->workspace = g_strconcat(params->workspace, doc_path, "\n", NULL);
}

GListForeachParams * 
g_list_foreach_params_new (void) 
{
  GListForeachParams * params = 
    (GListForeachParams *) malloc(sizeof (GListForeachParams));
  params->untitled = g_strdup(_ ("Untitled file"));
  params->workspace = g_strdup("");
  return params;
}

static void 
qt_app_window_save_workspace (QtAppWindow * self) 
{
  GListForeachParams * params = g_list_foreach_params_new();
  GList * docs = qt_document_bar_get_doc_list(self->priv->doc_bar);
  GList * extra_docs = qt_document_bar_get_extra_doc_list(self->priv->doc_bar);
  
  if (docs != NULL)
    g_list_foreach(docs, qt_app_window_concat_to_saved_workspace, params);
  
  if (extra_docs != NULL)
    g_list_foreach(extra_docs, qt_app_window_concat_to_saved_workspace, params);
  
  GError * inner_error = NULL;
  gboolean saved = g_file_set_contents(
    g_strconcat(g_get_home_dir(), "/.quetzal/saved-workspace", NULL), 
    params->workspace, -1,
    &inner_error
  );
  
  if (!saved || (inner_error != NULL)) {
    g_error("g_file_set_contents: I/O error: %s", inner_error->message);
	  g_error_free(inner_error);
  }
}

static void 
qt_app_window_quit_cb (GtkWidget * sender, gpointer data) 
{
  QtAppWindow * self = QT_APP_WINDOW (data);
  gboolean save = qt_text_editor_get_save_workspace(self->priv->editor);
  
  if ((self->priv->editor != NULL) && save) {
    qt_app_window_save_workspace(self);
  }
}

void 
qt_app_window_set_arg_files (QtAppWindow * self, 
                             GFile ** arg_files, 
                             gint n_files) 
{
  gboolean load_workspace = 
    qt_text_editor_get_save_workspace(self->priv->editor);
  if ((self->priv->editor != NULL) && load_workspace) {
    qt_app_window_load_workspace(self);
  }
  
  gint i;
  if (arg_files != NULL) {
    for (i = 0; i < n_files; i++) {
      if (g_file_query_exists(arg_files[i], NULL)) 
        qt_app_window_add_doc_from_file(self, arg_files[i]);
    }
  }
}

static void 
qt_app_window_on_drag_n_drop (QtDocument * sender, 
                              GFile * file, 
                              gpointer data) 
{
  QtAppWindow * self = QT_APP_WINDOW (data);
  qt_app_window_add_doc_from_file(self, file);
}

static void 
qt_app_window_changes_done (QtSourceView * sender, 
                            gboolean buffer_modified, 
                            gpointer data) 
{
  QtAppWindow * self = QT_APP_WINDOW (data);
  QtDocument * current_doc = 
    qt_document_bar_get_current_doc(self->priv->doc_bar);
  
  gboolean doc_modified = qt_document_get_doc_is_modified(current_doc);
  
  if (buffer_modified) {
    if (!doc_modified) {
      // refrescar statusbar 
      qt_document_set_doc_is_modified(current_doc, TRUE);
    }
  } else {
    if (doc_modified) {
      qt_document_set_doc_is_modified(current_doc, FALSE);
    }
  }
}

static void 
qt_app_window_add_new_doc (QtAppWindow * self, QtDocument * new_doc) 
{
  g_signal_connect (new_doc, "view-drag-n-drop", 
                    G_CALLBACK (qt_app_window_on_drag_n_drop), 
                    self);
  
	const gchar * doc_name = g_strdup_printf("tab - %d", self->priv->counter++);
  GtkScrolledWindow * doc_scroll = qt_document_get_doc_scroll(new_doc);
	gtk_stack_add_named(self->priv->documents, GTK_WIDGET (doc_scroll), doc_name);
	qt_document_bar_add_doc(self->priv->doc_bar, new_doc);
  
  QtSourceView * doc_view = qt_document_get_doc_view(new_doc);
  g_signal_connect (G_OBJECT (doc_view), 
                    "buffer-modified", 
                    G_CALLBACK (qt_app_window_changes_done), 
                    self);
	
	const gchar * header_title = g_strdup(qt_document_get_doc_path(new_doc));
  gtk_header_bar_set_subtitle(GTK_HEADER_BAR (self->priv->fs_header_bar), 
                              header_title);
  gtk_header_bar_set_subtitle(GTK_HEADER_BAR (self->priv->header_bar), 
                              header_title);
  qt_app_window_check_doc_count(self);
}

void 
qt_app_window_create_new_doc (GObject * sender, gpointer data) 
{
	QtAppWindow * self = QT_APP_WINDOW (data);
	QtDocument * doc = qt_document_new(self->priv->editor, NULL);
	qt_app_window_add_new_doc(self, doc);
}

static void 
qt_app_window_add_doc_from_file (QtAppWindow * self, GFile * file) 
{
  if (qt_document_bar_doc_is_opened(self->priv->doc_bar, file)) {
    qt_app_window_check_doc_count(self);
    return ;
  }
  
  QtDocument * doc = qt_document_new(self->priv->editor, file);
  qt_app_window_add_new_doc(self, doc);
  
  /* refrescar lenguaje */
  qt_app_window_check_doc_count(self);
}

static void 
qt_app_window_on_selected_files (gpointer data, gpointer user_data) 
{
  GFile * entry = G_FILE (data);
  QtAppWindow * self = QT_APP_WINDOW (user_data);
  qt_app_window_add_doc_from_file(self, entry);
}

void 
qt_app_window_open_file (GObject * sender, gpointer data) 
{
  QtAppWindow * self = QT_APP_WINDOW (data);
  GtkFileChooserDialog * file_chooser;
  file_chooser = GTK_FILE_CHOOSER_DIALOG (
    gtk_file_chooser_dialog_new(
      _ ("Open file"), GTK_WINDOW (self), 
      GTK_FILE_CHOOSER_ACTION_OPEN, 
      _ ("Cancel"), GTK_RESPONSE_CANCEL, 
      _ ("Open"), GTK_RESPONSE_ACCEPT, 
      NULL
    )
  );
  
  /* set_current_folder */
  
  g_object_set(G_OBJECT (file_chooser), 
               "select-multiple", TRUE, 
               NULL);
  
  if (gtk_dialog_run(GTK_DIALOG (file_chooser)) == GTK_RESPONSE_ACCEPT) {
    GSList * selected_files = 
      gtk_file_chooser_get_files(GTK_FILE_CHOOSER (file_chooser));
      g_slist_foreach(selected_files, qt_app_window_on_selected_files, self);
  }
  
  gtk_widget_destroy(GTK_WIDGET (file_chooser));
}

void 
qt_app_window_save_doc_to_file (GObject * sender, gpointer data) 
{
  QtAppWindow * self = QT_APP_WINDOW (data);
  QtDocument * current_doc = 
    qt_document_bar_get_current_doc(self->priv->doc_bar);
  
  if (current_doc == NULL) return ;
  const gchar * current_doc_path = qt_document_get_doc_path(current_doc);
  
  if (g_strcmp0(current_doc_path, self->priv->untitled) == 0) {
    QtSourceView * current_view = qt_document_get_doc_view(current_doc);
    const gchar * current_view_text;
    g_object_get(
      G_OBJECT (gtk_text_view_get_buffer(GTK_TEXT_VIEW (current_view))), 
      "text", &current_view_text, 
      NULL
    );
    
    if (g_strcmp0(current_view_text, "") == 0) return ;
    
    GtkFileChooserDialog * file_chooser;
    file_chooser = GTK_FILE_CHOOSER_DIALOG (
      gtk_file_chooser_dialog_new(
        _ ("Save file"), GTK_WINDOW (self), 
        GTK_FILE_CHOOSER_ACTION_SAVE, 
        _ ("Cancel"), GTK_RESPONSE_CANCEL, 
        _ ("Save"), GTK_RESPONSE_ACCEPT, 
        NULL
      )
    );
  
    g_object_set(G_OBJECT (file_chooser), 
                 "do-overwrite-confirmation", TRUE,  
                 "create-folders", TRUE,
                 NULL);
    
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (file_chooser), 
                                        g_get_home_dir());

    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER (file_chooser), 
                                      self->priv->untitled);
    
    if (gtk_dialog_run(GTK_DIALOG (file_chooser)) == GTK_RESPONSE_ACCEPT) {
      GFile * file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER (file_chooser));
      qt_document_set_doc_path(current_doc, g_file_get_path(file));
      qt_document_set_doc_title(current_doc, g_file_get_basename(file));
      
      gtk_header_bar_set_subtitle(GTK_HEADER_BAR(self->priv->header_bar), 
                                  g_file_get_path(file));
      gtk_header_bar_set_subtitle(GTK_HEADER_BAR(self->priv->fs_header_bar), 
                                  g_file_get_path(file));
      qt_source_view_save_file(current_view, file);
      qt_document_mark_title(current_doc);
      
      /* refrescar lenguaje */
    }
                                                                
    gtk_widget_destroy(GTK_WIDGET (file_chooser));
  } /*else if () {
    qt_source_view_save_file(current_view, NULL);
  }*/
  
  /*******************************************/
  /*******************************************/
  /*******************************************/
  /*******************************************/
  /*******************************************/
  /*******************************************/
  /*******************************************/
  /*******************************************/
  /*******************************************/
}

static void 
qt_app_window_on_doc_switched (GtkWidget * sender, 
                               QtDocument * doc, 
                               gpointer data) 
{
  QtAppWindow * self = QT_APP_WINDOW (data);
  const gchar * doc_path = qt_document_get_doc_path(doc);
  gtk_header_bar_set_subtitle(GTK_HEADER_BAR (self->priv->header_bar), 
                              doc_path);
  gtk_header_bar_set_subtitle(GTK_HEADER_BAR (self->priv->fs_header_bar), 
                              doc_path);
  
  if (gtk_widget_get_visible(GTK_WIDGET (self->priv->terminal))) {
    vte_terminal_reset(self->priv->terminal, TRUE, TRUE);
    gtk_widget_hide(GTK_WIDGET (self->priv->terminal));
  }
  
  QtSourceView * doc_view = qt_document_get_doc_view(doc);
  gtk_widget_grab_focus(GTK_WIDGET (doc_view));
  /* refrescar lenguaje */
}

static void 
qt_app_window_on_doc_closed (GtkWidget * sender, 
                             QtDocument * doc, 
                             gpointer data) 
{
  QtAppWindow * self = QT_APP_WINDOW (data);
  /* agregar para re-abrir */
  
  gtk_widget_destroy(GTK_WIDGET (qt_document_get_doc_scroll(doc)));
  gtk_widget_destroy(GTK_WIDGET (doc));
  qt_app_window_check_doc_count(self);
  
  QtDocument * current_doc = 
    qt_document_bar_get_current_doc(self->priv->doc_bar);
  qt_document_bar_switch_doc(self->priv->doc_bar, current_doc);
  
  /* refrescar lenguaje */
}

static void 
qt_app_window_create_widgets (QtAppWindow * self) 
{
	GtkSettings * gtk_settings = gtk_settings_get_default();
	g_object_set(G_OBJECT (gtk_settings), 
							 "gtk-application-prefer-dark-theme", 
							 qt_text_editor_get_prefer_dark(self->priv->editor), 
							 NULL);
	
	self->priv->header_bar = qt_header_bar_new(self);
	self->priv->fs_header_bar = qt_header_bar_new(self);
	gtk_widget_show(GTK_WIDGET (self->priv->header_bar));
	gtk_window_set_titlebar(GTK_WINDOW (self), 
												  GTK_WIDGET (self->priv->header_bar));

	self->priv->search_entry = GTK_SEARCH_ENTRY (gtk_search_entry_new());
	gtk_entry_set_placeholder_text(GTK_ENTRY (self->priv->search_entry), 
																 _ ("Enter your search..."));
	gtk_entry_set_width_chars(GTK_ENTRY (self->priv->search_entry), 60);
	/* conectar señales */
	gtk_widget_show(GTK_WIDGET (self->priv->search_entry));
	
	self->priv->search_next = GTK_BUTTON (
		gtk_button_new_from_icon_name("go-down-symbolic", GTK_ICON_SIZE_MENU));
	/* conectar señal */
	gtk_widget_show(GTK_WIDGET (self->priv->search_next));
	
	self->priv->search_prev = GTK_BUTTON (
		gtk_button_new_from_icon_name("go-up-symbolic", GTK_ICON_SIZE_MENU));
	/* conectar señal */
	gtk_widget_show(GTK_WIDGET (self->priv->search_prev));
	
	GtkBox * hbox = GTK_BOX (gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5));
	gtk_box_pack_start(hbox, GTK_WIDGET (self->priv->search_entry), 
										 FALSE, FALSE, 0);
	gtk_box_pack_start(hbox, GTK_WIDGET (self->priv->search_prev), 
										 FALSE, FALSE, 0);
	gtk_box_pack_start(hbox, GTK_WIDGET (self->priv->search_next), 
										 FALSE, FALSE, 0);
	gtk_widget_show(GTK_WIDGET (hbox));
	
	self->priv->search_bar = GTK_SEARCH_BAR (gtk_search_bar_new());
	gtk_search_bar_connect_entry(self->priv->search_bar, 
															 GTK_ENTRY (self->priv->search_entry));
	gtk_container_add(GTK_CONTAINER (self->priv->search_bar), GTK_WIDGET (hbox));
	gtk_widget_show(GTK_WIDGET (self->priv->search_bar));
		
	self->priv->documents = GTK_STACK (gtk_stack_new());
	gtk_stack_set_transition_type(self->priv->documents, 
															  GTK_STACK_TRANSITION_TYPE_OVER_LEFT_RIGHT);
	gtk_stack_set_transition_duration(self->priv->documents, 250);
	gtk_widget_show(GTK_WIDGET (self->priv->documents));
	
  /* doc_bar */
	self->priv->doc_bar = qt_document_bar_new();
	qt_document_bar_set_stack(self->priv->doc_bar, self->priv->documents);
	gtk_widget_show(GTK_WIDGET (self->priv->doc_bar));
	
  g_signal_connect (self->priv->doc_bar, 
                    "doc-switched", 
                    G_CALLBACK (qt_app_window_on_doc_switched), 
                    self);
  
  g_signal_connect (self->priv->doc_bar, 
                    "doc-closed", 
                    G_CALLBACK (qt_app_window_on_doc_closed), 
                    self);
	
	/* statusbar */
  
  
	GtkSeparator * separator = 
		GTK_SEPARATOR (gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));
		
	GtkBox * vbox = GTK_BOX (gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
	gtk_box_pack_start(vbox, 
										 GTK_WIDGET (self->priv->fs_header_bar),
										 FALSE, TRUE, 0);
	gtk_box_pack_start(vbox, 
										 GTK_WIDGET (self->priv->search_bar), 
										 FALSE, TRUE, 0);
	gtk_box_pack_start(vbox, 
										 GTK_WIDGET (self->priv->doc_bar),
										 FALSE, TRUE, 7);
	gtk_box_pack_start(vbox, 
										 GTK_WIDGET (separator),
										 FALSE, TRUE, 0);
	gtk_box_pack_start(vbox, 
										 GTK_WIDGET (self->priv->documents),
										 TRUE, TRUE, 0);
	/* statusbar
	 * gtk_box_pack_start(vbox, 
										 GTK_WIDGET (self->priv->), 
										 FALSE, TRUE, 0); */
	g_object_set (GTK_WIDGET (vbox), "height-request", 400, NULL);
	gtk_widget_show(GTK_WIDGET (vbox));
	
	self->priv->terminal = VTE_TERMINAL (vte_terminal_new());
	self->priv->content = GTK_PANED (gtk_paned_new(GTK_ORIENTATION_VERTICAL));
	gtk_paned_pack1(self->priv->content, GTK_WIDGET (vbox), FALSE, FALSE);
	gtk_paned_pack2(
		self->priv->content, GTK_WIDGET (self->priv->terminal), TRUE, TRUE
	);
	
	gtk_widget_show(GTK_WIDGET (self->priv->content));
	gtk_container_add(GTK_CONTAINER (self), GTK_WIDGET (self->priv->content));
}

static void 
qt_app_window_on_toggle_search_cb (GSimpleAction * sender, 
																	GVariant * parameter, 
																	gpointer data) 
{
	QtAppWindow * self = QT_APP_WINDOW (data);
	gboolean search_mode_enabled;
	g_object_get(G_OBJECT (self->priv->search_bar), 
							 "search-mode-enabled", &search_mode_enabled,
							 NULL);
	
	if (!search_mode_enabled)
	 {
		g_object_set(
			G_OBJECT (self->priv->search_bar), 
			"search-mode-enabled", TRUE, 
			NULL
		);
		
		/* Cambiar funcionalidad por la correcta */
		QtSourceView * current_doc = QT_SOURCE_VIEW (
			gtk_stack_get_visible_child(self->priv->documents)
		);
		
		self->priv->search_context = GTK_SOURCE_SEARCH_CONTEXT (
			gtk_source_search_context_new(
				qt_source_view_get_source_buffer(current_doc), 
				qt_source_view_get_search_settings(current_doc)
			)
		);
		
		gtk_source_search_context_set_highlight(self->priv->search_context, TRUE);
	} else 
	 {
		g_object_set(
			G_OBJECT (self->priv->search_bar), 
			"search-mode-enabled", FALSE, 
			NULL
		);
	}
}

static void 
qt_app_window_on_fullscreen_cb (GSimpleAction * sender, 
															 GVariant * parameter, 
															 gpointer data) 
{
	QtAppWindow * self = QT_APP_WINDOW (data);
	qt_header_bar_toggle_fullscreen(self->priv->fs_header_bar);
	
	GdkWindow * window = gtk_widget_get_window(GTK_WIDGET (self));
	GdkWindowState window_state = gdk_window_get_state(window);
	
	if ((window_state & GDK_WINDOW_STATE_FULLSCREEN) != 0) {
		gtk_window_unfullscreen(GTK_WINDOW (self));
		gtk_widget_hide(GTK_WIDGET (self->priv->fs_header_bar));
	} else {
		gtk_window_fullscreen(GTK_WINDOW (self));
	 	gtk_widget_show_all(GTK_WIDGET (self->priv->fs_header_bar));
	}
}

static void 
qt_app_window_on_show_terminal_cb (GSimpleAction * sender, 
																  GVariant * parameter, 
																	gpointer data)
{
	QtAppWindow * self = QT_APP_WINDOW (data);
	
	if (gtk_widget_get_visible(GTK_WIDGET (self->priv->terminal))) {
		vte_terminal_reset(self->priv->terminal, TRUE, TRUE);
		gtk_widget_hide(GTK_WIDGET (self->priv->terminal));
	} else {
		const gchar * working_dir = g_get_home_dir();
	 
		vte_terminal_set_cursor_blink_mode(self->priv->terminal, 
																		   VTE_CURSOR_BLINK_ON);
	 	vte_terminal_set_cursor_shape(self->priv->terminal, 
																	VTE_CURSOR_SHAPE_BLOCK);
		vte_terminal_set_input_enabled(self->priv->terminal, TRUE);
		vte_terminal_set_allow_bold(self->priv->terminal, TRUE);
	 
	 	gchar * shells[2] = {
			vte_get_user_shell(), 
			NULL
	 	};
	 
	 	GError * inner_error = NULL;
		vte_terminal_spawn_sync(self->priv->terminal, 
													  VTE_PTY_DEFAULT, 
													  working_dir, 
													  shells, 
													  NULL, 
														G_SPAWN_DO_NOT_REAP_CHILD, 
														NULL, 
														NULL, 
														NULL, 
														NULL, 
														&inner_error);
		if (inner_error != NULL) {
			g_error("vte_terminal_spawn_sync: I/O error: %s", inner_error->message);
			g_error_free(inner_error);
		}
					
		gtk_widget_show(GTK_WIDGET (self->priv->terminal));
		gtk_widget_grab_focus(GTK_WIDGET (self->priv->terminal));
	}
}

static void 
qt_app_window_connect_signals (QtAppWindow * self) 
{
	GSimpleAction * action_fullscreen = g_simple_action_new("fullscreen", NULL);
	g_signal_connect (action_fullscreen, 
									  "activate", 
									  G_CALLBACK (qt_app_window_on_fullscreen_cb), 
									  self);
	g_action_map_add_action(G_ACTION_MAP (self), G_ACTION (action_fullscreen));
	
	GSimpleAction * action_show_terminal = g_simple_action_new(
		"toggle_terminal", NULL);
	g_signal_connect (action_show_terminal, 
									  "activate", 
									  G_CALLBACK (qt_app_window_on_show_terminal_cb), 
									  self);
	g_action_map_add_action(G_ACTION_MAP (self), G_ACTION (action_show_terminal));
	
	GSimpleAction * action_toggle_search = g_simple_action_new(
		"search_mode", NULL);
	g_signal_connect (action_toggle_search, 
									  "activate", 
									  G_CALLBACK (qt_app_window_on_toggle_search_cb),
									  self);
	g_action_map_add_action(G_ACTION_MAP (self), G_ACTION (action_toggle_search));
  
  g_signal_connect (self, "destroy", 
                    G_CALLBACK (qt_app_window_quit_cb), 
                    self);
}

QtAppWindow * 
qt_app_window_new (QtApplication * app, QtTextEditor * editor) 
{
	QtAppWindow * new_window;
	new_window = QT_APP_WINDOW (
		g_object_new(QT_APP_WINDOW_TYPE, "application", app, NULL)
	);
	
	new_window->priv = qt_app_window_get_instance_private(new_window);
	new_window->priv->editor = editor;
	
	new_window->priv->untitled = _ ("Untitled file");
	new_window->priv->counter = 0;
	
	g_object_set(GTK_WINDOW (new_window), 
							 "window-position", 
							 GTK_WIN_POS_CENTER, 
							 NULL);
	
	gtk_window_set_default_size(GTK_WINDOW (new_window), 1000, 700);
	gtk_container_set_border_width(GTK_CONTAINER (new_window), 0);
	// gtk_window_maximize(GTK_WINDOW (new_window));
	
	qt_app_window_create_widgets(new_window);
	qt_app_window_connect_signals(new_window);
	return new_window;
}

