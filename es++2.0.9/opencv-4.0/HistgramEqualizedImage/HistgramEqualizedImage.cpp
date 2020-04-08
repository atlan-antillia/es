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
 *  HistgramEqualizedImage.cpp
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
  typedef enum {
    COLOR_MODEL_HSV = 0,
    COLOR_MODEL_YCRCB
  } COLOR_MODEL;

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

  class HistgrammedImageView :public Es::OpenCVScrolledImageView {
  private:
    cv::Mat dest_image;

  public:
    HistgrammedImageView()
    {
    }

    void loadImage(const std::string& filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scaling_ratio=100)
    {
      OpenCVScrolledImageView::loadImage(filename, imageLoadingFlag,
                                scaling_ratio);
 
      cv::Mat& original_image = getOriginalImage();
      dest_image = original_image.clone();

      scaleImage(dest_image, scaling_ratio);
    }    

    void equalize_histgram(int colorModel, int scaling_ratio)
    {
      try {
        cv::Mat& original_image = getOriginalImage();
        int equalizeChannel  = 0;
        int toNewColorFormat = cv::COLOR_BGR2HSV;
        int toOldColorFormat = cv::COLOR_HSV2BGR;
        
        switch (colorModel) {
        case COLOR_MODEL_HSV:
            equalizeChannel  = 2;
            toNewColorFormat = cv::COLOR_BGR2HSV;
            toOldColorFormat = cv::COLOR_HSV2BGR;
          break;
          
        case COLOR_MODEL_YCRCB:
            equalizeChannel = 0;
            toNewColorFormat = cv::COLOR_BGR2YCrCb;
            toOldColorFormat = cv::COLOR_YCrCb2BGR;          
          break;
          
        default:
          break;
        }
    
        cv::Mat colorModeledImage;

        //Convert color-format from BGR to HSV or YCrCB
        cvtColor(original_image, colorModeledImage, toNewColorFormat);
        
        //Split ycrcbImage to three channles.
        std::vector<Mat> channels(3);
        
        cv::split(colorModeledImage, channels);
    
        //Equalize a channel of channels specified equalizedChannel index.
        cv::equalizeHist(channels[equalizeChannel], channels[equalizeChannel]);
    
        //Merge three channels images to ycrcbImage
        cv::merge(channels, colorModeledImage);
        
        //Convert color-format HSV orYCrCb to  BGR
        cv::cvtColor(colorModeledImage, dest_image, toOldColorFormat);
 
        scaleImage(dest_image, scaling_ratio);
      } catch (cv::Exception& ex) {
        ; //
      }
    } 

    void rescale(int scaling_ratio)
    {
      OpenCVScrolledImageView::scaleImage(dest_image, scaling_ratio);
    }
  };
  // Inner classes end.
  /////////////////////////////////////////////////////// 

  Es::Label              filepath;

  Es::HorizontalLayout   horiz_layout;

  Es::VerticalLayout     control_pane;

  OriginalImageView      original_image;

  HistgrammedImageView   histgrammed_image;

  int                    loading_flag;

  Es::OpenCVScaleComboBox scale_combobox;

  int                     color_model;
  Es::LabeledComboBox     color_model_combobox;

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
    histgrammed_image.set_size_request(ww, h);
    control_pane.set_size_request( CONTROLPANE_WIDTH, h);

    horiz_layout.pack_start(original_image);
    horiz_layout.pack_start(histgrammed_image);

    horiz_layout.pack_start(control_pane, FALSE, FALSE, 0);

    scale_combobox.set_selection(scaling_ratio);
    scale_combobox.set_changed_callback(
             sigc::mem_fun(*this, &MainView::scale_changed) );

    const char* models[] = {
      "HSV",
      "YCrCb",
    };
    color_model = COLOR_MODEL_HSV;
 
    color_model_combobox.set_label("ColorModel");
    color_model_combobox.append_items(models, CountOf(models));
    color_model_combobox.set_active_text(models[0]);

    color_model_combobox.set_changed_callback(
           sigc::mem_fun(*this, &MainView::color_model_changed));

    control_pane.set_spacing(20);
    control_pane.pack_start(scale_combobox, Gtk::PACK_SHRINK);

    control_pane.pack_start(color_model_combobox, Gtk::PACK_SHRINK);

    loading_flag = IMREAD_COLOR;
    original_image.loadImage(filename, loading_flag, scaling_ratio);
    histgrammed_image.loadImage(filename, loading_flag, scaling_ratio);

    histgrammed_image.equalize_histgram(color_model, 
               scaling_ratio);
    show_all();
  }

  void scale_changed()
  {
    std::string scale = scale_combobox.get_active_text();
    printf("scale_changed %s\n", scale.c_str());
    scaling_ratio = scale_combobox.get_selection();

    original_image.rescale(scaling_ratio);
    histgrammed_image.rescale(scaling_ratio);
  }


  void color_model_changed()
  {
    Es::Pair<int, const char*> models[] =  {
      {COLOR_MODEL_HSV,   "HSV"},
      {COLOR_MODEL_YCRCB, "YCrCb"},
    };
    std::string text = color_model_combobox.get_active_text();
    const char* item = text.c_str();
    for (size_t i = 0; i<CountOf(models); i++) {
     if (strcmp(models[i].second, item) == 0) {
        color_model = models[i].first;
        break;
     } 
    }
    printf("color_model %s\n", text.c_str());
    histgrammed_image.equalize_histgram(color_model, 
               scaling_ratio);
  }

  void file_open()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string filename = file_dialog.get_filename();

      filepath.set_label(filename);

      original_image.loadImage(filename, loading_flag, scaling_ratio);
      histgrammed_image.loadImage(filename, loading_flag, scaling_ratio);

      histgrammed_image.equalize_histgram(color_model, 
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

