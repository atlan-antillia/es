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
 *  ScrolledFileListView.h
 *
 *****************************************************************************/

//On deprecate StockID, see: https://developer.gnome.org/gtkmm/stable/namespaceGtk_1_1Stock.html

#pragma once

#include <es++/DirectoryScanner.h>
#include <es++/MonetaryFormatter.h>
#include <es++/FileAttributes.h>
#include <es++/LocalTime.h>
#include <es++/SmartArray.h>
#include <es++/SmartPtr.h>
#include <es++/SmartPtrs.h>

namespace Es {

class ScrolledFileListView : public Gtk::ScrolledWindow {
private:
  /////////////////////////////////////////////////////////
  class FileAttributeRecord : public Gtk::TreeModel::ColumnRecord {

  public:
    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> >  col_image;
    Gtk::TreeModelColumn<Glib::ustring> col_name;
    Gtk::TreeModelColumn<Glib::ustring> col_datetime;
    Gtk::TreeModelColumn<Glib::ustring> col_size;

    FileAttributeRecord()
    {
      add(col_image );
      add(col_name );
      add(col_datetime );
      add(col_size );
    }
  };

  /////////////////////////////////////////////////////////

private:
  Gtk::TreeView                 treeview;

  FileAttributeRecord           fileattribute_record;
  Glib::RefPtr<Gtk::ListStore>  liststore;

  Gtk::TreeViewColumn           col_filename;
  Gtk::TreeViewColumn           col_datetime;

  Gtk::TreeViewColumn           col_size;

  Gtk::CellRendererPixbuf       ren_pixbuf;
  Gtk::CellRendererText         ren_text1;
  Gtk::CellRendererText         ren_text2;
  Gtk::CellRendererText         ren_text3;

  Glib::RefPtr<Gtk::IconTheme>  icon_theme;
  Glib::RefPtr<Gdk::Pixbuf>     pixbuf_folder;
  Glib::RefPtr<Gdk::Pixbuf>     pixbuf_file;

  typedef enum {
   TYPE_FOLDER = 1,
   TYPE_FILE   = 2
  } FileType;

public:
  ScrolledFileListView()
  :col_filename("FileName"),
  col_datetime("DateTime"),
  col_size("Size")
  {
    liststore = Gtk::ListStore::create( fileattribute_record );

    treeview.set_model( liststore );

    treeview.append_column( col_filename );
    treeview.append_column( col_datetime );
    treeview.append_column( col_size );

    col_filename.pack_start( ren_pixbuf, false );
    col_filename.add_attribute( ren_pixbuf, "pixbuf", 0 ); 

    col_filename.pack_start( ren_text1, true );
    col_datetime.pack_start( ren_text2, true );
    col_size.    pack_start( ren_text3, true );

    col_filename.add_attribute( ren_text1, "text", 1 ); 
    col_datetime.add_attribute( ren_text2, "text", 2 ); 
    col_size.    add_attribute( ren_text3, "text", 3 ); 

    icon_theme_load();

    add(treeview);
  }

  void icon_theme_load()
  { 
    icon_theme = Gtk::IconTheme::get_for_screen(get_screen());
    int width  = 0;
    int height = 0;
    Gtk::IconSize::lookup(Gtk::ICON_SIZE_BUTTON, width, height);
    width = width*17/10;  //1.7

    //Directory icon
    //Gtk::Stock::DIRECTORY is deprecated.
    //Gtk::Widget::render_icon( Gtk::Stock::DIRECTORY, Gtk::ICON_SIZE_MENU );

    std::string folder_icon_name = "folder";
    pixbuf_folder = icon_theme->load_icon(folder_icon_name, width, 
                    Gtk::ICON_LOOKUP_GENERIC_FALLBACK);
    //File icon
    //Gtk::Stock::FILE is deprecated.
    //Gtk::Widget::render_icon( Gtk::Stock::FILE, Gtk::ICON_SIZE_MENU );

    std::string file_icon_name = "text-x-generic";
    pixbuf_file = icon_theme->load_icon(file_icon_name, width, 
                    Gtk::ICON_LOOKUP_GENERIC_FALLBACK);
  }

