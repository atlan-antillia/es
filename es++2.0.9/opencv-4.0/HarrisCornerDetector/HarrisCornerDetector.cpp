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
 *  HarrisCornerDetector.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/Label.h>
#include <es++/gtkmm-3.0/LabeledTrackBar.h>
#include <es++/gtkmm-3.0/FileOpenDialog.h>
#include <es++/opencv-4.0/OpenCVMainView.h>
#include <es++/opencv-4.0/OpenCVScrolledImageView.h>
#include <es++/opencv-4.0/OpenCVScaleComboBox.h>

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
      scaleImage(gray_image, scaling_ratio);
    }    

    //The following detect_cornert method  is based on the code:
    //http://www.swarthmore.edu/NatSci/mzucker1/opencv-2.4.10-docs/
    //  doc/tutorials/features2d/trackingmotion/harris_detector/harris_detector.html

    int detect_corner(int blockSize, int ksize, int threshold, int scaling_ratio)
    {
      cv::Mat& original_image = getOriginalImage();
     
      //blockSize: Neighborhood size.

      //ksize : Aperture parameter for the Sobel operator, 
      // which should be odd.
      ksize = (ksize/2)*2 + 1;

      //k: Harris detector free parameter.
      double k = 0.04;

      detected_image.release();

      cv::Mat dest_image  = cv::Mat::zeros(original_image.size(), CV_32FC1 );

      cv::cornerHarris(gray_image, dest_image,
            blockSize, ksize, k, BORDER_DEFAULT );

      cv::Mat dest_normalized; 
      cv::normalize(dest_image, dest_normalized, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );

      //We draw circles at the corners detected by cornerHarris on the originalImage 
      // by using the destNormalized image data and the threshold parameter 
      // which is used to draw the circles..

      detected_image = original_image.clone(); 
      int detectedNumber = 0;
      for( int j = 0; j < dest_normalized.rows ; j++ ) { 
        for( int i = 0; i < dest_normalized.cols; i++ ) {
          if( (int) dest_normalized.at<float>(j,i) > threshold ) {
            cv::circle(detected_image, cv::Point( i, j ), 5,  
                           cv::Scalar(0, 0, 255),  //red
                           1, 8, 0);
            detectedNumber++;
          }
        }
      }
      scaleImage(detected_image, scaling_ratio);
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

  int                    blockSize;
  Es::LabeledTrackBar    blockSize_trackbar;
  
  int                    ksize;
  Es::LabeledTrackBar    ksize_trackbar;

  int                    detectorThreshold;
  Es::LabeledTrackBar    detectorThreshold_trackbar;

  Es::Label              detectedNumber;

  int                    scaling_ratio;  //Percentage.
  static const int       CONTROLPANE_WIDTH = 200;

  std::string            filename;
 
  Es::FileOpenDialog     file_dialog;

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

    filename = "../../images/Geometry.png";

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

    blockSize = 7;
    blockSize_trackbar.set_label("BlockSize: [2, 100]");
    blockSize_trackbar.configure_adjuster((double)blockSize, 2.0f, 100.0f);
    blockSize_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::blockSize_value_changed) );


    ksize = 5;
    ksize_trackbar.set_label("KernelSize: [0, 31]");
    ksize_trackbar.configure_adjuster((double)ksize, 0.0f, 31.0f);
    ksize_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::ksize_value_changed) );

    detectorThreshold = 120;

    detectorThreshold_trackbar.set_label("DetectorThreshold:[100,240]");
    detectorThreshold_trackbar.configure_adjuster((double)ksize, 100.0f, 240.0f);
    detectorThreshold_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::detectorThreshold_value_changed) );

    control_pane.set_spacing(20);
    control_pane.pack_start(scale_combobox, Gtk::PACK_SHRINK);
    control_pane.pack_start(blockSize_trackbar, Gtk::PACK_SHRINK);
    control_pane.pack_start(ksize_trackbar, Gtk::PACK_SHRINK);
    control_pane.pack_start(detectorThreshold_trackbar, Gtk::PACK_SHRINK);
    control_pane.pack_start(detectedNumber, Gtk::PACK_SHRINK);

    loading_flag = IMREAD_COLOR;
    original_image.loadImage(filename, loading_flag, scaling_ratio);
    detected_image.loadImage(filename, loading_flag, scaling_ratio);

    int number = detected_image.detect_corner(blockSize, ksize, 
                          detectorThreshold,
                          scaling_ratio);
    detectedNumber.set_number("Detected:", number);

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

  void blockSize_value_changed()
  {
    blockSize = (int)blockSize_trackbar.get_value();
    
    int number = detected_image.detect_corner(blockSize, ksize, 
                          detectorThreshold,
                          scaling_ratio);
    detectedNumber.set_number("Detected:", number);
  }

  void ksize_value_changed()
  {
    ksize = (int)ksize_trackbar.get_value();
    
    int number = detected_image.detect_corner(blockSize, ksize, 
                          detectorThreshold,
                          scaling_ratio);
    detectedNumber.set_number("Detected:", number);
  }

  void detectorThreshold_value_changed()
  {
    detectorThreshold = (int)detectorThreshold_trackbar.get_value();
  
    int number = detected_image.detect_corner(blockSize, ksize,
                          detectorThreshold,
                          scaling_ratio);
    detectedNumber.set_number("Detected:", number);
  }

  void file_open()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string filename = file_dialog.get_filename();

      filepath.set_label(filename);

      original_image.loadImage(filename, loading_flag, scaling_ratio);
      detected_image.loadImage(filename, loading_flag, scaling_ratio);

      int number = detected_image.detect_corner(blockSize, ksize, 
                          detectorThreshold,
                          scaling_ratio);
      detectedNumber.set_number("Detected:", number);
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
    args.set(XmNheight, 400);
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

