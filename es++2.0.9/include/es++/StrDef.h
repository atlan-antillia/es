/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer.
 *  
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 *  StrDef.h
 *
 *****************************************************************************/


#pragma once

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>
#include <wchar.h>

#include <assert.h>

namespace Es {

// strdup
inline wchar_t* __strdup(const wchar_t *source ) 
{
  assert(source);
  
  wchar_t* string = new wchar_t[wcslen(source)+1];
  wcscpy(string, source);
  return string;
}

// strdup
inline char* __strdup(const char *source) 
{
  assert(source);
  char* string = new char[strlen(source)+1];
  strcpy(string, source);
  return string;
}


// strtok: split wide-character string into tokens

inline wchar_t *__strtok(wchar_t *token, const wchar_t *delimit, wchar_t **ptr)
{
  assert(token);
  assert(delimit);
  
  return wcstok(token, delimit, ptr);
}


inline char *__strtok(char *token, const char *delimit, char **unused)
{
  assert(token);
  assert(delimit);

  return strtok(token, delimit);
}



//strcat
inline wchar_t* __strcat(wchar_t *dest, const wchar_t *source) 
{
  assert(dest);
  assert(source);
  return wcscat(dest, source);
}

inline char* __strcat(char *dest, const char *source) 
{
  assert(dest);
  assert(source);

  return strcat(dest, source);
}



//strncat
inline wchar_t* __strncat(wchar_t *dest, const wchar_t *source, size_t count) 
{
  assert(dest);
  assert(source);
  assert(count>0);
  
  return wcsncat(dest, source, count);
}

inline char* __strncat(char *dest, const char *source, size_t count) 
{
  assert(dest);
  assert(source);
  assert(count>0);

  return strncat(dest, source, count);
}



//strcmp
inline int __strcmp(const wchar_t *string1, const wchar_t *string2) 
{
  assert(string1);
  assert(string2);
  
  return wcscmp(string1, string2);
}

//strcmp
inline int __strcmp(const char *string1, const char *string2) 
{
  assert(string1);
  assert(string2);

  return strcmp(string1, string2);
}

inline int __strcasecmp(const wchar_t *string1,const wchar_t *string2)
{
  assert(string1);
  assert(string2);

  return wcscasecmp(string1,string2);
}

inline int __strcasecmp(const char *string1, const char *string2)
{
  assert(string1);
  assert(string2);

  return strcasecmp(string1,string2);
}


//strncmp
inline int __strncmp(const wchar_t *string1, const wchar_t *string2, size_t count) 
{
  assert(string1);
  assert(string2);
  assert(count>0);
  
  return wcsncmp(string1, string2, count);
}

inline int __strncmp(const char *string1, const char *string2, size_t count) 
{
  assert(string1);
  assert(string2);
  assert(count>0);

  return strncmp(string1, string2, count);
}

//strnicmp
inline int __strncasecmp(const wchar_t *string1, const wchar_t *string2, size_t count) 
{
  assert(string1);
  assert(string2);
  assert(count>0);

  return wcsncasecmp(string1, string2, count);
}

inline int __strncasecmp(const char *string1, const char *string2, size_t count) 
{
  assert(string1);
  assert(string2);
  assert(count>0);

  return strncasecmp(string1, string2, count);
}



//strspn
inline size_t __strspn(const wchar_t *str,const wchar_t *accept)
{
  assert(str);
  assert(accept);
  
  return wcsspn(str, accept);
}

inline size_t __strspn(const char *str, const char *accept)
{
  assert(str);
  assert(accept);

  return strspn(str, accept);
}


