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
 *      LabeledComboBox.h
 *
 *****************************************************************************/

#pragma once

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/combobox.h>
#include <es++/gtkmm-3.0/TypeDef.h>

namespace Es {

class LabeledComboBox : public Gtk::VBox {
private:
  Gtk::Label        label;
  Gtk::ComboBoxText combobox;

public:
  LabeledComboBox()
  {
    label.set_alignment(Gtk::ALIGN_START);

    pack_start(label, Gtk::PACK_SHRINK);
    pack_start(combobox, Gtk::PACK_SHRINK);
  }

  void set_label(const char* text)
  {
    label.set_label(text);
  }
  
  void set_label(std::string& text)
  {
    label.set_label(text);
  }
  
  void append_items(const char* items[], size_t count)
  {
    assert(items);

    for (size_t i = 0; i<count; i++) {
      combobox.append(items[i]);
    }
  }

  void append_item(const char* item) 
  {
    assert(item);

    combobox.append(item);
  }

  void set_changed_callback(SigcCallback callback)
  {
    combobox.signal_changed().connect(callback);
  }

  Gtk::ComboBoxText& get_combobox()
  {
    return combobox;
  }

  Glib::ustring get_active_text()
  {
    return combobox.get_active_text();
  }

  int get_active_row_number()
  {
    return combobox.get_active_row_number();
  }
 
  int get_active_number()
  {
    std::string text = combobox.get_active_text();
    const char* v = text.c_str();
    return atoi(v);
  }

  void set_active_text(const char* text)
  {
    combobox.set_active_text(text);
  }
  void set_active(int i)
  {
    combobox.set_active(i);
  }
};

}
