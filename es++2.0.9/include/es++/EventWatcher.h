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
 *  EventWatcher.h
 *
 *****************************************************************************/

#include <es++/EventDescriptor.h>
#include <es++/Thread.h>
#include <es++/Callback.h>

namespace Es {

class EventWatcher: public Thread {

private:
  bool looping;  
  EventDescriptor eventDescriptor;
  
  //Notified callback 2015/06/10
  Callback<Object, Notified, uint64_t> callback;

public:
  EventWatcher(unsigned int initval = 0, int flags = 0)
  :Thread(),
  looping(true),
  eventDescriptor(initval, flags)
  {
  }

public:
  
  void setCallback(Object* object, Notified notified)
  {
    callback.set(object, notified);
  }

  void callCallback(uint64_t value)
  {
    callback.call(value);
  }

  virtual void run()
  {
    while (looping) {
      uint64_t value;
      eventDescriptor.read(value);
      callCallback(value);
    }
  }

  void stop()
  {
    looping = false;
    eventDescriptor.close();
  }
};
}

