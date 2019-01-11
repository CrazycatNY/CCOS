#ifndef __INCLUDE_STRING_H
#define __INCLUDE_STRING_H
void memcpy(unsigned char *dest, const unsigned char *src, unsigned int len);

void memset(void *dest, unsigned char val, unsigned int len);

void bzero(void *dest, unsigned int len);

int strcmp(const char *str1, const char *str2);

char *strcpy(char *dest, const char *src);

char *strcat(char *dest, const char *src);

int strlen(const char *src);
#endif
