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

//2017/06/25

#pragma once

#include <gtkmm/window.h>

#include <cairomm/context.h>
#include <cairomm/surface.h>

#include <giomm/resource.h>
#include <gdkmm/general.h> 
#include <glibmm/fileutils.h>
#include <iostream>

#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>
#include <es++/File.h>
#include <es++/ModuleFileName.h>

namespace Es {

class ImageView : public Gtk::DrawingArea {
private:
  Glib::RefPtr<Gdk::Pixbuf> pixbuf;
 
public:
  ImageView()
  {
  }

  ~ImageView()
  {
  }

  virtual void loadImage(const std::string& filename)
  {
    const char* name = filename.c_str();
    if (name == NULL) {
       throw IException("Invalid filename %s", name);
    }

    std::string path = filename;
    if (name[0] != '/') {
      //2019/04/03 Modified to use fullpath (module-path + filename)
      ModuleFileName module;
      path = module.getPath();
      path += "/";
      path += filename;
    }
    Es::File file(path);//filename);
    //printf("%s¥n", path.c_str());

    if (file.exists() == false) {
      throw IException("imagefile not found %s", filename.c_str());
    } 

    try {
      pixbuf = Gdk::Pixbuf::create_from_file(path); //filename);

    } catch(const Gio::ResourceError& ex) {
      std::cerr << "ResourceError: " << ex.what() << std::endl;
      throw IException("ResourceError filenamae:%s", filename.c_str());
    } catch(const Gdk::PixbufError& ex) {
      std::cerr << "PixbufError: " << ex.what() << std::endl;
      throw IException("PixbufError filenamae:%s", filename.c_str());
    }

    if (pixbuf) {
      int w = pixbuf -> get_width();
      int h = pixbuf -> get_height();
      set_size_request(w, h);
    }
  }

  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) 
  {
    if (!pixbuf)
      return false;

    Gdk::Cairo::set_source_pixbuf(cr, pixbuf, 0, 0);

    cr->paint();

    return true;
  }

  void set_pixbuf( Glib::RefPtr<Gdk::Pixbuf> buf )
  {
    this->pixbuf = buf;
  }
};
}

