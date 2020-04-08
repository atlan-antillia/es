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
 *  SignalQueue.h
 *
 *****************************************************************************/


#pragma once

#include <es++/SignalSet.h>

namespace Es {

class SignalQueue :public Object {
private:
  pid_t pid;
  int   sig;

public:
  SignalQueue(int sig)
  :pid(getpid()),
   sig(sig)
  {
  }

public:
  SignalQueue(pid_t pid, int sig)
  :pid(pid),
   sig(sig)
  {
  }

/*
typedef union sigval{
  int   sival_int;
  void* sival_ptr;
} sigval_t;
 */
  void post(sigval_t value)
  {
    if (sigqueue(pid, sig, value) <0 ) {
      throw Es::IException("Failed to sigqueueu");
    }
  }

  void post(int value) 
  {
    sigval_t sval;
    sval.sival_int = value;
    if (sigqueue(pid, sig, sval) < 0) {
      throw Es::IException("Failed to sigqueueu");
    }
  }

  void post(void* ptr)
  {
    sigval_t sval;
    sval.sival_ptr = ptr;
    if (sigqueue(pid, sig, sval) < 0) {
      throw Es::IException("Failed to sigqueueu");
    }
  }

};

}


