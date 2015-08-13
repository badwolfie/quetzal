/* qt-documentbar.c
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

#include "qt-documentbar.h"
#include "qt-document.h"

#include "config.h"
#include <glib/gi18n.h>
#include <stdlib.h>

typedef struct _GTimeoutArgs GTimeoutArgs;

struct _GTimeoutArgs 
{
  QtDocumentBar * self;
  gint label_timeout;
  gboolean open_doc;
};

struct _QtDocumentBar 
{
	GtkBox parent;
	QtDocumentBarPrivate * priv;
};

struct _QtDocumentBarClass 
{
	GtkBoxClass parent_class;
};

struct _QtDocumentBarPrivate 
{
	gint doc_num;
	gint doc_extra_num;
	GtkStack * stack;
	
	GtkBox * extra_box;
	GtkEventBox * extra_menu;
	GtkPopover * extra_popover;
	GtkLabel * extra_label;
};

G_DEFINE_TYPE_WITH_PRIVATE (QtDocumentBar, qt_document_bar, GTK_TYPE_BOX);

GList * 
qt_document_bar_get_doc_list (QtDocumentBar * self) 
{
  return gtk_container_get_children(GTK_CONTAINER (self));
}

GList * 
qt_document_bar_get_extra_doc_list (QtDocumentBar * self) 
{
  return gtk_container_get_children(GTK_CONTAINER (self->priv->extra_box));
}

gboolean 
qt_document_bar_doc_is_opened (QtDocumentBar * self, GFile * file) 
{
  gchar * needle = g_strdup(g_file_get_path(file));
  gint i, item_number;
  GList * list;

  list = qt_document_bar_get_doc_list(self);
  item_number = g_list_length(list);
  for (i = 0; i < item_number; i++) {
    QtDocument * entry = g_list_nth_data(list, i);
    if (g_strcmp0(qt_document_get_doc_path(entry), needle) == 0)
      return TRUE;
  }
  
  list = qt_document_bar_get_extra_doc_list(self);
  item_number = g_list_length(list);
  for (i = 0; i < item_number; i++) {
    QtDocument * entry = g_list_nth_data(list, i);
    if (g_strcmp0(qt_document_get_doc_path(entry), needle) == 0)
      return TRUE;
  }
  
  return FALSE;
}

static gboolean 
qt_document_bar_toggle_extra_menu_cb (GtkWidget * sender, 
                                      GdkEvent * event, 
                                      gpointer data) 
{
  QtDocumentBar * self = QT_DOCUMENT_BAR (data);
  GtkPopover * extra_menu = self->priv->extra_popover;
  
  gboolean extra_menu_is_visible = 
    gtk_widget_get_visible(GTK_WIDGET (extra_menu));
  
  if (extra_menu_is_visible) {
    gtk_widget_hide(GTK_WIDGET (extra_menu));
  } else {
    gtk_widget_show_all(GTK_WIDGET (extra_menu));
  }
  
  return TRUE;
}

static void 
qt_document_bar_create_widgets (QtDocumentBar * self) 
{
	self->priv->extra_box = GTK_BOX (gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
	GtkImage * extra_menu_img = GTK_IMAGE (
		gtk_image_new_from_icon_name("view-list-symbolic", GTK_ICON_SIZE_MENU)
	);
	
	self->priv->extra_menu = GTK_EVENT_BOX (gtk_event_box_new());
	gtk_event_box_set_above_child(self->priv->extra_menu, TRUE);
	g_object_set (GTK_WIDGET (self->priv->extra_menu), 
							  "width-request", 20,   
							  NULL);
	g_signal_connect (self->priv->extra_menu, 
                    "button-press-event", 
                    G_CALLBACK (qt_document_bar_toggle_extra_menu_cb), 
                    self);
	
	self->priv->extra_label = GTK_LABEL (gtk_label_new(""));
	gtk_label_set_use_markup(self->priv->extra_label, TRUE);
	
	GtkBox * box = GTK_BOX (gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3));
	gtk_box_pack_start(box, GTK_WIDGET (extra_menu_img), TRUE, TRUE, 0);
	gtk_box_pack_start(box, GTK_WIDGET (self->priv->extra_label), TRUE, TRUE, 0);
	g_object_set (GTK_WIDGET (self->priv->extra_menu), 
							  "child", box,   
							  NULL);
	
	self->priv->extra_popover = 
		GTK_POPOVER (gtk_popover_new(GTK_WIDGET (extra_menu_img)));
	gtk_container_add(GTK_CONTAINER (self->priv->extra_popover), 
									  GTK_WIDGET (self->priv->extra_box));
	g_object_set (GTK_WIDGET (self->priv->extra_popover), 
							  "width-request", 320,   
							  NULL);
	
	gtk_box_pack_end(GTK_BOX (self), 
									 GTK_WIDGET (self->priv->extra_menu), 
									 FALSE, TRUE, 3);
	gtk_widget_show_all(GTK_WIDGET (self));
	gtk_widget_hide(GTK_WIDGET (self->priv->extra_menu));
}

void 
qt_document_bar_set_stack (QtDocumentBar * self, GtkStack * stack) 
{
	self->priv->stack = stack;
}

QtDocument * 
qt_document_bar_get_current_doc (QtDocumentBar * self) 
{
  QtDocument * current_doc = NULL;
  GtkScrolledWindow * current_scroll = 
    GTK_SCROLLED_WINDOW (gtk_stack_get_visible_child(self->priv->stack));
  gint item_number, i;
  GList * list;
  
  if (current_scroll != NULL) {
    list = qt_document_bar_get_doc_list(self);
    item_number = g_list_length(list);
    
    for (i = 0; i < item_number; i++) {
      QtDocument * doc = g_list_nth_data(list, i);
      if (qt_document_get_doc_scroll(doc) == current_scroll) 
        current_doc = doc;
    }

    if (current_doc == NULL) {
      list = qt_document_bar_get_extra_doc_list(self);
      item_number = g_list_length(list);
      
      for (i = 0; i < item_number; i++) {
        QtDocument * doc = g_list_nth_data(list, i);
        if (qt_document_get_doc_scroll(doc) == current_scroll) 
          current_doc = doc;
      }
    }
  }
  
  return current_doc;
}

GTimeoutArgs * 
g_timeout_args_new (QtDocumentBar * self, gboolean open_doc) 
{
  GTimeoutArgs * new_args = (GTimeoutArgs *) malloc(sizeof (GTimeoutArgs));
  new_args->self = self;
  new_args->label_timeout = 0;
  new_args->open_doc = open_doc;
  return new_args;
}

static void
g_timeout_args_destroy (void * data) 
{
  GTimeoutArgs * args = (GTimeoutArgs *) data;
  args->self = NULL;
  free(args);
}

static gboolean 
g_timeout_func_cb (gpointer data) 
{
  GTimeoutArgs * args = (GTimeoutArgs *) data;
  switch (args->label_timeout) {
    default:
    case 0: {
      if (args->open_doc) {
        gtk_label_set_label(
          args->self->priv->extra_label, 
          "<b>+1 </b>");
      } else {
        gtk_label_set_label(
          args->self->priv->extra_label, 
          "<b>-1 </b>");
      }
      
      break;
    } case 1: {
      const gchar * new_label = 
        g_strdup_printf("<b>%d </b>", args->self->priv->doc_extra_num);
      
      gtk_label_set_label(
        args->self->priv->extra_label, 
        new_label);
      return FALSE;
    }
  }
  
  args->label_timeout++;
  return TRUE;
}

static void 
qt_document_bar_refresh_each_title (gpointer data, gpointer user_data) 
{
  if (!QT_IS_DOCUMENT (data)) return ;
  QtDocument * entry = QT_DOCUMENT (data);
  qt_document_refresh_title(entry);
}

static void 
qt_document_bar_refresh_marked (QtDocumentBar * self) 
{
  GList * open_docs = qt_document_bar_get_doc_list(self);
  GList * open_extra_docs = qt_document_bar_get_extra_doc_list(self);
  
  if (open_docs != NULL) 
    g_list_foreach(open_docs, qt_document_bar_refresh_each_title, NULL);
  if (open_extra_docs != NULL)
    g_list_foreach(open_extra_docs, qt_document_bar_refresh_each_title, NULL);
  
}

void 
qt_document_bar_switch_doc (QtDocumentBar * self, QtDocument * doc)
{
  GtkScrolledWindow * visible_child = qt_document_get_doc_scroll(doc);
  gtk_stack_set_visible_child(self->priv->stack, GTK_WIDGET (visible_child));
  qt_document_bar_refresh_marked(self);
  qt_document_mark_title(doc);
  
  g_signal_emit_by_name(self, "doc-switched", doc);
}

static void 
qt_document_bar_switch_doc_cb (QtDocument * sender, 
                               QtDocument * doc, 
                               gpointer data) 
{
  QtDocumentBar * self = QT_DOCUMENT_BAR (data);
  qt_document_bar_switch_doc(self, doc);
}

void 
qt_document_bar_close_doc (QtDocumentBar * self, QtDocument * doc) 
{
  if (doc == NULL) return ;
  
  GList * docs = qt_document_bar_get_doc_list(self);
  GList * extra_docs = qt_document_bar_get_extra_doc_list(self);
  
  gint doc_index = g_list_index(docs, doc);
  if (doc_index != -1) {
    // docs = g_list_remove(docs, doc);
    self->priv->doc_num--;
    
    if (self->priv->doc_extra_num > 0) {
      gpointer first = g_list_first(extra_docs)->data;
      QtDocument * aux_doc = QT_DOCUMENT (g_object_ref(first));
      
      if (aux_doc != NULL) {
        gtk_container_remove(GTK_CONTAINER (self->priv->extra_box), 
                             GTK_WIDGET (aux_doc));
        qt_document_bar_add_doc(self, aux_doc);
        self->priv->doc_extra_num--;
        
        const gchar * extra_menu_tooltip = 
          g_strdup_printf(_ ("Hidden tabs: %d"), self->priv->doc_extra_num);
		    gtk_widget_set_tooltip_text(GTK_WIDGET (self->priv->extra_menu), 
                                    extra_menu_tooltip);
		
        GTimeoutArgs * timeout_args = g_timeout_args_new(self, FALSE);
		    g_timeout_add_full(G_PRIORITY_HIGH, 250,
                           g_timeout_func_cb, 
                           timeout_args, 
                           g_timeout_args_destroy);
      }
    }
  }
  
  doc_index = g_list_index(extra_docs, doc);
  if (doc_index != -1) {
    // extra_docs = g_list_remove(extra_docs, doc);
    self->priv->doc_extra_num--;
    
    const gchar * extra_menu_tooltip = 
      g_strdup_printf(_ ("Hidden tabs: %d"), self->priv->doc_extra_num);
		gtk_widget_set_tooltip_text(GTK_WIDGET (self->priv->extra_menu), 
                                extra_menu_tooltip);
		
    GTimeoutArgs * timeout_args = g_timeout_args_new(self, FALSE);
		g_timeout_add_full(G_PRIORITY_HIGH, 250,
                       g_timeout_func_cb, 
                       timeout_args, 
                       g_timeout_args_destroy);
  }
  
  if ((self->priv->doc_num <= 5) && (self->priv->doc_extra_num == 0)) {
    gtk_widget_hide(GTK_WIDGET (self->priv->extra_menu));
  }
  
  g_signal_emit_by_name(self, "doc-closed", doc);
}

static void 
qt_document_bar_close_doc_cb (QtDocument * sender, 
                              QtDocument * doc, 
                              gpointer data) 
{
  QtDocumentBar * self = QT_DOCUMENT_BAR (data);
  qt_document_bar_close_doc(self, doc);
}

void 
qt_document_bar_add_doc (QtDocumentBar * self, QtDocument * doc) 
{
	if (self->priv->doc_num < 5) {
		if (self->priv->doc_extra_num == 0) 
			gtk_widget_hide(GTK_WIDGET (self->priv->extra_menu));
		
		gtk_box_pack_start(GTK_BOX (self), GTK_WIDGET (doc), TRUE, TRUE, 5);
		self->priv->doc_num++;
	} else {
		gtk_widget_show(GTK_WIDGET (self->priv->extra_menu));
		gtk_box_pack_start(GTK_BOX (self->priv->extra_box), 
										   GTK_WIDGET (doc), 
										   FALSE, TRUE, 7);
		self->priv->doc_extra_num++;

		const gchar * extra_menu_tooltip = 
      g_strdup_printf(_ ("Hidden tabs: %d"), self->priv->doc_extra_num);
		gtk_widget_set_tooltip_text(GTK_WIDGET (self->priv->extra_menu), 
                                extra_menu_tooltip);
		
    GTimeoutArgs * timeout_args = g_timeout_args_new(self, TRUE);
		g_timeout_add_full(G_PRIORITY_HIGH, 250,
                       g_timeout_func_cb, 
                       timeout_args, 
                       g_timeout_args_destroy);
	}
  
  g_signal_connect (doc, "doc-clicked", 
                    G_CALLBACK (qt_document_bar_switch_doc_cb), 
                    self);
  
  g_signal_connect (doc, "close-clicked", 
                    G_CALLBACK (qt_document_bar_close_doc_cb),
                    self);
  
  qt_document_bar_switch_doc(self, doc);
  qt_document_bar_refresh_marked(self);
  qt_document_mark_title(doc);
}

static void 
qt_document_bar_init (QtDocumentBar * self) 
{}

static void 
qt_document_bar_class_init (QtDocumentBarClass * class) 
{
  g_signal_new("doc-closed", 
							 QT_DOCUMENT_BAR_TYPE, 
							 G_SIGNAL_RUN_LAST, 0, 
							 NULL, NULL, 
							 g_cclosure_marshal_VOID__OBJECT,
							 G_TYPE_NONE, 1, 
							 QT_TYPE_DOCUMENT);
  
  g_signal_new("doc-switched", 
							 QT_DOCUMENT_BAR_TYPE, 
							 G_SIGNAL_RUN_LAST, 0, 
							 NULL, NULL, 
							 g_cclosure_marshal_VOID__OBJECT,
							 G_TYPE_NONE, 1, 
							 QT_TYPE_DOCUMENT);
}

QtDocumentBar * 
qt_document_bar_new (void) 
{
	QtDocumentBar * new_doc_bar = g_object_new (QT_DOCUMENT_BAR_TYPE, 
		"orientation", GTK_ORIENTATION_HORIZONTAL,
		"spacing", 0, 
		NULL
	);
	
	new_doc_bar->priv = qt_document_bar_get_instance_private(new_doc_bar);
	qt_document_bar_create_widgets(new_doc_bar);
	
	return new_doc_bar;
}