  //strcspn
inline size_t __strcspn(const wchar_t *str,const wchar_t *reject)
{
  assert(str);
  assert(reject);
  
  return wcscspn(str, reject);
}

inline size_t __strcspn(const char *str, const char *reject)
{
  assert(str);
  assert(reject);

  return strcspn(str, reject);
}

//strncpy
inline wchar_t *__strncpy(wchar_t *dest, const wchar_t *source, size_t count) 
{
  assert(dest);
  assert(source);
  assert(count>0);
  
  return wcsncpy(dest, source, count);
}

inline char *__strncpy(char *dest, const char *source, size_t count) 
{
  assert(dest);
  assert(source);
  assert(count>0);

  return strncpy(dest, source, count);
}



// strtod
inline double __strtod(const wchar_t *nptr, wchar_t **endptr)
{
  assert(nptr);
  assert(endptr);
  
  return wcstod(nptr, endptr);
}

inline double __strtod(char *nptr, char **endptr)
{
  assert(nptr);
  assert(endptr);

  return strtod(nptr,endptr);
}

// strtol
inline long __strtol(const wchar_t *nptr, wchar_t **endptr, int base) 
{
  assert(nptr);
  assert(endptr);
  assert(base>=0);
  
  return wcstol(nptr, endptr, base);
}

// strtol
inline long strtol(const char *nptr, char **endptr, int base) 
{
  assert(nptr);
  assert(endptr);
  assert(base>=0);

  return strtol(nptr, endptr, base);
}

// strtoul
inline unsigned long __strtoul(const wchar_t *nptr, wchar_t **endptr, int base)
{
  assert(nptr);
  assert(endptr);
  assert(base>=0);
  
  return wcstoul(nptr,endptr, base);
}

inline unsigned long __strtoul(const char *nptr, char **endptr, int base)
{
  assert(nptr);
  assert(endptr);
  assert(base>=0);

  return strtoul(nptr,endptr, base);
}



//strlen
inline size_t __strlen(const wchar_t *string) 
{
  assert(string);
  
  return wcslen(string);
}

inline size_t __strlen(const char *string) 
{
  assert(string);

  return strlen(string);
}



//strcpy
inline wchar_t* __strcpy(wchar_t *dest, const wchar_t *source) 
{
  assert(dest);
  assert(source);
  
  return wcscpy(dest, source);
}


inline char* __strcpy(char *dest, const char *source) 
{
  assert(dest);
  assert(source);

  return strcpy(dest, source);
}




//strchr
inline const wchar_t* __strchr(const wchar_t *string, wchar_t c) 
{
  assert(string);
  
  return wcschr(string, c);
}

inline const char* __strchr(const char *string, char c) 
{
  assert(string);

  return strchr(string, c);
}



//strstr
inline const wchar_t* __strstr(const wchar_t *string1, const wchar_t *string2) 
{
  assert(string1);
  assert(string2);

  return wcsstr(string1, string2);
}

inline const char* __strstr(const char *string1, const char *string2) 
{
  assert(string1);
  assert(string2);

  return strstr(string1, string2);
}



//strrchr
inline wchar_t *__strrchr(wchar_t *str, wchar_t c)
{ 
  assert(str);
  
  return wcsrchr(str, c); 
}

inline const char *__strrchr(const char *str, char c)
{
  assert(str);

  return strrchr(str, c); 
}
	
//2015/02/22 Added __startsWith and __endsWith.
inline bool __startsWith(const char* string, const char* start)
{
  assert(string);
  assert(start);
  int len  = strlen(string);
  int slen = strlen(start);

  if (slen > len) {
    return false;
  } 
  if (strncmp(string, start, slen) == 0) {
    return true;
  } else {
    return false;	
  }
}

inline bool __startsWith(const wchar_t* string, const wchar_t* start)
{
  assert(string);
  assert(start);
  int len  = wcslen(string);
  int slen = wcslen(start);

  if (slen > len) {
    return false;
  } 
  if (wcsncmp(string, start, slen) == 0) {
    return true;
  } else {
    return false;	
  }
}

inline bool __endsWith(const char* string, char ch)
{
  assert(string);
  int len = strlen(string);
  if (len > 0 && string[len-1] == ch) {
    return true;
  } else {
    return false;
  }
}

inline bool __endsWith(const wchar_t* string, wchar_t ch)
{
  assert(string);
  int len = __strlen(string);
  if (len > 0 && string[len-1] == ch) {
    return true;
  } else {
    return false;
  }
}
	
//2015/02/22
inline bool __endsWith(const char* string, const char* end)
{
  assert(string);
  assert(end);
  int len  = strlen(string);
  int elen = strlen(end);

  if (elen > len) {
    return false;
  } 
  const char* start = string + (len - elen);
  if (strncmp(start, end, elen) == 0) {
    return true;
  } else {
    return false;	
  }
}

inline bool __endsWith(const wchar_t* string, const wchar_t* end)
{
  assert(string);
  assert(end);
  int len  = wcslen(string);
  int elen = wcslen(end);

  if (elen > len) {
    return false;
  } 
  const wchar_t* start = string + (len - elen);
  if (wcsncmp(start, end, elen) == 0) {
    return true;
  } else {
    return false;	
  }
}

//strftime
inline size_t __strftime(wchar_t *dest, size_t maxsize, const wchar_t *format, const struct tm *timeptr) 
{
  assert(dest);
  assert(maxsize>0);
  assert(format);
  assert(timeptr);
  
  return wcsftime(dest, maxsize, format, timeptr);
}

inline size_t __strftime(char *dest, size_t maxsize, const char *format, const struct tm *timeptr) 
{
  assert(dest);
  assert(maxsize>0);
  assert(format);
  assert(timeptr);

  return strftime(dest, maxsize, format, timeptr);
}



//memchr
inline wchar_t* __memchr(wchar_t *a, wchar_t b, size_t c) {

  return wmemchr(a,b,c);
}


inline char* __memchr(const char *a, char b, size_t c) {
  return (char*)memchr((void*)a, b, c);
}



//strpbrk
inline const wchar_t* __strpbrk(const wchar_t *a, const wchar_t *b) 
{
  assert(a);
  assert(b);
  
  return wcspbrk(a,b);
}

inline const char* __strpbrk(const char *a, const char *b) 
{
  assert(a);
  assert(b);

  return strpbrk(a, b);
}



inline int __strcoll(const wchar_t* a, const wchar_t* b)
{
  assert(a);
  assert(b);

  return wcscoll(a, b);
}

inline int __strcoll(const char* a, const char* b)
{
  assert(a);
  assert(b);

  return strcoll(a, b);
}



//strxfrm
inline size_t __strxfrm(wchar_t *dest, const wchar_t *source, size_t count)
{
  assert(dest);
  assert(source);
  assert(count>0);
  
  return wcsxfrm(dest, source, count);
}


inline size_t _strxfrm(char *dest, const char *source, size_t count)
{
  assert(dest);
  assert(source);
  assert(count>0);
  
  return  strxfrm(dest, source, count);
}


//vprintf

inline int __vprintf(const wchar_t *format,  va_list args) 
{
  return vwprintf(format, args);
}

inline int __vprintf(const char *format,  va_list args) 
{
  return vprintf(format, args);
}


//vsnprintf
inline int __vsnprintf(wchar_t *dest, size_t maxlen, const wchar_t *format, va_list args) 
{
  assert(dest);
  assert(maxlen>0);
  assert(format);
  
  return vswprintf(dest, maxlen, format, args);
}

inline int __vsnprintf(char *dest, size_t maxlen, const char *format, va_list args) 
{
  assert(dest);
  assert(maxlen>0);
  assert(format);

  return vsnprintf(dest, maxlen, format, args);
}




//memcmp

inline int __memcmp(const wchar_t *buf1, const wchar_t *buf2, size_t count)
{
  assert(buf1);
  assert(buf2);
  assert(count>0);
  
  return wmemcmp(buf1, buf2, count);
}

inline int __memcmp(const char *buf1, const char *buf2, size_t count)
{
  assert(buf1);
  assert(buf2);
  assert(count>0);

  return memcmp(buf1, buf2, count);
}


//memcpy

inline wchar_t *__memcpy(wchar_t *dest, const wchar_t *source, size_t count) 
{
  assert(dest);
  assert(source);
  assert(count>0);
  
  return wmemcpy(dest, source, count);
}

inline char *__memcpy(char *dest, const char *source, size_t count)
{
  assert(dest);
  assert(source);
  assert(count>0);

  return (char*)memcpy(dest,  source, count);
}


//memset
inline wchar_t *__memset(wchar_t *dest, wchar_t ch, size_t size)
{
  assert(dest);
  
  return wmemset(dest, ch, size);
}

inline char *__memset(char *dest, char ch, size_t size)
{
  assert(dest);
  
  return (char*)memset(dest, ch, size);
}

}

  
