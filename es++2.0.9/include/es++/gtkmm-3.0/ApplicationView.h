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
 *  ApplicationView.h
 *
 *****************************************************************************/

//2017/07/01
// Simple toplevel application view class with a menubar.

//2017/08/15 Addeed a handler for a signal_delete_event.
//

#pragma once

#define _GTKMM_GUI_
#include <string>
#include <iostream>
#include <glibmm/i18n.h>
#include <gtkmm.h>
#include <gtkmm/window.h>

#include <es++/gtkmm-3.0/ZException.h>
#include <es++/gtkmm-3.0/Args.h>
#include <es++/gtkmm-3.0/MainLayout.h>
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/HorizontalLayout.h>
#include <es++/gtkmm-3.0/VerticalLayout.h>
#include <es++/gtkmm-3.0/ErrorDialog.h>
#include <es++/gtkmm-3.0/PulldownMenu.h>

namespace Es {

class ApplicationView : public Gtk::Window {
private:
  Es::Application&  application; 

  std::string&      title;

  //Vertical main_layout manage
  Es::MainLayout    main_layout;

  //MenuBar
  Gtk::MenuBar      menubar;

  Es::PulldownMenu  file_pulldown;
  Es::PulldownMenu  edit_pulldown;
  Es::PulldownMenu  help_pulldown;

public:
  ApplicationView(
             Es::Application& applet,
             std::string& name,
             Es::Args& args)
  :Window()
   ,application(applet)
   ,title(name)
   ,file_pulldown(menubar, "File")
   ,edit_pulldown(menubar, "Edit")
   ,help_pulldown(menubar, "Help")
  {
    args.setIfNotSet(XmNuseDefaultFilePulldown, true);
    args.setIfNotSet(XmNuseDefaultEditPulldown, true);
    args.setIfNotSet(XmNuseDefaultHelpPulldown, true);

    //ErrorDialog::createErrorDialog(*this);

    int x = (int)args.get(XmNx);
    int y = (int)args.get(XmNy);
    move(x, y);

    set_title(title);
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);
    set_default_size(w, h);

    this->add(main_layout);

    main_layout.pack_start(menubar, Gtk::PACK_SHRINK);

    if (args.get(XmNuseDefaultFilePulldown)) {
      static Es::SigcMenuCallback file_menu_callbacks[] = {
          {"New",    sigc::mem_fun(*this, &ApplicationView::file_new)},
          {"Open",   sigc::mem_fun(*this, &ApplicationView::file_open)},
          {"Save",   sigc::mem_fun(*this, &ApplicationView::file_save)},
          {"Save_As",sigc::mem_fun(*this, &ApplicationView::file_save_as)},
          {"Quit",   sigc::mem_fun(*this, &ApplicationView::file_quit)},
      };
      file_pulldown.append(file_menu_callbacks, CountOf(file_menu_callbacks));
    }

    if (args.get(XmNuseDefaultEditPulldown)) {
      static Es::SigcMenuCallback edit_menu_callbacks[] = {
          {"Cut",    sigc::mem_fun(*this, &ApplicationView::edit_cut)},
          {"Copy",   sigc::mem_fun(*this, &ApplicationView::edit_copy)},
          {"Paste",  sigc::mem_fun(*this, &ApplicationView::edit_paste)},
      };
      edit_pulldown.append(edit_menu_callbacks, CountOf(edit_menu_callbacks));
    }

    if (args.get(XmNuseDefaultHelpPulldown)) {
      static Es::SigcMenuCallback help_menu_callbacks[] = {
          {"Help Info",  sigc::mem_fun(*this, &ApplicationView::help_info)},
          {"Version",   sigc::mem_fun(*this, &ApplicationView::help_version)},
      };
      help_pulldown.append(help_menu_callbacks, CountOf(help_menu_callbacks));
    }

    //2019/04/05 Addeed the following signal handler
    signal_key_press_event().connect(
      sigc::mem_fun(*this, &ApplicationView::keyPressEvent), false);

    //2017/08/15 Addeed the following signal handler
    signal_delete_event().connect(sigc::mem_fun(*this,
                        &ApplicationView::quit_application));
    show_all_children();
  }


  virtual ~ApplicationView()
  {
  }

  bool quit_application(GdkEventAny* event)
  {
    file_quit();
    return true; 
  }

  void file_pulldown_append( Es::SigcMenuCallback* callbacks,
              int count)
  {
    file_pulldown.append(callbacks, count);
  }

  void edit_pulldown_append( Es::SigcMenuCallback* callbacks,
              int count)
  {
    edit_pulldown.append(callbacks, count);
  }

