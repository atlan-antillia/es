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
 *  OpenCVVideoCapture.h
 *
 *****************************************************************************/

// 2017/08/10
// On cv::VideoCapture
// See: http://docs.opencv.org/trunk/d8/dfe/classcv_1_1VideoCapture.html

#pragma once

#include <es++/opencv-3.0/OpenCVObject.h>
#include <es++/opencv-3.0/OpenCVApplication.h>

namespace Es {
  
class OpenCVVideoCapture :public OpenCVObject {
private:
  cv::VideoCapture videoCapture;
  int              read_retry_count; 
  static const int  MAX_READ_RETRY_COUNT = 5; 

public:
  //Constructor

  OpenCVVideoCapture(int retry_count = MAX_READ_RETRY_COUNT)
  :read_retry_count(retry_count)
  {    
  }

  ~OpenCVVideoCapture()
  {
    close();
  }
  
  double getProperty(int prop)
  {
    double value = 0.0;
    if (isOpened()) {
       value = videoCapture.get(prop);
    }
    return value;
  }

  bool setProperty(int prop, double value)
  {
    bool rc = false;
    if (isOpened()) {
      rc = videoCapture.set(prop, value);
    }
    return rc;
  }

  void open(int deviceIndex)
  {
    //close();
    
    if (videoCapture.open(deviceIndex)) {
      if (!isOpened()) {
        throw IException("Failed to open deviceIndex=%d", deviceIndex);
      }
    }
  }

  void open(const std::string& filename)
  {
    //close();
    
    if (videoCapture.open(filename)) {
      if (!isOpened()) {
        throw IException("Failed to open file=%d", filename.c_str());
      }
    }
  }

  void close()
  {
    if (isOpened()) {
      videoCapture.release();
    }
  }
  
  bool read(cv::Mat& frame)
  {
    bool rc = false;

    if (isOpened() ) {
       rc = videoCapture.read(frame);
/*
      //for (int i = 0; i<read_retry_count; i++) {
        if (videoCapture.read(frame)) {
          if (frame.empty()) {
            //If the frame is empty, try to read until to get a 
            // nonempty frame;
            continue;
          } else {
            rc = true;
            break;
          }
      //  }
      }
*/
    }
    return rc;
  }

  bool isOpened()
  {
    return videoCapture.isOpened();
  }

  cv::VideoCapture& getVideoCapture()
  {
    return videoCapture;
  }
  
  bool getVideoFrameSize(int& w, int& h)
  {
    bool rc = false;
    w = 0;
    h = 0;
    if (isOpened()) {
      w = (int)videoCapture.get(CAP_PROP_FRAME_WIDTH);
      h = (int)videoCapture.get(CAP_PROP_FRAME_HEIGHT);
      rc = true;
    }
    return rc;
  }

  bool setVideoFrameSize(int w, int h)
  {
    bool rc = false;
    if (isOpened()) {
      videoCapture.set(CAP_PROP_FRAME_WIDTH,  (double)w); 
      videoCapture.set(CAP_PROP_FRAME_HEIGHT, (double)h);
      rc = false;
    }
    return rc;
  }
};

}


