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
 *  EventDescriptor.h
 *
 *****************************************************************************/

//2015/06/01 Modified to use Callback class.

#pragma once

#include <es++/Object.h>
#include <es++/UnsignedInteger64.h>
#include <es++/FileDescriptor.h>
#include <es++/Exception.h>
#include <es++/Callback.h> 
#include <sys/eventfd.h> 

namespace Es {

typedef void (Object::*Notified)(uint64_t);

class EventDescriptor :public FileDescriptor {
private:
  //Callback
  Callback<Object*, Notified, uint64_t> callback;

public:
  EventDescriptor(int fd)
  :FileDescriptor()
  {
    if (fd == INVALID_FD) {
      throw Es::IException("Failed eventfd :%s", ERROR_STRING); // strerror(errno));
    }
    setFD(fd);
  }

public:
  EventDescriptor(unsigned int initval, int flags)
  :FileDescriptor()
  {
    int fd = eventfd(initval, flags);
    if (fd == INVALID_FD) {
      throw Es::IException("Failed eventfd :%s", ERROR_STRING); // strerror(errno));
    }
    setFD(fd);
  }

  void setCallback(Object* object, Notified notified)
  {
    callback.set(object, notified);
  }

  virtual void callCallback(uint64_t& value)
  {
    callback.call(value);
    //UnsignedInteger64 u64(value);
    //printf("EventDescriptor::handle %s\n", u64.toString() );
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
      throw Es::IException("Failed to write: %s", ERROR_STRING)
    }
    callCallback(value);

    return size;
  }
};
}

