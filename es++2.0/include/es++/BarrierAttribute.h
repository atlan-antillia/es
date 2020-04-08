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
 *  BarrierAttribute.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <pthread.h>

namespace Es {

class BarrierAttribute :public Object {

private:
  pthread_barrierattr_t attr;

public:
  BarrierAttribute()
  :barrierAttribute(0),
  {
    int rc = pthread_barrierattr_init(&attr);
    if (rc < 0) {
      throw Es::IException("Failed to pthread_barrierattr_init: %s", error());
    }
  }

  ~BarrierAttribute()
  {
    try {
      destroy();
    } catch (Es::Exception& ex) {
      ex.display();
    }
  }

  void destroy()
  {
    if (barrier != 0) {
      int rc = pthread_barrierattr_destroy(&attr);
      if (rc != 0) {
        throw Es::IException("Failed to pthread_barrier_destroy:%s",
           error());
      }
    }
  }

  int getShared (int& pshared)
  {
    int rc = pthread_barrierattr_getpshared (&attr, &pshared);
    if (rc != 0) {
      throw Es::IException("Faield to pthread_barrierattr_getshared: %s", error());
    }
    return rc;
  }

  int setShared (int pshared)
  {
    int rc = pthread_barrierattr_setpshared (&attr, pshared);
    if (rc != 0) {
      throw Es::IException("Faield to pthread_barrierattr_setshared: %s", error());
    }
    return rc;
  }

};
}

