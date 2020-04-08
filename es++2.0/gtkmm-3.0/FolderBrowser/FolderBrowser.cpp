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
 *  FolderBrowser.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/ApplicationView.h>
#include <es++/gtkmm-3.0/TextField.h>
#include <es++/gtkmm-3.0/ScrolledFileListView.h>
#include <es++/PathnameParser.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::ApplicationView {

private:
  Es::TextField   filepath;

  Es::ScrolledFileListView scrolled_listview;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet,
          std::string& name,
          Es::Args& args)
  :Es::ApplicationView(applet, name, args) 
  ,scrolled_listview()
  {
    Es::MainLayout& main_layout = get_main_layout();

    const char* dir     = (const char*)args.get(XmNdirectory);
    const char* pattern = (const char*)args.get(XmNpattern);
    std::string path = dir;
                path +="/";
                path += pattern;
    filepath.set_text(path);
    filepath.set_activate_callback(
           sigc::mem_fun(*this, &MainView::filepath_changed) );

    main_layout.pack_start(filepath, FALSE, FALSE, 0);
    main_layout.pack_start(scrolled_listview);

    scrolled_listview.listup_folders(dir, pattern);
    scrolled_listview.listup_files(dir, pattern);
    scrolled_listview.get_treeview()
                     .signal_row_activated().connect(
          sigc::mem_fun(*this, &MainView::listview_row_activated) );

    show_all();
  }

  void filepath_changed()
  {
    Glib::ustring path = filepath.get_text();
    PathnameParser parser(path.c_str());
    printf("Path=%s\n", path.c_str());
    const char* dir = parser.getPath();
    const char* pattern = parser.getFileName();
    printf("dirname=%s, basename=%s\n", dir, pattern);

    scrolled_listview.clear();
    scrolled_listview.listup_folders(dir, pattern);
    scrolled_listview.listup_files(dir, pattern);
  }
 
  void listview_row_activated(const TreeModel::Path& path,
                               TreeViewColumn* column)
  {
    Glib::RefPtr<Gtk::TreeSelection> selection =
                         scrolled_listview.get_treeview()
                                          .get_selection();
    Gtk::TreeModel::iterator iter = selection->get_selected();
    if(iter) {
      Gtk::TreeModel::Row row = *iter;
      scrolled_listview.display(row);
      std::string filename = scrolled_listview.get_filename(row);
      printf("Selected %s\n", filename.c_str());
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

    const char* home = getenv("HOME");
    Es::Args args;
    args.set(XmNx, 20);
    args.set(XmNy, 20);
    args.set(XmNwidth, 500);
    args.set(XmNheight,280);
    args.set(XmNdirectory, home);
    args.set(XmNpattern,   "*");

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