  void help_pulldown_append( Es::SigcMenuCallback* callbacks,
              int count)
  {
    help_pulldown.append(callbacks, count);
  }

  Gtk::MenuBar& get_menubar()
  {
    return menubar;
  }

  void set_program_title()
  {  
    std::string tname = title;
    int npos = tname.rfind("/");
    if (npos > 0) {
      tname = title.substr(npos+1);
    }
    set_title(tname);
  }

  void set_filenamed_title(const std::string& filename)
  {
    std::string name = filename;
    int pos  = filename.rfind("/");
    if (pos > 0) {
      name = filename.substr(pos+1);
    }
    std::string tname = title;
    int npos = tname.rfind("/");
    if (npos > 0) {
      tname = title.substr(npos+1);
    }
    std::string full_title = name + " - " + tname;

    set_title(full_title);
  }

  Es::MainLayout& get_main_layout()
  {
    return main_layout;
  }

//protected:
public:
  //Default virtual menu callbacks.
  //Please define your own callback in a subclass derived from this class.

  virtual void file_new()
  {
    Gtk::MessageDialog dialog(*this, "Message");
    dialog.set_title("Information");
    dialog.set_secondary_text("ApplicationView::file_new" );
    dialog.run();
  }

  virtual void file_open()
  {
    Gtk::MessageDialog dialog(*this, "Message");
    dialog.set_title("Information");
    dialog.set_secondary_text("ApplicationView::file_open" );
    dialog.run();
  }

  virtual void file_save()
  {
    Gtk::MessageDialog dialog(*this, "Message");
    dialog.set_title("Information");
    dialog.set_secondary_text("ApplicationView::file_save" );
    dialog.run();
  }

  virtual void file_save_as()
  {
    Gtk::MessageDialog dialog(*this, "Message");
    dialog.set_title("Information");
    dialog.set_secondary_text("ApplicationView::file_save_as" );
    dialog.run();
  }

  virtual void file_quit()
  {
    Gtk::MessageDialog dialog(*this, "Confirmation", 
             false,
             Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
    dialog.set_title("Question");
    dialog.set_secondary_text("Are you sure you want to quit?" );
    if (dialog.run() == Gtk::RESPONSE_OK) {
      application.quit();
    }
  }

  virtual void edit_cut()
  {
    std::cout << "edit_cut\n";
    Gtk::MessageDialog dialog(*this, "Message");
    dialog.set_title("Information");
    dialog.set_secondary_text("ApplicationView::edit_cut" );
    dialog.run();
  }

  virtual void edit_copy()
  {
    Gtk::MessageDialog dialog(*this, "Message");
    dialog.set_title("Information");
    dialog.set_secondary_text("ApplicationView::edit_copy" );
    dialog.run();
  }

  virtual void edit_paste()
  {
    Gtk::MessageDialog dialog(*this, "Message");
    dialog.set_title("Information");
    dialog.set_secondary_text("ApplicationView::edit_paste" );
    dialog.run();
  }

  virtual void help_info()
  {
    Gtk::MessageDialog dialog(*this, "Message");
    dialog.set_title("Information");
    dialog.set_secondary_text("ApplicationView::help_info" );
    dialog.run();
  }

  virtual void help_version()
  {
    Gtk::MessageDialog dialog(*this, "Copyright(C) 2017-2019 Antillia.com ALL RIGHTS RESERVED");
    dialog.set_secondary_text("Es++ application on Gtkmm 3" );
    dialog.set_title("Version");
    dialog.run();
  }

  //After realized , you can get client width and height.
  int get_client_width()
  {
    return main_layout.get_width();
  }

  int get_client_height()
  {
     return main_layout.get_height() - menubar.get_height();
  }

  std::string& get_appname()
  {
    return title;
  }

 
  // 2019/04/05: Please define your own keyPressEvent in a subclass derived from this class.
  virtual bool keyPressEvent(GdkEventKey* event)
  {
    // Write your own event handler in a subclass
    // in the following way.
    /*
    switch (event->keyval) {

      case GDK_p:
        break;

      case GDK_s:
        if (event->state & GDK_SHIFT_MASK) {
         //
        } else if (event->state & GDK_CONTROL_MASK) {
         //
        } else {
          //
        }
        break;

      case GDK_m:
        if (event->state & GDK_SHIFT_MASK) {
          // 
        } else if (event->state & GDK_CONTROL_MASK) {
          //
        } else {
          //
        }
        break;
      default:
        break;
    } 
    */
    return false;
  }
};

}

