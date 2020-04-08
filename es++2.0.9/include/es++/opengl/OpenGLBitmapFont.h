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
 *  OpenGLBitmapFont.h
 *
 *****************************************************************************/

#pragma once

#include <es++/opengl/OpenGLObject.h>
#include <es++/opengl/OpenGLFont.h>

namespace Es {

class OpenGLBitmapFont :public OpenGLFont {

public:
  OpenGLBitmapFont(void* font = GLUT_BITMAP_TIMES_ROMAN_24)
  :OpenGLFont(font)
  {
     if (validate(font) == false) {
       throw IException("Invalid font");
     }
  }

  bool validate(void* font)
  {
     // See /usr/include/GL/freeglut_std.h
     void* fonts[] = {
       GLUT_BITMAP_9_BY_15,
       GLUT_BITMAP_8_BY_13,
       GLUT_BITMAP_TIMES_ROMAN_10, 
       GLUT_BITMAP_TIMES_ROMAN_24,
       GLUT_BITMAP_HELVETICA_10, 
       GLUT_BITMAP_HELVETICA_12,
       GLUT_BITMAP_HELVETICA_18,
     };
     bool rc = false;
     for (int i = 0; i<SizeOf(fonts); i++) {
       if (font == fonts[i]) {
         rc = true;
         break;
       }
     }
    return rc;
  }

  void bitmapCharacter(int character) 
  {
    glutBitmapCharacter(getFont(), character);
  }

  int  bitmapWidth(int character)
  {
    return glutBitmapWidth(getFont(), character);
  }

  int  bitmapLength(const unsigned char* string)
  {
    return glutBitmapLength(getFont(), string);
  }

  void drawString(const char* string)
  {
    if (string) {
      size_t len = strlen(string);
      for (size_t i = 0; i < len; i++) {
        bitmapCharacter(string[i]);
      }
    }
  }

  void drawString(int x, int y, const char* string)
  {
    if (string) {
      glRasterPos2i(x, y);
      size_t len = strlen(string);
      for (size_t i = 0; i < len; i++) {
        bitmapCharacter(string[i]);
      }
    }
  }
};

}

