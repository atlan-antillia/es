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
 *      FileComboBox.h
 *
 *****************************************************************************/

#pragma once

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/combobox.h>
#include <es++/gtkmm-3.0/TypeDef.h>
#include <es++/SmartPtrs.h>
#include <es++/DirEntryList.h>
#include <es++/DirectoryScanner.h>


namespace Es {

class FileComboBox : public Gtk::HBox {
private:
  Gtk::Label        label;
  Gtk::ComboBoxText combobox;

public:
  FileComboBox()
  {
    pack_start(label, Gtk::PACK_SHRINK);
    pack_start(combobox, Gtk::PACK_SHRINK);
  }

  void set_label(const char* text)
  {
    label.set_label(text);
  }
  
  void set_label(const std::string& text)
  {
    label.set_label(text);
  }
  
  void append(const std::string& dir, const std::string& pattern)
  {
    append(dir.c_str(), pattern.c_str()); 
  }

  void append(const char* dir, const char* pattern)
  {
    DirEntryList fileList;
    DirectoryScanner scanner(dir);
    bool skipDotFile = true;
    int n = scanner.scanFile(pattern, fileList, &skipDotFile);
    if (n > 0) {
      SmartPtrs<char*> fileNames = new char*[n];
      char** names = (char**)fileNames; 
      fileList.getNames(names);
      append_items((const char**)names, n);
    }
    //fileList.display();
  }

  void append_items(const char** items, size_t count)
  {
    try {
      for (size_t i = 0; i<count; i++) {
        append_item(items[i]);
      }
    } catch (...) {
    }
  }

  void append_item (const Glib::ustring& text) 
  {
    combobox.append(text);
  } 
 
  void  insert (int position, const Glib::ustring& text) 
  {
    combobox.insert(position, text);
  }

  void  prepend (const Glib::ustring& text) 
  {
    combobox.prepend(text);
  }

  void remove_all()
  {
    combobox.remove_all();
  }

  void set_changed_callback(Es::SigcCallback callback)
  {
    combobox.signal_changed().connect(callback);
  }

  Gtk::ComboBoxText& get_combobox()
  {
    return combobox;
  }

  Gtk::TreeModel::iterator get_active()
  {
    return combobox.get_active();
  }

  int get_active_row_number()
  {
    return combobox.get_active_row_number();
  }


  void set_active_text(const char* text)
  {
    return combobox.set_active_text(text);
  }

  void set_active_text(const std::string& text)
  {
    return combobox.set_active_text(text);
  }

  Glib::ustring get_active_text()
  {
    //Check whether this combobox is not empty.
    if (get_active_row_number() >0) { 
      return combobox.get_active_text();
    } else {
      throw IException("ComboBox is empty");
    }
  }

};

}
