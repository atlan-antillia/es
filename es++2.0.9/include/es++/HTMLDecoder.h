/******************************************************************************
 *
 * Copyright (c) 2014 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  HTMLDecoder.h
 *
 *****************************************************************************/


#pragma once


#include <es++/Object.h>
#include <es++/CharString.h>

namespace Es {

/**
 *
 */
class HTMLDecoder :public Object {

public:
  /**
   */
  HTMLDecoder() {

  }

public:
  /**
   *
   */
  bool decode(const char* string, CharString& decoded) {
    bool rc = false;
    if (string) {
      char* dec = decode(string);
      decoded = dec;
      delete [] dec;
      rc = true;
    }
    return rc;
  }


public:
  CharString decode(const CharString bstring) {
    const char* dec = NULL;
    const char* string = (const char*)bstring;
    if (string) {
      dec = decode(string);
    }
    return CharString(dec, false);
  }


public:
  char* decode(const char* string) {

    static const char*  entities[] = {"&amp;", "&lt;", "&gt;", "&#039;", "&quot;", "&nbsp;"};
    static const char   specials[] = {'&',     '<',    '>',    '\'',     '"',     ' '};

    if (string == NULL) {
      return NULL;
    }

    char* decoded = new char[strlen(string)+1];
    const char* src = string;

    char* dst = decoded;
    size_t entlen = SizeOf(entities);
    
    while (*src) {
      if (*src == '&') {
        for (size_t i= 0; i<entlen; i++) {
          const char* entity = entities[i];
          if (memcmp(src, entity, strlen(entity)) ==0) {
            char s = specials[i];
            *dst++ = s;
            src = src + strlen(entity);
            break;                
          }
        }
      } else {
        *dst++ = *src++;
      }
    }  
    *dst = ZERO;
    
    return decoded;
  }
};


}
