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
 *  ImageEroding.cpp
 *
 *****************************************************************************/

//
#include <es++/Pair.h>
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

  class TransformedImageView :public Es::OpenCVScrolledImageView {
  private:
    cv::Mat transformed_image;

  public:
    TransformedImageView()
    {
    }

    void loadImage(const std::string& filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scaling_ratio=100)
    {
      OpenCVScrolledImageView::loadImage(filename, imageLoadingFlag,
                                scaling_ratio);
 
      cv::Mat& original_image = getOriginalImage();

      transformed_image  = original_image.clone();
      scaleImage(transformed_image, scaling_ratio);
      
    }    

   void transform(int morph_shape, int ksize, int scaling_ratio)
    {
      cv::Mat& original_image = getOriginalImage();

      ksize = (ksize/2)*2 +1;

      //The transformedImage is scaled in the following way, and the scaled image
      //is displayed in this image view.
      cv::Mat element = getStructuringElement(morph_shape,
                       cv::Size( ksize, ksize),
                       cv::Point( -1, -1) );
      
      cv::erode(original_image, transformed_image, element);
      scaleImage(transformed_image, scaling_ratio);
    } 

    void rescale(int scaling_ratio)
    {
      OpenCVScrolledImageView::scaleImage(transformed_image, scaling_ratio);
    }
  };
  // Inner classes end.
  /////////////////////////////////////////////////////// 

  Es::Label              filepath;

  Es::HorizontalLayout   horiz_layout;

  Es::VerticalLayout     control_pane;

  OriginalImageView      original_image;

  TransformedImageView   transformed_image;

  int                    loading_flag;

  Es::OpenCVScaleComboBox scale_combobox;

  int                     morph_shape;
  Es::LabeledComboBox     morph_shape_combobox;

  int                     ksize;
  Es::LabeledTrackBar     ksize_trackbar;

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
    transformed_image.set_size_request(ww, h);
    control_pane.set_size_request( CONTROLPANE_WIDTH, h);

    horiz_layout.pack_start(original_image);
    horiz_layout.pack_start(transformed_image);

    horiz_layout.pack_start(control_pane, FALSE, FALSE, 0);

    scale_combobox.set_selection(scaling_ratio);
    scale_combobox.set_changed_callback(
             sigc::mem_fun(*this, &MainView::scale_changed) );

    const char* shapes[] = {
        "MORPH_RECT",   //0
        "MORPH_CROSS",  //1
        "MORPH_ELLIPSE" //2
      };
    morph_shape = cv::ADAPTIVE_THRESH_MEAN_C;
 
    morph_shape_combobox.set_label("MorphShape");
    morph_shape_combobox.append_items(shapes, CountOf(shapes));
    morph_shape_combobox.set_active_text(shapes[0]);

    morph_shape_combobox.set_changed_callback(
           sigc::mem_fun(*this, &MainView::morph_shape_changed));

    ksize = 10;
    ksize_trackbar.set_label("KernelSize: [0, 31]");
    ksize_trackbar.configure_adjuster(10.0f, 0.0f, 31.0f);
    ksize_trackbar.set_value_changed_callback(
            sigc::mem_fun(*this, &MainView::trackbar_value_changed) );

    control_pane.set_spacing(20);
    control_pane.pack_start(scale_combobox, Gtk::PACK_SHRINK);

    control_pane.pack_start(morph_shape_combobox, Gtk::PACK_SHRINK);
    control_pane.pack_start(ksize_trackbar, Gtk::PACK_SHRINK);

    loading_flag = IMREAD_COLOR;
    original_image.loadImage(filename, loading_flag, scaling_ratio);
    transformed_image.loadImage(filename, loading_flag, scaling_ratio);

    transformed_image.transform(morph_shape,
               ksize, scaling_ratio);
    show_all();
  }

  void scale_changed()
  {
    std::string scale = scale_combobox.get_active_text();
    printf("scale_changed %s\n", scale.c_str());
    scaling_ratio = scale_combobox.get_selection();

    original_image.rescale(scaling_ratio);
    transformed_image.rescale(scaling_ratio);
  }


  void morph_shape_changed()
  {
    Es::Pair<int, const char*> shapes[] =  {
      {MORPH_RECT,   "MORPH_RECT"},
      {MORPH_CROSS,   "MORPH_CROSS"},
      {MORPH_ELLIPSE, "MORPH_ELLIPSE"},
    }; 
    std::string text = morph_shape_combobox.get_active_text();
    const char* item = text.c_str();
    for (size_t i = 0; i<CountOf(shapes); i++) {
     if (strcmp(shapes[i].second, item) == 0) {
        morph_shape = shapes[i].first;
        break;
     } 
    }
    printf("morph_shape %s\n", text.c_str());
    transformed_image.transform(morph_shape, 
               ksize, scaling_ratio);
  }

  void trackbar_value_changed()
  {
    ksize = (int)ksize_trackbar.get_value();
    
    transformed_image.transform(morph_shape, 
               ksize, scaling_ratio);
  }

  void file_open()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string filename = file_dialog.get_filename();

      filepath.set_label(filename);

      original_image.loadImage(filename, loading_flag, scaling_ratio);
      transformed_image.loadImage(filename, loading_flag, scaling_ratio);

      transformed_image.transform(morph_shape, 
               ksize, scaling_ratio);
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

