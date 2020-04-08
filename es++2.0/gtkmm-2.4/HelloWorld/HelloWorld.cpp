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
 *  HelloWorld.cpp
 *
 *****************************************************************************/

//
// See:
// https://www.cairographics.org/documentation/cairomm/reference/classCairo_1_1ToyFontFace.html
//

#include <gtkmm/drawingarea.h>
#include <cairomm/cairomm.h>

#include <es++/gtkmm-2.4/ApplicationView.h>
#include <es++/gtkmm-2.4/HorizontalLayout.h>
#include <es++/gtkmm-2.4/VerticalLayout.h>
#include <es++/gtkmm-2.4/TextDrawingArea.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::ApplicationView {
private:
  Es::VerticalLayout   center_frame;
  Es::TextDrawingArea  text_area;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet,
          std::string& title,
          Es::Args& args)
  :Es::ApplicationView(applet, title, args) 
  {
    text_area.set_font("Times New Roman ", 
              Cairo::FONT_SLANT_OBLIQUE,
              Cairo::FONT_WEIGHT_NORMAL);

    text_area.set_text("Hello world.", 
              64.0f, RGB(0.0f, 0.0f, 1.0f) );

    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);

    Es::MainLayout& main_layout = get_main_layout();
    main_layout.set_spacing(0);

    main_layout.pack_start(center_frame);

    center_frame.set_size_request(w, h);

    center_frame.pack_start(text_area);
    
    text_area.set_size_request(w, h);

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
    args.set(XmNwidth, 600);
    args.set(XmNheight,300);
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

