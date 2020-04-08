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
 *  ControlPane.cpp 
 *
 *****************************************************************************/

// Simple toplevel application view with a menubar.

#define _GTKMM_GUI_

#include <gtkmm/window.h>
#include <gtkmm.h>

#include <gtkmm.h>
#include <es++/gtkmm-2.4/ZException.h>
#include <es++/gtkmm-2.4/Args.h>
#include <es++/gtkmm-2.4/MainLayout.h>
#include <es++/gtkmm-2.4/Application.h>
#include <es++/gtkmm-2.4/HorizontalLayout.h>
#include <es++/gtkmm-2.4/VerticalLayout.h>
#include <es++/gtkmm-2.4/PulldownMenu.h>
#include <es++/gtkmm-2.4/PushButton.h>

namespace Es {

class MainView : public Gtk::Window {
private:
  Es::Application& application; 

  //Vertical main_layout manage
  Es::MainLayout    main_layout;

  //MenuBar to be added to main_layout
  Gtk::MenuBar      menubar;
  Es::PulldownMenu  file_pulldown;

  //Center layout to be added to main_layout
  Es::HorizontalLayout  center_layout;
    Gtk::Label           label;
    Es::VerticalLayout   control_pane;
      Es::PushButton     help;
      Es::PushButton     version;
      Es::PushButton     quit;

  static const int CONTROLPANE_WIDTH =160;

public:
  MainView(Es::Application& applet,
           std::string& name,
           Es::Args& args)
  :Window()
   ,application(applet)
   ,file_pulldown(menubar, "File")
  {
    int x = (int)args.get(XmNx);
    int y = (int)args.get(XmNy);
    move(x, y);

    set_title(name);
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);
    set_default_size(w, h);

    this->add(main_layout);

    main_layout.pack_start(menubar, Gtk::PACK_SHRINK);

    static Es::SigcMenuCallback file_menu_callbacks[] = {
      {"New",    sigc::mem_fun(*this, &MainView::file_new)},
      {"Open",   sigc::mem_fun(*this, &MainView::file_open)},
      {"Save",   sigc::mem_fun(*this, &MainView::file_save)},
      {"Save_As",sigc::mem_fun(*this, &MainView::file_save_as)},
      {"Quit",   sigc::mem_fun(*this, &MainView::file_quit)},
    };
    file_pulldown.append(file_menu_callbacks, CountOf(file_menu_callbacks));

    main_layout.pack_start(center_layout);
    int ww = w - CONTROLPANE_WIDTH;
    label.set_size_request(ww, h);
    control_pane.set_size_request(CONTROLPANE_WIDTH, h);

    center_layout.pack_start(label);
    center_layout.pack_start(control_pane, FALSE, FALSE, 0);
    control_pane.set_spacing(20);

    control_pane.pack_start(help, FALSE, FALSE, 0); 
    control_pane.pack_start(version, FALSE, FALSE, 0); 
    control_pane.pack_start(quit, FALSE, FALSE, 0);

    label.set_label("Hello world. こんにちは世界");

    help.set_label("Help");
    help.set_clicked_callback(sigc::mem_fun(*this, &MainView::on_help));

    version.set_label("Version");
    version.set_clicked_callback(sigc::mem_fun(*this, &MainView::on_version));

    quit.set_label("Quit");
    quit.set_clicked_callback(sigc::mem_fun(*this, &MainView::file_quit));
 
    show_all_children();
  }

  Gtk::MenuBar& get_menubar()
  {
    return menubar;
  }

  Es::MainLayout& get_main_layout()
  {
    return main_layout;
  }

protected:

  virtual void file_new()
  {
    Gtk::MessageDialog dialog(*this, "Message");
    dialog.set_title("Information");
    dialog.set_secondary_text("MainView::file_new" );
    dialog.run();
  }

  virtual void file_open()
  {
    Gtk::MessageDialog dialog(*this, "Message");
    dialog.set_title("Information");
    dialog.set_secondary_text("MainView::file_open" );
    dialog.run();
  }

  virtual void file_save()
  {
    Gtk::MessageDialog dialog(*this, "Message");
    dialog.set_title("Information");
    dialog.set_secondary_text("MainView::file_save" );
    dialog.run();
  }

  virtual void file_save_as()
  {
    Gtk::MessageDialog dialog(*this, "Message");
    dialog.set_title("Information");
    dialog.set_secondary_text("MainView::file_save_as" );
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

  virtual void on_help()
  {
    Gtk::MessageDialog dialog(*this, "Message");
    dialog.set_title("Help");
    dialog.set_secondary_text("Es++ example. Help your self." );
    dialog.run();
  }

  virtual void on_version()
  {
    Gtk::MessageDialog dialog(*this, "Message");
    dialog.set_title("Information");
    dialog.set_secondary_text("Es++ example on gtkmm-2.4" );
    dialog.run();
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
    args.set(XmNwidth, 600);
    args.set(XmNheight, 300);
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

