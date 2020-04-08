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
 *  FixedLayoutImageView.cpp
 *
 *****************************************************************************/

// Simple example to use Gtk::Fixed container and Es::ScrolledImageView

#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/ApplicationView.h>
#include <es++/gtkmm-3.0/ScrolledImageView.h>
#include <es++/gtkmm-3.0/FileOpenDialog.h>
#include <gtkmm/fixed.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::ApplicationView {
private:
  //Container
  Gtk::Fixed    fixed;

  Gtk::Label    label;

  Es::ScrolledImageView image_view;

  Es::FileOpenDialog file_dialog;

  static const int IMAGEVIEW_X = 100;

  static const int IMAGEVIEW_Y = 40;
 
public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet, 
          std::string&      name,
          Es::Args&         args)
  :ApplicationView(applet, name, args)
  ,file_dialog(*this, Es::FileOpenDialog::IMAGE_FILES)
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);

    Es::MainLayout& main_layout = get_main_layout();
   
    main_layout.add(fixed);

    //label.set_alignment(Gtk::ALIGNMENT_LEFT);
    label.set_size_request(w, IMAGEVIEW_Y-1);
 
    fixed.put(label,  0, 0);
    fixed.put(image_view, IMAGEVIEW_X, IMAGEVIEW_Y);

    const std::string filename = "../../images/flower.png";
    label.set_text(filename);

    image_view.loadImage(filename);
    image_view.set_size_request(w- IMAGEVIEW_X, h- IMAGEVIEW_Y);
    show_all();
  }

/*
struct GdkEventConfigure {
  GdkEventType type;
  GdkWindow *window;
  gint8 send_event;
  gint x, y;
  gint width;
  gint height;
};
 */
  bool on_configure_event(GdkEventConfigure*  configure_event)
  {
    Window::on_configure_event(configure_event);
    //Do nothing here.

    return true;
  }

  void file_open()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string& filename = file_dialog.get_filename();

      label.set_text(filename);

      image_view.loadImage(filename);
    }
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
    args.set(XmNwidth, 640);
    args.set(XmNheight, 400);
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

