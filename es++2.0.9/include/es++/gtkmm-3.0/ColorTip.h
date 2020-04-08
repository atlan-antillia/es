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
 *  ColorTip.h
 *
 *****************************************************************************/

//2017/10/25
#pragma once

#include <gtkmm/drawingarea.h>
#include <cairomm/cairomm.h>
#include <es++/gtkmm-3.0/RGB.h>

using namespace Gtk;

namespace Es {

class ColorTip: public Gtk::DrawingArea {
private:
  Es::RGB         background;
  int             top_margin;
  int             bottom_margin;
public:
  ColorTip(int top=12, int bottom=0)
  :DrawingArea()
  ,background(1.0f, 1.0f, 1.0f) //white
  ,top_margin(top)
  ,bottom_margin(bottom)
  {
  }

  void set_background(float r, float g, float b)
  {
    set_background(RGB(r, g, b));
  }

  void set_background(RGB background)
  {
    this -> background = background;
  }

  //gtkmm 3.0
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) 
  {
    Gtk::Allocation allocation = get_allocation();
    int w = allocation.get_width();
    int h = allocation.get_height();

    cr->set_source_rgb(background.r, background.g, background.b); 
    cr->rectangle(0, top_margin, w, h-top_margin-bottom_margin); 
    cr->fill();

    return true;
  }

  void refresh()
  {
    unmap();
    map();
  }
};
}

