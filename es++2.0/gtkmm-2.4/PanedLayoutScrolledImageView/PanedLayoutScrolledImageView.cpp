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
 *  PanedLayoutScrolledImageView.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-2.4/Application.h>
#include <es++/gtkmm-2.4/ApplicationView.h>
#include <es++/gtkmm-2.4/ScrolledImageView.h>
#include <es++/gtkmm-2.4/FileOpenDialog.h>
#include <gtkmm/paned.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::ApplicationView {

private:
  Gtk::Paned             hpanedw;
  Es::ScrolledImageView  image_view1;
  Es::ScrolledImageView  image_view2;
  std::string            filename1;
  std::string            filename2;
 
  Es::FileOpenDialog     file_dialog1;
  Es::FileOpenDialog     file_dialog2;

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
    Es::MainLayout& main_layout = get_main_layout();
    main_layout.add(hpanedw);
   
    hpanedw.add1(image_view1); 
    hpanedw.add2(image_view2); 

    hpanedw.set_position(w/2);

    filename1 = "../../images/Tower1.png";
    filename2 = "../../images/Tower2.png";

    image_view1.loadImage(filename1);

    image_view2.loadImage(filename2);
    
    show_all();
  }

  void file_open1() 
  {
    int rc = file_dialog1.popup();

    if (rc == Gtk::RESPONSE_OK) {
      filename1 = file_dialog1.get_filename();
      image_view1.loadImage(filename1);
    }
  }

  void file_open2()
  {
    int rc = file_dialog2.popup();

    if (rc == Gtk::RESPONSE_OK) {
      filename2 = file_dialog2.get_filename();
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

