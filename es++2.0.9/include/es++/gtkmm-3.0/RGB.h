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
 *  RGB.h
 *
 *****************************************************************************/

#pragma once

#include <gtkmm/drawingarea.h>
#include <cairomm/cairomm.h>

using namespace Gtk;

namespace Es {

class RGB {
private:
  static const int RGB_MAX=1;
  static const int RGB_MIN=0;

public:
  double r, g, b;

  RGB(double r, double g, double b)
  :r(r),g(g),b(b)
  {
    //set(r, g, b);
  }

  RGB(const RGB& rgb)
  {
    r = rgb.r;
    g = rgb.g;
    b = rgb.b;
  }

  void get(double& rr, double& rg, double& rb)
  {
    rr = r;
    rg = g;
    rb = b;
  }

  void set(double r, double g, double b)
  {
    if (r < double(RGB_MIN)) {
      r = double(RGB_MIN);
    }
    if (g < double(RGB_MIN)) {
      g = double(RGB_MIN);
    }
    if (b < double(RGB_MIN)) {
      b = double(RGB_MIN);
    }

    if (r > double(RGB_MAX)) {
      r = double(RGB_MAX);
    }
    if (g > double(RGB_MAX)) {
      g = double(RGB_MAX);
    }
    if (b > double(RGB_MAX)) {
      b = double(RGB_MAX);
    }

  }
};
}

