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
 *  ScrolledTextListView.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-2.4/Application.h>
#include <es++/gtkmm-2.4/ApplicationView.h>
#include <es++/gtkmm-2.4/ScrolledTextListView.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::ApplicationView {

private:
  Es::ScrolledTextListView scrolled_listview;
 
  static const int COLUMN_COUNT = 2;
  static const int MONTH_COUNT  = 12;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet,
          std::string& name,
          Es::Args& args)
  :Es::ApplicationView(applet, name, args) 
  ,scrolled_listview(COLUMN_COUNT)
  {
    Es::MainLayout& main_layout = get_main_layout();

    main_layout.pack_start(scrolled_listview);

    const char* title[] = {"No", "Month" };

    const char* data[MONTH_COUNT][COLUMN_COUNT] = {
     {"1", "January" }, 
     {"2", "Feburay" }, 
     {"3", "March" }, 
     {"4", "April" }, 
     {"5", "May" }, 
     {"6", "Jun" }, 
     {"7", "July" }, 
     {"8", "August" }, 
     {"9", "September" }, 
     {"10", "October" }, 
     {"11", "November" }, 
     {"12", "December" }, 
    };

    Es::TextListView& listview = scrolled_listview.get_text_listview();

    listview.set_column_title(title, CountOf(title));

    for (int i = 0; i< MONTH_COUNT; i++) {
      listview.append_text(data[i], COLUMN_COUNT);
    }

    listview.signal_row_activated().connect(
          sigc::mem_fun(*this, &MainView::listview_row_activated) );

    show_all();
  }

  void listview_row_activated(const TreeModel::Path& path,
                               TreeViewColumn* column)
  {
    int row = scrolled_listview.get_selected_row();

    Glib::ustring col1 = scrolled_listview.get_text(row, 0);
    Glib::ustring col2 = scrolled_listview.get_text(row, 1);
    printf("listview_row_activate row=%d  %s %s\n", 
               row, col1.c_str(), col2.c_str());
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
    args.set(XmNheight,280);
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

