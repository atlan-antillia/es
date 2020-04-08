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
 *  ScrolledTextView.h
 *
 *****************************************************************************/

#pragma once

#include <es++/gtkmm-3.0/TextView.h>

namespace Es {

class ScrolledTextView :public Gtk::ScrolledWindow {
private:
  Es::TextView    text_view;

public:
  ScrolledTextView()
  {
    set_policy(Gtk::POLICY_AUTOMATIC,
               Gtk::POLICY_AUTOMATIC);
    add(text_view);
  }

  void set_text(const char* text)
  {
    text_view.set_text(text);
  }

  void set_text_buffer(Glib::RefPtr<Gtk::TextBuffer> text_buffer)
  {
    text_view.set_text_buffer(text_buffer);
  }

  void loadText(const std::string& filename)
  {
    text_view.loadText(filename);
  }

  Gtk::TextView& get_text_view()
  {
    return text_view;
  }

  Glib::RefPtr<Gtk::TextBuffer> get_text_buffer()
  {
    return text_view.get_text_buffer();
  }
};

}


