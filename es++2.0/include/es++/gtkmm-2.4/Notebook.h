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
 *      Notebook.h
 *
 *****************************************************************************/

#pragma once

#include <gtkmm/notebook.h>
#include <es++/gtkmm-2.4/TypeDef.h>

namespace Es {

class Notebook : public Gtk::Notebook {

public:
  Notebook(Gtk::PositionType pos=Gtk::POS_TOP, bool scroll=true)
  {
    set_tab_pos(pos);
    set_scrollable(scroll);
  }

  void set_last_page_current()
  {
    int page_num = get_n_pages();
    set_current_page(page_num-1);
    // Much simpler way:set_current_page(-1);
  }

  Widget* get_active_page() 
  {
    int page_num = get_current_page();

    return get_nth_page(page_num);
  }

  Glib::ustring get_nth_tab_text(int page_num)
  {
    Widget* widget =  get_nth_page(page_num);
    return get_tab_label_text(*widget);
  }

};

}

