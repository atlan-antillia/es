/******************************************************************************
 *
 * Copyright (c) 2017  TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
 *  TableLayoutScrolledImageView.cpp 
 *
 *****************************************************************************/

// Simple toplevel application view with a menubar, a table-layout, and
// scrolled image views.

#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/ApplicationView.h>
#include <es++/gtkmm-3.0/ScrolledImageView.h>

namespace Es {

class MainView : public Es::ApplicationView {
private:
  static const int IMAGE_COUNT = 4;

  Gtk::Table      table_layout;
    Es::ScrolledImageView image_view[IMAGE_COUNT];

public:
  MainView(Es::Application& applet,
           std::string& name,
           Es::Args& args)
  :ApplicationView(applet, name, args)
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);

    Es::MainLayout& main_layout = get_main_layout();
    
    main_layout.pack_start(table_layout);
    table_layout.resize(2, 2);
    int i = 0;
    table_layout.attach(image_view[i++], 0, 1, 0,1);
    table_layout.attach(image_view[i++], 1, 2, 0,1);
 
    table_layout.attach(image_view[i++], 0, 1, 1,2);
    table_layout.attach(image_view[i++], 1, 2, 1,2);
    static const char* image_files[] = {
      "flower.png", "flower2.jpg",
      "flower3.jpg", "flower4.jpg"
    };
    int ww = w/2;
    int hh = h/2;

    for (int i = 0; i<IMAGE_COUNT; i++) {
      std::string filepath = "../../images/";
                  filepath +=  image_files[i];
      image_view[i].loadImage(filepath);
      image_view[i].set_size_request(ww, hh);
    }

    show_all();
  }
};

}

int main(int argc, char** argv)
{
  Es::Environment env;
  try {
    std::string name = argv[0];

    Es::Application applet(argc, argv);

    Es::Args args;
    args.set(XmNx, 20);
    args.set(XmNy, 40);
    args.set(XmNwidth, 900);
    args.set(XmNheight,480);
    Es::MainView mainv(applet, name, args);

    mainv.show();

    applet.run(mainv);
  } catch (Es::Exception& ex) {
    caught(ex);

  } catch (...) {
    printf("Get exception \n");
  }

  return 0;
}

