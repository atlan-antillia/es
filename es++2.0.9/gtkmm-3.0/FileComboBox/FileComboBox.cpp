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
 *  FileComboBox.cpp
 *
 *****************************************************************************/

//

#include <es++/gtkmm-3.0/ApplicationView.h>
#include <es++/gtkmm-3.0/ScrolledImageView.h>
#include <es++/gtkmm-3.0/FileComboBox.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::ApplicationView {
private:
  Es::FileComboBox      file_combobox;

  Es::ScrolledImageView image_view;
 
  std::string     dir;
  std::string     pattern;


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

    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);

    image_view.set_size_request(w, h);

    main_layout.pack_start(file_combobox, Gtk::PACK_SHRINK);
    main_layout.pack_start(image_view, Gtk::PACK_SHRINK);

    dir     = "../../images";
    pattern = "*.*";

    file_combobox.set_label(dir);
    file_combobox.append(dir, pattern);
    file_combobox.set_changed_callback(
             sigc::mem_fun(*this, &MainView::file_combobox_changed) );

    std::string filename = "NiohLiken.png";
    file_combobox.set_active_text(filename);

    const std::string path = dir + "/" + filename;

    image_view.loadImage(path);
    
    show_all();
  }

  void file_combobox_changed()
  {
    std::string filename = file_combobox.get_active_text();
    std::string path = dir + "/" + filename;

    printf("Selected filepath =%s\n", path.c_str());
    image_view.loadImage(path);
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

