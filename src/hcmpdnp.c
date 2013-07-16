/*
 * Simple HexChat plugin to fetch info on the currently playing track in MPD,
 * and print it to the channel.
 *
 * Author: Matt Chance
 * License: WTFPL
 */

#include <stdio.h>
#include <stdlib.h>
#include <hexchat-plugin.h>

#define PNAME "HCMPDNP"
#define PDESC "Prints the currently playing song in MPD"
#define PVERSION "0.1"

static hexchat_plugin *ph;

static int nowplaying_cb (char *word[], char *word_eol[], void *userdata)
{
	FILE *mpc;
	int cap = 1;
	int size = 0;
	int c = EOF;
	char *np = malloc(sizeof(char) * cap);

	if (np == NULL)
	{
		hexchat_print(ph, "Error allocating memory in HCMPDNP plugin\n");
		return HEXCHAT_EAT_ALL;
	}

	mpc = popen("mpc current -f '[[Artist: %artist% :: [Album: %album% :: ]][[Track: %track% :: ]Title: %title%]]|[%file%]'", "r");
	if (mpc == NULL)
	{
		hexchat_print(ph, "Error executing mpc\n");
		free(np);
		return HEXCHAT_EAT_ALL;
	}

	while ((c = fgetc(mpc)) != EOF)
	{
		if (c == '\n' || c == '\r')
			continue;

		np[size++] = c;
		if (size == cap)
		{
			np = realloc(np, (cap + 10) * sizeof(char));
			if (np == NULL)
			{
				hexchat_print(ph, "Error allocating memory in HCMPDNP plugin\n");
				pclose(mpc);
				free(np);
				return HEXCHAT_EAT_ALL;
			}
			cap = cap + 10;
		}
	}

	if (size == cap)
	{
		np = realloc(np, (cap + 1) * sizeof(char));
		if (np == NULL)
		{
			hexchat_print(ph, "Error allocating memory in HCMPDNP plugin\n");
			pclose(mpc);
			free(np);
			return HEXCHAT_EAT_ALL;
		}
		cap++;
	}

	np[size++] = '\0';

	if (*np == '\0')
	{
		hexchat_print(ph, "Nothing is currently playing or something went wrong\n");
		pclose(mpc);
		free(np);
		return HEXCHAT_EAT_ALL;
	}

	hexchat_commandf(ph, "ACTION np: %s", np);

	pclose(mpc);
	free(np);

	return HEXCHAT_EAT_ALL;
}

int hexchat_plugin_init (hexchat_plugin *plugin_handle, char **plugin_name, char **plugin_desc, char **plugin_version, char *arg)
{
	ph = plugin_handle;

	*plugin_name = PNAME;
	*plugin_desc = PDESC;
	*plugin_version = PVERSION;

	hexchat_hook_command(ph, "NP", HEXCHAT_PRI_NORM, nowplaying_cb, "Usage: NP, prints currently playing track info in the active window", NULL);

	hexchat_print(ph, "HCMPDNP Plugin Loaded\n");

	return 1;
}
