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
 *  VideoDeviceEnumerator.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/FileOpenDialog.h>
#include <es++/gtkmm-3.0/PushButton.h>
#include <es++/opencv-4.0/OpenCVScaleComboBox.h>
#include <es++/gtkmm-3.0/LabeledVideoDeviceComboBox.h>
#include <es++/opencv-4.0/OpenCVVideoCaptureView.h>
#include <es++/opencv-4.0/OpenCVScrolledImageView.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenCVVideoCaptureView {
private:
  Es::HorizontalLayout  hbox;
    Es::OpenCVScrolledImageView image_view;

    Es::VerticalLayout          control_pane;
      Es::OpenCVScaleComboBox       scale_combobox;
      Es::LabeledVideoDeviceComboBox device_combobox;
      Es::PushButton                 start_button;
      Es::PushButton                 stop_button;

  int scaling_ratio;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet, 
           std::string&      name,
           Es::Args&         args)
  :OpenCVVideoCaptureView(applet, name, args)
  ,scaling_ratio(100)
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);
    const int CONTROL_BOX_WIDTH = 240;

    int ww = (w - CONTROL_BOX_WIDTH);
    if(args.has(XmNscalingRatio)) {
      scaling_ratio = (int)args.get(XmNscalingRatio);
    }

    Es::MainLayout& main_layout = get_main_layout();
 
    main_layout.pack_start(hbox);
    image_view.set_size_request(ww, h);
    control_pane.set_spacing(10);
    control_pane.set_size_request(CONTROL_BOX_WIDTH, h);
  
    hbox.pack_start(image_view);
    hbox.pack_start(control_pane, Gtk::PACK_SHRINK);

    control_pane.pack_start(scale_combobox, FALSE, FALSE, 0);
    control_pane.pack_start(device_combobox, FALSE, FALSE, 0);
    start_button.set_label("Start Capture");
    start_button.set_clicked_callback(
              sigc::mem_fun(*this, &MainView::start_capture) );

    control_pane.pack_start(start_button, FALSE, FALSE, 0);
    stop_button.set_label("Stop Capture");
    stop_button.set_clicked_callback(
              sigc::mem_fun(*this, &MainView::stop_capture) );
    stop_button.disable();

    control_pane.pack_start(stop_button, FALSE, FALSE, 0);

    scale_combobox.set_changed_callback(
            sigc::mem_fun(*this, &MainView::scale_changed) );
    scale_combobox.set_selection(scaling_ratio);

    device_combobox.set_changed_callback(
            sigc::mem_fun(*this, &MainView::device_changed) );

    show_all();
  }

  void scale_changed()
  {
    Glib::ustring text = scale_combobox.get_active_text();
    printf("scale_changed: %s\n", text.c_str());
    scaling_ratio = scale_combobox.get_selection();
  }

  void device_changed()
  {
    Glib::ustring text = device_combobox.get_active_text();
    int index = device_combobox.get_selected_index();
    printf("device_changed: %s :index=%d\n", text.c_str(), index);
    set_device_index(index);
  }

  void start_capture()
  {
    int index = device_combobox.get_selected_index();
    printf("start_capture %d\n", index);
    set_device_index(index);
    open_video_device(index);

    start_button.disable();
    stop_button.enable();
  }

  void stop_capture()
  {
    start_button.enable();
    stop_button.disable();
    close_video_device();
  }

  virtual bool render()
  {
    try {
      cv::Mat frame;
      if (read_video_frame(frame)) {
        image_view.scaleImage(frame, scaling_ratio);
      }
    } catch (...) {
      //Ignore
    }
    return true;
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
    args.set(XmNx,               20);
    args.set(XmNy,               40);
    args.set(XmNwidth,           640);
    args.set(XmNheight,          480);
    args.set(XmNscalingRatio,    80);
    args.set(XmNcaptureAutoStart, false);

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

