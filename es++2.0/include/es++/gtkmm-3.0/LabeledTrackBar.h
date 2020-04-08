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
 *      LabeledTrackBar.h
 *
 *****************************************************************************/

#pragma once

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/scale.h>
#include <es++/gtkmm-3.0/TypeDef.h>

/*
Gtk::Adjustment(float value,
                float lower,
                float upper,
                float step_increment = 1.0f,
                float page_increment = 10.0f,
                float page_size = 0.0f);

 */

namespace Es {

class LabeledTrackBar : public Gtk::VBox {
private:
  Glib::RefPtr<Gtk::Adjustment> adjuster;
  Gtk::HScale     scale;
  Gtk::Label      label;

public:
  LabeledTrackBar()
  :adjuster(Gtk::Adjustment::create(0.0, 0.0, 100.0 ))
  ,scale(adjuster)
  {
    label.set_alignment(Gtk::ALIGN_START);
    pack_start(label, Gtk::PACK_SHRINK);
    pack_start(scale, Gtk::PACK_SHRINK);
  }

  LabeledTrackBar(double value, double lower, double upper,
                  double step_inc = 1.0f, 
                  double page_inc = 10.0f, 
                  double page_size= 0.0f)
  :adjuster(Gtk::Adjustment::create( value, lower, upper, 
                  step_inc, page_inc, page_size ))
  ,scale(adjuster)
  {
    label.set_alignment(Gtk::ALIGN_START);

    pack_start(label, Gtk::PACK_SHRINK);
    pack_start(scale, Gtk::PACK_SHRINK);
  }

  void configure_adjuster(double value, double lower, double upper,
                  double step_inc = 1.0f,
                  double page_inc = 10.0f,
                  double page_size= 0.0f)
  {
    adjuster->configure( value, lower, upper, step_inc, page_inc, page_size);
  }

  void configure_adjuster(int value, int lower, int upper,
                  int step_inc = 1,
                  int page_inc = 10,
                  int page_size= 0)
  {
    adjuster->configure( 
        (double)value, (double)lower, (double)upper, 
        (double)step_inc, (double)page_inc, (double)page_size);
  }

  void set_label(const char* text)
  {
    label.set_label(text);
  }
  
  void set_label(std::string& text)
  {
    label.set_label(text);
  }
  
  void set_range(double min, double max)
  {
    adjuster->set_lower(min);
    adjuster->set_upper(min);
  }

  void set_value_changed_callback(SigcCallback callback)
  {
    adjuster->signal_value_changed().connect(callback);
  }

  Glib::RefPtr<Gtk::Adjustment>& get_adjustment()
  {
    return adjuster;
  }

  void set_value(double value)
  {
    adjuster->set_value(value);
  }

  double get_value()
  {
    return adjuster->get_value();
  }

};

}
