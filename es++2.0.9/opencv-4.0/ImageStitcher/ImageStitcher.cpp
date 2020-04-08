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
 *  ImageStitchr.cpp
 *
 *****************************************************************************/

//
// 2019/04/06
#include <opencv2/stitching.hpp>

#include <es++/opencv-4.0/OpenCVMainView.h>
#include <es++/opencv-4.0/OpenCVScaleComboBox.h>
#include <es++/opencv-4.0/OpenCVScrolledImageView.h>

#include <es++/Pair.h>
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/FileOpenDialog.h>
#include <es++/gtkmm-3.0/Label.h>
#include <es++/gtkmm-3.0/PushButton.h>

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

  class StitchedImageView :public Es::OpenCVScrolledImageView {
  private:
    cv::Mat stitched_image; 

  public:
    StitchedImageView()
    {
    }

    void rescale(int scaling_ratio)
    {
      OpenCVScrolledImageView::scaleImage(stitched_image, scaling_ratio);
    }

    void set_stitched_image(cv::Mat& mat, int scaling_ratio)
    {
      stitched_image = mat.clone();
      scaleImage(stitched_image, scaling_ratio);
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
      Es::HorizontalLayout   source_view_layout;
        SourceImageView        source_images[IMAGES_COUNT];
      StitchedImageView       stitched_image;

    Es::VerticalLayout      control_pane;
      Es::OpenCVScaleComboBox combobox;

      Es::LabeledComboBox     stitcher_mode_combobox;

      Es::PushButton          stitch_button;

  int                     stitcher_mode_index;  

  cv::Ptr<cv::Stitcher>     stitcher;

  cv::Stitcher::Mode      mode;

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
    stitched_image.set_size_request(w, h/2);

    control_pane.set_spacing(10);
    control_pane.set_size_request(CONTROLPANE_WIDTH, h);

    main_layout.pack_start(filepath, FALSE, FALSE, 0);
      main_layout.pack_start(horiz_layout);
        horiz_layout.pack_start(view_layout);
          view_layout.pack_start(source_view_layout);
            source_view_layout.pack_start(source_images[FIRST]);
            source_view_layout.pack_start(source_images[SECOND]);
          view_layout.pack_start(stitched_image);
           
        horiz_layout.pack_start(control_pane);
        control_pane.pack_start(combobox, Gtk::PACK_SHRINK );
        control_pane.pack_start(stitcher_mode_combobox, Gtk::PACK_SHRINK);
        control_pane.pack_start(stitch_button, Gtk::PACK_SHRINK);
        stitch_button.set_label("Stitch");

    combobox.set_selection(scaling_ratio);
    combobox.set_changed_callback(sigc::mem_fun(*this, &MainView::scale_changed) );

    stitch_button.set_clicked_callback(sigc::mem_fun(*this, 
                     &MainView::stitch_button_clicked) );

    const char* types[] =  {
        "Panorama",
        "Scans",
    };
    stitcher_mode_combobox.set_label("Detector");
    stitcher_mode_combobox.append_items(types, CountOf(types));
    stitcher_mode_combobox.set_active_text(types[0]); //Panorama

    stitcher_mode_combobox.set_changed_callback(sigc::mem_fun(*this, 
                     &MainView::stitcher_mode_changed) );


    stitcher_mode_index  = 0;

    //7 Create an instance of cv::Stitcher.
    mode = cv::Stitcher::PANORAMA;
    stitcher = cv::Stitcher::create(mode);

    filenames[FIRST]  = "../../images/Lake2.png";
    filenames[SECOND] = "../../images/Lake1.png";

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
    stitched_image.rescale(scaling_ratio);
  }

  void update_filepath()
  {
    std::string space = "    ";
    filepath.set_label(filenames[FIRST] + space +  filenames[SECOND]);
  }

  cv::Stitcher::Mode get_stitcher_mode(int index) 
  {
    int n = 0;
    Pair<int, cv::Stitcher::Mode> types[] = {
      {n++, cv::Stitcher::PANORAMA  },
      {n++, cv::Stitcher::SCANS     },
    };
    mode = cv::Stitcher::PANORAMA; 
    if (index >= 0 && index <n) {
      mode = types[index].second;
    } 
    return mode;
  }

  void stitcher_mode_changed()
  {
    int prev_index = stitcher_mode_index; 
    stitcher_mode_index = stitcher_mode_combobox.get_active_row_number();
    printf("stitcher_mode_index %d \n", stitcher_mode_index);
    if (stitcher_mode_index != prev_index) {
      mode = get_stitcher_mode(stitcher_mode_index);
      //Rebuild stitcher.
      stitcher = cv::Stitcher::create(mode);
      stitch_images();
    }
  }

  
  //When the stitch button is clicked.
  void stitch_button_clicked()
  {
    //call stitch_images method
    stitch_images();
  }

/*
enum   Status {  
  OK = 0, 
   ERR_NEED_MORE_IMGS = 1, 
   ERR_HOMOGRAPHY_EST_FAIL = 2, 
   ERR_CAMERA_PARAMS_ADJUST_FAIL = 3 
  */

  const char* getErrorMessage(cv::Stitcher::Status status)
  {
    static const Pair<const char*,  cv::Stitcher::Status> errors[] = {
      {"Need more images",          cv::Stitcher::Status::ERR_NEED_MORE_IMGS},
      {"Homography est fail",       cv::Stitcher::Status::ERR_HOMOGRAPHY_EST_FAIL},
      {"Camera params adjust fail", cv::Stitcher::Status::ERR_CAMERA_PARAMS_ADJUST_FAIL},
    };
    const char* err = "";
    for (int i = 0; i <CountOf(errors); i++) {
      if (status == errors[i].second) {
        err = errors[i].first;
        break;
      }
    }
    return err;
  }
  
  void stitch_images()
  {
    try {
      std::vector<cv::Mat> images;
      images.push_back( source_images[FIRST  ].getOriginalImage() );
      images.push_back( source_images[SECOND ].getOriginalImage() );

      cv::Mat panorama;
      cv::Stitcher::Status status = stitcher->stitch(images, panorama);
      printf("status %d¥n", status);

      if (status == cv::Stitcher::Status::OK) {
        stitched_image.set_stitched_image(panorama, scaling_ratio);
      } else {
        const char* err = getErrorMessage(status);
        throw IException("Failed to Stitcher::stitch method error=%s", err);
      }
      
    } catch (Es::Exception& ex) {
      caught(ex);
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

