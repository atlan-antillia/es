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
 *  EventPollerEvents.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <es++/EventPollerEvent.h>
#include <sys/epoll.h>

/*
struct epoll_event {
   __u32 events;  // events 
  union {
    void *ptr;
    int fd;
    __u32 u32;
    __u64 u64;
  } data;
};
 */

namespace Es {

class EventPollerEvents :public Object {

private:
  static const int DEFAULT_MAX_EVENTS = 128;

private:
  struct epoll_event* epollEvents;
  int    maxEvents;
  int    polledEventsNum;

public:
  EventPollerEvents(int maxEvents = DEFAULT_MAX_EVENTS)
  :epollEvents(NULL),
  maxEvents(maxEvents),
  polledEventsNum(0)
  {
    if (maxEvents < 1) {
      throw Es::IException("Invalid argument %d", maxEvents);
    } else {
      epollEvents = new struct epoll_event[maxEvents];
      if (epollEvents) {
        for (int i = 0; i < maxEvents; i++) {
           memset(&epollEvents[i], 0, sizeof(struct epoll_event));
        }
      } 
    }
  }

  ~EventPollerEvents()
  {
     delete [] epollEvents;
     epollEvents = NULL;
  }

  struct epoll_event* getEvents()
  {
     return epollEvents;
  } 

  int  getMaxEvents()
  {
    return maxEvents;
  }

  void setNumberOfEvents(int num)
  {
     polledEventsNum = num;
  }

  int getNumberOfEvents()
  {
    return polledEventsNum;
  }

};

}


