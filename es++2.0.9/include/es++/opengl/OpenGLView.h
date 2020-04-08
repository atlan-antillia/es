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
 *  OpenGLView.h
 *
 *****************************************************************************/

#pragma once

#include <gtkmm/drawingarea.h>

#include <es++/opengl/OpenGLContext.h>
#include <es++/opengl/OpenGLGC.h>
#include <es++/opengl/OpenGLBitmap.h>
#include <es++/opengl/OpenGLMainView.h>
#include <es++/opengl/OpenGLImageInfo.h>

#include <gdkmm-3.0/gdkmm.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>
#include <gtkmm/drawingarea.h>

using namespace Gtk;

namespace Es {

class OpenGLView :public Gtk::DrawingArea {
private:
  ::Display*      xdisplay;
  ::Window        xwindow;
  OpenGLContext*  context;
  int             bufferType;

public:
  OpenGLView()
  :DrawingArea()
  ,xdisplay(NULL)
  ,xwindow(None)
  {
    xdisplay = gdk_x11_get_default_xdisplay();

    set_app_paintable(TRUE);

    add_events(Gdk::POINTER_MOTION_MASK |
             Gdk::BUTTON_PRESS_MASK |
             Gdk::BUTTON_RELEASE_MASK);
  }

  //X11 Default Display
  ::Display* getDisplay()
  {
    return xdisplay;
  }

  //X11 Window
  ::Window   getWindow()
  {
    return xwindow;
  }


  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override
  {
    //2019/04/10
    DrawingArea::on_draw(cr);
    render();
    return true;
  }
 
  void on_size_allocate(Gtk::Allocation& allocation)
  {
    DrawingArea::on_size_allocate(allocation);
    int width = allocation.get_width();
    int height = allocation.get_height();
    
    resize(width, height);
  }

  virtual void render()
  {
    //Call a virtual method display.
    display();

    glFlush();
        
    if (bufferType == OpenGL_DOUBLE_BUFFER) {
      swapBuffers();
    }
    XFlush(xdisplay);
  }

  virtual void resize(int width, int height)
  {
    setViewPort(width, height);
  }

  void swapBuffers()
  {
    glXSwapBuffers(getDisplay(), getWindow());
  }

  // Please define your own display method in a sublcass
  // derived from this class.
  virtual void display()
  {
  }    

  void on_map()
  {
    DrawingArea::on_map();

    Glib::RefPtr<Gdk::Window> gdk_window = get_window();
    xwindow =  gdk_x11_window_get_xid(Glib::unwrap(gdk_window) );

    if (xwindow != None) {
      context -> makeCurrent(xwindow);
    }

    //Call a virtual initialize method.
    initialize();
  }

  // Define your own initialize method in a subclass derived
  // from this class.
  virtual void initialize()
  {
  }

  int width()
  {
    Gtk::Allocation allocation = get_allocation();
    return allocation.get_width();
  }

  int height()
  {
    Gtk::Allocation allocation = get_allocation();
    return allocation.get_height();
  }

  void setViewPort()
  {
    int w = width();
    int h = height();

    glViewport(0, 0, w, h);
  }

  void setViewPort(int w, int h)
  {
    glViewport(0, 0, w, h);
  }

  void setPerspective(GLdouble fovy=16.0,
        GLdouble zNear=0.5,
        GLdouble zFar =100.0f)
  {
    //Set a proportional perspective.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int w = width();
    int h = height();
    if (h > 0) {
      GLdouble aspect = (GLdouble)w / (GLdouble)h;
      gluPerspective(fovy, aspect, zNear, zFar);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }

  void set_opengl_context(OpenGLContext* context)
  {
    this->context = context;
    this->bufferType = context->getBufferType();
  }

  virtual bool on_key_press_event(GdkEventKey* event)
  {
    printf("KeyPressed\n");
    return true;
  }

  virtual void save(const char* filename)
  {
    if (StringT<char>::endsWithIgnoreCase(filename, ".jpg")) {
      saveAsJPG(filename);
    } 
    else if (StringT<char>::endsWithIgnoreCase(filename, ".png")) {
      saveAsPNG(filename);
    } else {
        //Ignore
    }
  }

  virtual void saveAsJPG(const char* filename)
  {
    try {
      int w = width();
      int h = height();
      int rw = (w / 8) * 8;
          
      OpenGLBitmap bitmap(0, 0, rw, h, 24, GL_RGB);

      bitmap.read(GL_FRONT);
      bitmap.saveAsJPG(filename);
     printf("saveAsJPG %s\n", filename);
    } catch (Exception& ex) {
      ex.display();
    }      
  }

  virtual void saveAsPNG(const char* filename)
  {
    try {
      int w = width();
      int h = height();
      int rw = (w / 8) * 8;
          
      //OpenGLBitmap bitmap(0, 0, w, h, 8, GL_RGBA);
      OpenGLBitmap bitmap(0, 0, rw, h, 8, GL_RGBA);
      bitmap.read(GL_FRONT);
      bitmap.saveAsPNG(filename);
     printf("saveAsPNG %s\n", filename);
    } catch (Exception& ex) {
      ex.display();
    }      
  }

};

}


