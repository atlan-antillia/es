/******************************************************************************
 *
 * Copyright (c) 2017  TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
 *  FileSystemBrowser.cpp 
 *
 *****************************************************************************/

//Simple FileSystemBrowser based on Es::Notebook, and Es::ScrolledFileListView

#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/ApplicationView.h>
#include <es++/gtkmm-3.0/Notebook.h>
#include <es++/gtkmm-3.0/Label.h>
#include <es++/gtkmm-3.0/ScrolledFileListView.h>
#include <es++/ListT.h>

using namespace Gtk;

namespace Es {

class MainView : public Es::ApplicationView {
private:

  Es::Notebook                  notebook;
  Es::ListT<Es::Label >         labels;
  Es::ListT<
    Es::ScrolledFileListView >  listviews;

    std::string rootdir;
    std::string pattern;

public:
  MainView(Es::Application& applet,
           std::string& name,
           Es::Args& args)
  :ApplicationView(applet, name, args)
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);

    rootdir = (const char*)args.get(XmNdirectory);
    pattern = (const char*)args.get(XmNpattern);

    Es::MainLayout& main_layout = get_main_layout();
    
    main_layout.pack_start(notebook);
    //main_layout.add(tabbed_window);

    add_page(rootdir, rootdir, pattern);

    show_all();
  }

  void add_page(std::string& tab, 
                std::string& dir, std::string& pattern)
  {
    Es::Label* label = new Es::Label();
    label->set_label(tab);
    labels.add(label);

    Es::ScrolledFileListView* listview =
               new Es::ScrolledFileListView();

    listview->listup_folders(dir, pattern);
    listview->listup_files(dir, pattern);
    listview->get_treeview()
                     .signal_row_activated().connect(
          sigc::mem_fun(*this, &MainView::listview_row_activated) );

    listviews.add(listview);
    notebook.append_page(*listview, *label);
    notebook.show_all();
    notebook.set_current_page(-1);
  }

  void update_page(Es::ScrolledFileListView* listview, std::string& tab, 
                   std::string& dir, std::string& pattern)
  {
    Es::Label* label = (Es::Label*)notebook.get_tab_label(*listview);
    if (label) {
      label->set_label(tab);
    }
 
    listview->clear();
    listview->listup_folders(dir, pattern);
    listview->listup_files(dir, pattern);
  }

  void listview_row_activated(const Gtk::TreeModel::Path& path,
                               Gtk::TreeViewColumn* column)
  {
    int current_page = notebook.get_current_page();
    std::string fullpath = notebook.get_nth_tab_text(0);

    for (int i = 1; i<=current_page; i++) {
      std::string text = notebook.get_nth_tab_text(i);
      fullpath += text;
      fullpath += "/";
    }

    std::string text = notebook.get_nth_tab_text(current_page);
    printf("fullpath %s\n", fullpath.c_str());

    Es::ScrolledFileListView* listview=
               (Es::ScrolledFileListView*)notebook.get_active_page();

    if (listview) {
      Glib::RefPtr<Gtk::TreeSelection> selection =
                         listview->get_treeview()
                                          .get_selection();
      Gtk::TreeModel::iterator iter = selection->get_selected();
      if(iter) {
        Gtk::TreeModel::Row row = *iter;
        listview->display(row);
        bool folder = listview->is_folder(row);

        std::string filename = listview->get_filename(row);
        fullpath += filename;
        if (folder) {
          printf("Selected folder = %s\n", filename.c_str());
          printf("Fullpath %s\n", fullpath.c_str());
          int next_page = current_page + 1;

          Es::ScrolledFileListView* listview = 
                 (Es::ScrolledFileListView*)notebook.get_nth_page(next_page);
          if (listview) {
            //Update an existing Es::ScrolledFileListView

            update_page(listview, filename, fullpath, pattern);
            notebook.set_current_page(next_page); 
            int total_page_num = notebook.get_n_pages();

            for (int i = total_page_num-1; i>next_page; i--) {
              printf("remove %d\n", i);
              Es::ScrolledFileListView* lview = (Es::ScrolledFileListView*)
                         notebook.get_nth_page(i);
              notebook.remove_page(*lview);
              listviews.remove(lview);              
            }
            notebook.show_all();

          } else {
            //Add a new Es::ScrolledFileListView
            add_page(filename, fullpath, pattern);
          }
        } else {
          printf("Selected file = %s\n", fullpath.c_str());
        }
      }
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

    const char* rootdir = "/"; //getenv("HOME");
    const char* pattern = "*";

    Es::Args args;
    args.set(XmNx, 20);
    args.set(XmNy, 40);
    args.set(XmNwidth, 800);
    args.set(XmNheight,460);
    args.set(XmNdirectory, rootdir);
    args.set(XmNpattern,   pattern);

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

