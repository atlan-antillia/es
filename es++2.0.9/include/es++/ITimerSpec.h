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
 *  ITimerSpec.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <sys/time.h>

namespace Es {

class ITimerSpec :public Object {
private:
  struct itimerspec its;

public:
  ITimerSpec(const struct timespec& ts, const struct timespec& interval)
  {
    memset(&its, 0, sizeof(its));
    its.it_value.tv_sec  = ts.tv_sec;
    its.it_value.tv_nsec = ts.tv_nsec;

    its.it_interval.tv_sec  = interval.tv_sec;
    its.it_interval.tv_nsec = interval.tv_nsec;
  }

public:
  ITimerSpec(struct timespec& ts, time_t itsec, long itnsec)
  {
    memset(&its, 0, sizeof(its));
    its.it_value.tv_sec  = ts.tv_sec;
    its.it_value.tv_nsec = ts.tv_nsec;

    its.it_interval.tv_sec  = itsec;
    its.it_interval.tv_nsec = itnsec;
  }

  struct itimerspec get()
  {
    return its;
  }
};

}

