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
 *  SignalWatcher.cpp
 *
 *****************************************************************************/

#include <es++/SignalWatcher.h>
#include <es++/SignalSet.h>
#include <es++/Signal.h>

namespace Es {

class WorkerThread: public Thread {
private:
  bool looping;  
  
public:
  WorkerThread()
  :Thread(),
  looping(true)
  {
  }

public:
  virtual void signaled(SignalInfo& siginfo)
  {
    int signo = siginfo.ssi_signo;
    const char* name = Signal::toString(signo);
    printf("WorkerThread::signaled called. signo=%d %s\n", signo, name);
  }
  

  virtual void run()
  {
    while (looping) {
      sleep(1);
    }
  }

  void stop()
  {
    looping = false;
  }
};
}

//
int main(int argc, char* argv[])
{
  try {

    SignalSet mask;
    mask.add(SIGINT);
    mask.add(SIGQUIT);
    mask.block();
    SignalWatcher watcher(-1, mask, 0);

    WorkerThread worker;
    //Set a signal callback (WorkerThread::signaled) to the watcher.
    watcher.setCallback(&worker, (Signaled)&WorkerThread::signaled);

    watcher.start();
    worker.start();

    watcher.wait();
    worker.wait();

  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}
