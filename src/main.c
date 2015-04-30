/* vi: set sw=4 ts=4 wrap ai: */
/*
 * main.c: This file is part of ____
 *
 * Copyright (C) 2015 yetist <xiaotian.wu@i-soft.com.cn>
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

#include <stdio.h>
#include <string.h>
//#include "config.h"

#include <gio/gio.h>
#include "moses-ip-location-generated.h"
#include "qqwry.h"

#define MOSES_DBUS_NAME "org.moses.iplocation"
#define MOSES_DBUS_PATH "/org/moses/IpLocation"

static void on_name_acquired (GDBusConnection *connection, const gchar *name, gpointer user_data)
{
	g_debug("Acquired the name %s\n", name);
}

static void on_name_lost (GDBusConnection *connection, const gchar *name, gpointer user_data)
{
	g_debug("Lost the name %s\n", name);
}

gchar* get_location_by_uint32(guint32 ip)
{
	char country[1024] = { '\0' };
	char area[1024] = { '\0' };

	GString *locstr;
	GError *error = NULL;

	gchar *datfile;
	FILE *wry_file;
	gchar *gbloc;
	gsize read, written;

	datfile = g_build_filename(g_get_user_config_dir() , "iplocation", "qqwry.dat", NULL);
	if (!g_file_test(datfile, G_FILE_TEST_EXISTS |G_FILE_TEST_IS_REGULAR))
	{
		g_free(datfile);
		datfile = g_build_filename("..", "data", "qqwry.dat", NULL);
		if (!g_file_test(datfile, G_FILE_TEST_EXISTS |G_FILE_TEST_IS_REGULAR))
		{
			g_free(datfile);
			datfile = g_build_filename(IP_LOCATION_DIR, "qqwry.dat", NULL);
		}
	}

	if (! g_file_test(datfile, G_FILE_TEST_EXISTS |G_FILE_TEST_IS_REGULAR))
	{
		g_free(datfile);
		return g_strdup("Unknown");
	}

	wry_file = fopen(datfile, "r");
	g_free(datfile);
	qqwry_get_location_by_long(country, area, ip, wry_file);
	fclose(wry_file);

	locstr = g_string_new(NULL);

	if (strlen(country) > 0) {
		locstr = g_string_append(locstr, country);
		if (strlen(area) > 0) {
			g_string_append_printf(locstr, " %s",  area);
		}
	} else {
		return g_strdup("Unknown");
	}

	gbloc = g_string_free(locstr, FALSE);

	gchar *location =  g_convert (gbloc, strlen(gbloc), "UTF-8", "GBK", &read, &written, &error);
	g_free(gbloc);
	if (error != NULL) {
		g_error_free (error);
	}
	return location;
}

gchar* get_location(const gchar* ip)
{
	return get_location_by_uint32(ip2long(ip));
}

gboolean ip_location_get_get_location_by_int (
		MosesIpLocation *object,
		GDBusMethodInvocation *invocation,
		guint arg_ip)
{
	gchar* location;
	location = get_location_by_uint32(arg_ip);
	if (location == NULL) {
		g_dbus_method_invocation_return_dbus_error (invocation,
				"org.moses.IpLocation.Error.Failed.",
				"Can not get location for this ip.");
		return FALSE;
	} else {
		moses_ip_location_complete_get_location_by_int (object, invocation, location);
		g_free(location);
		return TRUE;
	}
}

gboolean ip_location_get_get_location (
		MosesIpLocation *object,
		GDBusMethodInvocation *invocation,
		const gchar *arg_ip, gpointer user_data)
{
	gchar* location;
	location = get_location(arg_ip);
	if (location == NULL) {
		g_dbus_method_invocation_return_dbus_error (invocation,
				"org.moses.IpLocation.Error.Failed.",
				"Can not get location for this ip.");
		return FALSE;
	} else {
		moses_ip_location_complete_get_location (object, invocation, location);
		g_free(location);
		return TRUE;
	}
}

static gboolean register_interface(GDBusConnection *connection) 
{
	GError *error = NULL;
	MosesIpLocation *skeleton;

	connection = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, &error);
	if (connection == NULL) {
		if (error != NULL) {
			g_critical ("error getting session bus: %s", error->message);
			g_error_free (error);
		}
		return FALSE;
	}
	skeleton = moses_ip_location_skeleton_new ();
	g_signal_connect (skeleton, "handle-get-location", G_CALLBACK (ip_location_get_get_location), NULL);
	g_signal_connect (skeleton, "handle-get-location-by-int", G_CALLBACK (ip_location_get_get_location_by_int), NULL);
	if (!g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON (skeleton),
				connection,
				"/org/moses/IpLocation",
				&error))
	{
		if (error != NULL) {
			g_critical ("error getting session bus: %s", error->message);
		}
	}
	return TRUE;
}

static void on_bus_acquired (GDBusConnection *connection, const gchar *name, gpointer user_data)
{
	GDBusObjectManagerServer *object_manager;
	object_manager = g_dbus_object_manager_server_new (MOSES_DBUS_PATH);
	g_dbus_object_manager_server_set_connection (object_manager, connection);

	register_interface(connection);
}

int main(int argc, char *argv[])
{
	GMainLoop *loop;
	loop = g_main_loop_new (NULL, FALSE);
	g_bus_own_name (G_BUS_TYPE_SESSION,
			MOSES_DBUS_NAME,
			G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT | G_BUS_NAME_OWNER_FLAGS_REPLACE,
			on_bus_acquired,
			on_name_acquired,
			on_name_lost,
			NULL,
			NULL);
	g_main_loop_run (loop);
	g_main_loop_unref (loop);
	return 0;
}
