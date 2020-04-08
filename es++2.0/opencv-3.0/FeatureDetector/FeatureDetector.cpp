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
 *  FeatureDetector.cpp
 *
 *****************************************************************************/

#include <es++/Pair.h>
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/Label.h>
#include <es++/gtkmm-3.0/LabeledTrackBar.h>
#include <es++/gtkmm-3.0/PushButton.h>
#include <es++/gtkmm-3.0/FileOpenDialog.h>
#include <es++/opencv-3.0/OpenCVMainView.h>
#include <es++/opencv-3.0/OpenCVScrolledImageView.h>
#include <es++/opencv-3.0/OpenCVScaleComboBox.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenCVMainView {
private:
  typedef enum {
      /* AgastFeatureDetector */   DETECTOR_AGAST,
      /* AKAZEFeatureDetector */   DETECTOR_AKAZE,
      /* BRISKFeatureDetector */   DETECTOR_BRISK,
      /* FastFeatureDetector  */   DETECTOR_FAST,
      /* GFTTDetector         */   DETECTOR_GFTT,
      /* KAZEFeatureDetector  */   DETECTOR_KAZE,
      /* MSERFeatureDetector  */   DETECTOR_MSER,
      /* ORBFeatureDetector   */   DETECTOR_ORB,
  } DETECTOR;

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
      detected_image = original_image.clone();

      scaleImage(detected_image, scaling_ratio);
    }    

    void detect(DETECTOR detector, int b, int g, int r, 
          int scaling_ratio)
    {
      cv::Mat& image = getOriginalImage();
      
      std::vector<cv::KeyPoint> keypoints;
      switch (detector) {
        case DETECTOR_AGAST: {      
              Ptr<AgastFeatureDetector> detector =  cv::AgastFeatureDetector::create(); 
              detector->detect(image, keypoints);
            }
            break;
        
      case DETECTOR_AKAZE:{
              Ptr<AKAZE> detector =  cv::AKAZE::create();
              detector->detect(image, keypoints);
            }
            break;
        
      case DETECTOR_BRISK: {
              Ptr<BRISK> detector =  cv::BRISK::create();
              detector->detect(image, keypoints);
            }
            break;
        
      case DETECTOR_FAST: {
              Ptr<FastFeatureDetector> detector =  cv::FastFeatureDetector::create();
              detector->detect(image, keypoints);
            }
            break;
        
      case DETECTOR_GFTT:{
              Ptr<GFTTDetector> detector =  cv::GFTTDetector::create();
              detector->detect(image, keypoints);
            }
            break;
        
      case DETECTOR_KAZE:{
              Ptr<KAZE> detector =  cv::KAZE::create();
              detector->detect(image, keypoints);              
            }
            break;
      case DETECTOR_MSER: {
              Ptr<MSER> detector =  cv::MSER::create();
              detector->detect(image, keypoints);
            }
            break;
      case DETECTOR_ORB:{
              Ptr<ORB> detector =  cv::ORB::create();  //use default argments
              detector->detect(image, keypoints);
            }
            break;
      default:
        break;
      } 
      drawKeypoints(image, keypoints, detected_image, cv::Scalar(b, g, r));
      scaleImage(detected_image, scaling_ratio);
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

  DetectedImageView   detected_image;

  int                    loading_flag;

  Es::OpenCVScaleComboBox scale_combobox;

  DETECTOR                detector_type;
  
  Es::LabeledComboBox     detector_type_combobox;

  Es::PushButton          clear_button;
  Es::PushButton          detect_button;

  int                     red;
  int                     green;
  int                     blue;

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

    static const char* detector_types[] = {
      "AgastFeatureDetector",
      "AKAZEFeatureDetector",
      "BRISKFeatureDetector",
      "FastFeatureDetector",
      "GFTTDetector",       
      "KAZEFeatureDetector",
      "MSERFeatureDetector",
      "ORBFeatureDetector",
    };
    detector_type = DETECTOR_AGAST;
 
    detector_type_combobox.set_label("DetectorType");
    detector_type_combobox.append_items(detector_types, 
                 CountOf(detector_types));
    detector_type_combobox.set_active_text(detector_types[0]);

    detector_type_combobox.set_changed_callback(
           sigc::mem_fun(*this, &MainView::detector_type_changed));

    clear_button.set_label("Clear");
    clear_button.set_clicked_callback(
           sigc::mem_fun(*this, &MainView::clear_button_clicked));

    detect_button.set_label("Detect");
    detect_button.set_clicked_callback(
           sigc::mem_fun(*this, &MainView::detect_button_clicked));

    control_pane.set_spacing(20);
    control_pane.pack_start(scale_combobox, Gtk::PACK_SHRINK);

    control_pane.pack_start(detector_type_combobox, Gtk::PACK_SHRINK);
    control_pane.pack_start(clear_button, Gtk::PACK_SHRINK);
    control_pane.pack_start(detect_button, Gtk::PACK_SHRINK);

    loading_flag = IMREAD_COLOR;
    original_image.loadImage(filename, loading_flag, scaling_ratio);
    detected_image.loadImage(filename, loading_flag, scaling_ratio);

    red   = 0xff;
    green = 0;
    blue  = 0;
    detected_image.detect(detector_type, blue, green, red, 
               scaling_ratio);
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


  void detector_type_changed()
  {
    static const Pair<const char*, DETECTOR> types[] = {
      {"AgastFeatureDetector", DETECTOR_AGAST},
      {"AKAZEFeatureDetector", DETECTOR_AKAZE},
      {"BRISKFeatureDetector", DETECTOR_BRISK},
      {"FastFeatureDetector",  DETECTOR_FAST},
      
      {"GFTTDetector",         DETECTOR_GFTT},
      {"KAZEFeatureDetector",  DETECTOR_KAZE},
      {"MSERFeatureDetector",  DETECTOR_MSER},
      {"ORBFeatureDetector",   DETECTOR_ORB},
    };
    
    std::string text = detector_type_combobox.get_active_text();
    const char* item = text.c_str();
    for (size_t i = 0; i<CountOf(types); i++) {
      if (strcmp(types[i].first, item) == 0) {
        detector_type = (DETECTOR)types[i].second;
        break;
      } 
    }
    printf("detector_type %s\n", text.c_str());
    detected_image.detect(detector_type, blue, green, red, 
               scaling_ratio);
  }

  void clear_button_clicked()
  {
      detected_image.loadImage(filename, loading_flag, scaling_ratio);
  }

  void detect_button_clicked()
  {
    detected_image.detect(detector_type, blue, green, red, 
               scaling_ratio);
  }

  void file_open()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string filename = file_dialog.get_filename();

      filepath.set_label(filename);

      original_image.loadImage(filename, loading_flag, scaling_ratio);
      detected_image.loadImage(filename, loading_flag, scaling_ratio);

      detected_image.detect(detector_type, blue, green, red, 
               scaling_ratio);
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

