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
 *  Barrier.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <pthread.h>

namespace Es {

class Barrier :public Object {

private:
  pthread_barrier_t barrier;

  size_t  size;

public:
  Barrier(size_t size, const pthread_barrierattr_t* attr=NULL)
  :size(size)
  {
    if (size < 1) {
      throw Es::IException("Invalid argument %d", size);
    }

    int rc = pthread_barrier_init(&barrier, attr, size);
    if (rc < 0) {
      throw Es::IException("Failed to pthread_barrier_init: %s", error());
    }
  }

  ~Barrier()
  {
    try {
      destroy();
    } catch (Es::Exception& ex) {
      ex.display();
    }
  }

  void destroy()
  {
    int rc = pthread_barrier_destroy(&barrier);
    if (rc != 0) {
      throw Es::IException("Failed to pthread_barrier_destroy:%s",
           error());
    }
  }

  void wait()
  {
    int rc = pthread_barrier_wait(&barrier);
    if (rc == 0 || rc == PTHREAD_BARRIER_SERIAL_THREAD ) {
      printf("Barrier::wait rc %d\n", rc);
      ; //OK
    } else {
      throw Es::IException("Failed to pthread_barrier_wait: %s",
             error() );
    }
  }
};

}

