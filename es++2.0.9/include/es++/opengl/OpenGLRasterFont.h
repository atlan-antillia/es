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
 *  OpenGLRasterFont.h
 *
 *****************************************************************************/

// Please see: http://www-f9.ijs.si/~matevz/docs/007-2392-003/sgi_html/ch03.html

#pragma once

#include <es++/opengl/OpenGLObject.h>
#include <es++/opengl/OpenGLLists.h>
#include <es++/Exception.h>

namespace Es {

class OpenGLRasterFont :public OpenGLObject {
private:
  OpenGLLists* lists;

public:
  OpenGLRasterFont(Display* display, const char* fontName)
  :OpenGLObject(display),
  lists(NULL)
  {
    XFontStruct* fontStruct = XLoadQueryFont(display, fontName);

    if (fontStruct == NULL) {
      throw IException("Failed to XLoadQueryFont %s", fontName);
    }

    Font fid = fontStruct->fid;
    unsigned int first = fontStruct -> min_char_or_byte2;
    unsigned int last  = fontStruct -> max_char_or_byte2;
    lists = new OpenGLLists(last+1); 
    GLuint base = lists -> getBase();
    glXUseXFont(fid, first, last-first+1, base+first);
  }

  ~OpenGLRasterFont()
  {
    delete lists;
  }

  void draw(int x, int y, const char* string) 
  {
    if (string) {
      glRasterPos2i(x, y);
      lists -> call(strlen(string), GL_UNSIGNED_BYTE, (void*)string);
    }
  }

  //2015/06/20 This really works for a wide char string?
  void draw(int x, int y, const wchar_t* string)
  {
    if (string) {
      glRasterPos2i(x, y);
      lists -> call(wcslen(string), GL_4_BYTES, (void*)string);
    }
  }
};
}

