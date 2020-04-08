/******************************************************************************
 *
 * Copyright (c) 2014 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  LocalDateTime.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/CharString.h>
#include <sys/time.h>

namespace Es {

/*
 Get a localTime by using API getLocalTime and return a string expression of a format  
 "YYYY/MM/DD HH:MM:SS" or "YYYY/MM/DD HH:MM:SS ssss"
*/

class LocalDateTime :public Object {

public:
  LocalDateTime()
  {
  }

public:
  CharString nowToSeconds() {
    char buff[128];
    memset(buff, (char)0, sizeof(buff));
    nowToSeconds(buff, sizeof(buff));
    return CharString(buff);
  }

public:
  const char* nowToSeconds(CharString& dateTime) {
    char buff[128];
    memset(buff, (char)0, sizeof(buff));
    nowToSeconds(buff, sizeof(buff));
    dateTime = buff;
    return (const char*)dateTime;
  }

public:
  CharString nowToMilliseconds()
  {
    char buff[128];
    memset(buff, (char)0, sizeof(buff));
    nowToMilliseconds(buff, sizeof(buff));  
    return CharString(buff);
  }

public:
  const char* nowToMilliseconds(CharString& dateTime) {
    char buff[128];
    memset(buff, (char)0, sizeof(buff));
    nowToMilliseconds(buff, sizeof(buff));
    dateTime = buff;
    return (const char*)dateTime;
  }

public:
  /**
     */
  void nowToSeconds(char* buff, size_t size) {    
    time_t timer; 
    time(&timer);
    struct tm tm;
    localtime_r(&timer, &tm);
    memset(buff, 0, size);
    
    snprintf(buff, size,
      "%04d/%02d/%02d %02d:%02d:%02d",
      tm.tm_year+1900,
      tm.tm_mon+1,
      tm.tm_mday,
      tm.tm_hour,
      tm.tm_min,
      tm.tm_sec);
  }

public:
  /**
     */
  void nowToSeconds(wchar_t* buff, size_t size) {
    time_t timer; 
    time(&timer);
    struct tm tm;
    localtime_r(&timer, &tm);
    memset(buff, 0, size);
    
    swprintf(buff, size,
      L"%04d/%02d/%02d %02d:%02d:%02d",
      tm.tm_year+1900,
      tm.tm_mon+1,
      tm.tm_mday,
      tm.tm_hour,
      tm.tm_min,
      tm.tm_sec);
  }

public:
  /**
     */
  void nowToMilliseconds(char* buff, int size) {
    
    struct timeval  tv;
    gettimeofday(&tv, NULL);

    time_t timer = (time_t)tv.tv_sec;
    
    struct tm tm;
    localtime_r(&timer, &tm);
    memset(buff, 0, size);
  
    snprintf(buff, size,
      "%04d/%02d/%02d %02d:%02d:%02d %ld",
      tm.tm_year+1900,
      tm.tm_mon+1,
      tm.tm_mday,
      tm.tm_hour,
      tm.tm_min,
      tm.tm_sec,
      tv.tv_usec);
  }

public:
  /**
     */
  void nowToMilliseconds(wchar_t* buff, int size) {
    
    struct timeval  tv;
    gettimeofday(&tv, NULL);

    time_t timer = (time_t)tv.tv_sec;
    
    
    struct tm tm;
    localtime_r(&timer, &tm);
    memset(buff, 0, size);
  
    swprintf(buff, size,
      L"%04d/%02d/%02d %02d:%02d:%02d %ld",
      tm.tm_year+1900,
      tm.tm_mon+1,
      tm.tm_mday,
      tm.tm_hour,
      tm.tm_min,
      tm.tm_sec,
      tv.tv_usec);
  }
};

}


