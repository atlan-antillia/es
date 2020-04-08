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
 *  PulldownMenu.h
 *
 *****************************************************************************/

//2017/06/25

#pragma once

#include <es++/AuxDef.h>
#include <es++/ListT.h>

#include <gtkmm.h>
#include <es++/gtkmm-3.0/TypeDef.h>

namespace Es {

class PulldownMenu :public Gtk::MenuItem {

private: 
  Gtk::Menu      submenu;   //pulldown menu container to be set to menu_item  
  ListT<Gtk::MenuItem>  pulldown_menu_items;

protected:
  PulldownMenu()
  {
  }

public:
  PulldownMenu(Gtk::MenuBar& menubar, const char* menu_name)
  {
    menubar.append(*this);

    set_label(menu_name);

    set_submenu(submenu);
  }
   
  ~PulldownMenu()
  {
  }

  PulldownMenu& append(
              Es::SigcMenuCallback* callbacks,
              int count)
  {
    for (int i = 0; i<count; i++) {
      MenuItem* item = new MenuItem();
      item->set_label(callbacks[i].name);
      item->signal_activate().connect (
                 callbacks[i].callback); 

      pulldown_menu_items.add(item);

      submenu.append(*item);
    }
    return *this;
  }
};

}

