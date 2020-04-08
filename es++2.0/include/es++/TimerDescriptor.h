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
 *  TimerDescriptor.h
 *
 *****************************************************************************/

//2015/06/01 Modified to use Callback class.

#pragma once

#include <es++/Object.h>
#include <es++/FileDescriptor.h>
#include <es++/UnsignedInteger64.h>
#include <es++/ITimerSpec.h>
#include <es++/Callback.h>
#include <es++/Exception.h>
#include <sys/timerfd.h> 
#include <sys/time.h> 

namespace Es {

//Elapsed callback
typedef void (Object::*Elapsed)(uint64_t value);

class TimerDescriptor :public FileDescriptor {
private:
  //Callback which can be called from read method.
  Callback<Object*, Elapsed, uint64_t> callback;

public:
  TimerDescriptor(int clockid, int flags)
  :FileDescriptor()
  {
    int fd = timerfd_create(clockid, flags);
    if (fd == INVALID_FD) {
      throw Es::IException("Failed timerfd_create :%s", ERROR_STRING);
    }
    setFD(fd);
  }

  int setTime(int flags, ITimerSpec& new_value)
  {
    struct itimerspec its = new_value.get();
    return timerfd_settime(getFD(), flags, &its, NULL);
  }

  int setTime(int flags, const struct itimerspec& new_value) 
  {
    return timerfd_settime(getFD(), flags, &new_value, NULL);
  }

  int setTime(int flags, const struct itimerspec& new_value, 
              struct itimerspec& curr_value)
  {
    return timerfd_settime(getFD(), flags, &new_value, &curr_value);
  }

  int getTime(struct itimerspec& curr_value)
  {
    return timerfd_gettime(getFD(), &curr_value);
  }

  void setCallback(Object* object, Elapsed elapsed)
  {
    callback.set(object, elapsed);
  }

  void callCallback(uint64_t& value)
  {
    callback.call(value);
  }

  ssize_t read(uint64_t& value)
  {
    ssize_t size = FileDescriptor::read((void*)&value, sizeof(value));
    if (size != sizeof(value)) {
      throw Es::IException("Failed to read: %s", ERROR_STRING);
    }
    callCallback(value);
    return size;
  }

  ssize_t write(uint64_t& value)
  {
    ssize_t size = FileDescriptor::write((void*)&value, sizeof(value));
    if (size != sizeof(value)) {
      throw Es::IException("Failed to write: %s", ERROR_STRING);
    }
    return size;
  }
};
}

