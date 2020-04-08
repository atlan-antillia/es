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
 *  EventPoller.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/Thread.h>
#include <es++/Exception.h>
#include <es++/EventPollerEvent.h> //2015/06/02
#include <es++/EventPollerEvents.h> //2015/06/01

namespace Es {

class EventPoller :public Thread {

private:
  static const int DEFAULT_EPOLL_SIZE = 128;

private:
  int epollFD;  

public:
  EventPoller(int size = DEFAULT_EPOLL_SIZE) 
  :epollFD(INVALID_FD)
  {
    if (size < 1) {
      throw Es::IException("Invalid argument %d", size);
    }
    epollFD = epoll_create(size);
    if (epollFD < 0) {
      throw Es::IException("Failed to epoll_create :%s", strerror(errno));
    }
  }

  ~EventPoller()
  {
    close();
  }

  void close()
  { 
    if (epollFD != INVALID_FD) {
      ::close(epollFD);
      epollFD = INVALID_FD;
    }
  }

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

  int add(int fd, EventPollerEvent& epollEvent)
  {
    struct epoll_event& event = epollEvent.getEvent();
    int rc =  epoll_ctl (epollFD, EPOLL_CTL_ADD, fd, &event);
    if (rc < 0) {
      throw Es::IException("Failed to epoll_ctl: %s", strerror(errno));
    }
    return rc;
  }


  int add(int fd, struct epoll_event* event)
  {
    int rc =  epoll_ctl (epollFD, EPOLL_CTL_ADD, fd, event);
    if (rc < 0) {
      throw Es::IException("Failed to epoll_ctl: %s", strerror(errno));
    }
    return rc;
  }

  int remove(int fd, struct epoll_event* event)
  {
    int rc =  epoll_ctl (epollFD, EPOLL_CTL_DEL, fd, event);
    if (rc < 0) {
      throw Es::IException("Failed to epoll_ctl: %s", strerror(errno));
    }
    return rc;
  }

  int modify(int fd, struct epoll_event* event)
  {
    int rc =  epoll_ctl (epollFD, EPOLL_CTL_MOD, fd, event);
    if (rc < 0) {
      throw Es::IException("Failed to epoll_ctl: %s", strerror(errno));
    }
    return rc;
  }

  int wait(EventPollerEvents& pollerEvents, int timeoutMilliSec = -1)
  {
    int num =  epoll_wait (epollFD,
                pollerEvents.getEvents(),
                pollerEvents.getMaxEvents(),
                timeoutMilliSec);
    pollerEvents.setNumberOfEvents(num);
    return num;
  }

  void wait()
  {
    Thread::wait();
  }

  //Thread main procedure
  virtual void run()
  {
    //Define your own run method in your subclass derived from this class.
  }
};
}

