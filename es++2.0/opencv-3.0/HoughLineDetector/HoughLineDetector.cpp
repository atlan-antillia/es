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
 *  HoughLineDetector.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/Label.h>
#include <es++/gtkmm-3.0/LabeledTrackBar.h>
#include <es++/gtkmm-3.0/FileOpenDialog.h>
#include <es++/opencv-3.0/OpenCVMainView.h>
#include <es++/opencv-3.0/OpenCVScrolledImageView.h>
#include <es++/opencv-3.0/OpenCVScaleComboBox.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenCVMainView {
private:
  /////////////////////////////////////////////////////// 
  //Inner classes start.

  class OriginalImageView :public Es::OpenCVScrolledImageView {
  public:
    OriginalImageView()
    {
    }    
    void rescale(int scaling_ratio)
    {
      OpenCVScrolledImageView::scaleImage(scaling_ratio);
    }
  };

  class DetectedImageView :public Es::OpenCVScrolledImageView {
  private:
    cv::Mat gray_image;
    cv::Mat detected_image;

  public:
    DetectedImageView()
    {
    }

    void loadImage(const std::string& filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scaling_ratio=100)
    {
      OpenCVScrolledImageView::loadImage(filename, imageLoadingFlag,
                                scaling_ratio);
 
      cv::Mat& original_image = getOriginalImage();
      cv::cvtColor(original_image, gray_image, COLOR_BGR2GRAY );

      detected_image  = original_image.clone();
      scaleImage(detected_image, scaling_ratio);
    }    

    int detect(int threshold1, int threshold2,
            int scalingRatio)
    {
      cv::Mat& original_image = getOriginalImage();

      threshold1 = (threshold1/2)*2 + 1;
      threshold2 = (threshold2/2)*2 + 1;
      //1 Apply cv::Canny to the grayImage to get edges in the grayImage.
      cv::Canny(gray_image, detected_image, 
		(double)threshold1, (double)threshold2);
              
      //2 Apply HoughLinesP to detectedImage returned by cv::Canny.  
      vector<Vec4i> lines;
      HoughLinesP(detected_image, lines, 1, CV_PI/180, 50, 50, 10 );

      //3 Put a clone of ogirinalImage to the detectedImage, 
      // since the detectedImage is not necessary any more. 
      detected_image.release();
      detected_image = original_image.clone();
      int detectedNumber = lines.size();
      //4 Draw lines on the clone of the original image.
      for( size_t i = 0; i < lines.size(); i++ ) {
        Vec4i l = lines[i];
        line( detected_image, Point(l[0], l[1]), 
              Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
      }
            
      scaleImage(detected_image, scalingRatio);
      return detectedNumber;
    }

    void rescale(int scaling_ratio)
    {
      OpenCVScrolledImageView::scaleImage(detected_image, scaling_ratio);
    }
  };
  // Inner classes end.
  /////////////////////////////////////////////////////// 

  Es::Label              filepath;

  Es::HorizontalLayout   horiz_layout;

  Es::VerticalLayout     control_pane;

  OriginalImageView      original_image;

  DetectedImageView      detected_image;

  int                    loading_flag;

  Es::OpenCVScaleComboBox scale_combobox;

  int                     threshold1; 
  Es::LabeledTrackBar     threshold1_trackbar;
 
  int                     threshold2;  
  Es::LabeledTrackBar     threshold2_trackbar;
 
  int                     scaling_ratio;  //Percentage.
  static const int        CONTROLPANE_WIDTH = 200;

  std::string             filename;
 
  Es::Label               detected_number;

  Es::FileOpenDialog      file_dialog;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet, 
          std::string&      name,
          Es::Args&         args)
  :OpenCVMainView(applet, name, args)
  ,scaling_ratio(100)  //100%
  ,file_dialog(*this, Es::FileOpenDialog::IMAGE_FILES)
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);

    int ratio = (int)args.get(XmNscalingRatio);

    scaling_ratio = OpenCVImageView::validateScale(ratio);

    Es::MainLayout& main_layout = get_main_layout();

    main_layout.pack_start(filepath, FALSE, FALSE, 0);

    main_layout.pack_start(horiz_layout);

    filename = "../../images/GinzaWako.png";

    filepath.set_label(filename);
    int ww = (w - CONTROLPANE_WIDTH)/2;

    filepath.set_size_request(w, 30);
    original_image.set_size_request(ww, h);
    detected_image.set_size_request(ww, h);
    control_pane.set_size_request( CONTROLPANE_WIDTH, h);

    horiz_layout.pack_start(original_image);
    horiz_layout.pack_start(detected_image);

    horiz_layout.pack_start(control_pane, FALSE, FALSE, 0);

    scale_combobox.set_selection(scaling_ratio);
    scale_combobox.set_changed_callback(
             sigc::mem_fun(*this, &MainView::scale_changed) );

    threshold1_trackbar.set_label("Canny Threshold1:[0,300]");
    threshold1    = 120;
    threshold1_trackbar.configure_adjuster(threshold1, 0, 300);

    threshold1_trackbar.set_value_changed_callback(
        sigc::mem_fun(*this, &MainView::threshold1_changed) );
            
    threshold2_trackbar.set_label("Canny Threshold2:[0,300]");
    threshold2    = 130;
    threshold2_trackbar.configure_adjuster(threshold2, 0, 300);

    threshold2_trackbar.set_value_changed_callback(
       sigc::mem_fun(*this, &MainView::threshold2_changed) );

    control_pane.set_spacing(20);
    control_pane.pack_start(scale_combobox, Gtk::PACK_SHRINK);

    control_pane.pack_start(threshold1_trackbar, Gtk::PACK_SHRINK);
    control_pane.pack_start(threshold2_trackbar, Gtk::PACK_SHRINK);
    control_pane.pack_start(detected_number, Gtk::PACK_SHRINK);

    loading_flag = IMREAD_COLOR;
    original_image.loadImage(filename, loading_flag, scaling_ratio);
    detected_image.loadImage(filename, loading_flag, scaling_ratio);

    int number = detected_image.detect(threshold1, 
		threshold2, scaling_ratio);
    detected_number.set_number("DetectedNumber:", number);

    show_all();
  }

  void scale_changed()
  {
    std::string scale = scale_combobox.get_active_text();
    printf("scale_changed %s\n", scale.c_str());
    scaling_ratio = scale_combobox.get_selection();

    original_image.rescale(scaling_ratio);
    detected_image.rescale(scaling_ratio);
  }

  void threshold1_changed()
  {
    threshold1 = threshold1_trackbar.get_value();
    int number = detected_image.detect(threshold1, 
	threshold2, scaling_ratio);
    detected_number.set_number("DetectedNumber:", number);
  }
  
  void threshold2_changed()
  {
    threshold2 = threshold2_trackbar.get_value();
    int number = detected_image.detect(threshold1, 
	threshold2, scaling_ratio);
    detected_number.set_number("DetectedNumber:", number);
  }

  void file_open()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string filename = file_dialog.get_filename();

      filepath.set_label(filename);

      original_image.loadImage(filename, loading_flag, scaling_ratio);
      detected_image.loadImage(filename, loading_flag, scaling_ratio);

      int number = detected_image.detect(threshold1, 
	threshold2, scaling_ratio);
      detected_number.set_number("DetectedNumber:", number);
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
    args.set(XmNx, 20);
    args.set(XmNy, 40);
    args.set(XmNwidth, 900);
    args.set(XmNheight, 440);
    args.set(XmNscalingRatio, 60); //60%
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

