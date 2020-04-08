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
 *  LabeledVideoDeviceComboBox.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-3.0/ApplicationView.h>
#include <es++/gtkmm-3.0/HorizontalLayout.h>
#include <es++/gtkmm-3.0/VerticalLayout.h>
#include <es++/gtkmm-3.0/ScrolledImageView.h>
#include <es++/gtkmm-3.0/FileOpenDialog.h>
#include <es++/gtkmm-3.0/LabeledVideoDeviceComboBox.h>
#include <es++/gtkmm-3.0/LabeledTrackBar.h>
#include <es++/gtkmm-3.0/PushButton.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::ApplicationView {

private:
  Gtk::Label            filepath;
  Es::HorizontalLayout  hbox;
  Es::VerticalLayout    vbox;
  Es::LabeledVideoDeviceComboBox   combobox;
  Es::LabeledTrackBar   trackbar;
  Es::PushButton        quit_button;
  std::string           filename;

  Es::ScrolledImageView image_view;

 
public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet,
          std::string& title,
          Es::Args& args)
  :Es::ApplicationView(applet, title, args) 
  ,trackbar(20.0f, 10.0f, 100.0f)
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);
    const int CONTROL_BOX_WIDTH = 240;
    int ww = (w - CONTROL_BOX_WIDTH);

    Es::MainLayout& main_layout = get_main_layout();

    filepath.set_alignment(Gtk::ALIGN_START);

    main_layout.pack_start(filepath, Gtk::PACK_SHRINK);
    main_layout.pack_start(hbox);

    image_view.set_size_request(ww, h);
    vbox.set_spacing(10);
    vbox.set_size_request(CONTROL_BOX_WIDTH, h);
 
    //hbox.pack_start(image_view, FALSE, FALSE, 0); 
    hbox.pack_start(image_view); 
    //hbox.pack_start(vbox, FALSE, FALSE, 0);
    hbox.pack_start(vbox, Gtk::PACK_SHRINK);

    std::string labell ="TrackBar:[10, 100]";
    trackbar.set_label(labell);
    trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::trackbar_value_changed) );

    //vbox.pack_start(trackbar);
    vbox.pack_start(trackbar, FALSE, FALSE, 0);

    combobox.set_changed_callback(
              sigc::mem_fun(*this, &MainView::combobox_changed)  );
    vbox.pack_start(combobox );

    quit_button.set_label("Quit");
    quit_button.set_clicked_callback( 
              sigc::mem_fun(*this, &MainView::file_quit)  );

    vbox.pack_start(quit_button, FALSE, FALSE, 0);

    filename = "../../images/MaterializedToruses.png";

    set_title(title);
    filepath.set_text(filename);

    image_view.loadImage(filename);
    
    show_all_children();
  }

  void trackbar_value_changed()
  {
    printf("trackbar_value_changed: %lf\n",
       trackbar.get_value());
  }

  void combobox_changed()
  {
    Glib::ustring text = combobox.get_active_text();
    printf("combobox_changed: %s\n", text.c_str());
    
  }
/*
  void file_open() 
  {
    Es::FileOpenDialog dialog(*this, 
               "Image file selector",
               FileOpenDialog::IMAGE_FILES);
    int rc = dialog.run();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string filename = dialog.get_filename();
      set_filenamed_title(filename);
      image_view.loadImage(filename);
    }
  }
  */
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
    args.set(XmNwidth, 640);
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

