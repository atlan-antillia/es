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
 *  VideoWriter.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/FileSaveDialog.h>
#include <es++/opencv-4.0/OpenCVVideoCaptureView.h>
#include <es++/opencv-4.0/OpenCVVideoWriter.h>
#include <es++/opencv-4.0/OpenCVScrolledImageView.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenCVVideoCaptureView {
private:
  Es::OpenCVScrolledImageView image_view;

  Es::FileSaveDialog          file_dialog;

  int                         scaling_ratio;

  int                         fps;
  Es::OpenCVVideoWriter       writer;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet, 
           std::string&      name,
           Es::Args&         args)
  :OpenCVVideoCaptureView(applet, name, 
                     //We don't use the defaultFilePulldown menu.
                     args.set(XmNuseDefaultFilePulldown, false) )
  ,scaling_ratio(100)
  ,file_dialog(*this, Es::FileSaveDialog::VIDEO_FILES)
  ,fps(20)
  {
   //Define File pulldown menu with multiple [Open File] pulldown menu items.
    Es::SigcMenuCallback file_menu_callbacks[] = {
      {"New",        sigc::mem_fun(*this, &MainView::file_new)},
      {"Open File",  sigc::mem_fun(*this, &MainView::file_open)},
      {"Save",       sigc::mem_fun(*this, &MainView::file_save)},
      {"Close",      sigc::mem_fun(*this, &MainView::file_close)},
      {"Quit",       sigc::mem_fun(*this, &MainView::file_quit)},
    };

    file_pulldown_append( file_menu_callbacks,
                         CountOf(file_menu_callbacks));

    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);
    if(args.has(XmNscalingRatio)) {
      scaling_ratio = (int)args.get(XmNscalingRatio);
    }
    if (args.has(XmNfps)) {
      fps = (int)args.get(XmNfps);
    }

    Es::MainLayout& main_layout = get_main_layout();
    main_layout.pack_start(image_view);

    show_all();
  }

  virtual bool render()
  {
    try {
      cv::Mat frame;
      if (read_video_frame(frame)) {
        image_view.scaleImage(frame, scaling_ratio);
        if (writer.isOpened()) {
          writer.write(frame);
        }
      }
    } catch (...) {
      //Ignore
    }
    return true;
  }

  void file_close()
  {
    writer.close();
    set_title(get_appname());
  }

  void file_save()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string filename = file_dialog.get_filename();
      cv::VideoCapture& cap = get_videoCapture();
        
      char title[PATH_MAX];
      sprintf(title, "%s - %s", filename.c_str(), get_appname().c_str());
      set_title(title);
        
      writer.open(cap, filename.c_str(), fps);
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

    Es::Args args;
    args.set(XmNx,               20);
    args.set(XmNy,               40);
    args.set(XmNwidth,           640);
    args.set(XmNheight,          480);
    args.set(XmNfps,             25);
    args.set(XmNscalingRatio,    80);
    args.set(XmNvideoDeviceIndex, 0);   //video device index
    args.set(XmNcaptureAutoStart, true);

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

