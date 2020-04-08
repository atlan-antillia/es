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
 *  OpenCVMultipleScrolledImageView.cpp
 *
 *****************************************************************************/

//
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/FileOpenDialog.h>
#include <es++/gtkmm-3.0/PushButton.h>
#include <es++/opencv-3.0/OpenCVMainView.h>
#include <es++/opencv-3.0/OpenCVScaleComboBox.h>
#include <es++/opencv-3.0/OpenCVScrolledImageView.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenCVMainView {

private:
  Gtk::Label              filepath;
  Es::HorizontalLayout    horiz_layout;
  Es::VerticalLayout      control_pane;
  Es::OpenCVScaleComboBox combobox;

  Es::PushButton          quit_button;

  Es::OpenCVScrolledImageView image_view1;
  Es::OpenCVScrolledImageView image_view2;
 
  static const int      FILEPATH_HEIGHT   = 30; 
  static const int      CONTROLPANE_WIDTH = 160;

  int                   scale;
  std::string           filename1;
  std::string           filename2;

  static const int      loadingFlag = IMREAD_COLOR; 

  Es::FileOpenDialog    file_dialog1;

  Es::FileOpenDialog    file_dialog2;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet,
          std::string& title,
          Es::Args& args)
  :OpenCVMainView(applet, title, 
                     //We don't use the defaultFilePulldown menu.
                     args.set(XmNuseDefaultFilePulldown, false) )
  ,scale(100)
  ,file_dialog1(*this, Es::FileOpenDialog::IMAGE_FILES)
  ,file_dialog2(*this, Es::FileOpenDialog::IMAGE_FILES)
  {
   //Define File pulldown menu with multiple [Open File] pulldown menu items.
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
    int ww = (w - CONTROLPANE_WIDTH)/2;

    Es::MainLayout& main_layout = get_main_layout();

    filepath.set_size_request(w, FILEPATH_HEIGHT);

    filepath.set_alignment(Gtk::ALIGN_START);

    main_layout.pack_start(filepath, FALSE,FALSE, 0);
    main_layout.pack_start(horiz_layout, FALSE, FALSE, 0);

    image_view1.set_size_request(ww, h);
    image_view2.set_size_request(ww, h);
    control_pane.set_spacing(10);
    control_pane.set_size_request(CONTROLPANE_WIDTH, h);
 
    horiz_layout.pack_start(image_view1); 
    horiz_layout.pack_start(image_view2); 
    horiz_layout.pack_start(control_pane, FALSE, FALSE, 0);

    scale = (int)args.get(XmNscalingRatio);

    combobox.set_selection(scale);
    combobox.set_changed_callback(
              sigc::mem_fun(*this, &MainView::scale_changed)
              );

    quit_button.set_label("Quit");
    quit_button.signal_clicked().connect( 
              sigc::mem_fun(*this, &MainView::file_quit) 
               );

    control_pane.pack_start(combobox, FALSE, FALSE, 0);
    control_pane.pack_start(quit_button, FALSE, FALSE, 0);

    filename1 = "../../images/Tower1.png";
    filename2 = "../../images/Tower2.png";

    image_view1.loadImage(filename1, loadingFlag, scale);
    //image_view1.scaleImage(scale);

    image_view2.loadImage(filename2, loadingFlag, scale);
    //image_view2.scaleImage(scale);    

    update_filepath();
    show_all();
  }

  bool on_configure_event(GdkEventConfigure*  configure_event)
  {
    Window::on_configure_event(configure_event);
    //Do nothing here.
    return true;
  }

  void scale_changed()
  {
    scale = combobox.get_selection();
    printf("scale_changed %d\n", scale);
    image_view1.scaleImage(scale);
    image_view2.scaleImage(scale);
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
      filename1 = file_dialog1.get_filename();
      update_filepath();
      image_view1.loadImage(filename1, loadingFlag, scale);
    }
  }

  void file_open2()
  {
    int rc = file_dialog2.popup();

    if (rc == Gtk::RESPONSE_OK) {
      filename2 = file_dialog2.get_filename();
      update_filepath();
      image_view2.loadImage(filename2, loadingFlag, scale);
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
    args.set(XmNscalingRatio,60);
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

