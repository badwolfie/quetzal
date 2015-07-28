#include "qt-document.h"

#include "config.h"
#include <glib/gi18n.h>

struct _QtDocument 
{
	GtkBox parent;
	QtDocumentPrivate * priv;
};

struct _QtDocumentClass 
{
	GtkBoxClass parent_class;
};

struct _QtDocumentPrivate 
{
	QtTextEditor * editor;
	const gchar * untitled;
	QtSourceView * doc_view;
	
	GtkEventBox * evt_box;
	GtkLabel * title_label;
	GtkEventBox * close_button;
	GtkScrolledWindow * doc_scroll;
	GtkLabel * doc_is_modified;
	const gchar * doc_title;
  const gchar * doc_path;
};

G_DEFINE_TYPE_WITH_PRIVATE (QtDocument, qt_document, GTK_TYPE_BOX);

static void 
qt_document_init (QtDocument * self) 
{}

static void 
qt_document_on_drag_n_drop (QtSourceView * sender, 
														const gchar * filename, 
														gpointer data) 
{
	g_signal_emit_by_name(data, "view-drag-n-drop", filename);
}

static void 
qt_document_refresh_title (QtDocument * self) 
{
	if (self->priv->title_label != NULL) {
		gtk_label_set_label(self->priv->title_label, self->priv->doc_title);
	}
}

static void 
qt_document_mark_title (QtDocument * self) 
{
	gchar * marked_title = g_strconcat(
		"<span underline='single' font_weight='bold'>", 
		self->priv->doc_title, 
		"</span>", 
		NULL
	);
	
	gtk_label_set_label(
		self->priv->title_label, 
		marked_title
	);
}

void 
qt_document_change_language (QtDocument * self, const gchar * language) 
{
	qt_source_view_change_language(self->priv->doc_view, language);
}

const gchar * 
qt_document_get_doc_path (QtDocument * self) 
{
  return self->priv->doc_path;
}

const gchar * 
qt_document_get_doc_title (QtDocument * self) 
{
	return self->priv->doc_title;
}

void 
qt_document_set_doc_title (QtDocument * self, const gchar * value)
{
	if (g_strcmp0(self->priv->doc_title, value) == 0) {
		return ;
	}
	
	self->priv->doc_title = g_strdup(value);
	qt_document_refresh_title(self);
}

gboolean 
qt_document_get_doc_is_modified (QtDocument * self) 
{
	gboolean modified = 
		gtk_widget_get_visible(GTK_WIDGET (self->priv->doc_is_modified));
	return modified;
}

void 
qt_document_set_doc_is_modified (QtDocument * self, gboolean value) 
{
	gtk_widget_set_visible(GTK_WIDGET (self->priv->doc_is_modified), value);
}

QtSourceView * 
qt_document_get_doc_view (QtDocument * self) 
{
	return self->priv->doc_view;
}

GtkScrolledWindow * 
qt_document_get_doc_scroll (QtDocument * self) 
{
	return self->priv->doc_scroll;
}

/* signal senders */

