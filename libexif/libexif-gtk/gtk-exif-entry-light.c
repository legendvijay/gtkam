/* gtk-exif-entry-light.c
 *
 * Copyright (C) 2001 Lutz M�ller <lutz@users.sourceforge.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details. 
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <config.h>
#include "gtk-exif-entry-light.h"

#include <gtk/gtkcheckbutton.h>
#include <gtk/gtkradiobutton.h>
#include <gtk/gtkvbox.h>
#include <gtk/gtksignal.h>
#include <gtk/gtkframe.h>
#include <gtk/gtklabel.h>
#include <gtk/gtkentry.h>
#include <gtk/gtkoptionmenu.h>
#include <gtk/gtkmenuitem.h>
#include <gtk/gtkmenu.h>
#include <gtk/gtkhbox.h>

#include <libexif/exif-i18n.h>

struct _GtkExifEntryLightPrivate {
	ExifEntry *entry;
};

#define PARENT_TYPE GTK_EXIF_TYPE_ENTRY_MENU
static GtkExifEntryMenuClass *parent_class;

enum {
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = {0};

static void
gtk_exif_entry_light_destroy (GtkObject *object)
{
	GtkExifEntryLight *entry = GTK_EXIF_ENTRY_LIGHT (object);

	if (entry->priv->entry) {
		exif_entry_unref (entry->priv->entry);
		entry->priv->entry = NULL;
	}

	GTK_OBJECT_CLASS (parent_class)->destroy (object);
}

static void
gtk_exif_entry_light_finalize (GtkObject *object)
{
	GtkExifEntryLight *entry = GTK_EXIF_ENTRY_LIGHT (object);

	g_free (entry->priv);

	GTK_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gtk_exif_entry_light_class_init (GtkExifEntryLightClass *klass)
{
	GtkObjectClass *object_class;

	object_class = GTK_OBJECT_CLASS (klass);
	object_class->destroy  = gtk_exif_entry_light_destroy;
	object_class->finalize = gtk_exif_entry_light_finalize;

	gtk_object_class_add_signals (object_class, signals, LAST_SIGNAL);

	parent_class = gtk_type_class (PARENT_TYPE);
}

static void
gtk_exif_entry_light_init (GtkExifEntryLight *entry)
{
	entry->priv = g_new0 (GtkExifEntryLightPrivate, 1);
}

GtkType
gtk_exif_entry_light_get_type (void)
{
	static GtkType entry_type = 0;

	if (!entry_type) {
		static const GtkTypeInfo entry_info = {
			"GtkExifEntryLight",
			sizeof (GtkExifEntryLight),
			sizeof (GtkExifEntryLightClass),
			(GtkClassInitFunc)  gtk_exif_entry_light_class_init,
			(GtkObjectInitFunc) gtk_exif_entry_light_init,
			NULL, NULL, NULL};
		entry_type = gtk_type_unique (PARENT_TYPE, &entry_info);
	}

	return (entry_type);
}

static GtkExifEntryMenuList LightSource[] = {
	{  0, N_("Unknown")},
	{  1, N_("Daylight")},
	{  2, N_("Fluorescent")},
	{  3, N_("Tungsten")},
	{ 17, N_("Standard light A")},
	{ 18, N_("Standard light B")},
	{ 19, N_("Standard light C")},
	{ 20, N_("D55")},
	{ 21, N_("D65")},
	{ 22, N_("D75")},
	{255, N_("Other")},
	{  0, NULL}
};

GtkWidget *
gtk_exif_entry_light_new (ExifEntry *e)
{
	GtkExifEntryLight *entry;

	g_return_val_if_fail (e != NULL, NULL);
	g_return_val_if_fail (e->tag == EXIF_TAG_LIGHT_SOURCE, NULL);
	g_return_val_if_fail (e->format == EXIF_FORMAT_SHORT, NULL);

	entry = gtk_type_new (GTK_EXIF_TYPE_ENTRY_LIGHT);
	entry->priv->entry = e;
	exif_entry_ref (e);
	gtk_exif_entry_menu_construct (GTK_EXIF_ENTRY_MENU (entry), e,
					_("Light Source:"), LightSource);

	return (GTK_WIDGET (entry));
}
