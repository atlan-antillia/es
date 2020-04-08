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
 *  OpenGLVisualBuffer.h
 *
 *****************************************************************************/

#pragma once

//2015/04/15 Modified classname

#include <es++/opengl/OpenGLObject.h>
#include <es++/Pair.h>

namespace Es {

const int OpenGL_SINGLE_BUFFER = 1;
const int OpenGL_DOUBLE_BUFFER = 2;
	
const char* XmNopenGLBufferDepth = "openglBufferDepth";
const char* XmNopenGLBufferType  = "openglBufferType";

class OpenGLVisualBuffer :public OpenGLObject {
	
private:
  static const int ATTR_MAX = 50;
	
private:
  Pair<int, int> attributes[ATTR_MAX];
  int count;  


public:
  OpenGLVisualBuffer()
  :OpenGLObject()
  {
    clear();
  }

public:
  void clear()
  {
  	count = 0;
  	for(int i = 0; i < ATTR_MAX; i++) {
  		attributes[i].first  = None;
  		attributes[i].second = None;
  	}
  }
	
public:
  void set(int id, int value)
  {
  	if (count <ATTR_MAX) {
  	  attributes[count].first  = id;
  	  attributes[count].second = value;
  	  count++;
  	}
  }

public:
  int* getAttributes()
  {
    return (int*)attributes;
  }

public:
  int* singleBuffer(int bitsPerColor, int bitDepth)
  {
    // Attributes for a single buffered visual in RGBA format
    set(GLX_RGBA,          True);
   // set(GLX_DOUBLEBUFFER, False); 2015/04/13
    set(GLX_RED_SIZE,      bitsPerColor);
    set(GLX_GREEN_SIZE,    bitsPerColor);
    set(GLX_BLUE_SIZE,     bitsPerColor);
    set(GLX_DEPTH_SIZE,   bitDepth);
    set(None, None);
    return (int*)attributes;
  } 

  int* singleBufferDepth16()
  {
    return singleBuffer(4, 16);
  }

  int* singleBufferDepth24()
  {
    return singleBuffer(8, 24);
  }

  int* doubleBuffer(int bitsPerColor, int bitDepth)
  {
    // Attributes for a double buffered visual in RGBA format
    set(GLX_RGBA,         True);
    set(GLX_DOUBLEBUFFER, True);
    set(GLX_RED_SIZE,     bitsPerColor);
    set(GLX_GREEN_SIZE,   bitsPerColor);
    set(GLX_BLUE_SIZE,    bitsPerColor);
    set(GLX_DEPTH_SIZE,   bitDepth);
    set(None, None);
    
    return (int*)attributes;
  }

  int* doubleBufferDepth16()
  {
    return doubleBuffer(4, 16);
  }

  int* doubleBufferDepth24()
  {
    return doubleBuffer(8, 24);
  }
};
	
}

