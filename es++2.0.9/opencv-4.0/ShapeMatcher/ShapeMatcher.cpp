/******************************************************************************
 *
 * Copyright (c) 2019 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
 *
 * Redistribution and use in binarized and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of binarized code must retain the above copyright
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
 *  ShapeMatcher.cpp
 *
 *****************************************************************************/

#include <es++/Pair.h>
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/FileOpenDialog.h>
#include <es++/gtkmm-3.0/Label.h>
#include <es++/gtkmm-3.0/LabeledComboBox.h>
#include <es++/gtkmm-3.0/LabeledTrackBar.h>
#include <es++/gtkmm-3.0/PushButton.h>
#include <es++/opencv-4.0/OpenCVMainView.h>
#include <es++/opencv-4.0/OpenCVScaleComboBox.h>
#include <es++/opencv-4.0/OpenCVScrolledImageView.h>
#include <stdlib.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenCVMainView {
private:
  /////////////////////////////////////////////////////
  //Inner classes start.
  class BinarizedImageView :public Es::OpenCVScrolledImageView {
  private:
    cv::Mat gray_image;
    cv::Mat binarized_image;

  public:
    BinarizedImageView()
    {
    }

    void loadImage(const std::string& filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scaling_ratio=100)
    {
      OpenCVScrolledImageView::loadImage(filename, imageLoadingFlag,
                                scaling_ratio);
 
      cv::Mat& original_image = getOriginalImage();
      cv::cvtColor(original_image, gray_image, COLOR_BGR2GRAY);

      binarized_image  = gray_image.clone();
      scaleImage(binarized_image, scaling_ratio);
    }    

    cv::Mat& getBinarizedImage()
    {
      return binarized_image;
    }

    void binarize(int threshold_type, 
               int threshold_value, int scaling_ratio)
    {
      try {
        cv::threshold( gray_image, binarized_image, threshold_value, 
             THRESHOLD_VALUE_MAX, threshold_type );

      } catch (cv::Exception& ex) {
        //Sometimes we get an exception; I don't know the reason why it happens.
      }      

      scaleImage(binarized_image, scaling_ratio);
    }

    void rescale(int scaling_ratio)
    {
      OpenCVScrolledImageView::scaleImage(binarized_image, scaling_ratio);
    }

  };

  class MatchedImageView :public Es::OpenCVScrolledImageView {
  private:
    cv::Mat matched_image; 

  public:
    MatchedImageView()
    {
    }

    void loadImage(const std::string& filename,
                int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scaling_ratio=100)
    {
      OpenCVScrolledImageView::loadImage(filename, imageLoadingFlag,
                                scaling_ratio);

      cv::Mat& original_image = getOriginalImage();

      matched_image  = original_image.clone();

      scaleImage(matched_image, scaling_ratio);
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
  static const int        IMAGES_COUNT = 2;
  static const int        FIRST  = 0;
  static const int        SECOND = 1;
  std::string             filenames[IMAGES_COUNT];
  
  Es::Label               filepath;

  Es::HorizontalLayout   horiz_layout;
    Es::VerticalLayout     view_layout;
      Es::HorizontalLayout   binarized_view_layout;
        BinarizedImageView        binarized_images[IMAGES_COUNT];
      MatchedImageView       matched_image;

    Es::VerticalLayout      control_pane;
      Es::OpenCVScaleComboBox combobox;

      Es::LabeledComboBox     threshold_type_combobox;

      Es::LabeledTrackBar     threshold_value_trackbar;

      Es::LabeledTrackBar     minimum_size_trackbar;

      Es::LabeledTrackBar     maximum_size_trackbar;

      Es::PushButton          match_button;

  int                     threshold_type_index;  

  int                     threshold_value;

  int                     minimum_size;

  int                     maximum_size;

  static const int        FILEPATH_HEIGHT   = 30; 

  static const int        CONTROLPANE_WIDTH = 200;

  int                     scaling_ratio;

  static const int        loading_flag = IMREAD_COLOR; 

  Es::FileOpenDialog      file_dialog;

  static const int THRESHOLD_VALUE_MAX = 255;

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
  ,binarized_view_layout(0)  //SPACING
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

    binarized_images[FIRST].set_size_request(ww, h/2);
    binarized_images[SECOND].set_size_request(ww, h/2);
    matched_image.set_size_request(w, h/2);

    control_pane.set_spacing(10);
    control_pane.set_size_request(CONTROLPANE_WIDTH, h);

    main_layout.pack_start(filepath, FALSE, FALSE, 0);
      main_layout.pack_start(horiz_layout);
        horiz_layout.pack_start(view_layout);
          view_layout.pack_start(binarized_view_layout);
            binarized_view_layout.pack_start(binarized_images[FIRST]);
            binarized_view_layout.pack_start(binarized_images[SECOND]);
          view_layout.pack_start(matched_image);
           
        horiz_layout.pack_start(control_pane);
        control_pane.pack_start(combobox, Gtk::PACK_SHRINK );
        match_button.set_label("Match");

        control_pane.pack_start(threshold_type_combobox, Gtk::PACK_SHRINK);
        control_pane.pack_start(threshold_value_trackbar, Gtk::PACK_SHRINK);
        control_pane.pack_start(minimum_size_trackbar, Gtk::PACK_SHRINK);
        control_pane.pack_start(maximum_size_trackbar, Gtk::PACK_SHRINK);
        control_pane.pack_start(match_button, Gtk::PACK_SHRINK);

    combobox.set_selection(scaling_ratio);
    combobox.set_changed_callback(sigc::mem_fun(*this, &MainView::scale_changed) );

    match_button.set_label("Match");
    match_button.set_clicked_callback(sigc::mem_fun(*this, 
                     &MainView::match_button_clicked) );

    //Create a threshold type comboBox in controlPane.
    const char* types[] = {
        "Binary",
        "Binary Inverted",
        "Threshold Truncated",
        "Threshold to Zero",
        "Threshold to Zero Inverted",
        //"Mask", 
        "Otsu", 
      };

    threshold_type_index  = 0;
    threshold_type_combobox.set_label("ThresholdType");

    threshold_type_combobox.append_items(types, CountOf(types));
    threshold_type_combobox.set_active_text(
                       types[threshold_type_index]);//Binary 

    threshold_type_combobox.set_changed_callback(sigc::mem_fun(*this, 
                     &MainView::threshold_type_changed) );

    threshold_value_trackbar.set_label("ThresholdValue:[0,255]");
    threshold_value = 60;
    threshold_value_trackbar.configure_adjuster(threshold_value,
                      0, 255);
    threshold_value_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::threshold_value_changed) );


    minimum_size = 11;
    minimum_size_trackbar.set_label("MatchMinSize:[10,100]"); 
    minimum_size_trackbar.configure_adjuster(minimum_size,
                      0, 100);
    minimum_size_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::minimum_size_changed) );

    maximum_size = 101;
      
    maximum_size_trackbar.set_label("MatchMaxSize:[100,400]"); 
    maximum_size_trackbar.configure_adjuster(maximum_size,
                      100, 400);
    maximum_size_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::maximum_size_changed) );

    filenames[FIRST]  = "../../images/CatImage.png";
    filenames[SECOND] = "../../images/CatFace.png";

    for (int i = 0; i<IMAGES_COUNT; i++) { 
      binarized_images[i].loadImage(filenames[i], loading_flag, scaling_ratio);
      binarized_images[i].binarize(
                                getThresholdType(threshold_type_index),
                                threshold_value,
                                scaling_ratio );
    }

    //binarized_images[SECOND].loadImage(filenames[SECOND], loading_flag, scaling_ratio);

    matched_image.loadImage(filenames[FIRST], loading_flag, scaling_ratio);
