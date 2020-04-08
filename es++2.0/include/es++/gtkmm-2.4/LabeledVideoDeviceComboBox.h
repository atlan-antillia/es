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
 *      LabeledVideDeviceComboBox.h
 *
 *****************************************************************************/

#pragma once

#include <es++/gtkmm-2.4/LabeledComboBox.h>
#include <es++/gtkmm-2.4/VideoInputDeviceEnumerator.h>

namespace Es {

class LabeledVideoDeviceComboBox : public LabeledComboBox {
  
public:
  LabeledVideoDeviceComboBox()
  :LabeledComboBox()
  {
    VideoInputDeviceEnumerator enumerator;
    ListT<VideoDevice> devices;
    enumerator.enumerate(devices);

    for (int i = 0; i<devices.getLength(); i++) {
      VideoDevice* device = devices.getNth(i);
      char text[256];
      sprintf(text, "%d: %s", device->index, device->name.c_str());
      append_item(text);      
      if (i == 0) {
       set_active_text(text);
      }
    }
    set_label("VideoDevice");
  }

  void set_selection(const char* text)
  {
    set_active_text(text); 
  }

  int  get_selected_index()
  {
    std::string item = get_active_text();
    char text[128];
    strcpy(text, item.c_str());
    char* p = strchr(text, ':');
    if (p) {
      *p = '\0';
    }
    return atoi(text);
  }

};
}

