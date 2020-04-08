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
 *  ScrolledTextListView.h
 *
 *****************************************************************************/

#pragma once

#include <es++/gtkmm-2.4/TextListView.h>

namespace Es {

class ScrolledTextListView :public Gtk::ScrolledWindow {
private:
  Es::TextListView  list_view;

public:
  ScrolledTextListView(int column=1)
  :list_view(column)
  {
    set_policy(Gtk::POLICY_AUTOMATIC,
               Gtk::POLICY_AUTOMATIC);
    add(list_view);
  }

  Es::TextListView& get_text_listview()
  {
    return list_view;
  }

  std::vector<int> get_selected()
  {
    return list_view.get_selected();
  }

  int get_selected_row() //On single selection mode
  {
    std::vector<int> selected_list =  list_view.get_selected();
    return selected_list[0];
  }

  Glib::ustring get_text(int row, int column)
  {
    return list_view.get_text(row, column);
  }

  void set_text(guint row, guint column, const Glib::ustring& value)
  {
    list_view.set_text(row, column, value);
  }

};

}


