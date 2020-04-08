/******************************************************************************
 *
 * Copyright (c) 2019 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  ScrolledListView.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/ApplicationView.h>
#include <es++/gtkmm-3.0/ScrolledListView.h>
#include <es++/gtkmm-3.0/FileOpenDialog.h>
#include <es++/gtkmm-3.0/ScrolledListView.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::ApplicationView {

private:
  Es::ScrolledListView scrolled_listview;
 
  Es::FileOpenDialog   file_dialog;


public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet,
          std::string& title,
          Es::Args& args)
  :Es::ApplicationView(applet, title, args) 
  ,scrolled_listview()
  ,file_dialog(*this, Es::FileOpenDialog::TEXT_FILES)
  {
    Es::MainLayout& main_layout = get_main_layout();
    main_layout.pack_start(scrolled_listview);

    const std::string filename = "./CORED.jpg.csv";

    set_filenamed_title(filename);

    Es::ListView& listv = scrolled_listview.get_list_view();

    const char* columns[] = {
     "id", "object", "score", "x", "y", "w", "h",
    };
    listv.createModel(columns, CountOf(columns));

    main_layout.pack_start(scrolled_listview);

    char line[256];
    for (int i = 0; i<20; i++) {
      sprintf(line, "%d, object_%d, %1.02f, %d, %d, %d, %d",
                 i, i, 1.00-0.1*(float)i, 10+i, 20+2*i, 50+i, 100+2*i);
      listv.addLine(line);
    }

    fflush(stdout);
    show_all();
  }

  void file_open() 
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      try {
        const std::string& filename = file_dialog.get_filename();
        set_filenamed_title(filename);
        //scrolled_textview.loadText(filename);
      } catch (Es::Exception& ex) {
        caught(ex);
      }
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
    args.set(XmNwidth, 500);
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

