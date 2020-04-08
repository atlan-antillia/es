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
 *  Thread.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <pthread.h>

//2015/05/10
#include <es++/SignalSet.h>

namespace Es {

#define INVALID_THREAD  (pthread_t)-1
  
class Thread :public Es::Object {

private:

  pthread_attr_t  attr;
  
  pthread_t  threadId;
  
  static  void* start_routine(void* arg) {
    Thread* thread = (Thread*)arg;
    if(thread) {
      thread ->threadId = pthread_self();
      thread -> run();
    }
    return NULL;
  }

public:
  Thread()
  :threadId(INVALID_THREAD) 
  {
  }

  ~Thread() 
  {
  }

public:
  //2015/05/25
  static void signalMask(int how, Es::SignalSet& set, sigset_t* oldset)
  {
    sigset_t mask = set.getSet();
    if(pthread_sigmask(how, &mask, oldset) != 0) {
      throw Es::IException("pthread_sigmask error");
    }
  }

  static void block(Es::SignalSet& set, sigset_t* oldset = NULL)
  {
    signalMask(SIG_BLOCK, set, oldset);
  }

  static void unblock(Es::SignalSet& set, sigset_t* oldset = NULL)
  {
    signalMask(SIG_UNBLOCK, set, oldset);
  }

  static void setmask(Es::SignalSet& set, sigset_t* oldset = NULL)
  {
    signalMask(SIG_SETMASK, set, oldset);
  }


public:
  int start(pthread_attr_t* attr = NULL)
  {
    int rc = pthread_create(&threadId, attr, start_routine, this);
    if (rc != 0) {    
      throw Es::IException("Failed to create a thread.");
    }
    return rc;
  }

  pthread_t  getThreadId() 
  {
    return threadId;
  }

  void  exit(void* exitCode) 
  {
    pthread_exit(exitCode);
  }

  // Thread main loop. 
  // Please override this run method in a subclass derived from this Thread class.
  virtual void run() 
  {
    // Do something
    printf("Thread::run\n");
  }

  void  sleep(int time) 
  {
    ::sleep(time);
  }

  void sleep(struct timespec& ts)
  {
    nanosleep(&ts, NULL);
  }

  void sleepMilliSec(int msec)
  {
    const int MILLI_SEC = 1000000;
    struct timespec ts = {0, msec* MILLI_SEC};
    nanosleep(&ts, NULL);
  }

  void    cancel() 
  {
    pthread_cancel(this->threadId);
  }


  virtual void wait() 
  {
    if (this->threadId != INVALID_THREAD) {
      pthread_join(this->threadId, NULL);  
      this->threadId = INVALID_THREAD;
    }
  }
};

}


