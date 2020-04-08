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
 *  ImageSharpening.cpp
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

  class SharpenedImageView :public Es::OpenCVScrolledImageView {
  private:
    cv::Mat sharpened_image;

  public:
    SharpenedImageView()
    {
    }

    void loadImage(const std::string& filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scaling_ratio=100)
    {
      OpenCVScrolledImageView::loadImage(filename, imageLoadingFlag,
                                scaling_ratio);
 
      cv::Mat& original_image = getOriginalImage();
      sharpened_image  = original_image.clone();
      scaleImage(sharpened_image, scaling_ratio);
    }    

    void sharpen(int ksize, int sigma, int scaling_ratio)
    {
      cv::Mat& original_image = getOriginalImage();

      ksize = (ksize/2)*2 + 1;

      //void GaussianBlur(const Mat& src, Mat& dst, Size ksize, 
      //       double sigmaX, 
      //       double sigmaY  = 0, 
      //       int borderType = BORDER_DEFAULT)
      cv::Mat newImage = cv::Mat::zeros(original_image.size(), original_image.type() );
      cv::GaussianBlur(original_image, newImage, cv::Size(ksize, ksize), 
            (double)sigma, //sigmaX, 
            (double)sigma, //sigmaY
            BORDER_DEFAULT);
      
      // void addWeighted(InputArray src1, double alpha, InputArray src2, 
      //             double beta, double gamma, OutputArray dst, int dtype=-1)
      double alpha = 2.5;
      double beta  = 1.0 - alpha;
      cv::addWeighted(original_image, alpha, newImage, beta, 0.0, newImage);

      sharpened_image = newImage;
      scaleImage(sharpened_image, scaling_ratio);
    } 

    void rescale(int scaling_ratio)
    {
      OpenCVScrolledImageView::scaleImage(sharpened_image, scaling_ratio);
    }
  };
  // Inner classes end.
  /////////////////////////////////////////////////////// 

  Es::Label              filepath;

  Es::HorizontalLayout   horiz_layout;

  Es::VerticalLayout     control_pane;

  OriginalImageView      original_image;

  SharpenedImageView     sharpened_image;

  int                    loading_flag;

  Es::OpenCVScaleComboBox scale_combobox;

  Es::LabeledTrackBar     ksize_trackbar;
  int                     ksize;
 
  Es::LabeledTrackBar     sigma_trackbar;
  int                     sigma;
 
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

    filename = "../../images/GinzaWako.png";

    filepath.set_label(filename);
    int ww = (w - CONTROLPANE_WIDTH)/2;

    filepath.set_size_request(w, 30);
    original_image.set_size_request(ww, h);
    sharpened_image.set_size_request(ww, h);
    control_pane.set_size_request( CONTROLPANE_WIDTH, h);

    horiz_layout.pack_start(original_image);
    horiz_layout.pack_start(sharpened_image);

    horiz_layout.pack_start(control_pane, FALSE, FALSE, 0);

    scale_combobox.set_selection(scaling_ratio);

    ksize = 20;
    ksize_trackbar.set_label("KernelSize: [1, 31]");
    ksize_trackbar.configure_adjuster((double)ksize, 1.0f, 31.0f);
    ksize_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::ksize_value_changed) );


    sigma = 12;
    sigma_trackbar.set_label("Sigma: [0, 20]");
    sigma_trackbar.configure_adjuster((double)sigma, 0.0f, 20.0f);
    sigma_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::sigma_value_changed) );

    control_pane.set_spacing(20);
    control_pane.pack_start(scale_combobox, Gtk::PACK_SHRINK);
    control_pane.pack_start(ksize_trackbar, Gtk::PACK_SHRINK);
    control_pane.pack_start(sigma_trackbar, Gtk::PACK_SHRINK);

    scale_combobox.set_changed_callback(
             sigc::mem_fun(*this, &MainView::scale_changed) );

    loading_flag = IMREAD_COLOR;
    original_image.loadImage(filename, loading_flag, scaling_ratio);
    sharpened_image.loadImage(filename, loading_flag, scaling_ratio);

    sharpened_image.sharpen(ksize, sigma, scaling_ratio);

    show_all();
  }

  void scale_changed()
  {
    std::string scale = scale_combobox.get_active_text();
    printf("scale_changed %s\n", scale.c_str());
    scaling_ratio = scale_combobox.get_selection();

    original_image.rescale(scaling_ratio);
    sharpened_image.rescale(scaling_ratio);
  }

  void ksize_value_changed()
  {
    ksize = (int)ksize_trackbar.get_value();
    
    sharpened_image.sharpen(ksize, sigma, scaling_ratio);
  }

  void sigma_value_changed()
  {
    sigma = (int)sigma_trackbar.get_value();
    
    sharpened_image.sharpen(ksize, sigma, scaling_ratio);
  }

  void file_open()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string filename = file_dialog.get_filename();

      filepath.set_label(filename);

      original_image.loadImage(filename, loading_flag, scaling_ratio);
      sharpened_image.loadImage(filename, loading_flag, scaling_ratio);

      sharpened_image.sharpen(ksize, sigma, scaling_ratio);
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

