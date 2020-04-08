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
 *  VideoDeviceEnumerator.h
 *
 *****************************************************************************/

#pragma once

#include <es++/ListT.h>
#include <es++/gtkmm-3.0/VideoDevice.h>

namespace Es {

class VideoInputDeviceEnumerator {
private:
  static const int MAX_COUNT = 64;
  std::string sysclass;

public:
  VideoInputDeviceEnumerator(const char* path = "/sys/class/video4linux/")
  :sysclass(path)
  {
  }

  void enumerate(Es::ListT<Es::VideoDevice>& device_list)
  {
    char path[256];
    for (int i = 0; i<MAX_COUNT; i++) {
      sprintf(path, "%svideo%d", sysclass.c_str(), i); 
      struct stat sts;
      int r = stat(path, &sts);
      if (r ==-1 && errno == ENOENT) {
        break;
      } else {
        VideoDevice* device = new VideoDevice();

        read_integer(path, "index", device->index);
        read_string(path,   "dev",  device->dev);
        read_string(path,  "name",  device->name);
        device_list.add(device);
      }
    }
  } 

  void read_integer(const char* dir, const char* filename, int& index)
  {
    char filepath[PATH_MAX];
    sprintf(filepath, "%s/%s", dir, filename);
    char line[10];
    FILE* fp = fopen(filepath, "r");
    if (fp) {
      fgets(line, sizeof(line), fp);
      index = atoi(line);
      fclose(fp);
    }
  }

  void read_string(const char* dir, const char* filename, std::string& string)
  {
    char filepath[PATH_MAX];
    sprintf(filepath, "%s/%s", dir, filename);
    char line[256];
    FILE* fp = fopen(filepath, "r");
    if (fp) {
      fgets(line, sizeof(line), fp);
      line[strlen(line)-1] = '\0';
      string = line;
      fclose(fp);
    }
  }
};

}

