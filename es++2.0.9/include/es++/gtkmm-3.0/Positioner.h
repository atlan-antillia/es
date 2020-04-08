/******************************************************************************
 *
 * Copyright (c) 2017  TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  Positioner.h
 *
 *****************************************************************************/

#pragma once

#include <gtkmm/window.h>
#include <gtkmm.h>

#include <gtkmm.h>
#include <es++/gtkmm-3.0/ZException.h>
#include <es++/gtkmm-3.0/Args.h>
#include <es++/gtkmm-3.0/Label.h>
#include <es++/gtkmm-3.0/HorizontalTrackBar.h>
#include <es++/gtkmm-3.0/ColorTip.h>
#include <es++/gtkmm-3.0/FlowLayout.h>

namespace Es {

class Positioner :public Gtk::VBox {
private:
  static const int       THREE = 3;
  Es::Label              label;

  Es::FlowLayout         layouter[THREE];
  Es::Label              name[THREE];
  Es::HorizontalTrackBar trackbar[THREE];

  int                    name_width ;
  int                    trackbar_width;

public:
  Positioner(int namew=20, int trackbarw=120)
  :VBox()
  ,name_width(namew)
  ,trackbar_width(trackbarw)
  {
    set_spacing(2);
    label.set_label("Positioner");
    pack_start(label, Gtk::PACK_SHRINK);

    for (int i = 0; i<THREE; i++) {
      pack_start(layouter[i], Gtk::PACK_SHRINK);
    }

    const char* items[] = {"X", "Y", "Z"};
 
    for (int i = 0; i<THREE; i++) {
      name[i].set_label(items[i]);

      name[i].set_size_request(name_width,     20); 
      trackbar[i].set_size_request(trackbar_width,20);
      trackbar[i].set_range(0.0, 1.0);

      layouter[i].add(name[i]);
      layouter[i].add(trackbar[i]);
    }
  }
    
  void set_range(double min, double max)
  {
    for (int i =0; i<THREE; i++) {
      trackbar[i].set_range(min, max);
    }
  }

  void set_name(const char* items[], const size_t count)
  {
    if (items && count== THREE) {
      for (int i = 0; i<THREE; i++) {
        name[i].set_label(items[i]);
      }
    }
  }

  int get_preferred_width()
  {
    return name_width + trackbar_width + 4;
  }

  void get_values(float& x, float& y, float& z)
  {
    int i = 0;
    x = (float)trackbar[i++].get_value();
    y = (float)trackbar[i++].get_value();
    z = (float)trackbar[i  ].get_value();
  }

  void set_values(float x, float y, float z)
  {
    int i = 0;
    trackbar[i++].set_value(x);
    trackbar[i++].set_value(y);
    trackbar[i  ].set_value(z);
  }

  void set_value_changed_callback(SigcCallback callback)
  {
    for (int i = 0; i<THREE; i++) {
      trackbar[i].set_value_changed_callback(callback);
    }
  }

  void set_label(const char* string)
  {
    label.set_label(string);  
  }

  void set_label(std::string string)
  {
    label.set_label(string);  
  }

  void configure_adjuster(int index, double value, double lower, double upper,
                  double step_inc = 1.0f,
                  double page_inc = 10.0f,
                  double page_size= 0.0f)
  {
    if (index >=0 && index < THREE) {
      trackbar[index].configure_adjuster(value, lower, upper, 
             step_inc, page_inc, page_size);
    } else {
      throw IException("Invalid index parameter:%d", index);
    }
  }

  void configure_adjuster(int index, int value, int lower, int upper,
                  int step_inc = 1,
                  int page_inc = 10,
                  int page_size= 0)
  {
    if (index >= 0 && index < THREE) { 
      trackbar[index].configure_adjuster( 
            (double)value, (double)lower, (double)upper, 
            (double)step_inc, (double)page_inc, (double)page_size);
    }
  }

  void set_lower(int index, double min)
  {
    if (index >= 0 && index < THREE) {
      trackbar[index].set_lower(min);
    } else {
      throw IException("Invalid index parameter:%d", index);
    }
  }

  void set_upper(int index, double max)
  {
    if (index >= 0 && index < THREE) {
      trackbar[index].set_upper(max);
    } else {
      throw IException("Invalid index parameter:%d", index);
    }
  }

  void set_range(int index, double min, double max)
  {
    if (index >= 0 && index < THREE) {
      trackbar[index].set_range(min, max);
    } else {
      throw IException("Invalid index parameter:%d", index);
    }
  }
/*
  void set_value_changed_callback(SigcCallback callback)
  {
    //Register the same callback to each trackbar
    for (int i = 0; i<THREE; i++) {
      trackbar[i].set_value_changed_callback(callback);
    }
  }
*/
  Es::HorizontalTrackBar& get_trackbar(int index)
  {
    if (index >= 0 && index < THREE) {
      return trackbar[index];
    } else {
      throw IException("Invalid index parameter:%d", index);
    }
  }

  void set_value(int index, double value)
  {
    if (index >= 0 && index < THREE) {
      trackbar[index].set_value(value);
    } else {
      throw IException("Invalid index parameter:%d", index);
    }
  }

  void set_value(int index, int value)
  {
    if (index >= 0 && index < THREE) {
      trackbar[index].set_value((double)value);
    } else {
      throw IException("Invalid index parameter:%d", index);
    }
  }

  void set_values(double x, double y, double z)
  {
    int i = 0;
    trackbar[i++].set_value(x);
    trackbar[i++].set_value(y);
    trackbar[i  ].set_value(z);
  }

  double get_value(int index)
  {
    if (index >= 0 && index < THREE) {
      return trackbar[index].get_value();
    } else {
      throw IException("Invalid index parameter:%d", index);
    }
  }

  void get_values(double& x, double& y, double& z)
  {
    int i = 0;
    x = trackbar[i++].get_value();
    y = trackbar[i++].get_value();
    z = trackbar[i  ].get_value();
  }

};

}

