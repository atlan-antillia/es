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

#pragma once

#include <es++/Exception.h>
#include <es++/opencv-3.0/OpenCVObject.h>

namespace Es {

class OpenCVImageConverter {

public:
  //Convert cv::Mat to Gdk::Pixbuf; 
  static
  Glib::RefPtr<Gdk::Pixbuf> convertToPixbuf(cv::Mat& m)
  {
    IplImage image = m;

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
    
    //Flipping or Swapping of Red and Blue 
    cvConvertImage(mat, destImage,
                 (origin != 0 ? CV_CVTIMG_FLIP : 0) + CV_CVTIMG_SWAP_RB );

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

};

}

