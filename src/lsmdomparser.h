/* Lasem
 *
 * Copyright © 2007-2009 Emmanuel Pacaud
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
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#ifndef LSM_DOM_PARSER_H
#define LSM_DOM_PARSER_H

#include <lsmdomdocument.h>
#include <gio/gio.h>

G_BEGIN_DECLS

void 			lsm_dom_document_append_from_memory 	(LsmDomDocument *document, LsmDomNode *node,
								 const void *buffer, gssize size, GError **error);
LsmDomDocument * 	lsm_dom_document_new_from_memory 	(const void *buffer, gssize size, GError **error);
LsmDomDocument * 	lsm_dom_document_new_from_path 		(const char *path, GError **error);
LsmDomDocument * 	lsm_dom_document_new_from_url 		(const char *url, GError **error);

void			lsm_dom_document_save_to_stream		(LsmDomDocument *document,
								 GOutputStream *stream,
								 GError **error);
void			lsm_dom_document_save_to_memory		(LsmDomDocument *document,
								 void **buffer,
								 gsize *size,
								 GError **error);
void			lsm_dom_document_save_to_path		(LsmDomDocument *document,
								 const char *path,
								 GError **error);
void			lsm_dom_document_save_to_url		(LsmDomDocument *document,
								 const char *url,
								 GError **error);

G_END_DECLS

#endif
