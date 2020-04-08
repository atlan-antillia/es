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
 *      Label.h
 *
 *****************************************************************************/

#pragma once

#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/combobox.h>
#include <es++/gtkmm-2.4/TypeDef.h>

namespace Es {

class Label : public Gtk::Label {

public:
  Label()
  :Gtk::Label()
  {
    set_alignment(Gtk::ALIGN_LEFT);
  }

  Label(const char* label)
  :Gtk::Label()
  {
    set_label(std::string(label));
    set_alignment(Gtk::ALIGN_LEFT);
  }

  Label(std::string& label)
  :Gtk::Label()
  {
    set_label(label);
    set_alignment(Gtk::ALIGN_LEFT);
  }

  void set_number(const char* label, int number)
  {
    std::string text = label;
    char value[200];
    sprintf(value, " %d", number);
    text += value; 
    set_label(text);
  }

  void set_number(const char* label, double number)
  {
    std::string text = label;
    char value[200];
    sprintf(value, " %lf", number);
    text += value; 
    set_label(text);
  }
};

}

