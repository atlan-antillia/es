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
 *  AdaptiveImageThresholding.cpp
 *
 *****************************************************************************/

//
#include <es++/Pair.h>
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

  class BinarizedImageView :public Es::OpenCVScrolledImageView {
  private:
    cv::Mat gray_image;
    cv::Mat binarized_image;
    static const int MAX_PIXEL_VALUE = 255;
    static const int C               = 9;    //Constant subtracted from the mean or weighted mean
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

   void binarize(int adaptive_method, int threshold_type, 
               int block_size, int scaling_ratio)
    {
      block_size = (block_size/2)*2 + 1;

      try {
        block_size = (block_size/2)*2 + 1;
        if (block_size <3) {
          block_size=3;
        }
        
        cv::adaptiveThreshold(gray_image, binarized_image, MAX_PIXEL_VALUE, 
            adaptive_method, threshold_type, block_size,  (double)C);
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
  // Inner classes end.
  /////////////////////////////////////////////////////// 

  Es::Label              filepath;

  Es::HorizontalLayout   horiz_layout;

  Es::VerticalLayout     control_pane;

  OriginalImageView      original_image;

  BinarizedImageView     binarized_image;

  int                    loading_flag;

  Es::OpenCVScaleComboBox scale_combobox;

  int                     adaptive_method;
  Es::LabeledComboBox     adaptive_method_combobox;

  int                     threshold_type;
  Es::LabeledComboBox     threshold_type_combobox;
  
  static const int        BLOCK_SIZE_MIN = 3;
  static const int        BLOCK_SIZE_MAX = 43;

  int                     block_size;
  Es::LabeledTrackBar     block_size_trackbar;

 
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
  ,file_dialog(*this, FileOpenDialog::IMAGE_FILES)
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);

    int ratio = (int)args.get(XmNscalingRatio);

    scaling_ratio = OpenCVImageView::validateScale(ratio);

    Es::MainLayout& main_layout = get_main_layout();

    main_layout.pack_start(filepath, FALSE, FALSE, 0);

    main_layout.pack_start(horiz_layout);

    filename = "../../images/flower.png";

    filepath.set_label(filename);
    int ww = (w - CONTROLPANE_WIDTH)/2;

    filepath.set_size_request(w, 30);
    original_image.set_size_request(ww, h);
    binarized_image.set_size_request(ww, h);
    control_pane.set_size_request( CONTROLPANE_WIDTH, h);

    horiz_layout.pack_start(original_image);
    horiz_layout.pack_start(binarized_image);

    horiz_layout.pack_start(control_pane, FALSE, FALSE, 0);

    scale_combobox.set_selection(scaling_ratio);

    //Create an adaptiveMethodComboBox in controlPane.
    const char* methods[] = {
        "Adaptive_Thresh Mean_C",
        "Adaptive_Thresh_Gaussian_C",
    };

    adaptive_method = cv::ADAPTIVE_THRESH_MEAN_C;
 
    adaptive_method_combobox.set_label("Adaptive Method");
    adaptive_method_combobox.append_items(methods, CountOf(methods));
    adaptive_method_combobox.set_active_text(methods[0]);

    adaptive_method_combobox.set_changed_callback(
           sigc::mem_fun(*this, &MainView::adaptive_method_changed));

    const char* types[] = {
        "Binary",
        "Binary Inverted",
    };
    threshold_type = cv::THRESH_BINARY;
    threshold_type_combobox.set_label("Threshold Type");
    threshold_type_combobox.append_items(types, CountOf(types));
    threshold_type_combobox.set_active_text(types[0]);

    threshold_type_combobox.set_changed_callback(
           sigc::mem_fun(*this, &MainView::threshold_type_changed));
     
    block_size = 10;
    block_size_trackbar.set_label("BlockSize: [3, 43]");
    block_size_trackbar.configure_adjuster(10.0f, 3.0f, 43.0f);
    block_size_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::trackbar_value_changed) );

    control_pane.set_spacing(20);
    control_pane.pack_start(scale_combobox, Gtk::PACK_SHRINK);

    control_pane.pack_start(adaptive_method_combobox, Gtk::PACK_SHRINK);
    control_pane.pack_start(threshold_type_combobox, Gtk::PACK_SHRINK);
    control_pane.pack_start(block_size_trackbar, Gtk::PACK_SHRINK);

    scale_combobox.set_changed_callback(
             sigc::mem_fun(*this, &MainView::scale_changed) );

    loading_flag = IMREAD_COLOR;
    original_image.loadImage(filename, loading_flag, scaling_ratio);
    binarized_image.loadImage(filename, loading_flag, scaling_ratio);

    binarized_image.binarize(adaptive_method, threshold_type, 
               block_size, scaling_ratio);
    show_all();
  }

  void scale_changed()
  {
    std::string scale = scale_combobox.get_active_text();
    printf("scale_changed %s\n", scale.c_str());
    scaling_ratio = scale_combobox.get_selection();

    original_image.rescale(scaling_ratio);
    binarized_image.rescale(scaling_ratio);
  }


  void adaptive_method_changed()
  {
    Es::Pair<int, const char*> methods[] =  {
      {cv::ADAPTIVE_THRESH_MEAN_C, "Adaptive_Thresh Mean_C"},
      {cv::ADAPTIVE_THRESH_GAUSSIAN_C, "Adaptive_Thresh_Gaussian_C"},
    };
    std::string text = adaptive_method_combobox.get_active_text();
    const char* item = text.c_str();
    for (size_t i = 0; i<CountOf(methods); i++) {
     if (strcmp(methods[i].second, item) == 0) {
        adaptive_method = methods[i].first;
        break;
     } 
    }
    printf("adaptive_method %s\n", text.c_str());
    binarized_image.binarize(adaptive_method, threshold_type, 
               block_size, scaling_ratio);
  }

  void threshold_type_changed()
  {
    Es::Pair<int, const char*> types[] = {
      {cv::THRESH_BINARY, "Binary"},
      {cv::THRESH_BINARY_INV, "Binary Inverted"},
    };

    std::string text = threshold_type_combobox.get_active_text();
    const char* item = text.c_str();
    for (size_t i = 0; i<CountOf(types); i++) {
      if (strcmp(types[i].second, item) == 0) {
        threshold_type = types[i].first;
      } 
    }
    printf("threshold_type %s\n", text.c_str());
    binarized_image.binarize(adaptive_method, threshold_type, 
               block_size, scaling_ratio);
  }

  void trackbar_value_changed()
  {
    block_size = (int)block_size_trackbar.get_value();
    
    binarized_image.binarize(adaptive_method, threshold_type, 
               block_size, scaling_ratio);
  }

  void file_open()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string filename = file_dialog.get_filename();

      filepath.set_label(filename);

      original_image.loadImage(filename, loading_flag, scaling_ratio);
      binarized_image.loadImage(filename, loading_flag, scaling_ratio);

      binarized_image.binarize(adaptive_method, threshold_type, 
               block_size, scaling_ratio);
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

