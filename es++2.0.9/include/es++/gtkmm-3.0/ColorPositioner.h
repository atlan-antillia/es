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
 *      ColorPositioner.h
 *
 *****************************************************************************/

#pragma once

#include <es++/gtkmm-3.0/ColoredTrackBar.h>

namespace Es {

class ColorPositioner : public Gtk::VBox {
private:
  
  static const int    THREE = 3;
  Gtk::Label          label;
  Es::ColoredTrackBar trackbar[THREE];

public:
  ColorPositioner()
  :VBox()
  {
    set_spacing(2);
    pack_start(label, Gtk::PACK_SHRINK);

    double rgb[THREE][THREE] = {
       {1.0, 0.0, 0.0} ,
       {0.0, 1.0, 0.0} ,
       {0.0, 0.0, 1.0} ,
     };

    for (int i = 0; i<THREE; i++) {
      trackbar[i].set_range(0.0, 1.0);
      trackbar[i].set_background(rgb[i][0], rgb[i][1], rgb[i][2]);

      pack_start(trackbar[i], Gtk::PACK_SHRINK);
    }
    label.set_alignment(Gtk::ALIGN_START);

    label.set_label("ColorPositioner");
  }

  void set_background(float r, float g, float b)
  {
    int i = 0;
    trackbar[i++].set_background(  r, 0.0, 0.0);
    trackbar[i++].set_background(0.0,   g, 0.0);
    trackbar[i  ].set_background(0.0, 0.0,   b);
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
