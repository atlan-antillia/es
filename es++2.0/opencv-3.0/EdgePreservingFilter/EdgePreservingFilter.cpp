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
 *  EdgePreservingFilter.cpp
 *
 *****************************************************************************/

//
//photo.hpp

/** @brief Filtering is the fundamental operation in image and video processing. Edge-preserving smoothing
filters are used in many different applications @cite EM11 .

@param src Input 8-bit 3-channel image.
@param dst Output 8-bit 3-channel image.
@param flags Edge preserving filters:
-   **RECURS_FILTER** = 1
-   **NORMCONV_FILTER** = 2
@param sigma_s Range between 0 to 200.
@param sigma_r Range between 0 to 1.

CV_EXPORTS_W void edgePreservingFilter(InputArray src, OutputArray dst, int flags = 1,
        float sigma_s = 60, float sigma_r = 0.4f);

*/

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

  class FilteredImageView :public Es::OpenCVScrolledImageView {
  private:
    cv::Mat filtered_image;

  public:
    FilteredImageView()
    {
    }

    void loadImage(const std::string& filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scaling_ratio=100)
    {
      OpenCVScrolledImageView::loadImage(filename, imageLoadingFlag,
                                scaling_ratio);
 
      cv::Mat& original_image = getOriginalImage();
      filtered_image  = original_image.clone();
      scaleImage(filtered_image, scaling_ratio);
      
    }    

    void filter(int ksize, int sigmacolor, int sigmaspace, int scaling_ratio)
    {
      cv::Mat& original_image = getOriginalImage();
      ksize = (ksize/2)*2 + 1;

      try {
        int flag = RECURS_FILTER ; //NORMCONV_FILTER is very slow;
        cv::edgePreservingFilter(
            original_image, 
            filtered_image, 
            flag,
            (double)sigmaspace, 
            (double)sigmacolor/100.0f);

        scaleImage(filtered_image, scaling_ratio);

      } catch (cv::Exception& ex) {
        ; //Ignore
      }
    }

    void rescale(int scaling_ratio)
    {
      OpenCVScrolledImageView::scaleImage(filtered_image, scaling_ratio);
    }
  };
  // Inner classes end.
  /////////////////////////////////////////////////////// 

  Es::Label               filepath;

  Es::HorizontalLayout    horiz_layout;

  Es::VerticalLayout      control_pane;

  OriginalImageView       original_image;

  FilteredImageView       filtered_image;

  int                     loading_flag;

  Es::OpenCVScaleComboBox scale_combobox;

  Es::LabeledTrackBar     ksize_trackbar;
  int                     ksize;
 
  Es::LabeledTrackBar     sigmacolor_trackbar;
  int                     sigmacolor;

  Es::LabeledTrackBar     sigmaspace_trackbar;
  int                     sigmaspace;

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

    filename = "../../images/flower.png";

    filepath.set_label(filename);
    int ww = (w - CONTROLPANE_WIDTH)/2;

    filepath.set_size_request(w, 30);
    original_image.set_size_request(ww, h);
    filtered_image.set_size_request(ww, h);
    control_pane.set_size_request( CONTROLPANE_WIDTH, h);

    horiz_layout.pack_start(original_image);
    horiz_layout.pack_start(filtered_image);

    horiz_layout.pack_start(control_pane, FALSE, FALSE, 0);

    scale_combobox.set_selection(scaling_ratio);

    ksize = 3;
    ksize_trackbar.set_label("KernelSize: [1, 31]");
    ksize_trackbar.configure_adjuster((double)ksize, 1.0f, 31.0f);
    ksize_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::ksize_value_changed) );


    sigmacolor = 160;
    sigmacolor_trackbar.set_label("SigmaColor: [0, 200]");
    sigmacolor_trackbar.configure_adjuster((double)sigmacolor, 0.0f, 200.0f);
    sigmacolor_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::sigmacolor_value_changed) );

    sigmaspace = 40;
    sigmaspace_trackbar.set_label("SigmaColor: [0, 100]");
    sigmaspace_trackbar.configure_adjuster((double)sigmaspace, 0.0f, 100.0f);
    sigmaspace_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::sigmaspace_value_changed) );

    control_pane.set_spacing(20);
    control_pane.pack_start(scale_combobox, Gtk::PACK_SHRINK);
    control_pane.pack_start(ksize_trackbar, Gtk::PACK_SHRINK);
    control_pane.pack_start(sigmacolor_trackbar, Gtk::PACK_SHRINK);
    control_pane.pack_start(sigmaspace_trackbar, Gtk::PACK_SHRINK);

    scale_combobox.set_changed_callback(
             sigc::mem_fun(*this, &MainView::scale_changed) );

    loading_flag = IMREAD_COLOR;
    original_image.loadImage(filename, loading_flag, scaling_ratio);
    filtered_image.loadImage(filename, loading_flag, scaling_ratio);

    filtered_image.filter(ksize, sigmacolor, sigmaspace, scaling_ratio);

    show_all();
  }

  void scale_changed()
  {
    std::string scale = scale_combobox.get_active_text();
    printf("scale_changed %s\n", scale.c_str());
    scaling_ratio = scale_combobox.get_selection();

    original_image.rescale(scaling_ratio);
    filtered_image.rescale(scaling_ratio);
  }

  void ksize_value_changed()
  {
    ksize = (int)ksize_trackbar.get_value();
    
    filtered_image.filter(ksize, sigmacolor, sigmaspace, scaling_ratio);
  }

  void sigmacolor_value_changed()
  {
    sigmacolor = (int)sigmacolor_trackbar.get_value();

    filtered_image.filter(ksize, sigmacolor, sigmaspace, scaling_ratio);
  }

  void sigmaspace_value_changed()
  {
    sigmaspace = (int)sigmaspace_trackbar.get_value();

    filtered_image.filter(ksize, sigmacolor, sigmaspace, scaling_ratio);
  }

  void file_open()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string filename = file_dialog.get_filename();

      filepath.set_label(filename);

      original_image.loadImage(filename, loading_flag, scaling_ratio);
      filtered_image.loadImage(filename, loading_flag, scaling_ratio);

      filtered_image.filter(ksize, sigmacolor, sigmaspace, scaling_ratio);
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

