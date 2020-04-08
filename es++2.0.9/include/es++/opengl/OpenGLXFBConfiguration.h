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
 *  OpenGLXFBConfiguration.h 
 *
 *****************************************************************************/

#pragma once

#include <es++/opengl/OpenGLObject.h>

namespace Es {

class OpenGLXFBConfiguration : public OpenGLObject {

private:
  Display*   display;
  int        numItems;
  GLXFBConfig*  configurations;

public:
  OpenGLXFBConfiguration(Display* disp, int* attributes=NULL)
  :display(disp),
  numItems(0),
  configurations(NULL) 
  { 
    //Default attribute for pixelformat.
    static int defaultAttributes[]= {
      //GLX_X_RENDERABLE    , True,
      GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
      GLX_RENDER_TYPE     , GLX_RGBA_BIT,
      GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
      GLX_RED_SIZE        , 8,
      GLX_GREEN_SIZE      , 8,
      GLX_BLUE_SIZE       , 8,
      GLX_ALPHA_SIZE      , 8,
      GLX_DEPTH_SIZE      , 24,
      GLX_STENCIL_SIZE    , 8,
      GLX_DOUBLEBUFFER    , True,
      //GLX_SAMPLE_BUFFERS  , 1,
      //GLX_SAMPLES         , 4,
      None
    };

    if (attributes == NULL) {
      attributes = defaultAttributes;
    }
 
    //XSync(display, false);
    int screen = XDefaultScreen(display);
    configurations = glXChooseFBConfig( display, screen, 
                  attributes, &numItems);
  
    if (configurations == NULL || numItems <= 0) {
      throw IException("Failed to glXChooseFBConfig.");
    }
  }
  
  ~OpenGLXFBConfiguration()
  {
    XFree(configurations);
    configurations = NULL;
  }

  GLXFBConfig getConfig(int n = 0)
  {
    if (configurations && numItems > 0) {
     if ( n >= 0 && n <numItems) {
        return configurations[n];
      } else {
        throw IException("Invalid index parameter.");
      }
    } else {
      throw IException("No valid GLXFBConfig.");
    }
  }

  XVisualInfo* getVisualInfo(int n=0)
  {
    GLXFBConfig config = getConfig(n);
    XVisualInfo* vi = glXGetVisualFromFBConfig(display, config);
    if (vi == NULL) {
      throw IException("Failed to glxGetVisualFromFBConfig.");
    }
    return vi;
  }
};

}



