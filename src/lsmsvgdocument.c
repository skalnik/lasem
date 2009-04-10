/*
 * Copyright © 2009 Emmanuel Pacaud
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#include <lsmdebug.h>
#include <lsmsvgdocument.h>
#include <lsmsvgsvgelement.h>
#include <lsmsvggelement.h>
#include <lsmsvgdefselement.h>
#include <lsmsvguseelement.h>
#include <lsmsvgsymbolelement.h>
#include <lsmsvgrectelement.h>
#include <lsmsvgcircleelement.h>
#include <lsmsvgellipseelement.h>
#include <lsmsvglineelement.h>
#include <lsmsvgpolylineelement.h>
#include <lsmsvgpolygonelement.h>
#include <lsmsvgpathelement.h>
#include <lsmsvgtextelement.h>
#include <lsmsvglineargradientelement.h>
#include <lsmsvgradialgradientelement.h>
#include <lsmsvgstopelement.h>
#include <lsmsvgpatternelement.h>
#include <lsmsvgview.h>
#include <string.h>

/* LsmDomNode implementation */

static gboolean
lsm_svg_document_can_append_child (LsmDomNode *self, LsmDomNode *child)
{
	return (LSM_SVG_IS_SVG_ELEMENT (child));
}

/* LsmDomDocument implementation */

static LsmDomElement *
lsm_svg_document_create_element (LsmDomDocument *document, const char *tag_name)
{
	LsmDomNode *node = NULL;

	if (strcmp (tag_name, "svg") == 0)
		node = lsm_svg_svg_element_new ();
	else if (strcmp (tag_name, "g") == 0)
		node = lsm_svg_g_element_new ();
	else if (strcmp (tag_name, "rect") == 0)
		node = lsm_svg_rect_element_new ();
	else if (strcmp (tag_name, "circle") == 0)
		node = lsm_svg_circle_element_new ();
	else if (strcmp (tag_name, "ellipse") == 0)
		node = lsm_svg_ellipse_element_new ();
	else if (strcmp (tag_name, "path") == 0)
		node = lsm_svg_path_element_new ();
	else if (strcmp (tag_name, "line") == 0)
		node = lsm_svg_line_element_new ();
	else if (strcmp (tag_name, "polyline") == 0)
		node = lsm_svg_polyline_element_new ();
	else if (strcmp (tag_name, "polygon") == 0)
		node = lsm_svg_polygon_element_new ();
	else if (strcmp (tag_name, "text") == 0)
		node = lsm_svg_text_element_new ();
	else if (strcmp (tag_name, "linearGradient") == 0)
		node = lsm_svg_linear_gradient_element_new ();
	else if (strcmp (tag_name, "radialGradient") == 0)
		node = lsm_svg_radial_gradient_element_new ();
	else if (strcmp (tag_name, "stop") == 0)
		node = lsm_svg_stop_element_new ();
	else if (strcmp (tag_name, "pattern") == 0)
		node = lsm_svg_pattern_element_new ();
	else if (strcmp (tag_name, "use") == 0)
		node = lsm_svg_use_element_new ();
	else if (strcmp (tag_name, "defs") == 0)
		node = lsm_svg_defs_element_new ();
	else if (strcmp (tag_name, "symbol") == 0)
		node = lsm_svg_symbol_element_new ();

	if (node != NULL)
		lsm_debug ("[LsmSvgDocument::create_element] Create a %s element", tag_name);

	return LSM_DOM_ELEMENT (node);
}

static LsmDomView *
lsm_svg_document_create_view (LsmDomDocument *document)
{
	return LSM_DOM_VIEW (lsm_svg_view_new (LSM_SVG_DOCUMENT (document)));
}

/* LsmSvgDocument implementation */

LsmSvgDocument *
lsm_svg_document_new (void)
{
	return g_object_new (LSM_SVG_TYPE_DOCUMENT, NULL);
}

static void
lsm_svg_document_init (LsmSvgDocument *document)
{
}

LsmSvgSvgElement *
lsm_svg_document_get_root_element (const LsmSvgDocument *document)
{
	return LSM_SVG_SVG_ELEMENT (lsm_dom_document_get_document_element (LSM_DOM_DOCUMENT (document)));
}

/* LsmSvgDocument class */

static void
lsm_svg_document_class_init (LsmSvgDocumentClass *m_document_class)
{
	LsmDomNodeClass *d_node_class = LSM_DOM_NODE_CLASS (m_document_class);
	LsmDomDocumentClass *d_document_class = LSM_DOM_DOCUMENT_CLASS (m_document_class);

	d_node_class->can_append_child = lsm_svg_document_can_append_child;

	d_document_class->create_element = lsm_svg_document_create_element;
	d_document_class->create_view = lsm_svg_document_create_view;
}

G_DEFINE_TYPE (LsmSvgDocument, lsm_svg_document, LSM_DOM_TYPE_DOCUMENT)