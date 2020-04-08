/******************************************************************************
 *
 * Copyright (c) 2015  TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  ThreadAttribute.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <pthread.h>

namespace Es {

class ThreadAttribute :Object {
private:
  pthread_attr_t attr;

public:
  ThreadAttribute()
  { 
    memset(&attr, 0, sizeof(attr));

    int rc = pthread_attr_init (&attr);
    if (rc != 0) {
      throw Es::IException("Faield to pthread_attr_ini: %s", error());
    }
  }

  ~ThreadAttribute()
  {
    pthread_attr_destroy (&attr);
  }

  int getDetachState(int& state)
  {
    return pthread_attr_getdetachstate (&attr, &state);
  }

  int setDetachState(int state)
  {
    return pthread_attr_setdetachstate (&attr, state);
  }

  int getGuardSize(size_t& size)
  {
    return pthread_attr_getguardsize (&attr, &size);
  }

  int setGuardSize(size_t size)
  {
    return pthread_attr_setguardsize (&attr, size);
  }

  int getSchedParam (struct sched_param& param)
  {
    return pthread_attr_getschedparam (&attr, &param);
  }

  pthread_attr_t get()
  {
    return attr;
  }
};

}

