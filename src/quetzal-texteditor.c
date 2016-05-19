/* quetzal-texteditor.c
 *
 * Copyright (C) 2016 Ian Hernandez <ihernandezs@openmailbox.org>
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
#include <gtksourceview/completion-providers/words/gtksourcecompletionwords.h>
#include <glib/gi18n.h>

#include "quetzal-texteditor.h"
#include "config.h"

struct _QuetzalTexteditor
{
  GtkSourceView parent;
  QuetzalTexteditorPrivate * priv;
};

struct _QuetzalTexteditorClass
{
  GtkSourceViewClass parent_class;
};

struct _QuetzalTexteditorPrivate
{
  GtkSourceCompletionWords * word_completion;
  // GtkSourceSearchSettings * search_settings;
  GtkSourceFileLoader * file_loader;
  GtkSourceFileSaver * file_saver;
  GtkSourceFile * source_file
  // settings
};

G_DEFINE_TYPE_WITH_PRIVATE (QuetzalTexteditor, quetzal_texteditor, GTK_SOURCE_TYPE_VIEW);

static void quetzal_texteditor_init (void);
static void quetzal_texteditor_class_init (void);

static void quetzal_texteditor_create_widgets (QuetzalTexteditor * self);
static void quetzal_texteditor_connect_signals (QuetzalTexteditor * self);

QuetzalTexteditor *
quetzal_texteditor_new (GFile * file)
{
  QuetzalTexteditor * self = g_object_new (QUETZAL_TYPE_TEXTEDITOR, NULL);
  self->priv = quetzal_texteditor_get_instance_private (self);

  quetzal_texteditor_create_widgets (self);
  quetzal_texteditor_connect_signals (self);
  return self;
}

static void
quetzal_texteditor_init (void)
{}

static void
quetzal_texteditor_class_init (void)
{}

static void
quetzal_texteditor_create_widgets (QuetzalTexteditor * self)
{
  self->priv->word_completion =
      gtk_source_completion_words_new (_ ("Document words"), NULL);
  g_object_set (G_OBJECT (self->priv->word_completion),
                "interactive-delay", 25,
                "activation", GTK_SOURCE_COMPLETION_ACTIVATION_USER_REQUESTED
                    | GTK_SOURCE_COMPLETION_ACTIVATION_INTERACTIVE,
                NULL);

  GError * inner_error = NULL;
  GtkSourceCompletion * completion =
      gtk_source_view_get_completion (GTK_SOURCE_VIEW (self));
  gboolean provider_added = gtk_source_completion_add_provider (
      completion,
      GTK_SOURCE_COMPLETION_PROVIDER (self->priv->word_completion),
      &inner_error);
  if (!provider_added && (inner_error != NULL)) {
    g_error ("gtk_source_completion_add_provider error: %s",
             inner_error->message);
    g_error_free (inner_error);
  }

  g_object_set (G_OBJECT (completion),
                "show-headers", FALSE,
                "show-icons", TRUE,
                NULL);
}

static void
quetzal_texteditor_connect_signals (QuetzalTexteditor * self)
{}
