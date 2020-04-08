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
 *  TemplateMatcher.cpp
 *
 *****************************************************************************/

//
#include <es++/Pair.h>
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/Label.h>
#include <es++/gtkmm-3.0/PushButton.h>
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
  //Inner class starts.
  class SourceImageView :public Es::OpenCVScrolledImageView {
  private:
    cv::Mat source_image;

  public:
    SourceImageView()
    {
    }

    void loadImage(const std::string& filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scaling_ratio=100)
    {
      OpenCVScrolledImageView::loadImage(filename, imageLoadingFlag,
                                scaling_ratio);
 
      cv::Mat& original_image = getOriginalImage();

      source_image  = original_image.clone();
      scaleImage(source_image, scaling_ratio);
    }    

    void clear(int scaling_ratio)
    {
      cv::Mat& original_image = getOriginalImage();

      source_image  = original_image.clone();
      scaleImage(source_image, scaling_ratio);
    }

    void setMatchedImage(cv::Mat& image, int scaling_ratio)
    {
      source_image = image; 
      scaleImage(source_image, scaling_ratio);
    } 

    void rescale(int scaling_ratio)
    {
      OpenCVScrolledImageView::scaleImage(source_image, scaling_ratio);
    }
  };
  // Inner class ends.
  /////////////////////////////////////////////////////// 

private:
  typedef enum {
       SQDIFF = 0,
       SQDIFF_NORMED,
       CCORR,
       CCORR_NORMED,
       CCOEFF,
       CCOEFF_NORMED,
  } MATCHING_METHOD;

  static const int       IMAGES_COUNT = 2;
  
  static const int       FIRST  = 0;

  static const int       SECOND = 1;

  Es::Label              filepath;

  Es::HorizontalLayout   horiz_layout;

  Es::VerticalLayout     control_pane;

  SourceImageView        source_image[IMAGES_COUNT];

  int                    loading_flag;

  Es::OpenCVScaleComboBox scale_combobox;

  int                     matching_method;
  Es::LabeledComboBox     matching_method_combobox;

  Es::PushButton          clear_button;

  Es::PushButton          match_button;

  int                     scaling_ratio;  //Percentage.
  static const int        CONTROLPANE_WIDTH = 180;

  std::string             filename[IMAGES_COUNT];
 
  Es::FileOpenDialog      file_dialog;
