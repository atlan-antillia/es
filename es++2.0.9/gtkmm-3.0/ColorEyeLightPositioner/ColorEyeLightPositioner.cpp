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
 *  ColorEyeLightPositioner.cpp 
 *
 *****************************************************************************/

// Simple toplevel application view with a menubar.

#include <es++/gtkmm-3.0/ApplicationView.h>
#include <es++/gtkmm-3.0/HorizontalLayout.h>
#include <es++/gtkmm-3.0/VerticalLayout.h>
#include <es++/gtkmm-3.0/ColorBox.h>
#include <es++/gtkmm-3.0/ColorEyeLightPositioner.h>

namespace Es {

class MainView : public Es::ApplicationView {
private:
  Es::HorizontalLayout  hlayout; 
    Es::ColorBox        colorbox; 
    Es::VerticalLayout  vlayout;
      Es::ColorEyeLightPositioner      positioner;

public:
  MainView(Es::Application& applet,
           std::string& name,
           Es::Args& args)
  :ApplicationView(applet, name, args)
  {
    int width  = (int)args.get(XmNwidth);
    int height = (int)args.get(XmNheight);

    //MainLayout is a vertical layouter 
    Es::MainLayout& main_layout = get_main_layout();
    main_layout.set_spacing(0);
   
    main_layout.pack_start(hlayout);

    hlayout.pack_start(colorbox);
    hlayout.pack_start(vlayout);
    vlayout.pack_start(positioner);

    double r = 0.2;
    double g = 0.4;
    double b = 0.6;

    positioner.set_color_range(0.0, 255.0);
    positioner.set_eye_range(-20.0, 20.0);
    positioner.set_light_range(-20.0, 20.0);

    positioner.set_color_position(r*255.0, g*255.0, b*255.0);

    int pwidth = positioner.get_preferred_width();

    colorbox.set_size_request(width - pwidth-4, height);
    colorbox.set_background(r, g, b);

    positioner.set_size_request(pwidth, height);

    positioner.set_color_position_changed_callback(
          sigc::mem_fun(*this, &MainView::color_position_changed) );
   
    positioner.set_eye_position_changed_callback(
          sigc::mem_fun(*this, &MainView::eye_position_changed) );
   
    positioner.set_light_position_changed_callback(
          sigc::mem_fun(*this, &MainView::light_position_changed) );
   
    show_all();
  }

protected:
  void color_position_changed()
  {
    double r, g, b;
    positioner.get_color_position(r, g, b);

    printf("positioner_value_changed:(%lf, %lf, %lf)\n",
        r, g, b);
    colorbox.set_background(r/255.0, g/255.0, b/255.0);
    colorbox.refresh();
  }

  void eye_position_changed()
  {
    double x, y, z;
    positioner.get_eye_position(x, y, z);

    printf("eye_position_changed:(%lf, %lf, %lf)\n",
        x, y, z);
  }

  void light_position_changed()
  {
    double x, y, z;
    positioner.get_light_position(x, y, z);

    printf("light_position_changed:(%lf, %lf, %lf)\n",
        x, y, z);
  }

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

