/******************************************************************************
 *
 * Copyright (c) 2015 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  Clock.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/UnsignedInteger64.h>
#include <es++/Exception.h>
#include <sys/time.h> 

namespace Es {

class Clock :public Object {
private:
 clockid_t clockId;

public:
  Clock(clockid_t id = CLOCK_MONOTONIC)
  :clockId(id)
  {
  }

  int getResolution(struct timespec& ts)
  {
    int rc = clock_getres(clockId, &ts);
    if (rc == ERROR) {
      throw Es::IException("Failed to clock_getres %s", ERROR_STRING);
    }
    return rc;
  }

  int getTime(time_t& sec, long& nsec)
  {
    struct timespec ts;
    int rc = clock_gettime(clockId, &ts);
    if (rc == ERROR) {
      throw Es::IException("Failed to clock_gettime %s", ERROR_STRING);
    }
    sec  = ts.tv_sec;
    nsec = ts.tv_nsec; 
    return rc;
  }

  int getTime(struct timespec& ts)
  {
    int rc = clock_gettime(clockId, &ts);
    if (rc == ERROR) {
      throw Es::IException("Failed to clock_gettime %s", ERROR_STRING);
    } 
    return rc;
  }

 struct timespec getTime()
  {
    struct timespec ts;
    int rc = clock_gettime(clockId, &ts);
    if (rc == ERROR) {
      throw Es::IException("Failed to clock_gettime %s", ERROR_STRING);
    }
    return ts;
  }

  int setTime(time_t sec, long nsec)
  {
    struct timespec ts;
    ts.tv_sec  = sec;
    ts.tv_nsec = nsec;

    int rc = clock_settime(clockId, &ts);
    if (rc == ERROR) {
      throw Es::IException("Failed to clock_settime %s", ERROR_STRING);
    }
    return rc;
  }

  int setTime(struct timespec& ts)
  {
    int rc = clock_settime(clockId, &ts);
    if (rc == ERROR) {
      throw Es::IException("Failed to clock_settime %s", ERROR_STRING);
    } 
    return rc;
  }

  void display()
  {
    time_t sec;
    long  nsec;
    getTime(sec, nsec);
    printf("sec = %lu  nsec = %lu\n", sec, nsec);
  }
};
}

