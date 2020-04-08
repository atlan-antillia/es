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
 *  SignalSet.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <signal.h>

namespace Es {

class SignalSet :public Es::Object {
private:
  sigset_t set;

public:
  SignalSet()
  {
    emptify(); 
  }

  //Block all signals except SIGBUS, SIGFPE, SIGILL
  void blockAll()
  {
    fill();
    del(SIGBUS);
    del(SIGFPE);
    del(SIGILL);
    del(SIGSEGV);
  }

  void emptify()
  {
    sigemptyset(&set);
  }

  void add(int signo)
  {
    sigaddset(&set, signo);
  }

  void fill()
  {
    sigfillset(&set);
  }

  void del(int signo)
  {
    sigdelset(&set, signo);
  }

  int isMember(int signo)
  {
    return sigismember(&set, signo);
  }

  sigset_t getSet()
  {
    return set;
  }

  int wait(int& sig)
  {
    return sigwait(&set, &sig);
  }

  int wait(siginfo_t& siginfo)
  {
    return sigwaitinfo(&set, &siginfo);
  }

  int wait(siginfo_t& siginfo, struct timespec& timeout)
  {
    return sigtimedwait(&set, &siginfo, &timeout);
  }

  int wait(siginfo_t* siginfo, struct timespec& timeout)
  {
    return sigtimedwait(&set, siginfo, &timeout);
  }


  int procMask(int how, sigset_t* oldset = NULL)
  {
    return sigprocmask(how, &set, oldset);
  }

  int block(sigset_t* oldset = NULL)
  {
    return procMask(SIG_BLOCK, oldset);
  }

  int unblock(sigset_t* oldset = NULL)
  {
    return procMask(SIG_UNBLOCK, oldset);
  }

  int setMask(sigset_t* oldset = NULL)
  {
    return procMask(SIG_SETMASK, oldset);
  }
};
}

