/*
 * gmathmltest - Regression test utility for gmathml
 *
 * Copyright © 2008 Emmanuel Pacaud
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of the authors
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * The authors make no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 * THE AUTHORS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
 * NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Authors: Emmanuel Pacaud <emmanuel@gnome.org>
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <glib/gregex.h>
#include <gmathmlview.h>
#include <gmathmldocument.h>
#include <gmathmlparser.h>

#include <libxml/parser.h>

#define TEST_WIDTH 	480
#define XML_FILENAME	"gmathmltest.xml"

static const char *fail_face = "", *normal_face = "";
FILE *gmathml_test_html_file = NULL;

static void
gmathml_test_html (const char *fmt, ...)
{
	va_list va;
	FILE *file = gmathml_test_html_file ? gmathml_test_html_file : stdout;

	va_start (va, fmt);
	vfprintf (file, fmt, va);
	va_end (va);
}

static GRegex *regex_mml = NULL;

void
gmathml_test_render (char const *filename)
{
	GDomNode *document;
	GMathmlView *view;
	cairo_t *cairo;
	cairo_surface_t *surface;
	char *buffer;
	size_t size;
	char *png_filename;
	char *xml_filename;
	char *reference_png_filename;
	char *test_name;
	double width, height;

	test_name = g_regex_replace (regex_mml, filename, -1, 0, "", 0, NULL);

	png_filename = g_strdup_printf ("%s-out.png", test_name);
	xml_filename = g_strdup_printf ("%s.mml", test_name);
	reference_png_filename = g_strdup_printf ("%s.png", test_name);

	document = gmathml_document_from_file (xml_filename);

	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 1.0, 1.0);
	cairo = cairo_create (surface);
	cairo_surface_destroy (surface);

	view = gmathml_view_new (GMATHML_DOCUMENT (document), cairo);

	cairo_destroy (cairo);

	gmathml_view_measure (view, &width, &height);

	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width + .5, height + .5);
	cairo = cairo_create (surface);
	cairo_surface_destroy (surface);

	gmathml_view_set_cairo (view, cairo);

	cairo_destroy (cairo);

	gmathml_view_render (view);

	cairo_surface_write_to_png (surface, png_filename);

	g_object_unref (view);
	g_object_unref (document);

	gmathml_test_html ("<table border=\"1\" cellpadding=\"8\">\n");
	gmathml_test_html ("<tr>");
	gmathml_test_html ("<td>");

	if (g_file_get_contents (xml_filename, &buffer, &size, NULL)) {
		GRegex *regex;
		GError *error = NULL;
		char *filtered_buffer;

		regex = g_regex_new ("<math>", 0, 0, &error);
		assert (error == NULL);

		filtered_buffer = g_regex_replace (regex, buffer,
						   -1, 0,
						   "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">",
						   0, NULL);
		g_regex_unref (regex);

		gmathml_test_html (filtered_buffer);

		g_free (filtered_buffer);
		g_free (buffer);
	}

	gmathml_test_html ("</td>");
	gmathml_test_html ("<td><a href=\"%s\"><img border=\"0\" src=\"%s\"/></a></td>",
			   xml_filename, png_filename);
	gmathml_test_html ("<td><img src=\"%s\"/></td>", reference_png_filename);
	gmathml_test_html ("</tr>\n");
	gmathml_test_html ("</table>\n");

	g_free (png_filename);
	g_free (xml_filename);
	g_free (reference_png_filename);

	g_free (test_name);
}

void
gmathml_test_process_dir (const char *name)
{
	GDir *directory;
	GError *error = NULL;
	const char *entry;
	char *filename;

	directory = g_dir_open (name, 0, &error);
	assert (error == NULL);

	do {
		entry = g_dir_read_name (directory);
		if (entry == NULL ||
		    strstr (entry, "ignore-") == entry)
			break;

		filename = g_build_filename (name, entry, NULL);

		if (g_file_test (filename, G_FILE_TEST_IS_DIR))
			gmathml_test_process_dir (filename);
		else if (g_file_test (filename, G_FILE_TEST_IS_REGULAR) &&
			 g_regex_match (regex_mml, filename, 0, NULL)) {
			gmathml_test_render (filename);
		}

		g_free (filename);
	} while (entry != NULL);

	g_dir_close (directory);
}

int
main (int argc, char **argv)
{
	GError *error = NULL;
	unsigned int i;

#ifdef HAVE_UNISTD_H
	if (isatty (2)) {
		fail_face = "\033[41m\033[37m\033[1m";
		normal_face = "\033[m";
	}
#endif

	gmathml_test_html_file = fopen (XML_FILENAME, "w");

	printf ("===============\n"
		"Rendering tests\n"
		"===============\n");

	gmathml_test_html_file = fopen (XML_FILENAME, "w");

	gmathml_test_html ("<?xml version=\"1.0\"?>");
	gmathml_test_html ("<!DOCTYPE html PUBLIC "
			   "\"-//W3C//DTD XHTML 1.1 plus MathML 2.0 plus SVG 1.1//EN\" "
			   "\"http://www.w3.org/Math/DTD/mathml2/xhtml-math11-f.dtd\">");
	gmathml_test_html ("<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
	gmathml_test_html ("<body>\n");

	g_type_init ();

	regex_mml = g_regex_new ("\\.mml$", 0, 0, &error);
	assert (error == NULL);

	if (argc >= 2)
		for (i = 0; i < argc - 1; i++)
			gmathml_test_render (argv[i + 1]);
	else
		gmathml_test_process_dir (".");

	gmathml_test_html ("</body>\n");
	gmathml_test_html ("</html>\n");

	if (gmathml_test_html_file != NULL)
		fclose (gmathml_test_html_file);

	g_regex_unref (regex_mml);

	return 0;
}

/* vim: set sw=8 sts=8: -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 8 -*- */
