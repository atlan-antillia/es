/******************************************************************************
 *
 * Copyright (c) 2017  TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
 *  SignalQueue.cpp
 *
 *****************************************************************************/

#include <es++/CharString.h>
#include <es++/Thread.h>
#include <es++/Signal.h>
#include <es++/SignalQueue.h>
#include <es++/SignalSet.h>

namespace Es {

class SignalSender: public Thread {
private:
  bool looping;  
  SignalQueue queue;

public:
  SignalSender(int signo)
  :Thread(),
  looping(true),
  queue(signo)
  {
  }

  //Thread main procedure which can be called from  Thread::start method.
  virtual void run()
  {
    int i = 0;
    while (looping) {
      sleep(1);
      char buffer[128];
      sprintf(buffer, "Hello world %d", i++);
      CharString* msg = new CharString(buffer); 
      printf("SignalSender::run(); post \"%s\"\n", buffer);
      queue.post(msg);
    }
  }
};

class SignalReceiver :public Thread {
private:
  bool looping;
  Signal signal;
  int    signo;

public:
  SignalReceiver(int signo)
  :Thread(),
  looping(true),
  signal(signo),
  signo(signo)
  {
  }

  //Thread main procedure which can be called from  Thread::start method.
  virtual void run()
  {
    while (looping) {
      try {
        siginfo_t siginfo;
        signal.wait(siginfo);  
        int sno = siginfo.si_signo;
        const char* name = Signal::toString(sno);
        if (sno == signo) {
          CharString* msg = (CharString*)siginfo.si_value.sival_ptr;
          if (msg) {
            printf("SignalReceiver::run(); Got a signal %d %s  value = \"%s\"\n", 
                    sno, name, (const char*)(*msg));
            delete msg;
          }
        } else {
          printf("Unexpcted signal %d %s\n", sno, name);
        }      
        
      } catch (Es::Exception& ex) {
        caught(ex);
      }
    }
  }
};
}

//
int main(int argc, char* argv[])
{
  try {
    int signo = SIGUSR1;

    SignalSet set;
    set.add(signo);
    Thread::block(set);

    SignalSender   sender(signo);
    sender.start();

    SignalReceiver receiver(signo);
    receiver.start();
    
    receiver.wait();
    sender.wait();

  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}
