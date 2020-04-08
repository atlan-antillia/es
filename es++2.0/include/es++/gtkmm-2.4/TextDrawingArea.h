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
 *  TextDrawingArea.h
 *
 *****************************************************************************/


#include <gtkmm/drawingarea.h>
#include <cairomm/cairomm.h>
#include <es++/gtkmm-2.4/RGB.h>

using namespace Gtk;

namespace Es {

class TextDrawingArea: public Gtk::DrawingArea {

private:
  int           x;
  int           y;
  Glib::ustring   string;
  Es::RGB         foreground;
  Es::RGB         background;
  bool            centering;
  double          font_size; //point size

  Cairo::RefPtr<Cairo::ToyFontFace> font;

public:
  TextDrawingArea()
  :x(0), y(0)
  ,string("")
  ,background(1.0f, 1.0f, 1.0f)
  ,foreground(0.0f, 0.0f, 0.0f)
  ,centering(true)
  ,font_size(20.0f) 
  {
    font = Cairo::ToyFontFace::create("Arial",
                 Cairo::FONT_SLANT_NORMAL,
                 Cairo::FONT_WEIGHT_NORMAL);
  }

  void set_font(const char* font_family, 
           Cairo::FontSlant font_slant = Cairo::FONT_SLANT_NORMAL,
           Cairo::FontWeight font_weight = Cairo::FONT_WEIGHT_NORMAL)
  {
    font = Cairo::ToyFontFace::create(font_family,
                               font_slant,
                               font_weight);
  }

  void set_text(const char* text, double fsize, 
                 RGB foreground, RGB background=RGB(1.0f, 1.0f, 1.0f))
  {
    this -> centering  = true;
    this -> string     = text;
    this -> font_size  = fsize;
    this -> foreground = foreground;
    this -> background = background;
  }

  void set_text(int x, int y, const char* text, double fsize,
                 RGB foreground, RGB background=RGB(1.0f, 1.0f, 1.0f))
  {
    this -> centering = false;
    this -> x = x;
    this -> y = y;
    this -> string    = text;
    this -> font_size = fsize;
    this -> foreground = foreground;
    this -> background = background;
  }

  //gtkmm 2.4
  bool on_expose_event(GdkEventExpose* event)
  {
    Glib::RefPtr<Gdk::Window> window = get_window();
    if (window) {
      Gtk::Allocation allocation = get_allocation();
      int w = allocation.get_width();
      int h = allocation.get_height();

      //In gtkmm-2.4, you have to create an instance of Cairo::Context.
      Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
      if (event) {
        cr->rectangle(event->area.x, event->area.y,
                        event->area.width, event->area.height);
        cr->clip();
      }

      //Set a background color of the cr to be black.
      cr->set_source_rgb(background.r, background.g, background.b); 
      cr->paint();

      //Set a foreground color of the cr to become the color.
      cr->set_source_rgb(foreground.r, foreground.g, foreground.b); 

      cr->set_font_face(font);

      //Get a resolution.
      double resolution = get_pango_context()->get_resolution();
      double size = font_size*resolution/72.0f;
      cr->set_font_size(size);

      if (centering) {
        //Get a textextension of the string.
        Cairo::TextExtents ex;
        cr->get_text_extents(string, ex);

        //Move a starting point to the following point
        // to display the string in the center of this window. 
        cr->move_to((w - ex.width) /2 - ex.x_bearing,
                    (h - ex.height)/2 - ex.y_bearing);
      } else {
        cr->move_to(x, y);
      }
      cr->show_text(string);
    }
    return true;
  }
};
}

