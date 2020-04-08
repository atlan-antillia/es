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
 *  OpenCVObject.h
 *
 *****************************************************************************/

#pragma once

#ifdef GTKMM3
#include <gtkmm/application.h>
#endif
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm.h>
#include <es++/Object.h>
#include <es++/gtkmm-2.4/Environment.h>

namespace Es {

class Application : public Es::Object {
private:
#ifdef GTKMM3
  Glib::RefPtr<Gtk::Application> application;
#else
  //gtkmm 2.4
  Gtk::Main main;
#endif

public:
  Application(int argc, char** argv, 
             const char* domain="com.antillia.freeware")
#ifndef GTKMM3
  :main(argc, argv)
#endif
  {
#ifdef GTKMM3
    setenv("UBUNTU_MENUPROXY", "0", 1);
    application =
             Gtk::Application::create(argc, argv,
                 domain);
#else
#endif

  }

  ~Application()
  {
  }

  void run(Gtk::Window& window)
  {
#ifdef GTKMM3
    application->run(window);
#else 
    //Gtk::Main.run(window);
    main.run(window);
#endif
  }

  void quit()
  {
#ifdef GTKMM3
    application->quit(); 
#else
    //Gtk::Main.quit();
    main.quit();
#endif
  }
};

}

