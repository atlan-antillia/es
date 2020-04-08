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
 *      OpenCVScaleComboBox.h
 *
 *****************************************************************************/

#pragma once

#include <es++/gtkmm-3.0/LabeledComboBox.h>

namespace Es {

class OpenCVScaleComboBox : public LabeledComboBox {
  
public:
  OpenCVScaleComboBox()
  :LabeledComboBox()
  {
    static const char* items[] = {
     "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%","100%",
     "110%", "120%", "130%", "140%", "150%", "160%", "170%","180%", "190%", "200%"};
   
    append_items(items, CountOf(items));

    set_label("Scale");
  }

  void set_selection(int scale)
  {
    char scales[128];
    sprintf(scales, "%d%%", scale);
    set_active_text(scales); 
  }

  int  get_selection()
  {
    std::string item = get_active_text();
    char text[128];
    strcpy(text, item.c_str());
    char* p = strrchr(text, '%');
    if (p) {
      *p = '\0';
    }
    return atoi(text);
  }

};
}

