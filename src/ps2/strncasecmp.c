#include "strncasecmp_legacy_compat.h"

#ifdef F_strncasecmp
int	 strncasecmp(const char * string1, const char * string2, size_t n)
{
	if (n == 0)
		return 0;

	while ((n-- != 0) && (tolower(*string1) == tolower(*string2)))
    {
		if ((n == 0) || (*string1 == '\0') || (*string2 == '\0'))
			break;

		string1++;
		string2++;
	}

	return tolower(*(unsigned char *) string1) - tolower(*(unsigned char *) string2);
}
#endif
