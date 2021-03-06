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
 *  OpenCVScrolledImageView.h
 *
 *****************************************************************************/

#pragma once

#include <es++/opencv-4.0/OpenCVImageView.h>

namespace Es {

class OpenCVScrolledImageView :public Gtk::ScrolledWindow {
private:
  Es::OpenCVImageView imageView;

public:
  OpenCVScrolledImageView()
  {
    set_policy(Gtk::POLICY_AUTOMATIC,
               Gtk::POLICY_AUTOMATIC);
    add(imageView);
  }


  void loadImage(const std::string& filename, int loadingFlag, int scalingRatio=100)
  {
    imageView.loadImage(filename, loadingFlag, scalingRatio);
  }

  void scaleImage(int scalingRatio)
  {
    imageView.scaleImage(scalingRatio);
  }

  void scaleImage(cv::Mat& image, int scalingRatio)
  {
    imageView.scaleImage(image, scalingRatio);
  }

  cv::Mat& getOriginalImage()
  {
    return imageView.getOriginalImage();
  }

  cv::Mat getClone()
  {
    return imageView.getClone();
  }

  void setImage(cv::Mat& image)
  {
    imageView.setImage(image);
  }

  void refresh()
  {
    imageView.refresh();
  }

  Es::OpenCVImageView& getImageView()
  {
    return imageView;
  }
};

}


