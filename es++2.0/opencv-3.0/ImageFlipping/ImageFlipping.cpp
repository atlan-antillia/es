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
 *  OpenCVMultipleScrolledImageView.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/Label.h>
#include <es++/opencv-3.0/OpenCVMainView.h>
#include <es++/opencv-3.0/OpenCVScrolledImageView.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenCVMainView {
private:
  //////////////////////////////////////////////////////
  //Inner class starts.
  class FlippedImageView :public Es::OpenCVScrolledImageView {
  private:
    cv::Mat flipped;

  public:
    FlippedImageView()
    {
    }

    void loadImage(const std::string& filename, int loading_flag)
    {
      OpenCVScrolledImageView::loadImage(filename, loading_flag);
      cv::Mat& original_image = getOriginalImage();
    }

    void flip(int flip_mode, int scaling_ratio)
    {
      cv::Mat& original_image = getOriginalImage();
      cv::flip(original_image, flipped, flip_mode);
      scaleImage(flipped, scaling_ratio);
    }
  };
  //Inner class ends.
  //////////////////////////////////////////////////////

private:
  Es::Label             filepath;

  Es::HorizontalLayout  horiz_layout;

  static int const      VIEW_COUNT = 3;

  FlippedImageView      image_view[VIEW_COUNT];
 
  static const int      FILEPATH_HEIGHT   = 30; 

  int                   scale;
  std::string           filename;

  static const int      loadingFlag = IMREAD_COLOR; 

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet,
          std::string& title,
          Es::Args& args)
  :OpenCVMainView(applet, title, args) 
  ,scale(100)
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);
    int ww = w /3;

    Es::MainLayout& main_layout = get_main_layout();

    filepath.set_size_request(w, FILEPATH_HEIGHT);

    filepath.set_alignment(Gtk::ALIGN_START);

    main_layout.pack_start(filepath, FALSE,FALSE, 0);
    main_layout.pack_start(horiz_layout, FALSE, FALSE, 0);

    filename = "../../images/cat.jpg";

    int flip = -1;
    for (size_t i = 0; i<VIEW_COUNT; i++) {
      image_view[i].set_size_request(ww, h);
      image_view[i].loadImage(filename, loadingFlag);
      image_view[i].flip(flip++, scale);
      horiz_layout.pack_start(image_view[i]); 
    }

    update_filepath();
    show_all();
  }

  bool on_configure_event(GdkEventConfigure*  configure_event)
  {
    Window::on_configure_event(configure_event);
    //Do nothing here.
    return true;
  }

  void update_filepath()
  {
    filepath.set_label(filename);
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
    args.set(XmNwidth, 900);
    args.set(XmNheight,400);
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

