/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  OpenGLApplet.h
 *
 *****************************************************************************/

#pragma once

//#include <gtkmm/application.h>

#include <es++/opengl/IOpenGLMainView.h>
#include <gdk/gdkx.h>
#include <es++/gtkmm-3.0/Application.h>


namespace Es {

class OpenGLApplet : public Es::Application {
private:
  Display*  display;
  Window    root_window;
  int       delay;  //Milliseconds : For example 20

public:
  OpenGLApplet(int argc, char** argv, 
             const char* domain="com.antillia.freeware")
  :Application(argc, argv, domain)
  ,display(NULL)
  ,root_window(None)
  ,delay(40)
  {
    display = gdk_x11_get_default_xdisplay();
    if (display) {
      printf("Found default xdisplay\n");
    } else {
      printf("Not Found\n");
    }
    root_window = gdk_x11_get_default_root_xwindow();
    printf("Default root_window %lu\n", root_window);
  }

  ~OpenGLApplet()
  {
  }

  void idle()
  {
     usleep(delay*1000);
  }
/*
  virtual void run(Es::IOpenGLMainView& view)
  {
  }

    while (true) {

       while (gtk_events_pending ()) {
       gtk_main_iteration (); 
       
      }
       idle();
       view.display();
    }
  }
*/
  Display* getDisplay()
  {
    return display;
  }
};

}

