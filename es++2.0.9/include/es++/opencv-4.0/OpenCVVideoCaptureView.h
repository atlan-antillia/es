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
 *  OpenCVVideoCaptureView.h
 *
 *****************************************************************************/

// 2017/08/10
// On cv::VideoCapture
// See: http://docs.opencv-3.0.org/3.1.0/d8/dfe/classcv_1_1VideoCapture.html

#pragma once

#include <es++/opencv-4.0/OpenCVVideoCapture.h>
#include <es++/opencv-4.0/OpenCVImageView.h>
#include <es++/opencv-4.0/OpenCVMainView.h>

namespace Es {
  
class OpenCVVideoCaptureView :public OpenCVMainView {
private:
  static const int INVALID_DEVICE_INDEX       = -1;

  static const int DEFAULT_VIDEO_DEVICE_INDEX =  0;  

  int                 deviceIndex;

  OpenCVVideoCapture  videoCapture;
  
  sigc::connection    idling_connection;

public:
  //Constructor
  OpenCVVideoCaptureView(Es::Application& applet, 
                         std::string& caption, 
                         Es::Args& args)
  :OpenCVMainView(applet, caption, args) 
  ,deviceIndex(INVALID_DEVICE_INDEX)
  {    
    if (args.has(XmNvideoDeviceIndex)) {
      deviceIndex    = (int)args.get(XmNvideoDeviceIndex);
    }

    bool autoStart = (bool)args.get(XmNcaptureAutoStart);
    if (autoStart) {
      open_video_device(deviceIndex);
    }
  }

  ~OpenCVVideoCaptureView()
  {
    stop_idling();
    close_video_device();
  }


  int get_device_index()
  {
    return deviceIndex;
  }

  void set_device_index(int index)
  {
    deviceIndex = index;
  }

  void close_video_device()
  {
    try {
      stop_idling();
    } catch (...) {
      //
    }
    try {
      videoCapture.close();
    } catch (...) {
      //
    }
    std::string& appName = get_appname();
    
    set_title(appName);   
  }
  
  void stop_idling()
  {
    try {
      idling_connection.disconnect();
    } catch (...) {
    }
  }

  void start_idling()
  {
    try {
      stop_idling();

      idling_connection = Glib::signal_idle().connect(sigc::mem_fun(*this,
                                   &OpenCVVideoCaptureView::render));
    } catch (...) {
    }
  }

  void open_video_device(int device)
  {
    try {
      stop_idling();

      videoCapture.open(device);
      videoCapture.setVideoFrameSize(640, 480);

      start_idling();
   
      std::string& appName = get_appname();
    
      char title[PATH_MAX];
      sprintf(title, "Capturing deviceIndex=%d  - %s", 
                        deviceIndex, appName.c_str());
      set_title(title);
    } catch (Es::Exception& ex) {
      caught(ex);
    }
  }
  
  void open_video_device(const std::string& filename)
  {
    try {
      stop_idling();

      videoCapture.open(filename);
      videoCapture.setVideoFrameSize(640, 480);

      start_idling();

      std::string& appName = get_appname();

      char title[PATH_MAX];
      sprintf(title, "Opened a file=%s  - %s",
                        filename.c_str(), appName.c_str());
      set_title(title);
    } catch (Es::Exception& ex) {
      caught(ex);
    }
  }

  virtual void start_capture()
  {
    open_video_device(deviceIndex);
  }

  virtual void stop_capture()
  {
    close_video_device();
  }
  
  bool is_video_device_opened()
  {
    return videoCapture.isOpened();
  }

  bool read_video_frame(cv::Mat& frame)
  {
    return videoCapture.read(frame);
  }

  cv::VideoCapture& get_videoCapture()
  {
    return videoCapture.getVideoCapture();
  }
  
  bool get_video_frame_size(int& width, int& height)
  {
    videoCapture.getVideoFrameSize(width, height);
  }
  
  //Define your own render method in a subclass derived from this class.
  virtual bool render()
  {
    //Do something here 
    //For example, read_video_frame();
    //Return true to enable render.
    return true;
  }
};

}


