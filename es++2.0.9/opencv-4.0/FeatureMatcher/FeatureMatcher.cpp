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
 *  FeatureMatcher.cpp
 *
 *****************************************************************************/

//
#include <es++/Pair.h>
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/FileOpenDialog.h>
#include <es++/gtkmm-3.0/Label.h>
#include <es++/gtkmm-3.0/PushButton.h>
#include <es++/opencv-4.0/OpenCVMainView.h>
#include <es++/opencv-4.0/OpenCVScaleComboBox.h>
#include <es++/opencv-4.0/OpenCVScrolledImageView.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenCVMainView {
private:
  /////////////////////////////////////////////////////
  //Inner classes start.
  class SourceImageView :public Es::OpenCVScrolledImageView {
  private:
  public:
    SourceImageView()
    {
    }
    void rescale(int scaling_ratio)
    {
      OpenCVScrolledImageView::scaleImage(scaling_ratio);
    }
  };

  class MatchedImageView :public Es::OpenCVScrolledImageView {
  private:
    cv::Mat matched_image; 

  public:
    MatchedImageView()
    {
    }

    void rescale(int scaling_ratio)
    {
      OpenCVScrolledImageView::scaleImage(matched_image, scaling_ratio);
    }

    void setMatched(cv::Mat& mat, int scaling_ratio)
    {
      matched_image = mat.clone();
      scaleImage(matched_image, scaling_ratio);
    }
  };

  //Inner classes end.
  /////////////////////////////////////////////////////
  //