static void 
qt_document_create_widgets (QtDocument * self, GFile * file) 
{
	self->priv->title_label = GTK_LABEL (gtk_label_new(self->priv->doc_title));
	gtk_label_set_ellipsize(GTK_LABEL (self->priv->title_label), 
												  PANGO_ELLIPSIZE_END);
	gtk_label_set_max_width_chars(GTK_LABEL (self->priv->title_label), 20);
	gtk_label_set_use_markup(self->priv->title_label, TRUE);
	gtk_widget_show(GTK_WIDGET (self->priv->title_label));
	
	self->priv->close_button = GTK_EVENT_BOX (gtk_event_box_new());
	GtkImage * close_img = GTK_IMAGE (
		gtk_image_new_from_icon_name("window-close-symbolic", GTK_ICON_SIZE_MENU)
	);
	
	gtk_event_box_set_above_child(self->priv->close_button, TRUE);
	g_object_set(GTK_CONTAINER (self->priv->close_button), 
               "child", close_img,
							 NULL);
	/*conectar señal*/
	
	self->priv->evt_box = GTK_EVENT_BOX (gtk_event_box_new());
	gtk_event_box_set_above_child(self->priv->evt_box, TRUE);
	/*conectar señal*/
	
	gtk_box_pack_start(GTK_BOX (self), 
										 GTK_WIDGET (self->priv->evt_box), 
										 TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX (self), 
										 GTK_WIDGET (self->priv->close_button), 
										 FALSE, TRUE, 0);
	self->priv->doc_view = qt_source_view_new(self->priv->editor, file);
	/* conectar señal */
	gtk_widget_show(GTK_WIDGET (self->priv->doc_view));
	
	self->priv->doc_scroll = GTK_SCROLLED_WINDOW (
		gtk_scrolled_window_new(NULL, NULL));
	gtk_scrolled_window_set_policy(self->priv->doc_scroll, 
																 GTK_POLICY_AUTOMATIC, 
																 GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER (self->priv->doc_scroll), 
									  GTK_WIDGET (self->priv->doc_view));
	gtk_widget_show(GTK_WIDGET (self->priv->doc_scroll));
	
	GtkBox * hbox = GTK_BOX (gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
  gtk_widget_show(GTK_WIDGET (hbox));
  gtk_widget_show_all(GTK_WIDGET (self));
  
	self->priv->doc_is_modified = 
		GTK_LABEL (gtk_label_new("<b> \xe2\x80\xa2</b>"));
	gtk_label_set_use_markup(self->priv->doc_is_modified, TRUE);
	gtk_widget_hide(GTK_WIDGET (self->priv->doc_is_modified));
	
	gtk_box_pack_start(hbox, GTK_WIDGET (self->priv->title_label), 
										 FALSE, TRUE, 0);
	gtk_box_pack_start(hbox, GTK_WIDGET (self->priv->doc_is_modified), 
										 FALSE, TRUE, 0);
	g_object_set(GTK_CONTAINER (self->priv->evt_box), 
							 "child", hbox,
							 NULL);
	gtk_widget_set_halign(GTK_WIDGET(self->priv->title_label), GTK_ALIGN_CENTER);
	gtk_widget_set_halign(GTK_WIDGET(hbox), GTK_ALIGN_CENTER);
}

static void 
qt_document_class_init (QtDocumentClass * class) 
{
	g_signal_new("close_clicked", 
							 QT_DOCUMENT_TYPE, 
							 G_SIGNAL_RUN_LAST, 0, 
							 NULL, NULL, 
							 g_cclosure_marshal_VOID__OBJECT,
							 G_TYPE_NONE, 1, 
							 QT_DOCUMENT_TYPE);
	
	g_signal_new("tab_clicked", 
							 QT_DOCUMENT_TYPE, 
							 G_SIGNAL_RUN_LAST, 0, 
							 NULL, NULL, 
							 g_cclosure_marshal_VOID__OBJECT,
							 G_TYPE_NONE, 1, 
							 QT_DOCUMENT_TYPE);
	
	g_signal_new("view_drag_n_drop", 
							 QT_DOCUMENT_TYPE, 
							 G_SIGNAL_RUN_LAST, 0, 
							 NULL, NULL, 
							 g_cclosure_marshal_VOID__STRING,
							 G_TYPE_NONE, 1, 
							 G_TYPE_STRING);
}

QtDocument * 
qt_document_new (QtTextEditor * editor, GFile * file) 
{
	QtDocument * new_document;
	new_document = g_object_new (
		QT_DOCUMENT_TYPE, 
		"orientation", GTK_ORIENTATION_HORIZONTAL, 
		"spacing", 0, 
		NULL
	);
	
	new_document->priv = qt_document_get_instance_private(new_document);
	new_document->priv->untitled = _ ("Untitled file");
	new_document->priv->editor = editor;
	
	if (file != NULL) {
		new_document->priv->doc_title = g_strdup(g_file_get_basename(file));
    new_document->priv->doc_path = g_strdup(g_file_get_path(file));
	} else {
		new_document->priv->doc_title = g_strdup(new_document->priv->untitled);
    new_document->priv->doc_path = g_strdup(new_document->priv->untitled);
	}
	
	g_object_set (GTK_WIDGET (new_document), 
							  "width-request", 150,   
							  NULL);
	qt_document_create_widgets(new_document, file);
	return new_document;
}

