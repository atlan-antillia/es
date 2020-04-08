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
 *  OpenCVImageConverter.h
 *
 *****************************************************************************/

// 2019/01/01 Modified to support OpenCV-4.2.0 based on
// precomp.hpp and window_gtk.cpp in /opencv/sources/modules/highgui/src
// of OpenCV-4.2.0
// 
// 2020/02/12 Update to support OpenCV 4.2.0

#pragma once

#include <es++/Exception.h>
#include <es++/opencv-4.0/OpenCVObject.h>

namespace Es {

class OpenCVImageConverter {

public:
  //Convert cv::Mat to Gdk::Pixbuf; 
  static
  Glib::RefPtr<Gdk::Pixbuf> convertToPixbuf(cv::Mat& m)
  {
    IplImage image = cvIplImage(m); //2020/02/15 m;

    dprintf("fromMatToPixbuf depth=%d channels=%d\n", 
    	image.depth, image.nChannels);
    CvMat* mat, stub;
    mat = cvGetMat(&image, &stub);

    //CV_8UC3 is color 24 bit cvMat image. 
    CvMat* destImage = cvCreateMat(mat->rows, mat->cols, CV_8UC3);

    if (destImage == NULL) {
      throw Es::IException("Failed to cvCreateMat");
    }

    int origin = image.origin;
    
    convertToShow(cv::cvarrToMat(&image), destImage);

    Glib::RefPtr<Gdk::Pixbuf> 
      pixbuf = Glib::wrap(gdk_pixbuf_new_from_data(
               destImage->data.ptr,
               GDK_COLORSPACE_RGB ,
               false,
               8,
               destImage->cols,
               destImage->rows,
               destImage->step,
               NULL,
               NULL)
             );
    //Don't need release destImage.
    //    cvReleaseMat(&destImage);

    return pixbuf;
  }

  //From precomp.hpp
  static
  void convertToShow(const cv::Mat &src, cv::Mat &dst, bool toRGB = true)
  {
    const int src_depth = src.depth();
    CV_Assert(src_depth != CV_16F && src_depth != CV_32S);
    cv::Mat tmp;
    switch(src_depth)
    {
    case CV_8U:
        tmp = src;
        break;
    case CV_8S:
        cv::convertScaleAbs(src, tmp, 1, 127);
        break;
    case CV_16S:
        cv::convertScaleAbs(src, tmp, 1/255., 127);
        break;
    case CV_16U:
        cv::convertScaleAbs(src, tmp, 1/255.);
        break;
    case CV_32F:
    case CV_64F: // assuming image has values in range [0, 1)
        cv::convertScaleAbs(src, tmp, 256.);
        break;
    }
    cv::cvtColor(tmp, dst, toRGB ? cv::COLOR_BGR2RGB : cv::COLOR_BGRA2BGR, dst.channels());
  }

  //From precomp.hpp
  static
  void convertToShow(const cv::Mat &src, const CvMat* arr, bool toRGB = true)
  {
    cv::Mat dst = cv::cvarrToMat(arr);
    convertToShow(src, dst, toRGB);
    CV_Assert(dst.data == arr->data.ptr);
  }
};

}

