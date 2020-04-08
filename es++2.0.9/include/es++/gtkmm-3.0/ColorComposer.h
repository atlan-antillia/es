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
 *  ColorComposer.h
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

class ColorComposer :public Gtk::VBox {
private:
  static const int       THREE = 3;
  Es::Label              label;

  Es::FlowLayout         layouter[THREE];
  Es::ColorTip           colortip[THREE];
  Es::HorizontalTrackBar trackbar[THREE];

  int                    tip_width ;
  int                    trackbar_width;

public:
  ColorComposer(int tipw=20, int trackbarw=120)
  :VBox()
  ,tip_width(tipw)
  ,trackbar_width(trackbarw)
  {
    set_spacing(2);
    pack_start(label, Gtk::PACK_SHRINK);

    for (int i = 0; i<THREE; i++) {
      pack_start(layouter[i], Gtk::PACK_SHRINK);
    }

    label.set_label("ColorComposer");

    float rgb[THREE][THREE] = {
       {1.0, 0.0, 0.0} ,
       {0.0, 1.0, 0.0} ,
       {0.0, 0.0, 1.0} ,
    };

    for (int i = 0; i<THREE; i++) {
      layouter[i].add(colortip[i]);
      layouter[i].add(trackbar[i]);
      colortip[i].set_background(rgb[i][0], rgb[i][1], rgb[i][2]);
      //
      colortip[i].set_size_request(tip_width,     20); 
      trackbar[i].set_size_request(trackbar_width,20);
      trackbar[i].set_range(0.0, 1.0);

    }
  }
    
  void set_label(const char* string)
  {
    label.set_label(string);
  }

  void set_range(double min, double max)
  {
    for (int i = 0; i<THREE; i++) {
      trackbar[i].set_range(min, max);
    }
  }

  int get_preferred_width()
  {
    return tip_width + trackbar_width + 4;
  }

  void get_values(float& r, float& g, float& b)
  {
    int i = 0;
    r = (float)trackbar[i++].get_value();
    g = (float)trackbar[i++].get_value();
    b = (float)trackbar[i  ].get_value();
  }

  void set_values(float r, float g, float b)
  {
    int i = 0;
    trackbar[i++].set_value(r);
    trackbar[i++].set_value(g);
    trackbar[i  ].set_value(b);
  }

  void get_values(double& r, double& g, double& b)
  {
    int i = 0;
    r = trackbar[i++].get_value();
    g = trackbar[i++].get_value();
    b = trackbar[i  ].get_value();
  }

  void set_values(double r, double g, double b)
  {
    int i = 0;
    trackbar[i++].set_value(r);
    trackbar[i++].set_value(g);
    trackbar[i  ].set_value(b);
  }

  void set_value_changed_callback(SigcCallback callback)
  {
    for (int i = 0; i<THREE; i++) {
      trackbar[i].set_value_changed_callback(callback);
    }
  }
};

}

