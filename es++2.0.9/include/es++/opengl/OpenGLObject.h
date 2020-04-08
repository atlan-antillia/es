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
 *  OpenGLObject.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/SmartPtr.h>
#include <es++/Exception.h>
#define GL_GLEXT_PROTOTYPES

#include <X11/Xlib.h>

//2016/07/01
#include <GL/glew.h>
#include <GL/glxew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
   
//#include <GL/glext.h>       
#include <GL/glxext.h>       

#define MATH_PI   3.14159265f

namespace Es {

class OpenGLObject :public Object {

private:
  Display* display;
 
public:
  OpenGLObject()
  :display(NULL)
  {
  }

public:
  OpenGLObject(Display* display)
  :display(display)
  {
  }

public:
  Display* getDisplay()
  {
     return display;
  }

  GLenum getError()
  {
    return glGetError();
  }

  const char* errorString()
  {
    const char* errString = "";
    GLenum err = getError();
    if (err != GL_NO_ERROR) {
      errString = (const char*)gluErrorString(err);
    }
    return errString;
  }
 
  void* load(const char* name)
  {
    void* address = NULL;
    if (name && strlen(name) > 0) {
      address =  (void*)glXGetProcAddress((const GLubyte *)name);
      if (address == NULL) {
        throw IException("Failed to load function : %s", name);
      }
    } else {
      throw IException("Invalid argument.");
    }
    return address;
  }
  
  void checkError()
  {
    GLenum err = getError();
    if (err != GL_NO_ERROR) {
      const char* errString = (const char*)gluErrorString(err);
      throw Exception(err, "OpenGL error: %s", errString);
    }
  }

  inline float SIN(int degree)
  {
    float radian = (float)degree * MATH_PI / 180.0f;
    return (float)sin(radian);
  }
  
  inline float COS(int degree)
  { 
    float radian = (float)degree * MATH_PI / 180.0f;
    return (float)cos(radian);
  }

  inline float SIN(float degree)
  {
    float radian = degree * MATH_PI / 180.0f;
    return (float)sin(radian);
  }
  
  inline float COS(float degree)
  { 
    float radian = degree * MATH_PI / 180.0f;
    return (float)cos(radian);
  }
};

}

