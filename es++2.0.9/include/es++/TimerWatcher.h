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
 *  TimerWatcher.h
 *
 *****************************************************************************/

#include <es++/TimerDescriptor.h>
#include <es++/Clock.h>
#include <es++/ITimerSpec.h>
#include <es++/Thread.h>

namespace Es {

//Elapsed callback
typedef void (Object::*Elapsed)(uint64_t value);

class TimerWatcher: public Thread {

private:
  bool looping;  
  TimerDescriptor timerFD;
  Clock   clock;

public:
  TimerWatcher(int clockId = CLOCK_MONOTONIC,  
                    time_t intervalsec = 1, long intervalnsec= 0)
  :Thread(),
  looping(true),
  timerFD(clockId, 0),
  clock(clockId)
  {
    struct timespec now =  clock.getTime();
    ITimerSpec its(now, intervalsec, intervalnsec);

    timerFD.setTime(TFD_TIMER_ABSTIME, its);
  }

public:
  //Set an elapsed callback.
  void setCallback(Object* object, Elapsed elapsed)
  {
     timerFD.setCallback(object, elapsed); 
  }

public:
  
  virtual void run()
  {
    printf("TimeReceiver::run, start\n");
    while (looping) {
      try {
        uint64_t value;
        timerFD.read(value);
      } catch (Es::Exception& ex) {
        ex.display();
      } 
    }
  }

  void stop()
  {
     looping = false;
     timerFD.close();
  }
};
}

