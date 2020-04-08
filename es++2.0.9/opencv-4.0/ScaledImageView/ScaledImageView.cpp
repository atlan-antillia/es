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
 *  ScaledImageView.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/HorizontalLayout.h>
#include <es++/gtkmm-3.0/VerticalLayout.h>
#include <es++/gtkmm-3.0/FileOpenDialog.h>
#include <es++/opencv-4.0/OpenCVMainView.h>
#include <es++/opencv-4.0/OpenCVScrolledImageView.h>
#include <es++/opencv-4.0/OpenCVScaleComboBox.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenCVMainView {
private:
  Es::HorizontalLayout horizLayout;

  Es::VerticalLayout   controlPane;

  Es::OpenCVScrolledImageView  imageView;
  int                  loadingFlag;

  Es::OpenCVScaleComboBox scaleComboBox;
 
  int                 scalingRatio;  //Percentage.
  static const int    CONTROLPANE_WIDTH = 180;

  Es::FileOpenDialog  file_dialog;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet, 
          std::string&      name,
          Es::Args&         args)
  :OpenCVMainView(applet, name, args)
  ,scalingRatio(100)  //100%
  ,file_dialog(*this, Es::FileOpenDialog::IMAGE_FILES)
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);

    int ratio = (int)args.get(XmNscalingRatio);

    scalingRatio = OpenCVImageView::validateScale(ratio);

    Es::MainLayout& main_layout = get_main_layout();
    main_layout.pack_start(horizLayout);

    const std::string filename = "../../images/flower1.jpg";
    set_filenamed_title(filename);

    imageView.set_size_request(w - CONTROLPANE_WIDTH, h);
    controlPane.set_size_request( CONTROLPANE_WIDTH, h);

    horizLayout.pack_start(imageView);
    horizLayout.pack_start(controlPane, FALSE, FALSE, 0);

    scaleComboBox.set_selection(scalingRatio);

    controlPane.pack_start(scaleComboBox);

    scaleComboBox.set_changed_callback(
             sigc::mem_fun(*this, &MainView::scale_changed) 
             );
    loadingFlag = IMREAD_COLOR;
    imageView.loadImage(filename, loadingFlag, scalingRatio);

    show_all();
  }

  void scale_changed()
  {
    std::string scale = scaleComboBox.get_active_text();
    printf("scale_changed %s\n", scale.c_str());
    scalingRatio = scaleComboBox.get_selection();

    imageView.scaleImage(scalingRatio);
  }

  void file_open()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string filename = file_dialog.get_filename();
      set_filenamed_title(filename);

      imageView.loadImage(filename, loadingFlag, scalingRatio);
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

