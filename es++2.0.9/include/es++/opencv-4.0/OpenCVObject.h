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
 *  OpenCVObject.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <opencv2/core/core_c.h>
#include <opencv2/core/types_c.h>
#include <opencv2/imgcodecs/legacy/constants_c.h>

#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

//#include <opencv2/flann/miniflann.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;
//using namespace Gtk;

//OpenCV3.2
namespace cv{
  using std::vector;
}

typedef enum {
  IMAGE_KEEP_ASPECT_RATIO,
} IMAGE_SCALING_POLICY;

namespace Es {

class OpenCVObject :public Es::Object {
  
public:
  OpenCVObject()
  {
  }

  ~OpenCVObject()
  {
  }
};

}


