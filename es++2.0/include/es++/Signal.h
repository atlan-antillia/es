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
 *  Signal.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Pair.h>
#include <es++/SignalSet.h>

namespace Es {

class Signal :public Es::Object {
private:
  SignalSet sigset;

public:
  Signal(int signo)
  {
    sigset.add(signo); //2015/05/24
    sigset.block(); //2015/05/24
  }

  int wait(int& sig)
  {
    return sigset.wait(sig);
  }

/*
*/
  int wait(siginfo_t& siginfo)
  {
     return sigset.wait(siginfo);
  }


  int wait(siginfo_t& siginfo, int sec, int nsec=0)
  {
    struct timespec timeout;
    timeout.tv_sec = sec;
    timeout.tv_nsec = nsec;
    return sigset.wait(siginfo, timeout);
  }

  //Based on an output of "kill -l" command.
public:
  static const char* toString(int signo)
  {
    static const Pair<int, const char*> pairs[] = {
      { SIGHUP,      "SIGHUP" },
      { SIGINT,      "SIGINT" },
      { SIGQUIT,     "SIGQUIT" },
      { SIGILL,      "SIGILL" },
      { SIGTRAP,     "SIGTRAP" },
      { SIGABRT,     "SIGABRT" },
      { SIGBUS,      "SIGBUS" },
      { SIGFPE,      "SIGFPE" },
      { SIGKILL,     "SIGKILL" },
      { SIGUSR1,     "SIGUSR1" },
      { SIGSEGV,     "SIGSEGV" },
      { SIGUSR2,     "SIGUSR2" },
      { SIGPIPE,     "SIGPIPE" },
      { SIGALRM,     "SIGALRM" },
      { SIGTERM,     "SIGTERM" },
      { SIGSTKFLT,   "SIGSTKFLT" },
      { SIGCHLD,     "SIGCHLD" },
      { SIGCONT,     "SIGCONT" },
      { SIGSTOP,     "SIGSTOP" },
      { SIGTSTP,     "SIGTSTP" },
      { SIGTTIN,     "SIGTTIN" },
      { SIGTTOU,     "SIGTTOU" },
      { SIGURG,      "SIGURG" },
      { SIGXCPU,     "SIGXCPU" },
      { SIGXFSZ,     "SIGXFSZ" },
      { SIGVTALRM,   "SIGVTALRM" },
      { SIGPROF,     "SIGPROF" },
      { SIGWINCH,    "SIGWINCH" },
      { SIGIO,       "SIGIO" },
      { SIGPWR,      "SIGPWR" },
      { SIGSYS,      "SIGSYS" },
      { SIGRTMIN,    "SIGRTMIN" },
      { SIGRTMIN+1,  "SIGRTMIN+1" },
      { SIGRTMIN+2,  "SIGRTMIN+2" },	
      { SIGRTMIN+3,  "SIGRTMIN+3" },
      { SIGRTMIN+4,  "SIGRTMIN+4" },
      { SIGRTMIN+5,  "SIGRTMIN+5" },
      { SIGRTMIN+6,  "SIGRTMIN+6" },
      { SIGRTMIN+7,  "SIGRTMIN+7" },
      { SIGRTMIN+8,  "SIGRTMIN+8" },
      { SIGRTMIN+9,  "SIGRTMIN+9" },
      { SIGRTMIN+10, "SIGRTMIN+10" },
      { SIGRTMIN+11, "SIGRTMIN+11" },
      { SIGRTMIN+12, "SIGRTMIN+12" },
      { SIGRTMIN+13, "SIGRTMIN+13" },
      { SIGRTMIN+14, "SIGRTMIN+14" },
      { SIGRTMIN+15, "SIGRTMIN+15" },
      { SIGRTMAX-14, "SIGRTMAX-14" },
      { SIGRTMAX-13, "SIGRTMAX-13" },
      { SIGRTMAX-12, "SIGRTMAX-12" },
      { SIGRTMAX-11, "SIGRTMAX-11" },
      { SIGRTMAX-10, "SIGRTMAX-10" },
      { SIGRTMAX-9,  "SIGRTMAX-9" },
      { SIGRTMAX-8,  "SIGRTMAX-8" },
      { SIGRTMAX-7,  "SIGRTMAX-7" },
      { SIGRTMAX-6,  "SIGRTMAX-6" },
      { SIGRTMAX-5,  "SIGRTMAX-5" },
      { SIGRTMAX-4,  "SIGRTMAX-4" },
      { SIGRTMAX-3,  "SIGRTMAX-3" },
      { SIGRTMAX-2,  "SIGRTMAX-2" },
      { SIGRTMAX-1,  "SIGRTMAX-1" },
      { SIGRTMAX,    "SIGRTMAX" },
    };

    const char* name = "";
    for (int i = 0; i <SizeOf(pairs); i++) {
      if (signo == pairs[i].first) {
        name = pairs[i].second;
        break;
      }
    }
    return name;
  }

  static void ignore(int signo)
  {
    sigignore(signo);
  }

  static void raise(int pid, int signo)
  {
    kill(pid, signo);
  }
};
}

