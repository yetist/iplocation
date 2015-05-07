/* vi: set sw=4 ts=4 wrap ai: */
/*
 * m.c: This file is part of ____
 *
 * Copyright (C) 2014 yetist <xiaotian.wu@i-soft.com.cn>
 *
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <gio/gio.h>

#define MOSES_DBUS_NAME "org.moses.IpLocation"
#define MOSES_DBUS_PATH "/org/moses/IpLocation"

static GDBusProxy *sm_proxy = NULL;

static GDBusProxy* get_sm_proxy(void)
{
	GError *error = NULL;
	if (sm_proxy)
		return sm_proxy;
	sm_proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SESSION,
			G_DBUS_PROXY_FLAGS_NONE,
			NULL,
			MOSES_DBUS_NAME,
			MOSES_DBUS_PATH,
			MOSES_DBUS_NAME,
			NULL,
			&error);
	if (!sm_proxy)
	{
		g_printerr ("Unable to contact session manager: %s\n", error->message);
		exit (EXIT_FAILURE);
	}
	g_clear_error (&error);

	return sm_proxy;
}

gchar* get_ip_location(const gchar *ipaddr)
{
	GVariant *ret;
    GError *error = NULL;
	gchar *out_location;

	sm_proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SESSION,
			G_DBUS_PROXY_FLAGS_NONE,
			NULL,
			MOSES_DBUS_NAME,
			MOSES_DBUS_PATH,
			MOSES_DBUS_NAME,
			NULL,
			&error);

	if (!sm_proxy)
	{
		g_printerr ("Unable to contact session manager: %s\n", error->message);
		g_clear_error (&error);
		return NULL;
	}

	ret = g_dbus_proxy_call_sync (get_sm_proxy (),
			"GetLocation",
			g_variant_new ("(s)", ipaddr),
			G_DBUS_CALL_FLAGS_NONE,
			-1,
			NULL,
			&error);
	if (ret == NULL) {
		g_printerr ("Unable to call the method \"GetLocation\" for the session: %s\n", error->message);
		g_clear_error (&error);
		return NULL;
	} else {
		g_variant_get (ret, "(s)", &out_location);
		g_variant_unref (ret);
		return out_location;
	}
	return NULL;
}

static void usage (void)
{
    g_printerr ("Run 'iplocation --help' to see a full list of available command line options.");
    g_printerr ("\n");
}

static gboolean eval_cb (const GMatchInfo *info, GString *result, gpointer data)
{
	gchar *match;
	gchar *location;
	gchar *r;

	match = g_match_info_fetch (info, 0);
	if ((location = get_ip_location(match)) != NULL ) {
		g_string_append_printf(result, "%s(%s)", match, location);
		g_free(location);
	}
	g_free (match);
	return FALSE;
}

int main(int argc, char** argv)
{
	gchar *ipaddr;
	gint n_options;
    gchar **options;
    gint arg_index;

    for (arg_index = 1; arg_index < argc; arg_index++)
    {
        gchar *arg = argv[arg_index];

        if (!g_str_has_prefix (arg, "-"))
            break;
      
        if (strcmp (arg, "-h") == 0 || strcmp (arg, "--help") == 0)
        {
            g_printerr ("Usage:\n"
                        "  iplocation [OPTION...] <ipaddr> - IP Location tool\n"
                        "\n"
                        "Options:\n"
                        "  -h, --help        Show help options\n"
                        "  -v, --version     Show release version\n"
                        "\n");
            return EXIT_SUCCESS;
        }
        else if (strcmp (arg, "-v") == 0 || strcmp (arg, "--version") == 0)
        {
            g_printerr ("iplocation %s\n", VERSION);
            return EXIT_SUCCESS;
        }
        else
        {
            g_printerr ("Unknown option %s\n", arg);
            usage ();
            return EXIT_FAILURE;
        }
    }

    if (arg_index >= argc)
    {
		GRegex *reg;
		int size = 1024;  
		char* buff = (char*)malloc(size);  
		const gchar *pattern = "((?:(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d))))";
		gchar *res;

		reg = g_regex_new (pattern, 0, 0, NULL);
		while(NULL != fgets(buff, size, stdin)){
			res = g_regex_replace_eval (reg, buff, -1, 0, 0, eval_cb, NULL, NULL);
			g_printf("%s", res);
			g_free(res);
		}
		free(buff);
		return EXIT_SUCCESS;
    }
	
	gchar* location;
    for (; arg_index < argc; arg_index++) {
		ipaddr = argv[arg_index];
		if ((location = get_ip_location(ipaddr)) != NULL ) {
			g_printf("%s(%s)\n", ipaddr, location);
			g_free(location);
		}
	}
	return EXIT_SUCCESS;
}
