#include "ctype_legacy_compat.h"

#ifdef F_ctype
int tolower(int c)
{
	if (isupper(c))
		c+=32;

	return c;
}

int toupper(int c)
{
	if (islower(c))
		c-=32;

	return c;
}


int isupper(int c)
{
	if (c < 'A')
		return 0;

	if (c > 'Z')
		return 0;

	// passed both criteria, so it
	// is an upper case alpha char
	return 1;
}

int islower(int c)
{
	if (c < 'a')
		return 0;

	if (c > 'z')
		return 0;

	// passed both criteria, so it
	// is a lower case alpha char
	return 1;
}

int isalpha(int c)
{
	if (islower(c) || isupper(c))
		return 1;

	return 0;
}

int isdigit(int c)
{
	if (c < '0')
		return 0;

	if (c > '9')
		return 0;

	// passed both criteria, so it
	// is a numerical char
	return 1;
}

int isalnum(int c)
{
	if (isalpha(c) || isdigit(c))
		return 1;

	return 0;
}

int iscntrl(int c)
{
	if (c < 0x20)
		return 1;

	if (c == 0x7F)
		return 1;

	return 0;
}


int isgraph(int c)
{
	if (iscntrl(c))
		return 0;

	if (isspace(c))
		return 0;

	return 1;
}


int isprint(int c)
{
	if (iscntrl(c))
		return 0;

	return 1;
}

int ispunct(int c)
{
	if (iscntrl(c))
		return 0;
	
	if (isalnum(c))
		return 0;

	if (isspace(c))
		return 0;

	// It's a printable character
	// thats not alpha-numeric, or a space
	// so its a punctuation character
	return 1;
}

int isspace(int c)
{
	if ((c>=0x09) && (c<=0x0D))
		return 1;

	if (c==0x20)
		return 1;

	return 0;
}



int isxdigit(int c)
{
	if (isdigit(c))
		return 1;

	if ((c>='a') && (c<='f'))
		return 1;

	if ((c>='A') && (c<='F'))
		return 1;

	return 0;
}
#endif
