/******************************************************************************
 *
 * Copyright (c) 2015 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  OpenGLContext.h 
 *
 *****************************************************************************/

//2016/08/20 Update to use OpenGLXFBConfiguration in OpenGLContext constructor
// to create a verion dependent context.

#pragma once

#include <es++/gtkmm-3.0/XSmartPtr.h>
#include <es++/opengl/OpenGLObject.h>
#include <es++/opengl/OpenGLXFBConfiguration.h>
#include <es++/opengl/OpenGLVisualBuffer.h>
#include <es++/opengl/OpenGLVisual.h>

namespace Es {

class OpenGLContext :public OpenGLObject {
private:
  OpenGLXFBConfiguration* configuration;
  GLXContext context;
  XVisualInfo* visualInfo;
  Colormap     colormap;
  int          bufferDepth;
  int          bufferType;

public:
  //2016/08/20 updated.
  OpenGLContext(Display* display,
            int majorVersion=2, int minorVersion=1)
  :OpenGLObject(display),
   configuration(NULL),
   visualInfo(NULL),
   context(NULL)
  {
    int* buffer = NULL;

    OpenGLVisualBuffer openGLBuffer;
    this -> bufferType = OpenGL_SINGLE_BUFFER;  
    this -> bufferType = OpenGL_DOUBLE_BUFFER;  
    this -> bufferDepth = 24;
 
    switch(bufferType) {
    
    case OpenGL_SINGLE_BUFFER: 
     if (bufferDepth == 24) {
       buffer = openGLBuffer.singleBufferDepth24();
     }
     if (bufferDepth == 16) {
       buffer = openGLBuffer.singleBufferDepth16();
     }
     break;

    case OpenGL_DOUBLE_BUFFER: 
     if (bufferDepth == 24) {
       buffer = openGLBuffer.doubleBufferDepth24();
     }
     if (bufferDepth == 16) {
       buffer = openGLBuffer.doubleBufferDepth16();
     }
     break;
    }

    OpenGLVisual openGLVisual(display);
    XVisualInfo* vinfo = openGLVisual.choose(buffer);
    XSmartPtr<XVisualInfo> vi(vinfo);
    if (display && vinfo) {
      //Create a context in old-fashioned style.
      context = glXCreateContext(display, vinfo, NULL, GL_TRUE); 
      if (context == NULL) {
        throw IException("Failed to create GLXContext");
      }
    } else {      
      throw IException("Invalid argument");
    }
  
    glXMakeCurrent(getDisplay(), None, context);
   
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
      throw IException("Failed to glewInit");
    }
    
    int attributes[] = {  
      GLX_CONTEXT_MAJOR_VERSION_ARB, majorVersion,
      GLX_CONTEXT_MINOR_VERSION_ARB, minorVersion,
      GLX_CONTEXT_FLAGS_ARB,         0,
      None,      
    };
    
    GLXContext newContext = NULL;
    if (glxewIsSupported("GLX_ARB_create_context")== GL_TRUE) {
     
      configuration = new OpenGLXFBConfiguration(display);
      GLXFBConfig config = configuration->getConfig(0);
      //Create a version-dependent context.
      newContext  = glXCreateContextAttribsARB(display, 
         config, NULL, True, attributes); 

      if (context == NULL) {
        throw IException("Failed to GLX_ARB_create_context: majorVersion=%d, minorVersion=%d",
             majorVersion, minorVersion);
      }

      GLXContext  prevRC = context;
      glXMakeCurrent(display, None, NULL);
      this->context = newContext; 
      glXMakeCurrent(display, None, context);
      glXDestroyContext(display, prevRC);

      this->visualInfo = configuration->getVisualInfo();
      this->colormap = XCreateColormap(display, 
                  RootWindow(display, visualInfo->screen), visualInfo->visual, AllocNone);
 
      printf("GLX_ARB_create_context is supported.\n");
 
      XSync(display, False );

    } else {
      printf("Warning: GLX_ARB_create_context is not supported.\n");
    }
  } 

public:
  ~OpenGLContext()
  {
    if (visualInfo) {
      XFree(visualInfo);
      visualInfo = NULL;
    }
    if (context) {
      Display* display = getDisplay();
      glXMakeCurrent(display, None, NULL);
      glXDestroyContext(display, context);
      context = NULL;
    }
    delete configuration;
    configuration = NULL;
    XFreeColormap(getDisplay(), colormap);
  }

  XVisualInfo* getVisualInfo()
  {
    if (visualInfo) {
      return visualInfo;
    } else {
      throw IException("VisualInfo is NULL.");
    }
  }

  int getDepth()
  {
    return getVisualInfo()->depth;
  }
  
  Visual* getVisual()
  {
    return getVisualInfo()->visual;
  }

  Colormap getColormap()
  {
    return colormap;
  }

  void makeCurrent(Window window) 
  {
    glXMakeCurrent(getDisplay(), window, context); 
  } 

  int getBufferType()
  {
    return bufferType;
  }
};

}

