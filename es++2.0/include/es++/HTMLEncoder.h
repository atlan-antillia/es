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
 *  HTMLEncoder.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/CharString.h>
#include <es++/SmartPtrs.h>

namespace Es {


/**
 * HTMLEncoder class 
 */
class HTMLEncoder :public Object {

public:
  /**
   *
   */
  HTMLEncoder() {

  }

public:
  /**
   * Convert the characters '&', '<', '>', ''' and '"' in html string to the strings 
   * "&amp;", "&lt;", "&gt;", "&#039;" and "&quot;" 
   *
   * @param  string  html string to be encoded.
   * @param  encoded  encoded string.  
   * @return  bool    
   */
  bool encode(const char* string, CharString& encoded) {
    bool rc = false;
    if (string) {
      char* enc = encode(string);
      encoded = enc;
      delete [] enc;
      rc = true;
    }
    return rc;
  }


public:
  /**
   * Convert the characters '&', '<', '>', ''' , '"' and ' ' in html string to the strings 
   * "&amp;", "&lt;", "&gt;", "&#039;" ,"&quot;" and "&nbsp;" 
   *
   * @param  string  html string to be encoded.
   * @return  char*  encoded string.  
   */
   char*  encode(const char* string) {

     static const char  specials[] = {'&',     '<',    '>',    '\'',     '"',     ' '};
     static const char* entities[] = {"&amp;", "&lt;", "&gt;", "&#039;", "&quot;","&nbsp;"};

    if (string == NULL) { 
      return NULL;
    }

    int slen = strlen(string)*6 + 1;

    char* encoded = new char[slen];

    memset(encoded, 0, slen);

    int len    = strlen(string);
    int splen = sizeof(specials)/sizeof(specials[0]);
  
    int n = 0;
    for (int i = 0; i<len; i++) {
      char c = string[i];  
      const char* entity = NULL;
      
      for (int j = 0; j<splen; j++) {
        if (c == specials[j]) {
          entity = entities[j];
          break;
        }
      }
      if (entity != NULL) {
        for(size_t s = 0; s<strlen(entity); s++) {
          encoded[n++] = entity[s];
        }
      } else {
        encoded[n++] = c;
      }
    }

    encoded[n] = ZERO;

    return encoded;
  }

};

}

