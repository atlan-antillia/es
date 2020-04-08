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
 *  FolderOpenDialog.cpp
 *
 *****************************************************************************/

// Simple example to use 
//  Es::FolderOpenDialog, 
//  Es::FileComboBox, 
//  Es::ScrolledImageView

#include <es++/gtkmm-2.4/Application.h>
#include <es++/gtkmm-2.4/ApplicationView.h>
#include <es++/gtkmm-2.4/HorizontalLayout.h>
#include <es++/gtkmm-2.4/VerticalLayout.h>
#include <es++/gtkmm-2.4/ScrolledImageView.h>
#include <es++/gtkmm-2.4/FolderOpenDialog.h>
#include <es++/gtkmm-2.4/FileComboBox.h>
#include <es++/gtkmm-2.4/FileOpenDialog.h>
#include <es++/gtkmm-2.4/PushButton.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::ApplicationView {
private:
  Es::HorizontalLayout  top_work_area;
    Es::FileComboBox       file_combobox;
    Es::PushButton         folder_open_button;

  Es::ScrolledImageView  image_view;

  std::string            directory;
  std::string            pattern;

  static const int       CONTROLPANE_WIDTH = 200;

  Es::FolderOpenDialog   folder_dialog;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet, 
          std::string&      name,
          Es::Args&         args)
  :ApplicationView(applet, name, args)
  ,folder_dialog(*this)
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);

    Es::MainLayout& main_layout = get_main_layout();
    
    main_layout.pack_start(top_work_area, FALSE, FALSE, 0);
    main_layout.pack_start(image_view);


    image_view.set_size_request( w, h);
    
    directory  = "../../images/";
    pattern    = "*.*";

    const std::string filename = "MultipleTexturedCube.png";
    const std::string filepath = directory + filename;

    set_filenamed_title(filename);
    image_view.loadImage(filepath);

    file_combobox.set_label(directory);
    file_combobox.append(directory, pattern);
    file_combobox.set_active_text(filename);
    file_combobox.set_changed_callback(
             sigc::mem_fun(*this, &MainView::file_combobox_changed) );

    folder_open_button.set_label("Open folder");
    folder_open_button.set_clicked_callback(
             sigc::mem_fun(*this, &MainView::folder_open_button_clicked) );

    top_work_area.pack_start(file_combobox); 
    top_work_area.pack_start(folder_open_button, Gtk::PACK_SHRINK); 
   
    show_all();
  }

  void file_combobox_changed()
  {
    try {
      std::string filename = file_combobox.get_active_text();
      std::string path = directory + "/" + filename;

      printf("Selected filepath =%s\n", path.c_str());
      image_view.loadImage(path);

    } catch (Es::Exception& ex) {
     ; //Do nothing here. 
    }
  }

  void folder_open_button_clicked()
  {
    try {
      int rc = folder_dialog.popup();

      if (rc == Gtk::RESPONSE_OK) {
        directory = folder_dialog.get_filename();
        file_combobox.set_label(directory);

        //The following line will cause file_combobox_changed callback.
        file_combobox.remove_all(); 

        file_combobox.append(directory, pattern);
      }

    } catch (...) {
     ; //Do nothing here.
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

