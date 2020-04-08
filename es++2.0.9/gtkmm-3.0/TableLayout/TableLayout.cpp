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
 *  TableLayout.cpp 
 *
 *****************************************************************************/

// Simple toplevel application view with a menubar.

#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/ApplicationView.h>
#include <es++/gtkmm-3.0/Label.h>
#include <es++/gtkmm-3.0/PushButton.h>

namespace Es {

class MainView : public Es::ApplicationView {
private:
  Gtk::Table      table_layout;
    Es::Label          label[2];
    Es::PushButton     button[2];

public:
  MainView(Es::Application& applet,
           std::string& name,
           Es::Args& args)
  :ApplicationView(applet, name, args)
  {
    Es::MainLayout& main_layout = get_main_layout();
    
    main_layout.pack_start(table_layout);
    table_layout.resize(2, 2);
    table_layout.attach(label[0], 0, 1, 0,1);
    table_layout.attach(label[1], 1, 2, 0,1);
 
    table_layout.attach(button[0], 0, 1, 1,2);
    table_layout.attach(button[1], 1, 2, 1,2);

    label[0].set_label("You can reach any star.");
    label[1].set_label("Im Anfang war das Wort,\n"
                       " und das Wort war bei Gott,\n"
                       " und das Wort war Gott");
    button[0].set_label("Help");
    button[1].set_label("Quit");

    show_all();
  }

protected:

  virtual void on_help()
  {
    Gtk::MessageDialog dialog(*this, "Message");
    dialog.set_title("Help");
    dialog.set_secondary_text("Es++ example. Help your self." );
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

