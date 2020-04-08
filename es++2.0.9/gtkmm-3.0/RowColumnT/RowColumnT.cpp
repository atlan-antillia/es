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
 *  RowColumnT.cpp 
 *
 *****************************************************************************/

// Simple toplevel application view with a menubar.

#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/ApplicationView.h>
#include <es++/gtkmm-3.0/HorizontalLayout.h>
#include <es++/gtkmm-3.0/Label.h>
#include <es++/gtkmm-3.0/PushButton.h>
#include <es++/gtkmm-3.0/RowColumnT.h>

namespace Es {

class MainView : public Es::ApplicationView {
private:
  static const int  THREE = 3;
  static const int  FOUR  = 4;

  Es::HorizontalLayout hlayout;

  Es::RowColumnT<Es::Label, THREE, THREE> 
                    rowcolumn;
  Es::RowColumnT<Es::TextField, THREE, FOUR> 
                    textf_rowcolumn;

public:
  MainView(Es::Application& applet,
           std::string& name,
           Es::Args& args)
  :ApplicationView(applet, name, args)
  ,hlayout(4)
  ,rowcolumn()
  ,textf_rowcolumn()
  {
    Es::MainLayout& main_layout = get_main_layout();
    main_layout.pack_start(hlayout);
    hlayout.pack_start(rowcolumn, Gtk::PACK_SHRINK);
    hlayout.pack_start(textf_rowcolumn, Gtk::PACK_SHRINK);
 
    //Rowcolum of Labels.
    for (int i = 0; i<THREE; i++) {
      for (int j = 0; j<THREE; j++) {
        char label[128];
        sprintf(label, "%d-%d", i, j);
        rowcolumn.set_label(i, j,label);
      }
    } 
    
    //RowColumn of TextFields.
    for (int i = 0; i<THREE; i++) {
      for (int j = 0; j<FOUR; j++) {
        char label[128];
        sprintf(label, "%.02f-%.03f", (double)i, (double)j);
        textf_rowcolumn.set_width_chars(i, j, 10); //Set width for 5 chars.
        textf_rowcolumn.set_label(i, j,label);
      }
    }

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