private:
  typedef enum {
    /* AKAZEFeatureDetector */   DETECTOR_AKAZE=0,
    /* BRISKFeatureDetector */   DETECTOR_BRISK,
    /* ORBFeatureDetector   */   DETECTOR_ORB,
  } DETECTOR;


  static const int        IMAGES_COUNT = 2;
  static const int        FIRST  = 0;
  static const int        SECOND = 1;
  std::string             filenames[IMAGES_COUNT];
  
  Es::Label               filepath;

  Es::HorizontalLayout   horiz_layout;
    Es::VerticalLayout     view_layout;
      Es::HorizontalLayout   source_view_layout;
        SourceImageView        source_images[IMAGES_COUNT];
      MatchedImageView       matched_image;

    Es::VerticalLayout      control_pane;
      Es::OpenCVScaleComboBox combobox;

      Es::LabeledComboBox     detector_combobox;

      Es::LabeledComboBox     besttop_number_combobox;

      Es::PushButton          match_button;
      Es::Label               matched_number;

  int                     detector_index;  

  int                     besttop_number;

  static const int        FILEPATH_HEIGHT   = 30; 
  static const int        CONTROLPANE_WIDTH = 200;

  int                     scaling_ratio;

  static const int        loading_flag = IMREAD_COLOR; 

  Es::FileOpenDialog      file_dialog;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet,
          std::string& title,
          Es::Args& args)
  :OpenCVMainView(applet, title, 
                     //We don't use the defaultFilePulldown menu.
                     args.set(XmNuseDefaultFilePulldown, false) )
  ,scaling_ratio(100)
  ,source_view_layout(0)  //SPACING
  ,file_dialog(*this, Es::FileOpenDialog::IMAGE_FILES)
  {
   //Define File pulldown menu with multiple [Open File] pulldown menu items.
    Es::SigcMenuCallback file_menu_callbacks[] = {
      {"New",        sigc::mem_fun(*this, &MainView::file_new)},
      {"Open File1", sigc::mem_fun(*this, &MainView::file_open1)},
      {"Open File2", sigc::mem_fun(*this, &MainView::file_open2)},
      {"Save",       sigc::mem_fun(*this, &MainView::file_save)},
      {"Save_As",    sigc::mem_fun(*this, &MainView::file_save_as)},
      {"Quit",       sigc::mem_fun(*this, &MainView::file_quit)},
    };

    file_pulldown_append( file_menu_callbacks,
                         CountOf(file_menu_callbacks));

    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);
    int ww = (w - CONTROLPANE_WIDTH)/2;
    int hh = h/2;

    int ratio = (int)args.get(XmNscalingRatio);
    scaling_ratio = OpenCVImageView::validateScale(ratio);

    Es::MainLayout& main_layout = get_main_layout();

    filepath.set_size_request(w, FILEPATH_HEIGHT);

    filepath.set_alignment(Gtk::ALIGN_START);

    main_layout.pack_start(filepath, FALSE,FALSE, 0);
    main_layout.pack_start(horiz_layout, FALSE, FALSE, 0);

    source_images[FIRST].set_size_request(ww, h/2);
    source_images[SECOND].set_size_request(ww, h/2);
    matched_image.set_size_request(w, h/2);

    control_pane.set_spacing(10);
    control_pane.set_size_request(CONTROLPANE_WIDTH, h);

    main_layout.pack_start(filepath, FALSE, FALSE, 0);
      main_layout.pack_start(horiz_layout);
        horiz_layout.pack_start(view_layout);
          view_layout.pack_start(source_view_layout);
            source_view_layout.pack_start(source_images[FIRST]);
            source_view_layout.pack_start(source_images[SECOND]);
          view_layout.pack_start(matched_image);
           
        horiz_layout.pack_start(control_pane);
        control_pane.pack_start(combobox, Gtk::PACK_SHRINK );
        match_button.set_label("Match");
        control_pane.pack_start(match_button, Gtk::PACK_SHRINK);
        control_pane.pack_start(detector_combobox, Gtk::PACK_SHRINK);
        control_pane.pack_start(besttop_number_combobox, Gtk::PACK_SHRINK);
        control_pane.pack_start(matched_number, Gtk::PACK_SHRINK);

    combobox.set_selection(scaling_ratio);
    combobox.set_changed_callback(sigc::mem_fun(*this, &MainView::scale_changed) );

    match_button.set_label("Match");
    match_button.set_clicked_callback(sigc::mem_fun(*this, 
                     &MainView::match_button_clicked) );

    const char* detectors[] =  {
         "AKAZEFeatureDetector",
         "BRISKFeatureDetector",
         "ORBFeatureDetector"
    };
    detector_combobox.set_label("Detector");
    detector_combobox.append_items(detectors, CountOf(detectors));
    detector_combobox.set_active_text(detectors[DETECTOR_ORB]); //ORB

    detector_combobox.set_changed_callback(sigc::mem_fun(*this, 
                     &MainView::detector_changed) );
    detector_index  = DETECTOR_ORB;

    besttop_number_combobox.set_label("BestTopNumber");
    for (int i = 1; i<10; i++) {
      char number[20];
      sprintf(number, "%d", i*10);
      besttop_number_combobox.append_item(number);
      if (i == 1) {
        besttop_number_combobox.set_active_text(number);
      }
    }
    besttop_number = 10;
    besttop_number_combobox.set_changed_callback(sigc::mem_fun(*this, 
                          &MainView::besttop_number_changed) );
    filenames[FIRST]  = "../../images/Tower1.png";
    filenames[SECOND] = "../../images/Tower2.png";

    source_images[FIRST].loadImage(filenames[FIRST], loading_flag, scaling_ratio);

    source_images[SECOND].loadImage(filenames[SECOND], loading_flag, scaling_ratio);

    update_filepath();
    show_all();
  }

  void scale_changed()
  {
    scaling_ratio = combobox.get_selection();
    printf("scale_changed %d\n", scaling_ratio);
    source_images[FIRST].rescale(scaling_ratio);
    source_images[SECOND].rescale(scaling_ratio);
    matched_image.rescale(scaling_ratio);
  }

  void update_filepath()
  {
    std::string space = "    ";
    filepath.set_label(filenames[FIRST] + space +  filenames[SECOND]);
  }

  void detector_changed()
  {
    std::string text = detector_combobox.get_active_text();
    Es::Pair<int, std::string> pairs[] = {
      {DETECTOR_AKAZE, std::string("AKAZEFeatureDetector")},
      {DETECTOR_BRISK, std::string("BRISKFeatureDetector")},
      {DETECTOR_ORB,   std::string("ORBFeatureDetector")},
    };
    int prev_detector = detector_index;
    for (size_t i = 0; i<CountOf(pairs); i++) {
      if (pairs[i].second == text) {
        detector_index  = pairs[i].first;
        break;
      }  
    }
    //printf("detetorIndex %d %s\n", detector_index, text.c_str());
    if (detector_index != prev_detector) {
      printf("Detector updated call match\n");
      match_button_clicked();
    }
  }

  void besttop_number_changed()
  {
    int prev_besttop =besttop_number;

    besttop_number = besttop_number_combobox.get_active_number();

    //printf("besttop_number %d\n", besttop_number);
    if (besttop_number != prev_besttop) {
      printf("besttop_number updated call match\n");
      match_button_clicked();
    }
  }

  void set_matched_number(int bestMatched, int totalMatchedNumber)
  {
    char number[128];
    sprintf(number, //CountOf(number), 
         "Matched Number: %d / %d", bestMatched, totalMatchedNumber);
    
    matched_number.set_label(number);
  }
  
  //Matching operation. 
  void match_button_clicked()
  {
    try {
      cv::Ptr<cv::Feature2D> feature;
      switch (detector_index) {
      case DETECTOR_AKAZE:
            feature =  cv::AKAZE::create();
            break;
        
      case DETECTOR_BRISK:
            feature =  cv::BRISK::create();
            break;
        
      case DETECTOR_ORB:
            feature =  cv::ORB::create();  //use default argments
            break;
      default:
        break;
      } 

      std::vector<cv::KeyPoint> keypoints[IMAGES_COUNT]; 
      cv::Mat                   descriptors[IMAGES_COUNT];
      cv::Mat                   mats[IMAGES_COUNT];
    
      //Detect features and compute descriptors.
      for (int i = 0; i<IMAGES_COUNT; i++) {
        mats[i] = source_images[i].getOriginalImage();
        feature -> detect(mats[i], keypoints[i]);
        feature -> compute(mats[i], keypoints[i], descriptors[i]);
      }
      
      //Brute force matcher
      cv::BFMatcher    matcher(NORM_HAMMING, true);
    
      std::vector<cv::DMatch> matches;
      matcher.match(descriptors[FIRST], descriptors[SECOND], matches);

      int totalMatchedNumber = matches.size();
     
      if (besttop_number <= totalMatchedNumber) {
        std::nth_element(matches.begin(), matches.begin() + besttop_number - 1, matches.end());
        matches.erase(matches.begin() + besttop_number, matches.end());
      }
        
      int bestMatchedNumber = matches.size();
        
      Mat matchedImg;
      drawMatches(mats[FIRST],  keypoints[FIRST], 
                mats[SECOND], keypoints[SECOND], matches, matchedImg);
      
      matched_image.setMatched(matchedImg, scaling_ratio);
      
      set_matched_number(bestMatchedNumber, totalMatchedNumber);

    } catch (cv::Exception& ex) {
      //MessageBox(NULL, "Caught an excption", "Exception", MB_OK);
    }
  }
  void file_open1()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      filenames[FIRST] = file_dialog.get_filename();
      update_filepath();
      source_images[FIRST].loadImage(filenames[FIRST], loading_flag, scaling_ratio);
    }
  }

  void file_open2()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      filenames[SECOND] = file_dialog.get_filename();
      update_filepath();
      source_images[SECOND].loadImage(filenames[SECOND], loading_flag, scaling_ratio);
    }
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
    args.set(XmNheight,460);
    args.set(XmNscalingRatio,60);
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

