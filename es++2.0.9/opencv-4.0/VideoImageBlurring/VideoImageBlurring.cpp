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
 *  VideoImageBlurring.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-3.0/LabeledTrackBar.h>
#include <es++/opencv-4.0/OpenCVApplication.h>
#include <es++/opencv-4.0/OpenCVVideoCaptureView.h>
#include <es++/opencv-4.0/OpenCVScrolledImageView.h>
#include <es++/opencv-4.0/OpenCVScaleComboBox.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenCVVideoCaptureView {
private:
  /////////////////////////////////////////////////////// 
  //Inner class starts.
  class BlurredImageView :public Es::OpenCVScrolledImageView {
  private:
    //cv::Mat blurred_image;

  public:
    BlurredImageView()
    {
    }

    void blur(cv::Mat& original_image, 
             int ksize, int scaling_ratio)
    {
      ksize = (ksize/2)*2 + 1;
      //void boxFilter(InputArray src, OutputArray dst, int ddepth, Size ksize, 
      //      Point anchor=Point(-1,-1), 
      //      bool normalize=true, 
      //      int borderType=BORDER_DEFAULT )
      cv::Mat newImage = cv::Mat::zeros(original_image.size(), 
                                        original_image.type() );
      cv::boxFilter(original_image, newImage, 
                    original_image.type(), 
                    cv::Size(ksize, ksize), 
                    cv::Point(-1, -1), 
                    true, 
                    BORDER_DEFAULT); 

      OpenCVScrolledImageView::scaleImage(newImage, scaling_ratio);
    } 
  };
  // Inner class ends.
  /////////////////////////////////////////////////////// 


  Es::HorizontalLayout   horiz_layout;

  Es::VerticalLayout     control_pane;

  Es::OpenCVScrolledImageView original_image;

  BlurredImageView        blurred_image;


  Es::OpenCVScaleComboBox scale_combobox;

  Es::LabeledTrackBar     ksize_trackbar;
  int                     ksize;
 
  int                     scaling_ratio;  //Percentage.
  static const int        CONTROLPANE_WIDTH = 180;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::OpenCVApplication& applet, 
          std::string&      name,
          Es::Args&         args)
  :OpenCVVideoCaptureView(applet, name, args)
  ,scaling_ratio(100) 
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);

    int ratio = (int)args.get(XmNscalingRatio);

    scaling_ratio = OpenCVImageView::validateScale(ratio);

    Es::MainLayout& main_layout = get_main_layout();

    main_layout.pack_start(horiz_layout);

    int ww = (w - CONTROLPANE_WIDTH)/2;

    original_image.set_size_request(ww, h);
    blurred_image.set_size_request(ww, h);
    control_pane.set_size_request( CONTROLPANE_WIDTH, h);

    horiz_layout.pack_start(original_image);
    horiz_layout.pack_start(blurred_image);

    horiz_layout.pack_start(control_pane, FALSE, FALSE, 0);

    scale_combobox.set_selection(scaling_ratio);

    ksize = 11;
    ksize_trackbar.set_label("KernelSize: [1, 31]");
    ksize_trackbar.configure_adjuster((double)ksize, 1.0f, 31.0f);
    ksize_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::ksize_value_changed) );

    control_pane.set_spacing(20);
    control_pane.pack_start(scale_combobox, Gtk::PACK_SHRINK);
    control_pane.pack_start(ksize_trackbar, Gtk::PACK_SHRINK);

    scale_combobox.set_changed_callback(
             sigc::mem_fun(*this, &MainView::scale_changed) );

    show_all();
  }

  void scale_changed()
  {
    std::string scale = scale_combobox.get_active_text();
    scaling_ratio = scale_combobox.get_selection();
  }

  void ksize_value_changed()
  {
    ksize = (int)ksize_trackbar.get_value();
  }

  virtual bool render()
  {
    try {
      cv::Mat frame;
      if (read_video_frame(frame)) {
        original_image.scaleImage(frame, scaling_ratio);
        blurred_image.blur(frame, ksize, scaling_ratio);
      }
    } catch (...) {
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

    Es::OpenCVApplication applet(argc, argv);

    Es::Args args;
    args.set(XmNx, 20);
    args.set(XmNy, 40);
    args.set(XmNwidth, 900);
    args.set(XmNheight, 400);
    args.set(XmNscalingRatio, 60); //60%
    args.set(XmNvideoDeviceIndex, 0); 
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

