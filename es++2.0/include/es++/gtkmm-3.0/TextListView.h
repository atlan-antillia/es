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
 *  TextListView.h
 *
 *****************************************************************************/

#pragma once

#include <gtkmm/window.h>
#include <gtkmm/listviewtext.h>

namespace Es {

class TextListView : public Gtk::ListViewText {
 
public:
  TextListView(int col)
  :Gtk::ListViewText(col)
  {
  }

  ~TextListView()
  {
  }

  void set_column_title(const char* title[], size_t count)
  {
    size_t column = get_num_columns();
    if (count <= column && title) {
      for (size_t col = 0; col< count; col++) {
       const Glib::ustring string(title[col]); 
       Gtk::ListViewText::set_column_title(col, string);
      }
    }
  }

  void append_text(const char* text[], size_t count)
  {
    size_t column = get_num_columns();
    if (count <= column && text) {
      int row = append(); //get a new row_number to be appended.
      for (size_t col = 0; col <count; col++) {
        const Glib::ustring string(text[col]); 
        Gtk::ListViewText::set_text(row, col, string); 
      }
    }
  }

  void update_text(guint row, guint col, const char* text)
  {
    const Glib::ustring string(text);
    Gtk::ListViewText::set_text(row, col, string);
  }

  void update_text(guint row, guint col, const Glib::ustring& string)
  {
    Gtk::ListViewText::set_text(row, col, string);
  }

  guint append_at_first_column(const char* text )
  {
    const Glib::ustring string(text);
    return  Gtk::ListViewText::append(string);
  }

  guint append_at_first_column( const Glib::ustring& string)
  {
    return  Gtk::ListViewText::append(string);
  }

  void prepend_at_first_column(const char* text)
  {
    const Glib::ustring string(text);
    Gtk::ListViewText::prepend(string);
  }

  void prepend_at_first_column(const Glib::ustring& string)
  {
    Gtk::ListViewText::prepend(string);
  }

  void insert_into_first_column(guint row, const char* text)
  {
    const Glib::ustring string(text);
    Gtk::ListViewText::insert(row, string);
  }

  void insert_into_first_column(guint row, const Glib::ustring& string)
  {
    Gtk::ListViewText::insert(row, string);
  }

  //In Gtk::ListViewText
  // void clear_items();

  //In Gtk::ListViewText
  // Returns a vector of the indexes of the selected rows
  //std::vector<int> get_selected();

/*
  virtual void on_row_activated(const Gtk::TreeModel::Path& path, 
     Gtk::TreeViewColumn* column)
  {
    printf("on_row_activated \n");
  }
*/
  void set_cursor_changed_callback(Es::SigcCallback callback)
  {
    signal_cursor_changed().connect(callback);
  }
/*
  virtual void on_cursor_changed()
  {
    printf("on_cursor_changed\n");
  }
*/
};
}

