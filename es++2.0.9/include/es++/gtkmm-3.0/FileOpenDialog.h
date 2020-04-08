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
 *  FileOpenDialog.h
 *
 *****************************************************************************/

//2017/06/25
//2017/08/11 Updated.

#pragma once

#include <gtkmm.h>

namespace Es {

class FileOpenDialog : public Gtk::FileChooserDialog {

public:
  typedef enum {
     TEXT_FILES  = 1,
     IMAGE_FILES = 1<<2,
     VIDEO_FILES = 1<<3, //2017/08/11
     ANY_FILES   = 1<<4, 

  } FILTER_TYPE;
  
  static const int ALL_FILTERS_COUNT = 3;
  static const int ALL_FILTERS = 
                 (int)(TEXT_FILES|IMAGE_FILES|ANY_FILES); 
  //gtkmm-3.0
  Glib::RefPtr<Gtk::FileFilter> filters[ALL_FILTERS_COUNT];
 
public:
  FileOpenDialog(Gtk::Window& owner, 
         int filter_types    = ALL_FILTERS,
         const char* title = "File Open Dialog")
  :FileChooserDialog(title, Gtk::FILE_CHOOSER_ACTION_OPEN)
  {
    set_transient_for(owner);

    add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    add_button("_Open", Gtk::RESPONSE_OK);

    addFilters(filter_types);
  }

  void addFilters(int filter_types)
  {
    int i = 0;
    if (filter_types & TEXT_FILES) {
      printf("TEXT_FILES type\n");
      filters[i] = Gtk::FileFilter::create();
      filters[i]->set_name("Text files");
      filters[i]->add_mime_type("text/plain");
      add_filter(filters[i]);
      i++;
    } 
    if (filter_types & IMAGE_FILES) {
      printf("IMAGE_FILES type\n");
      filters[i] = Gtk::FileFilter::create();
      filters[i]->set_name("Image files");
      filters[i]->add_mime_type("image/png");
      filters[i]->add_mime_type("image/jpeg");
      filters[i]->add_mime_type("image/gif");
      filters[i]->add_pattern("*.png");
      filters[i]->add_pattern("*.jpg");
      filters[i]->add_pattern("*.jpeg");
      filters[i]->add_pattern("*.gif");
      filters[i]->add_pattern("*.tif");
      add_filter(filters[i]);
      i++;
    }
    //2017/08/15
    if (filter_types & VIDEO_FILES) {
      printf("VIDEO_FILES type\n");
      filters[i] = Gtk::FileFilter::create();
      filters[i]->set_name("Video files");
      filters[i]->add_mime_type("video/x-msvideo");
      filters[i]->add_mime_type("video/mp4");
      filters[i]->add_mime_type("video/mpeg");
      filters[i]->add_mime_type("video/mpg");
      filters[i]->add_mime_type("video/webm");

      filters[i]->add_pattern("*.avi");
      filters[i]->add_pattern("*.mp4");
      filters[i]->add_pattern("*.mpg4");
      filters[i]->add_pattern("*.mpg");
      filters[i]->add_pattern("*.webm");
      add_filter(filters[i]);
      i++;
    }

    if (filter_types & ANY_FILES) {
      printf("ANY_FILES type\n");
      filters[i] = Gtk::FileFilter::create();
      filters[i]->set_name("Any files");
      filters[i]->add_pattern("*");
      add_filter(filters[i]);
      i++;
    }     
  }

  int popup()
  {
    int rc = run();
    hide();
    return rc;
  }
};

}

