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
 *  PushButton.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/ApplicationView.h>
#include <es++/gtkmm-3.0/HorizontalLayout.h>
#include <es++/gtkmm-3.0/VerticalLayout.h>
#include <es++/gtkmm-3.0/Label.h>
#include <es++/gtkmm-3.0/PushButton.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::ApplicationView {

private:
  static const int       LABEL_MAX = 10;

  Es::HorizontalLayout center_frame;
    Es::VerticalLayout   label_pane;
      Es::Label          label[LABEL_MAX];
    Es::VerticalLayout   control_pane;
      Es::PushButton     quit_button;

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
    const int CONTROLPANE_WIDTH = 180;
    int ww = (w - CONTROLPANE_WIDTH)/2;
    Es::MainLayout& layout = get_main_layout();

    center_frame.set_size_request(w, h);

    layout.pack_start(center_frame);

    label_pane.set_size_request(w - CONTROLPANE_WIDTH, h);
    control_pane.set_size_request(CONTROLPANE_WIDTH, h);

    center_frame.pack_start(label_pane);
    center_frame.pack_start(control_pane, FALSE, FALSE);
 
    label_pane.set_spacing(10);

    int n = 0; 
    label[n].set_label(std::string("Es++ 1.0"));
    label_pane.pack_start(label[n++], Gtk::PACK_SHRINK);

    label[n].set_number("gtkmm3-", 0);
    label_pane.pack_start(label[n++], Gtk::PACK_SHRINK);

    label[n].set_number("opencv- ", 2.4);
    label_pane.pack_start(label[n++], Gtk::PACK_SHRINK);

    for (int i = n; i <LABEL_MAX; i++) {
      char text[128];
      sprintf(text, "Hello world. こんにちは世界 %d", i);

      label[i].set_label(std::string(text));
      label_pane.pack_start(label[i], Gtk::PACK_SHRINK);
    }

    quit_button.set_label("Quit");
    quit_button.set_clicked_callback( 
              sigc::mem_fun(*this, &MainView::file_quit)  );

    control_pane.pack_start(quit_button, FALSE, FALSE, 0);

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
    args.set(XmNheight,400);
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

