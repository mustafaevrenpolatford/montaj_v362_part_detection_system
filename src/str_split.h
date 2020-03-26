#ifndef _STR_SPLIT_
#define _STR_SPLIT_


#include"includelar.h"

extern const char ayrac = ':';
extern const char *pvs_gelen_veri_isimleri[] = {"VIN_NUMBER", "waste", "date", "hour", "minute", "second", "colour_car", "rim", "front_door_handle", "mirror_garnish", "rear_door_handle", "side_moulding"};

char **str_split(char *a_str, const char a_delim)
{
	char **result = 0;
	int i;
	int count = 0;
	char *tmp = a_str;
	char *last_comma = 0;
	char delim[2];
	delim[0] = a_delim;
	delim[1] = 0;

	// Count how many elements will be extracted.
	while (*tmp)
	{
		if (a_delim == *tmp)
		{
			count++;
			last_comma = tmp;
		}
		tmp++;
	}

	// Add space for trailing token.
	count += last_comma < (a_str + strlen(a_str) - 1);

	//Add space for terminating null string so caller
	//knows where the list of returned strings ends.
	count++;

	result = (char **)calloc(count, sizeof(char *));

	if (result)
	{
		int idx = 0;
		char *token = strtok(a_str, delim);

		while (token)
		{
			assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(result + idx) = 0;

		for (i = 0; i < count - 1; ++i)
		{
			syslog(LOG_INFO, "%s(): %s: '%s'", __func__, pvs_gelen_veri_isimleri[i], result[i]);
		}
		return result;
	}

	return NULL;
}

#endif