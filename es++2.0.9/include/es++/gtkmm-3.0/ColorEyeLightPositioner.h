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
 *      ColorEyeLigthPositioner.h
 *
 *****************************************************************************/

#pragma once

#include <es++/gtkmm-3.0/Positioner.h>
#include <es++/gtkmm-3.0/ColorComposer.h>

#include <es++/gtkmm-3.0/VerticalLayout.h>

namespace Es {

class ColorEyeLightPositioner : public Es::VerticalLayout {
private:
  Es::ColorComposer  color_positioner;
  Es::Positioner     eye_positioner;
  Es::Positioner     light_positioner;

public:
  ColorEyeLightPositioner(int space=4)
  :VerticalLayout(space)
  {
    pack_start(color_positioner);
    pack_start(eye_positioner);
    pack_start(light_positioner);
    color_positioner.set_label("ColorPositioner");
    //const char* names[] = {"R", "G", "B"};
    //color_positioner.set_name(names, 3);

    eye_positioner.set_label("EyePositioner");
    light_positioner.set_label("LightPositioner");
  }

  void set_color_position_changed_callback(SigcCallback callback)
  {
    color_positioner.set_value_changed_callback(callback);
  }

  void set_eye_position_changed_callback(SigcCallback callback)
  {
    eye_positioner.set_value_changed_callback(callback);
  }

  void set_light_position_changed_callback(SigcCallback callback)
  {
    light_positioner.set_value_changed_callback(callback);
  }


  void set_color_range(double min, double max)
  {
    color_positioner.set_range(min, max);
  }

  void set_eye_range(double min, double max)
  {
    eye_positioner.set_range(min, max);
  }

  void set_light_range(double min, double max)
  {
    light_positioner.set_range(min, max);
  }

  void get_color_position(double& r, double& g, double& b)
  {
    color_positioner.get_values(r, g, b);
  }

  void set_color_position(double r, double g, double b)
  {
    color_positioner.set_values(r, g, b);
  }

  void get_eye_position(double& x, double& y, double& z)
  {
    eye_positioner.get_values(x, y, z);
  }

  void set_eye_position(double x, double y, double z)
  {
    eye_positioner.set_values(x, y, z);
  }

  void get_light_position(double& x, double& y, double& z)
  {
    light_positioner.get_values(x, y, z);
  }

  void set_light_position(double x, double y, double z)
  {
    light_positioner.set_values(x, y, z);
  }


  int get_preferred_width()
  {
     return color_positioner.get_preferred_width();
  }  

};

}
