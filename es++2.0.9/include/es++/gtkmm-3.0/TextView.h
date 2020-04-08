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
 *  ImageView.h
 *
 *****************************************************************************/

//On Gtl::TextBuffer https://developer.gnome.org/gtkmm/stable/classGtk_1_1TextBuffer.html

#pragma once

#include <gtkmm/window.h>
#include <gtkmm/textview.h>

#include <es++/File.h>
#include <es++/CharFileReader.h>

namespace Es {

class TextView : public Gtk::TextView {
private:
  Glib::RefPtr<Gtk::TextBuffer> text_buffer;
 
public:
  TextView()
  :text_buffer(NULL)
  {
    text_buffer =  Gtk::TextBuffer::create();
  }

  ~TextView()
  {
  }

   void loadText(const std::string& filename)
  {
    Es::File file(filename);
    const char* name = filename.c_str();
    if (file.exists() == false) {
      throw IException("File not found %s", filename.c_str());
    } 

    CharFileReader reader(name);
    const char* text = reader.getData();
    size_t size = reader.getDataSize();
    if (text && size > 0) {
      text_buffer->set_text(text);
      set_buffer(text_buffer);
    }        
  }

  void set_text(const char* text) //null-terminated text string
  {
    if (text) {
      text_buffer->set_text(text);
    }    
  }

  void set_text_buffer( Glib::RefPtr<Gtk::TextBuffer> buf )
  {
    text_buffer = buf;
  }

  Gtk::TextView& get_text_view()
  {
    return *this;
  }

   Glib::RefPtr<Gtk::TextBuffer>  get_text_buffer()
  {
    return text_buffer;
  }
};
}

