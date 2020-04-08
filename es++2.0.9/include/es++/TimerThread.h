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
 *  TimerThread.h
 *
 *****************************************************************************/

//2015/05/01 This is a very simpe interval timer thread.
//2015/05/01 We use nanosleep instead of usleep, because usleep is already obsolete.

#pragma once

#include <es++/Object.h>
#include <es++/Thread.h>
#include <es++/LocalDateTime.h>

namespace Es {

typedef void (Es::Object::*Timeout)(Object* );

class TimerThread :public Thread {
private:
  static const int SLEEP_TIME = 100; //100msec

private:
  bool          looping;
  int           interval; //sleep interval of seconds
  Es::Object* object;
  Timeout       timeout;  //Timeout callback method of the object above
  int           sleepCount;
  timespec      ts;     

public:
  TimerThread(int second)
    :looping(true),
    interval(second),
    object(NULL),
    timeout(NULL)
  {
    sleepCount = interval * SLEEP_TIME/ 10;
    ts.tv_sec = 0;
    ts.tv_nsec = SLEEP_TIME * 1000000;
  }

public:
  void setTimeout(Es::Object* object, Timeout timeout)
  {
    if (object && timeout) {
      this -> object = object;
      this -> timeout = timeout;
    } else {
      throw Es::IException("Invalid argument");
    }
  }

  void run()
  {
    printf("TimerThead::run start\n");
    while (looping) {
      Es::LocalDateTime dateTime;
      Es::CharString string;
      dateTime.nowToSeconds(string);
       //printf("NowToSeconds %s\n", (const char*)string);
      if (object && timeout) {
         //Call timeout method of object.
         (object->*timeout)((Es::Object*)&string);
      }
      this -> sleep();
    }
    printf("TimerThead::run end\n");
  }
  
  void sleep()
  {
    for (int i = 0; i< sleepCount; i++) {
      if (looping == false) {
        break;
      }
      nanosleep(&ts, NULL);
    }
  }
 
  void stop()
  {
    looping = false;
  }
};
 
}


