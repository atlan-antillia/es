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
 *  ApplicationView.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-3.0/ApplicationView.h>
#include <es++/gtkmm-3.0/ImageView.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::ApplicationView {
private:
  Gtk::ScrolledWindow scrolled_window;

  Es::ImageView image_view;
 
public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet,
          std::string& title,
          Es::Args& args)
  :ApplicationView(applet, title, args)
  {
    Es::MainLayout& main_layout = get_main_layout();

    scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC,
                              Gtk::POLICY_AUTOMATIC);

    main_layout.pack_start(scrolled_window);

    scrolled_window.add(image_view);

    const std::string filename = "../../images/NiohLiken.png";

    image_view.loadImage(filename);
    
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
    args.set(XmNx, 10);
    args.set(XmNy, 10);
    args.set(XmNwidth, 640);
    args.set(XmNheight,480);
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

