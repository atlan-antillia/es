/******************************************************************************
 *
 * Copyright (c) 2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      OpenGLApplication.h
 *
 *****************************************************************************/


#pragma once


#include <es++/StringT.h>

#include <es++/gtkmm-3.0/Application.h>
#include <gdk/gdkx.h>

#include <es++/opengl/OpenGLContext.h>
#include <es++/opengl/IOpenGLMainView.h>
#include <GL/glut.h>

namespace Es {
const char* XmNopenGLContext = "openGLContext";
  
class OpenGLApplication :public Application {

private:
  ::Display* xdisplay;
  ::Window xroot_window;

  OpenGLContext*      glContext;
  int   majorVersion;
  int   minorVersion;

  bool  looping;

public:
  OpenGLApplication(int argc, char** argv)
  :Application(argc, argv),
  xdisplay(NULL),
  xroot_window(None),
  glContext(NULL),
  majorVersion(2),
  minorVersion(1),
  looping(true)
  {
    glutInit(&argc, argv);

    if (argc >= 3) {
      for (int i = 1; i< argc; i++) {
        if (StringT<char>::startsWith(argv[i], "major=")) {
          char* eq = strstr(argv[i], "=");
          int major = atoi(++eq);
          if (major > 1) {
            majorVersion = major;
          }
        }
        if (StringT<char>::startsWith(argv[i], "minor=")) {
          char* eq = strstr(argv[i], "=");
          int minor = atoi(++eq);
          if (minor >= 0) {
            minorVersion = minor; 
          }
        }
      }
    }
    xdisplay = gdk_x11_get_default_xdisplay();
    if (xdisplay) {
      printf("Found default xdisplay\n");
    } else {
      printf("Not Found\n");
    }
    xroot_window = gdk_x11_get_default_root_xwindow();
    printf("Major=%d, Minor=%d\n", majorVersion, minorVersion);
    glContext = new OpenGLContext(xdisplay, majorVersion, minorVersion);
  }

  ~OpenGLApplication() 
  {
    if (glContext) {
      delete glContext;
      glContext = NULL;
    }
  }

  ::Display* getDisplay()
  {
    return xdisplay;
  }

  ::Window getRootWindow()
  {
    return xroot_window;
  }
  Visual* getVisual()
  {
    return glContext->getVisual();
  }

  int     getDepth()
  {
    return glContext->getDepth();
  }

  Colormap getColormap()
  {
    return glContext->getColormap();
  }

  OpenGLContext* getOpenGLContext() const
  {
    return glContext;
  }


  int getMajorVersion() 
  {
    return majorVersion;
  }

  int getMinorVersion()
  {
    return minorVersion;
  }
 
  virtual void run(Es::IOpenGLMainView& window, int delay=40)
  {
    window.show();

    int microsec = delay*1000;

    while(looping) {
      while(Gtk::Main::events_pending()) {
        Gtk::Main::iteration(false);
      }
      usleep(microsec);
      window.render();
    }
  }
  
  void quit()
  {
     looping = false;
  }
 
};

}

