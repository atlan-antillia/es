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
 *  ObjectDetector.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-3.0/ZException.h>
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/FolderOpenDialog.h>
#include <es++/gtkmm-3.0/FileComboBox.h>
#include <es++/gtkmm-3.0/FileOpenDialog.h>
#include <es++/gtkmm-3.0/PushButton.h>

#include <es++/opencv-4.0/OpenCVMainView.h>
#include <es++/opencv-4.0/OpenCVScaleComboBox.h>
#include <es++/opencv-4.0/OpenCVScrolledImageView.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenCVMainView {
private:
  /////////////////////////////////////////////////////// 
  //Inner classes start.
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
      detected_image  = original_image.clone();
      scaleImage(detected_image, scaling_ratio);
    }    

    int detect(std::string& path, int scaling_ratio)
    {
      int detected_number = 0;  
      cv::Mat& original_image = getOriginalImage();
      try {
        CascadeClassifier classifier;
        classifier.load(path.c_str());
        cv::vector<Rect> faces;
        classifier.detectMultiScale(original_image, faces, 1.1, 3, 0, 
                 cv::Size(20,20));
        detected_number = faces.size();

        if (faces.size() == 0) {
        }
	for (int i = 0; i < faces.size(); i++){
          rectangle(original_image, Point(faces[i].x, faces[i].y), 
	       Point(faces[i].x + faces[i].width, 
                     faces[i].y + faces[i].height),
              Scalar(0, 200, 0), 3, CV_AA);
        }
      } catch (...) {
        throw IException("Failed to detect\n");
      }
      detected_image  = original_image.clone();
      scaleImage(detected_image, scaling_ratio);
      return detected_number;
    } 

    void rescale(int scaling_ratio)
    {
      OpenCVScrolledImageView::scaleImage(detected_image, scaling_ratio);
    }
  };
  // Inner class ends.
  /////////////////////////////////////////////////////// 

private:
  Es::HorizontalLayout  classifier_work_area;
    Es::FileComboBox       classifier_combobox;
    Es::PushButton         classifier_folder_open;
   std::string             classifier_directory;
   std::string             classifier_filepattern;   //*.xml

  Es::HorizontalLayout          center_work_area;
    DetectedImageView           image_view;
    Es::VerticalLayout           control_pane;
        Es::OpenCVScaleComboBox     scale_combobox;
        Es::PushButton              reload_button;
        Es::PushButton              detect_button;
  static int  const CONTROLPANE_WIDTH = 160;
 
  int                   loading_flag;
  int                   scaling_ratio;

  std::string           image_filename;

  Es::FolderOpenDialog  folder_dialog;
  Es::FileOpenDialog    file_dialog;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet, 
          std::string&      name,
          Es::Args&         args)
  :OpenCVMainView(applet, name, args)
  ,scaling_ratio(100)
  ,folder_dialog(*this)
  ,file_dialog(*this, Es::FileOpenDialog::IMAGE_FILES)
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);
    int ratio = (int)args.get(XmNscalingRatio);

    scaling_ratio = OpenCVImageView::validateScale(ratio);

    Es::MainLayout& main_layout = get_main_layout();

    image_filename = "../../images/WafukuMannequin.png";
    set_filenamed_title(image_filename);

    loading_flag = IMREAD_COLOR;
    image_view.loadImage(image_filename, loading_flag, scaling_ratio);
    
    int ww = w - CONTROLPANE_WIDTH;
    classifier_directory = "./";
    classifier_filepattern = "*.xml";

    setup_classifier_work_area(classifier_directory,
           classifier_filepattern);

    //classifier_work_area.set_size_request(w, 30);
    center_work_area.set_size_request(w, h);
    image_view.set_size_request(ww, h);
    control_pane.set_size_request(CONTROLPANE_WIDTH,  h);
 
    main_layout.pack_start(classifier_work_area, FALSE, FALSE, 0);
    main_layout.pack_start(center_work_area);

    center_work_area.pack_start(image_view);
    center_work_area.pack_start(control_pane, FALSE, FALSE, 0);

    setup_control_pane();

    scale_combobox.set_changed_callback(
             sigc::mem_fun(*this, &MainView::scale_changed) );
    scale_combobox.set_selection(scaling_ratio);

    show_all();
  }

  void setup_classifier_work_area(const std::string& directory,
            const std::string& pattern)
  {
    classifier_combobox.remove_all();

    classifier_combobox.set_label(directory);
    classifier_combobox.append(directory, pattern);
    classifier_combobox.set_changed_callback(
             sigc::mem_fun(*this, &MainView::classifier_combobox_changed) );

    classifier_folder_open.set_label("Classifier Folder");
    classifier_folder_open.set_clicked_callback(
             sigc::mem_fun(*this, &MainView::classifier_folder_open_clicked) );

    classifier_work_area.pack_start(classifier_combobox);
    classifier_work_area.pack_start(classifier_folder_open, Gtk::PACK_SHRINK);
  }

  void setup_control_pane()
  {
   control_pane.set_spacing(20);
    control_pane.pack_start(scale_combobox, Gtk::PACK_SHRINK);
    reload_button.set_label("Reload");
    reload_button.set_clicked_callback(
             sigc::mem_fun(*this, &MainView::reload_button_clicked) );
    control_pane.pack_start(reload_button, Gtk::PACK_SHRINK);

    detect_button.set_label("Detect");
    detect_button.set_clicked_callback(
             sigc::mem_fun(*this, &MainView::detect_button_clicked) );

    control_pane.pack_start(detect_button, Gtk::PACK_SHRINK);
  }


  void scale_changed()
  {
    std::string scale = scale_combobox.get_active_text();
    printf("scale_changed %s\n", scale.c_str());
    scaling_ratio = scale_combobox.get_selection();
    
    image_view.rescale(scaling_ratio);
  }

  void classifier_combobox_changed()
  {
    std::string scale = classifier_combobox.get_active_text();
    if (scale.empty()) {
     
    }
  }

  void classifier_folder_open_clicked()
  {
    printf("folder_open_button_clicked\n");

    try {
      int rc = folder_dialog.popup();

      if (rc == Gtk::RESPONSE_OK) {
        classifier_directory = folder_dialog.get_filename();
        classifier_combobox.set_label(classifier_directory);

        //The following line will cause file_combobox_changed callback.
        classifier_combobox.remove_all();

        classifier_combobox.append(classifier_directory, classifier_filepattern);
      }

    } catch (...) {
     ; //Do nothing here.
    }
  }

  void reload_button_clicked()
  {
    image_view.loadImage(image_filename, loading_flag, scaling_ratio); 
  }

  void detect_button_clicked()
  {
    try {
      std::string xmlfile = classifier_combobox.get_active_text();
      printf("detect_button_clicked %s\n", xmlfile.c_str());
       
      std::string classifier = classifier_directory;
      classifier += "/";
      classifier += xmlfile;
      int number = image_view.detect(classifier, scaling_ratio);
      if (number == 0) {
        Gtk::MessageDialog dialog(*this, "Message");
        dialog.set_title("Information");
        dialog.set_secondary_text("Nothing detected by detectMultiScale.");
        dialog.run();
      }
    } catch (Es::Exception& ex) {
      caught(ex);
    }
  }

  void file_open()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      image_filename = file_dialog.get_filename();
      set_filenamed_title(image_filename);

      image_view.loadImage(image_filename, loading_flag, scaling_ratio); 
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
    args.set(XmNwidth, 640);
    args.set(XmNheight, 460);
    args.set(XmNscalingRatio, 90); //60%

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