public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet, 
          std::string&      name,
          Es::Args&         args)
  :OpenCVMainView(applet, name, 
                    //We don't use the defaultFilePulldown menu.
                     args.set(XmNuseDefaultFilePulldown, false) )
  ,scaling_ratio(100)  //100%
  ,file_dialog(*this, FileOpenDialog::IMAGE_FILES)
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

    int ratio = (int)args.get(XmNscalingRatio);

    scaling_ratio = OpenCVImageView::validateScale(ratio);

    Es::MainLayout& main_layout = get_main_layout();

    main_layout.pack_start(filepath, FALSE, FALSE, 0);

    main_layout.pack_start(horiz_layout);

    filename[FIRST ] = "../../images/MaterializedToruses.png";
    filename[SECOND] = "../../images/GreenMaterializedTorus.png";

    update_filepath();

    int ww = (w - CONTROLPANE_WIDTH)/2;

    filepath.set_size_request(w, 30);
    source_image[FIRST ].set_size_request(ww, h);
    source_image[SECOND].set_size_request(ww, h);
    control_pane.set_size_request( CONTROLPANE_WIDTH, h);

    horiz_layout.pack_start(source_image[FIRST ]);
    horiz_layout.pack_start(source_image[SECOND]);

    horiz_layout.pack_start(control_pane, FALSE, FALSE, 0);

    scale_combobox.set_selection(scaling_ratio);

    //Create an matchingMethodComboBox in controlPane.
    const char* methods[] = {
               "SQDIFF",
               "SQDIFF_NORMED",
               "CCORR",
               "CCORR_NORMED",
               "CCOEFF",
               "CCOEFF_NORMED"
    };

    matching_method = SQDIFF;
 
    matching_method_combobox.set_label("Adaptive Method");
    matching_method_combobox.append_items(methods, CountOf(methods));
    matching_method_combobox.set_active_text(methods[0]);

    matching_method_combobox.set_changed_callback(
           sigc::mem_fun(*this, &MainView::matching_method_changed));

    clear_button.set_label("Clear");
    clear_button.set_clicked_callback(
             sigc::mem_fun(*this, &MainView::clear_button_clicked) );

    match_button.set_label("Match");
    match_button.set_clicked_callback(
             sigc::mem_fun(*this, &MainView::match_button_clicked) );

    control_pane.set_spacing(20);
    control_pane.pack_start(scale_combobox, Gtk::PACK_SHRINK);

    control_pane.pack_start(matching_method_combobox, Gtk::PACK_SHRINK);

    control_pane.pack_start(clear_button, Gtk::PACK_SHRINK);

    control_pane.pack_start(match_button, Gtk::PACK_SHRINK);

    scale_combobox.set_changed_callback(
             sigc::mem_fun(*this, &MainView::scale_changed) );

    loading_flag = IMREAD_COLOR;
    source_image[FIRST ].loadImage(filename[FIRST ], loading_flag, scaling_ratio);
    source_image[SECOND].loadImage(filename[SECOND], loading_flag, scaling_ratio);

    show_all();

    templateMatching();
  }

  void update_filepath()
  {
    std::string space = "    ";
    filepath.set_label(filename[FIRST] + space +  filename[SECOND]);
  }

  void scale_changed()
  {
    std::string scale = scale_combobox.get_active_text();
    printf("scale_changed %s\n", scale.c_str());
    scaling_ratio = scale_combobox.get_selection();

    source_image[FIRST ].rescale(scaling_ratio);
    source_image[SECOND].rescale(scaling_ratio);
  }

  void matching_method_changed()
  {
    matching_method = matching_method_combobox.get_active_row_number();
    printf("matching_method %d\n", matching_method);

    //Call templateMatching method.
    templateMatching();
  }

  void templateMatching()
  {
    try {
      static const Pair<MATCHING_METHOD, int> methods[] = {
       {SQDIFF,        CV_TM_SQDIFF},
       {SQDIFF_NORMED, CV_TM_SQDIFF_NORMED},
       {CCORR,         CV_TM_CCORR},
       {CCORR_NORMED,  CV_TM_CCORR_NORMED},
       {CCOEFF,        CV_TM_CCOEFF},
       {CCOEFF_NORMED, CV_TM_CCOEFF_NORMED},
      };
      int match_method = CV_TM_SQDIFF;
      for (int i = 0; i<CountOf(methods); i++) {
        if (matching_method == methods[i].first) {
          match_method = methods[i].second;
          break;
        }
      }

      cv::Mat& simage = source_image[FIRST ].getOriginalImage();
      cv::Mat& timage = source_image[SECOND].getOriginalImage();
 
      int result_cols = simage.cols - timage.cols + 1;
      int result_rows = simage.rows - timage.rows + 1;

      cv::Mat result;

      result.create( result_rows, result_cols, CV_32FC1 );

      cv::matchTemplate(simage, timage, result, match_method); 
      cv::normalize( result, result, 0, 1, NORM_MINMAX, -1, cv::Mat() );

      double minVal, maxVal; 
      cv::Point minLoc, maxLoc;
      cv::Point matchLoc;

      cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );

      if (match_method == TM_SQDIFF ||  match_method == TM_SQDIFF_NORMED ) { 
        matchLoc = minLoc; 
      } else { 
        matchLoc = maxLoc; 
      }

      cv::Mat matchedImage = simage.clone();

      cv::rectangle(matchedImage, matchLoc, 
            Point( matchLoc.x + timage.cols , matchLoc.y + timage.rows ), 
            CV_RGB(255, 0, 0), 3 );

      source_image[FIRST].setMatchedImage(matchedImage, scaling_ratio);
      
    } catch (cv::Exception& ex) {
    }
  }

  void file_open1()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      filename[FIRST] = file_dialog.get_filename();
      update_filepath();
      source_image[FIRST].loadImage(filename[FIRST], loading_flag, scaling_ratio);
      templateMatching();
    }
  }

  void file_open2()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      filename[SECOND] = file_dialog.get_filename();
      update_filepath();
      source_image[SECOND].loadImage(filename[SECOND], loading_flag, scaling_ratio);
      templateMatching();
    }
  }

  void clear_button_clicked()
  {
    source_image[FIRST].clear(scaling_ratio);
  }

  void match_button_clicked()
  {
    templateMatching();
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