/*
    binarized_images[FIRST].binarize(
                                getThresholdType(threshold_type_index),
                                threshold_value,
                                scaling_ratio );

    binarized_images[SECOND].binarize(
                                getThresholdType(threshold_type_index),
                                threshold_value,
                                scaling_ratio );
*/

    update_filepath();
    show_all();
  }

  void scale_changed()
  {
    scaling_ratio = combobox.get_selection();
    printf("scale_changed %d\n", scaling_ratio);
    binarized_images[FIRST].rescale(scaling_ratio);
    binarized_images[SECOND].rescale(scaling_ratio);
    matched_image.rescale(scaling_ratio);
  }

  void update_filepath()
  {
    std::string space = "    ";
    filepath.set_label(filenames[FIRST] + space +  filenames[SECOND]);
  }

  static int getThresholdType(int index) 
  {
    int n = 0;
    //We don't include the THRESH_OTSU type
    Pair<int, int> types[] = {
      {n++, THRESH_BINARY     },
      {n++, THRESH_BINARY_INV },
      {n++, THRESH_TRUNC      },
      {n++, THRESH_TOZERO     },
      {n++, THRESH_TOZERO_INV },
      {n++, THRESH_OTSU       }, 
    };
    int type = THRESH_BINARY; 
    if (index >= 0 && index <n) {
      type = types[index].second;
    } 
    return type;
  }

  void threshold_type_changed()
  {
    int prev_threshold_type = threshold_type_index;
   
    threshold_type_index = threshold_type_combobox.get_active_row_number();
    printf("threshold_type_index %d\n", threshold_type_index);
    
    if (threshold_type_index != prev_threshold_type) {
      printf("Detector updated call match\n");
      match_button_clicked();
    }
  }

  void threshold_value_changed()
  {
    threshold_value = threshold_value_trackbar.get_value();
    match_button_clicked();
  }
  
  void minimum_size_changed()
  {
    minimum_size = minimum_size_trackbar.get_value();
    match_button_clicked();
  }

  void maximum_size_changed()
  {
    maximum_size = maximum_size_trackbar.get_value();
    match_button_clicked();
  }

  //Matching operation. 
  void match_button_clicked()
  {
    //Detect connectedCommponentsWithStats on destImage 
    Mat labels, stats, centroids; 
    cv::Mat& originalImage = binarized_images[FIRST ].getOriginalImage();
    cv::Mat& srcBin = binarized_images[FIRST ].getBinarizedImage();
    cv::Mat& tmpBin = binarized_images[SECOND].getBinarizedImage();

    cv::connectedComponentsWithStats(srcBin,
            labels, stats, centroids);
     
    int tw = tmpBin.rows;
    int th = tmpBin.cols;
       
    cv::Mat destImage = originalImage.clone();
 
    const double MATCHING_THRESHOLD = 0.005;

    cv::Rect minimum(0, 0, 0, 0);
    double MIN_SIMILARITY = 1.0;
    bool found = false;
 
    for(int i=0; i < stats.rows; i++)  { 
      int x = stats.at<int>(Point(0, i)); 
      int y = stats.at<int>(Point(1, i)); 
      int w = stats.at<int>(Point(2, i)); 
      int h = stats.at<int>(Point(3, i)); 
      cv::Rect rect(x, y, w, h);

      cv::Mat rectOfInterest = srcBin(rect); 
      double similarity = cv::matchShapes(tmpBin, 
               rectOfInterest, CV_CONTOURS_MATCH_I1, 0);

      if ( (w >= minimum_size || h >= minimum_size ) && 
           (w <= maximum_size || h <= maximum_size )) { 
        if (similarity <= MIN_SIMILARITY) {
          MIN_SIMILARITY = similarity;       
          minimum = rect;
            printf("matching similarity=%.04f  x=%d y=%d w=%d h=%d\n", 
             similarity, x, y, w, h);
          found = true;
        }
      }
    } 

    if (found) {
      printf("found \n");
      cv::rectangle(destImage, minimum, CV_RGB(255, 0, 0), 3);
    }

    matched_image.setMatched(destImage, scaling_ratio); 

  }

  void file_open1()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      filenames[FIRST] = file_dialog.get_filename();
      update_filepath();
      binarized_images[FIRST].loadImage(filenames[FIRST], loading_flag, scaling_ratio);
      binarized_images[FIRST].binarize(
                                getThresholdType(threshold_type_index),
                                threshold_value,
                                scaling_ratio );
    }
  }

  void file_open2()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      filenames[SECOND] = file_dialog.get_filename();
      update_filepath();
      binarized_images[SECOND].loadImage(filenames[SECOND], loading_flag, scaling_ratio);
      binarized_images[SECOND].binarize(
                                getThresholdType(threshold_type_index),
                                threshold_value,
                                scaling_ratio );
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

