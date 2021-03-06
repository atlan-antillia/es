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
 *  Application.h
 *
 *****************************************************************************/

#pragma once

//#include <gtkmm/application.h>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm.h>
#include <es++/Object.h>
#include <es++/gtkmm-3.0/Environment.h>
#include <es++/gtkmm-3.0/MainWindow.h>

namespace Es {

class Application : public Es::Object {
private:
  Glib::RefPtr<Gtk::Application> application;

public:
  Application(int argc, char** argv, 
             const char* domain="com.antillia.freeware")
  {
    application =
             Gtk::Application::create(argc, argv,
                 domain);
  }

  ~Application()
  {
  }

  //2017/10/30
  virtual void run(Gtk::Window& window)
  {
    application->run(window);
  }

  virtual void quit()
  {
    application->quit(); 
  }

};

}

