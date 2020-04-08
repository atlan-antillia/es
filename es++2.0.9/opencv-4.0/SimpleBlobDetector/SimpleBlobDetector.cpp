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
 *  SimpleBlobDetector.cpp
 *
 *****************************************************************************/

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
    cv::Mat detected_image;
    cv::Mat gray_image;
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
      cv::cvtColor(original_image, gray_image, cv::COLOR_BGR2GRAY);
      detected_image  = gray_image.clone();

      scaleImage(detected_image, scaling_ratio);
    }    

    void detect(int min_dist_between_blobs, int min_area_size, 
                int max_area_size, int scaling_ratio)
    {
      try {
        cv::SimpleBlobDetector::Params params;

        params.thresholdStep = 10.0;
        params.minThreshold = 50.0;
        params.maxThreshold = 220.0;
 
        params.filterByArea = true;
        params.minArea = min_area_size;
        params.maxArea = max_area_size;

        params.filterByColor = true;
        params.blobColor     = 0;

        params.filterByCircularity = true;
        params.minCircularity = 0.5;

        params.filterByConvexity = true;
        params.minConvexity = 0.8;
 
        params.filterByInertia = true;
        params.minInertiaRatio = 0.1;
 
        params.minRepeatability = 2.0;
        params.minDistBetweenBlobs= 5.0;
        params.minDistBetweenBlobs= (double)min_dist_between_blobs;

        Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
        std::vector<KeyPoint> keypoints;
 
        detector->detect(gray_image, keypoints);
        
        drawKeypoints(gray_image, keypoints, 
              detected_image, CV_RGB(255, 0, 0), 
              DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
        scaleImage(detected_image, scaling_ratio);

      } catch (cv::Exception& ex) {
      }
    }

    void rescale(int scaling_ratio)
    {
      OpenCVScrolledImageView::scaleImage(detected_image, scaling_ratio);
    }
  };
  // Inner classes end.
  /////////////////////////////////////////////////////// 

  Es::Label               filepath;

  Es::HorizontalLayout    horiz_layout;

  Es::VerticalLayout      control_pane;

  OriginalImageView       original_image;

  DetectedImageView       detected_image;

  int                     loading_flag;
  static const int        TEMPLATE_WINDOW_SIZE_MAX = 31;
  static const int        SEARCH_WINDOW_SIZE_MAX = 31;
  static const int        H_PARAMETER_MAX = 31;

  Es::OpenCVScaleComboBox scale_combobox;

  Es::LabeledTrackBar     min_dist_between_blobs_trackbar;
  int                     min_dist_between_blobs;
 
  Es::LabeledTrackBar     min_area_size_trackbar;
  int                     min_area_size;

  Es::LabeledTrackBar     max_area_size_trackbar;
  int                     max_area_size;

  int                     scaling_ratio;  //Percentage.
  static const int        CONTROLPANE_WIDTH = 180;

  std::string             filename;
 
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

    filename = "../../images/cat.jpg";

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

    min_dist_between_blobs = 9;
    min_dist_between_blobs_trackbar.set_label("MinDistanceBetweenBlobs:[5,100]");
    min_dist_between_blobs_trackbar.configure_adjuster((double)min_dist_between_blobs, 5.0f, 100.0f);
    min_dist_between_blobs_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::min_dist_between_blobs_value_changed) );


    min_area_size = 15;
    min_area_size_trackbar.set_label("MinAreaSize:[1,100]");
    min_area_size_trackbar.configure_adjuster((double)min_area_size, 1.0f, 100.0f);
    min_area_size_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::min_area_size_value_changed) );

    max_area_size = 130;
    max_area_size_trackbar.set_label("MaxAreaSize:[100,2000]");
    max_area_size_trackbar.configure_adjuster((double)max_area_size, 100.0f, 2000.0f);
    max_area_size_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::max_area_size_value_changed) );

    control_pane.set_spacing(20);
    control_pane.pack_start(scale_combobox, Gtk::PACK_SHRINK);
    control_pane.pack_start(min_dist_between_blobs_trackbar, Gtk::PACK_SHRINK);
    control_pane.pack_start(min_area_size_trackbar, Gtk::PACK_SHRINK);
    control_pane.pack_start(max_area_size_trackbar, Gtk::PACK_SHRINK);

    scale_combobox.set_changed_callback(
             sigc::mem_fun(*this, &MainView::scale_changed) );

    loading_flag = IMREAD_COLOR;
    original_image.loadImage(filename, loading_flag, scaling_ratio);
    detected_image.loadImage(filename, loading_flag, scaling_ratio);

    detected_image.detect(min_dist_between_blobs, min_area_size, max_area_size, scaling_ratio);

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

  void min_dist_between_blobs_value_changed()
  {
    min_dist_between_blobs = (int)min_dist_between_blobs_trackbar.get_value();
    
    detected_image.detect(min_dist_between_blobs, min_area_size, max_area_size, scaling_ratio);
  }

  void min_area_size_value_changed()
  {
    min_area_size = (int)min_area_size_trackbar.get_value();

    detected_image.detect(min_dist_between_blobs, min_area_size, max_area_size, scaling_ratio);
  }

  void max_area_size_value_changed()
  {
    max_area_size = (int)max_area_size_trackbar.get_value();

    detected_image.detect(min_dist_between_blobs, min_area_size, max_area_size, scaling_ratio);
  }

  void file_open()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string filename = file_dialog.get_filename();

      filepath.set_label(filename);

      original_image.loadImage(filename, loading_flag, scaling_ratio);
      detected_image.loadImage(filename, loading_flag, scaling_ratio);

      detected_image.detect(min_dist_between_blobs, min_area_size, max_area_size, scaling_ratio);
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
    args.set(XmNscalingRatio, 100); //100%
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

