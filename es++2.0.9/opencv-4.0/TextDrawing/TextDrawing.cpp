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
 *  TextDrawing.cpp
 *
 *****************************************************************************/

//
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/FileOpenDialog.h>
#include <es++/gtkmm-3.0/PushButton.h>
#include <es++/opencv-4.0/OpenCVMainView.h>
#include <es++/opencv-4.0/OpenCVImageView.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenCVMainView {
private:
  //Inner class start
  class SimpleView: public OpenCVImageView {
  private:
    cv::Mat sharpenedImage; 

  public:
    SimpleView()
    {
    } 

    void loadImage(const std::string& filename, 
		int imageLoadingFlag= CV_LOAD_IMAGE_COLOR,
                int scaling_ratio=100)
    {
      try {
        OpenCVImageView::loadImage(filename, imageLoadingFlag,
                                scaling_ratio);
 
        cv::Mat& original_image = getOriginalImage();

        sharpenedImage  = original_image.clone();
        int ksize = 7;
        int sigma = 11;
        sharpen(ksize, sigma);
        drawText(sharpenedImage, "Hello world");
        scaleImage(sharpenedImage, 100);  

        refresh();
      } catch (Es::Exception& ex) {
        caught(ex);
      }
    }    

    void drawText(cv::Mat& image, const char* string)
    {
       std::string text = string;
       int fontFace = CV_FONT_HERSHEY_COMPLEX ;//FONT_HERSHEY_SCRIPT_SIMPLEX;
       double fontScale = 2;
       int thickness    = 2;
       int baseline     = 0;
       cv::Size textSize = getTextSize(text, fontFace,
                            fontScale, thickness, &baseline);
       baseline += thickness;

       cv::Point textOrg((image.cols - textSize.width)/2,
              (image.rows + textSize.height)/2);

       putText(image, text, textOrg, fontFace, fontScale,
            Scalar::all(255), thickness, 8);
    }

   void sharpen(int ksize, int sigma)
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

      sharpenedImage = newImage;
    }
  };
  //Inner class end
	
private:
  Es::HorizontalLayout center_layout;
    SimpleView         image_view;
    Es::VerticalLayout   control_pane;
    Es::PushButton       quit;

  int                loading_flag;
  static const int   CONTROLPANE_WIDTH = 160;

  Es::FileOpenDialog file_dialog;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::Application& applet, 
           std::string&      name,
           Es::Args&         args)
  :OpenCVMainView(applet, name, args)
   ,file_dialog(*this, FileOpenDialog::IMAGE_FILES)
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);
    int ww = w - CONTROLPANE_WIDTH;

    Es::MainLayout& main_layout = get_main_layout();
    const std::string filename = "../../images/earth.png";
    set_filenamed_title(filename);

    main_layout.pack_start(center_layout);
    image_view.set_size_request(ww, h);
    control_pane.set_size_request(CONTROLPANE_WIDTH, h);

    center_layout.pack_start(image_view);
    center_layout.pack_start(control_pane, FALSE, FALSE, 0);
    quit.set_label("Quit");
    quit.set_clicked_callback(
       sigc::mem_fun(*this, &MainView::quit_clicked) );

    control_pane.pack_start(quit, Gtk::PACK_SHRINK);
  
    loading_flag = IMREAD_COLOR;
    image_view.loadImage(filename, loading_flag);

    show_all();
  }

  void quit_clicked()
  {
    file_quit();
  }

  void file_open()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string filename = file_dialog.get_filename();
      set_filenamed_title(filename);

      image_view.loadImage(filename, loading_flag);
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

