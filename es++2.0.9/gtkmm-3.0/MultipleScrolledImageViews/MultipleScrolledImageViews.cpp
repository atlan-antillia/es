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
 *  MultipleScrolledImageView.cpp
 *
 *****************************************************************************/

// Simple example to use multiple File Open menus, 
// and mulitple horizontal scrolledImageViews.
//

#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/ApplicationView.h>
#include <es++/gtkmm-3.0/ScrolledImageView.h>
#include <es++/gtkmm-3.0/FileOpenDialog.h>
#include <es++/gtkmm-3.0/Label.h>
#include <es++/gtkmm-3.0/PushButton.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::ApplicationView {

private:
  Es::Label             filepath;
  Es::HorizontalLayout  horizontal_layout;
  Es::VerticalLayout    control_pane;
  Es::PushButton        quit_button;

  std::string           filename1;
  Es::ScrolledImageView image_view1;

  std::string           filename2;
  Es::ScrolledImageView image_view2;

  Es::FileOpenDialog    file_dialog1;
  Es::FileOpenDialog    file_dialog2;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet,
          std::string& title,
          Es::Args& args)
  :Es::ApplicationView(applet, title, 
                          args.set(XmNuseDefaultFilePulldown, false)
                         ) 
  ,file_dialog1(*this, Es::FileOpenDialog::IMAGE_FILES)
  ,file_dialog2(*this, Es::FileOpenDialog::IMAGE_FILES)
  ,filename1("")
  ,filename2("")
  {
    //Define File pulldown menu with multiple Open File pulldown menu items.
    Es::SigcMenuCallback file_menu_callbacks[] = {
      {"New",        sigc::mem_fun(*this, &MainView::file_new)},
      {"Open File1", sigc::mem_fun(*this, &MainView::file_open1)},
      {"Open File2", sigc::mem_fun(*this, &MainView::file_open2)},
      {"Save",       sigc::mem_fun(*this, &MainView::file_save)},
      {"Save_As",    sigc::mem_fun(*this, &MainView::file_save_as)},
      {"Quit",       sigc::mem_fun(*this, &MainView::file_quit)},
    };
   
    file_pulldown_append( file_menu_callbacks,
                         CountOf(file_menu_callbacks));

    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);
    const int CONTROL_BOX_WIDTH = 180;
    int ww = (w - CONTROL_BOX_WIDTH)/2;
    Es::MainLayout& main_layout = get_main_layout();

    main_layout.pack_start(filepath, FALSE, FALSE, 0);
    main_layout.pack_start(horizontal_layout);

    image_view1.set_size_request(ww, h);
    image_view2.set_size_request(ww, h);
    control_pane.set_spacing(10);
    control_pane.set_size_request(CONTROL_BOX_WIDTH, h);
 
    horizontal_layout.pack_start(image_view1); 
    horizontal_layout.pack_start(image_view2); 
    horizontal_layout.pack_start(control_pane, FALSE, FALSE, 0);

    quit_button.set_label("Quit");
    quit_button.set_clicked_callback(
              sigc::mem_fun(*this, &MainView::file_quit) );

    control_pane.pack_start(quit_button, FALSE, FALSE, 0);

    filename1 = "../../images/Nioh.png";
    filename2 = "../../images/NiohLiken.png";

    set_title(title);

    update_filepath();

    image_view1.loadImage(filename1);
    image_view2.loadImage(filename2);
    
    show_all();
  }

  void update_filepath()
  {
    std::string space = "    ";
    filepath.set_label(filename1 + space +  filename2);
  }

  void file_open1() 
  {
    int rc = file_dialog1.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string& name = file_dialog1.get_filename();
      filename1 = name;
      update_filepath();
      image_view1.loadImage(filename1);
    }
  }

  void file_open2()
  {
    int rc = file_dialog2.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string& name = file_dialog2.get_filename();
      filename2 = name;
      update_filepath();
      image_view2.loadImage(filename2);
    }
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
    args.set(XmNwidth, 900);
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

