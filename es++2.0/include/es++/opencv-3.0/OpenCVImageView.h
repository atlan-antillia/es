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
 *  OpenCVImageView.h
 *
 *****************************************************************************/

#pragma once

//#include <es++/Exception.h>
#include <es++/ModuleFileName.h>
#include <es++/gtkmm-3.0/ImageView.h>
#include <es++/opencv-3.0/OpenCVImageConverter.h>

namespace Es {

class OpenCVImageView: public ImageView {

private:
  cv::Mat rgbImage;
  cv::Mat scaledImage;
  double  scale;

  static const int SCALING_MIN =  10; //10%
  static const int SCALING_MAX = 300; //300%

public:
  static int validateScale(int scalingRatio)
  {
    if (scalingRatio < SCALING_MIN) {
      scalingRatio = SCALING_MIN;
    }
    if (scalingRatio > SCALING_MAX) {
      scalingRatio = SCALING_MAX;
    }
    return scalingRatio;
  }

public:
  virtual void scaleImage(int scalingRatio)
  {
    try {
      scalingRatio = validateScale(scalingRatio);

      scale = (float)scalingRatio/100.0f;

      cv::resize(rgbImage, scaledImage, cv::Size(), 
		scale, scale);
      Glib::RefPtr<Gdk::Pixbuf> pixbuf(
           OpenCVImageConverter::convertToPixbuf(scaledImage) );

      if (pixbuf) {
        set_pixbuf(pixbuf) ;
        int w = pixbuf -> get_width();
        int h = pixbuf -> get_height();
        set_size_request(w, h);
      }
      refresh();

     } catch (cv::Exception& ex) {
       throw IException("Failed to cv::resize."); 
     }
  }

  virtual void scaleImage(cv::Mat& image, int scalingRatio)
  {
    try {
      scalingRatio = validateScale(scalingRatio);

      scale = (float)scalingRatio/100.0f;

      cv::resize(image, scaledImage, cv::Size(),
                scale, scale);
      Glib::RefPtr<Gdk::Pixbuf> pixbuf(
           OpenCVImageConverter::convertToPixbuf(scaledImage) );

      if (pixbuf) {
        set_pixbuf(pixbuf) ;
        int w = pixbuf -> get_width();
        int h = pixbuf -> get_height();
        set_size_request(w, h);
      }
      refresh();

    } catch (cv::Exception& ex) {
      throw Es::IException("Failed to cv::resize.");
    }
  }

  virtual void loadImage(const std::string& filename, int loadingFlag, 
                          int scalingRatio=100)
  {
    rgbImage = readImage(filename, loadingFlag);

    if (scalingRatio == 100) {
    
      Glib::RefPtr<Gdk::Pixbuf> pixbuf(  
           OpenCVImageConverter::convertToPixbuf(rgbImage) );

      if (pixbuf) {
        set_pixbuf(pixbuf) ;
        int w = pixbuf -> get_width();
        int h = pixbuf -> get_height();
        set_size_request(w, h);
      } else {
      }
    } else {
       //We have to resize the cv::Mat rgbImage to the scaledImage method, 
       //and get the corresponding Gdk::Pixbuf to the resized the cv::Mat.

       scaleImage(rgbImage, scalingRatio);
    }
  }
  
  cv::Mat readImage(const std::string& filename, int loadingFlag)
  {
    std::string abspath = filename;

    ModuleFileName module;

    Es::File file(filename);
   
    if (file.exists() == false) {
      const char* name = filename.c_str();
      if (name[0] != '/') {
        abspath = module.getPath();
        abspath += "/";
        abspath += filename;
      }
      Es::File afile(abspath);
      if (afile.exists() == false) {
        throw Es::IException("imagefile not found %s", abspath.c_str());
      } 
    }

    //printf("OpenCVImageView: imagefile %s\n", abspath.c_str());
    cv::Mat image = imread(abspath.c_str(), loadingFlag);
    if (image.empty()) {
      throw Es::IException("Failed to imread: %s", abspath.c_str() );
    }
    return image;
  }

public:
  OpenCVImageView()
  :ImageView()
  {
  }

  ~OpenCVImageView()
  {
    rgbImage.release();
  }
	
  virtual void save(const char* filename, const cv::Mat& image)
  {
    if (!image.empty()) {
      if (!imwrite(filename, image)) {
        throw IException("Failed to write image file: %s", filename);
      }
    }
  }

  cv::Mat& getOriginalImage()
  {
    return rgbImage;
  }

  cv::Mat& getScaledImage()
  {
    return scaledImage;
  }

  void refresh()
  {
    queue_draw();    
  }

  void setImage(cv::Mat& image)
  {
    rgbImage = image;
  }
};

}

