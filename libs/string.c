#include "string.h"

inline void memcpy(unsigned char *dest, const unsigned char *src, unsigned int len)
{
	while(len -- )
		*dest++ = *src++;
}

inline void memset(void *dest, unsigned char val, unsigned int len)
{
	unsigned char *dst = dest;
	while(len --)
		*dst++ = val;
}

inline void bzero(void *dest, unsigned int len)
{
	unsigned char *dst = dest;
	while(len --)
		*dst = 0;
}

inline int strcmp(const char *str1, const char *str2)
{
	int ret = 0;
	do
	{
		ret = (int)*str1 - *str2;	
	}while((ret == 0) && (*str1++ != 0) && (*str2++ != 0));

	return ret;
}

inline char *strcpy(char *dest, const char *src)
{
	char *dst = dest;
	do
	{
		*dest++ = *src++;
	}while(*src != 0);

	return dst;
}

inline char *strcat(char *dest, const char *src)
{
	char *dst = dest;
	while(*dst)
		dst++;
	do
	{
		*dst++ = *src;
	}while(*src++ != 0);
	return dest;
}

inline int strlen(const char *src)
{
	int ret=0;
	while(*src++)
		ret++;
	return ret;
}