  void listup_folders(std::string& dir, std::string& pattern)
  {
    listup_folders(dir.c_str(), pattern.c_str());
  }

  void listup_folders(const char* dir, const char* pattern)
  {
    DirectoryScanner fileScanner(dir);
    DirEntryList fileList;
    int n = fileScanner.scanDir(pattern, fileList);
    if (n > 0) {
      SmartPtrs<char*> fileNames = new char*[n];
      fileList.getNames((char**)fileNames);
      add_items(dir, (const char**)(char**)fileNames, n, TYPE_FOLDER);
    }
  }

  void listup_files(std::string& dir, std::string& pattern)
  {
    listup_files(dir.c_str(), pattern.c_str());
  }

  void listup_files(const char* dir, const char* pattern)
  {
    DirectoryScanner fileScanner(dir);
    DirEntryList fileList;
    int n = fileScanner.scanFile(pattern, fileList);
    if (n > 0) {
      SmartPtrs<char*> fileNames = new char*[n];
      fileList.getNames((char**)fileNames);
      add_items(dir, (const char**)(char**)fileNames, n, TYPE_FILE);
    } 
  }

  void add_items(const char* dir, const char** items, int num, FileType type)
  {
    char path[PATH_MAX];
    setlocale(LC_NUMERIC, "");

    for (int i = 0; i<num; i++) {
      sprintf(path, "%s/%s", dir, items[i]);
      try {
        FileAttributes attr(path);
        
        LocalTime lt(attr.changedTime());
        std::string changed_time = lt.toCharString();
        char csize[128];
        unsigned long fs = attr.getSize();
        double kb = (double)fs/1024.0L;
        sprintf(csize, "%'0.1fKB", kb);

        Gtk::TreeModel::Row row = *(liststore->append());
        if (type == TYPE_FOLDER) {
          row[fileattribute_record.col_image   ] = pixbuf_folder;
        } else {
          row[fileattribute_record.col_image   ] = pixbuf_file;
        }
        row[fileattribute_record.col_name    ] = items[i];
        row[fileattribute_record.col_datetime] = changed_time;
        row[fileattribute_record.col_size    ] = csize;

      } catch (...) {
      }
    }
    setlocale(LC_ALL, "");
  }

  bool is_folder(Gtk::TreeModel::Row& row)
  {
    bool folder = false;
    Glib::RefPtr<Gdk::Pixbuf>     pixbuf = 
          row[fileattribute_record.col_image   ]; 
    if (pixbuf == pixbuf_folder) {
      folder = true;
    }
    return folder;
  }

  std::string get_filename(Gtk::TreeModel::Row& row)
  {
     Gtk::TreeValueProxy<Glib::ustring> filename
          = row[fileattribute_record.col_name];
     return std::string((Glib::ustring)filename);
  }

  std::string get_datetime(Gtk::TreeModel::Row& row)
  {
     Gtk::TreeValueProxy<Glib::ustring> filename
          = row[fileattribute_record.col_datetime];
     return std::string((Glib::ustring)filename);
  }

  std::string get_size(Gtk::TreeModel::Row& row)
  {
     Gtk::TreeValueProxy<Glib::ustring> filename
          = row[fileattribute_record.col_size];
     return std::string((Glib::ustring)filename);
  }

  void display(Gtk::TreeModel::Row& row) 
  {
    std::string name = get_filename(row);
    std::string datetime = get_datetime(row);
    std::string size = get_size(row);

    printf("%s %s %s\n", name.c_str(), datetime.c_str(), size.c_str());
  }

  Gtk::TreeView& get_treeview()
  {
    return treeview;  
  }

  Glib::RefPtr<Gtk::TreeSelection> get_selection()
  {
    return treeview.get_selection();
  } 

  void clear()
  {
    liststore->clear(); 
  } 
 
};

}

