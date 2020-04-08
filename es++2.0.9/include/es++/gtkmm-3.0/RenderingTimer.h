/******************************************************************************
 *
 * Copyright (c) 2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      RenderingTimer.h
 *
 *****************************************************************************/


// This thread sends the Expose event to the view periodically
// specified by interal(msec).
 
#pragma once

#include <es++/Thread.h>
#include <gtkmm/window.h>

namespace ES {

const char* XmNrenderingInterval = "renderingInterval";

class RenderingTimer :public Thread {
private:
  bool          looping;
  Gtk::Window&  view;
  int           interval; //msec
  static const int DEFAULT_INTERVAL = 100; //100msec 

public:
  RenderingTimer(Gtk::Window& view, int msec)
  :looping(true),
   view(view),
   interval(msec)
  {
    if (interval <10 && interval > 1000) {
      interval = DEFAULT_INTERVAL;
    }
  }
 
public:
  ~RenderingTimer()
  {
    terminate();
  }

public:
  virtual void terminate()
  {
    looping = false;
    usleep(interval*1000);
  }
  
public:
  virtual void run()
  {
     int microsec = interval*1000;

     while(looping) {
       //Sleep for microsec.
       usleep(microsec);
       //Send Expose event to the view.
       view.queue_draw();
       //view.flush();
     }
  }

  int getInterval()
  {
    return interval;
  }
};

}

