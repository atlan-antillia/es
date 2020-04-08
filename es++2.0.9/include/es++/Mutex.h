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
 *  Mutex.h
 *
 *****************************************************************************/

// 2016/03/29 Updated.

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <es++/CharString.h>
#include <pthread.h>

namespace Es {

class Mutex :public Object {
private:
  pthread_mutex_t mutex;

public:
  /**
   */
  Mutex(const  pthread_mutexattr_t *mutexAttr=NULL)
   :mutex((pthread_mutex_t)PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP)
  {
    if (mutexAttr) {
      int rc = pthread_mutex_init(&mutex,
                       mutexAttr);
      if (rc != 0) {
        throw Es::IException("Mutex#Mutex,1,Failed to pthread_mutex_init");  
      }
    }
  }

  

public:
  /**
   */
  ~Mutex() 
  {
    pthread_mutex_destroy(&mutex);  //2016/03/29
  }

public:
  int  lock() 
  {
    return pthread_mutex_lock(&mutex);    
  }

public:
  /**
   */
  int  unlock() 
  {
    return pthread_mutex_unlock(&mutex);    
  }

public:
  int tryLock() 
  {
    return pthread_mutex_trylock(&mutex); 
  }
  
};

}

