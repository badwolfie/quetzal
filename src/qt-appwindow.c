#include "qt-appwindow.h"
#include "qt-headerbar.h"
#include "qt-documentbar.h"
#include "qt-document.h"

#include "config.h"
#include <glib/gi18n.h>
#include <vte/vte.h>
#include <stdlib.h>

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

QtTextEditor * 
qt_appwindow_get_editor (QtAppWindow * self) 
{
	return self->priv->editor;
}

static void 
qt_appwindow_add_new_doc (QtAppWindow * self, QtDocument * new_doc) 
{
	const gchar * doc_title = g_strdup_printf("tab - %d", self->priv->counter++);
	
}

static void 
qt_appwindow_create_widgets (QtAppWindow * self) 
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
	
	/* BORRAR ESTA PARTE */
	QtSourceView * current_doc = qt_source_view_new(self->priv->editor, NULL);
	gtk_stack_add_named(self->priv->documents, GTK_WIDGET (current_doc), "algo");
	gtk_widget_show(GTK_WIDGET (current_doc));

	self->priv->doc_bar = qt_document_bar_new();
	qt_document_bar_set_stack(self->priv->doc_bar, self->priv->documents);
	/* conectar señales */
	gtk_widget_show(GTK_WIDGET (self->priv->doc_bar));
	
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
										 FALSE, TRUE, 5);
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
qt_appwindow_on_toggle_search_cb (GSimpleAction * sender, 
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
qt_appwindow_on_fullscreen_cb (GSimpleAction * sender, 
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
qt_appwindow_on_show_terminal_cb (GSimpleAction * sender, 
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
qt_appwindow_connect_signals (QtAppWindow * self) 
{
	GSimpleAction * action_fullscreen = g_simple_action_new("fullscreen", NULL);
	g_signal_connect (action_fullscreen, 
									  "activate", 
									  G_CALLBACK (qt_appwindow_on_fullscreen_cb), 
									  self);
	g_action_map_add_action(G_ACTION_MAP (self), G_ACTION (action_fullscreen));
	
	GSimpleAction * action_show_terminal = g_simple_action_new(
		"toggle_terminal", NULL);
	g_signal_connect (action_show_terminal, 
									  "activate", 
									  G_CALLBACK (qt_appwindow_on_show_terminal_cb), 
									  self);
	g_action_map_add_action(G_ACTION_MAP (self), G_ACTION (action_show_terminal));
	
	GSimpleAction * action_toggle_search = g_simple_action_new(
		"search_mode", NULL);
	g_signal_connect (action_toggle_search, 
									  "activate", 
									  G_CALLBACK (qt_appwindow_on_toggle_search_cb),
									  self);
	g_action_map_add_action(G_ACTION_MAP (self), G_ACTION (action_toggle_search));
}

QtAppWindow * 
qt_appwindow_new (QtApplication * app, QtTextEditor * editor) 
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
	
	qt_appwindow_create_widgets(new_window);
	qt_appwindow_connect_signals(new_window);
	return new_window;
}

