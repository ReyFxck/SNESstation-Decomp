#include "strstr_legacy_compat.h"

#ifdef F_strstr
char *	strstr(const char * string, const char * substring)
{
	char* strpos;

	if (string == 0)
		return 0;
	
	if (strlen(substring)==0)
		return (char*)string;

	strpos = (char*)string;

	while (*strpos != 0)
	{
		if (strncmp(strpos, substring, strlen(substring)) == 0)
		{
			return strpos;
		}

		strpos++;
	}

	return 0;
}
#endif
