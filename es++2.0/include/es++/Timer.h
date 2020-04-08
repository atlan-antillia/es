/******************************************************************************
 *
 * Copyright (c) 2014 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *  notice, this list of conditions, and the following disclaimer.
 * 
 * 2. The name of the author may not be used to endorse or promote products
 *  derived from this software without specific prior written permission.
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
 * Timer.h
 *
 *****************************************************************************/

#pragma once
#include <es++/Object.h>
#include <stdint.h>
#include <es++/CharString.h>

#include <sys/time.h>

/*
  struct timeval {
    time_t   tv_sec;   
    suseconds_t tv_usec;
  }
*/

namespace Es {

class Timer :public Object {

private:
  struct timeval tv;
  uint64_t       value;

private:
  uint64_t toInt64(timeval tv)
  {
    return (tv.tv_sec*1000000ull + tv.tv_usec);
  }
  
public:
  //Constructor
  Timer()
  :value(0ull)
  {
    timerclear(&tv);
    gettimeofday(&tv, 0);
    value = toInt64(tv);
  }

  Timer(time_t sec, suseconds_t usec)
  :value(0ull)
  {
    timerclear(&tv);
    tv.tv_sec = sec;
    tv.tv_usec = usec;
  	
    value = toInt64(tv);
  }

  Timer(timeval tval)
  :value(0ull)
  {
    timerclear(&tv);
    tv = tval;
    value = toInt64(tv);
  }


  Timer(const Timer& tval)
  {
    tv = tval.tv;
    value = tval.value;
  }

public:
  Timer operator+(Timer& time)
  {
    struct timeval result;
    timerclear(&result);
    struct timeval tmp = time.getTimeval();
    timeradd(&tv, &tmp, &result);

    return Timer(result);
  }


  Timer add(time_t sec, suseconds_t usec)
  {
    struct timeval result;
    timerclear(&result);
    
    struct timeval tmp;
    timerclear(&tmp);
    tmp.tv_sec = sec;
    tmp.tv_usec = usec;
    
    timeradd(&tv, &tmp, &result);
    
    return Timer(result);
  }
  
  Timer sub(time_t sec, suseconds_t usec)
  {
    struct timeval result;
    timerclear(&result);
    
    struct timeval tmp;
    timerclear(&tmp);
    tmp.tv_sec = sec;
    tmp.tv_usec = usec;
    
    timersub(&tv, &tmp, &result);
    
    return Timer(result);
  }

  Timer operator-(Timer& time)
  {
    struct timeval result;
    timerclear(&result);
    struct timeval tmp = time.getTimeval();
    timersub(&tv, &tmp, &result);
    
    return Timer(result);
  }

  uint64_t getCurrentTime() {
    return value;

  }
  
  
  struct timeval getTimeval() {
    return tv;
  }

  CharString toCharString(const char* year_delim="/", const char* time_delim=":", const char* usec_delim="." ) {
    char buffer[40];
    memset(buffer, 0, sizeof(buffer));

    time_t t = tv.tv_sec;
  
    struct tm *tms = ::localtime(&t);

    sprintf(buffer, "%04d%s%02d%s%02d %02d%s%02d%s%02d%s%06ld", 
      tms->tm_year + 1900, 
      year_delim,
      tms->tm_mon + 1,
      year_delim,
      tms->tm_mday,
     
      tms->tm_hour,
      time_delim,
      tms->tm_min,
      time_delim,
      tms->tm_sec,
      usec_delim,
      tv.tv_usec);

    return CharString(buffer);
  }


  void display()
  {
    printf("%s\n", (const char*)toCharString());
  }
};

}


