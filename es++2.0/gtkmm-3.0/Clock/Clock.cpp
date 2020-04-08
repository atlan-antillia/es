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
 *  Clock.cpp
 *
 *****************************************************************************/

//This is a simple timeout callback to display a current time on
// TextDrawingArea.

#include <gtkmm/drawingarea.h>
#include <es++/gtkmm-3.0/ApplicationView.h>
#include <es++/gtkmm-3.0/HorizontalLayout.h>
#include <es++/gtkmm-3.0/VerticalLayout.h>
#include <es++/gtkmm-3.0/TextDrawingArea.h>
#include <es++/LocalDateTime.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::ApplicationView {
  /////////////////////////////////////////////
  //Inner class starts.
  class Clock: public Es::TextDrawingArea {
  public:
    Clock()
    {
      set_font("Times New Roman ", 
              Cairo::FONT_SLANT_OBLIQUE,
              Cairo::FONT_WEIGHT_NORMAL);
 
       Glib::signal_timeout().connect(sigc::mem_fun(*this, 
                                      &Clock::on_timeout), 1000); //1000msec
    }


    bool on_timeout()
    {
      Glib::RefPtr<Gdk::Window> window = get_window();
      if (window) {
        Es::LocalDateTime datetime;
        CharString now = datetime.nowToSeconds();
        set_text((const char*)now,
              32.0f, RGB(0.0f, 0.0f, 1.0f));

        Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                get_allocation().get_height());
        window->invalidate_rect(r, false);
      }
      return true;
    }

  };
  /////////////////////////////////////////////


private:
  Es::HorizontalLayout center_frame;
    Clock   clock;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet,
          std::string& title,
          Es::Args& args)
  :Es::ApplicationView(applet, title, args) 
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);

    Es::MainLayout& main_layout = get_main_layout();

    center_frame.set_size_request(w, h);

    main_layout.pack_start(center_frame);

    center_frame.pack_start(clock);

    show_all();
  }
};
}


int main(int argc, char** argv)
{
  Es::Environment env;
  try {
    Es::Application applet(argc, argv);

    std::string title(argv[0]);

    Es::Args args;
    args.set(XmNx, 20);
    args.set(XmNy, 20);
    args.set(XmNwidth, 400);
    args.set(XmNheight,110);
    Es::MainView mainv(applet, title, args);

    mainv.show();

    applet.run(mainv);

  } catch (Es::Exception& ex) {
    caught(ex);
  } catch (...) {
    printf("Get exception \n");
  }
  return 0;
}

