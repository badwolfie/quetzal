#include "qt-documentbar.h"
#include "qt-document.h"
#include <glib/gprintf.h>
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
	
	GList * docs;
	GList * extra_docs;
};

G_DEFINE_TYPE_WITH_PRIVATE (QtDocumentBar, qt_document_bar, GTK_TYPE_BOX);

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
	/* conectar señal */
	
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
									  GTK_WIDGET (self->priv->extra_menu));
	g_object_set (GTK_WIDGET (self->priv->extra_popover), 
							  "width-request", 320,   
							  NULL);
	
	gtk_box_pack_end(GTK_BOX (self), 
									 GTK_WIDGET (self->priv->extra_menu), 
									 FALSE, TRUE, 3);
	gtk_widget_hide(GTK_WIDGET (self->priv->extra_menu));
	gtk_widget_show_all(GTK_WIDGET (self));
}

void 
qt_document_bar_set_stack (QtDocumentBar * self, GtkStack * stack) 
{
	self->priv->stack = stack;
}

void 
qt_document_bar_add_doc (QtDocumentBar * self, QtDocument * doc) 
{
	if (self->priv->doc_num < 5) {
		if (self->priv->doc_extra_num == 0) 
			gtk_widget_hide(GTK_WIDGET (self->priv->extra_menu));
		
		gtk_box_pack_start(GTK_BOX (self), GTK_WIDGET (doc), TRUE, TRUE, 5);
		/* append doc */
		self->priv->doc_num++;
	} else {
		gtk_widget_show(GTK_WIDGET (self->priv->extra_menu));
		gtk_box_pack_start(GTK_BOX (self->priv->extra_box), 
										   GTK_WIDGET (doc), 
										   FALSE, TRUE, 7);
		self->priv->doc_extra_num++;
	}
} 

static void 
qt_document_bar_init (QtDocumentBar * self) 
{}

static void 
qt_document_bar_class_init (QtDocumentBarClass * class) 
{
	
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

