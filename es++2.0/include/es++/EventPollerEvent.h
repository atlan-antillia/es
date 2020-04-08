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
 *  EventPollerEvent.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/Thread.h>
#include <es++/Exception.h>
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

class EventPollerEvent :public Object {

private:
  struct epoll_event event;
  
public:
  EventPollerEvent()
  {
    memset(&event, 0, sizeof(event));
  }

  EventPollerEvent(__u32 events, int fd)
  {
    memset(&event, 0, sizeof(event));
    event.events = events;
    event.data.fd = fd;
  }

  EventPollerEvent(struct epoll_event& ev)
  {
    event = ev;
  }
  
  struct epoll_event& getEvent()
  {
    return event;
  }

  __u32 events()
  {
    return event.events;
  }

  void events(__u32 ev)
  {
    event.events = ev;
  }

  int fd()
  {
    return event.data.fd;
  }

  void fd(int fd)
  {
    event.data.fd = fd;
  }

  void* ptr()
  {
    return event.data.ptr;
  }
  
  bool readable()
  {
    bool rc = false;
    if (event.events & EPOLLIN) {
      rc = true;
    }
    return rc;
  }

  bool writable()
  {
    bool rc = false;
    if (event.events & EPOLLOUT) {
      rc = true;
    }
    return rc;
  }
  
  bool peerClosedOrShutdownWriting()
  {
    bool rc = false;
    if (event.events & EPOLLRDHUP) {
      rc = true;
    }
    return rc;
  }
  
  bool urgentData()
  {
    bool rc = false;
    if (event.events & EPOLLPRI) {
      rc = true;
    }
    return rc;
  }

  bool pollError()
  {
    bool rc = false;
    if (event.events & EPOLLERR) {
      rc = true;
    }
    return rc;
  }

  bool hangup()
  {
    bool rc = false;
    if (event.events & EPOLLHUP) {
      rc = true;
    }
    return rc;
  }

  void edgeTransfer()
  {
    event.events |= EPOLLET;
  }
 
  void oneShot()
  {
    event.events |= EPOLLONESHOT;
  }
 //      EPOLLWAKEUP (since Linux 3.5)
};
}


