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
 *  URLEncoder.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/CharString.h>


/**
 */
namespace Es {

class URLEncoder :public Object {

public:
  /**
   */
  URLEncoder() 
  {
  }

  /**
   */
  ~URLEncoder() 
  {
  }

public:
  /**
   * @param data    [IN]  Null-terminated string to encode.
   * @param encoded  [OUT]  URL-encoded string.
   */
  bool encode(const char* data, CharString& encoded) {
    bool rc = false;
    char* enc = encode(data);
    if (enc) {
      encoded = enc;
      delete [] enc;
      rc = true;
    }
    return rc;
  }

public:
  CharString encode(const CharString bstring) {
    const char* enc = NULL;
    const char* string = (const char*)bstring;
    if (string) {
      enc = encode(string);
    }
    return CharString(enc, false);
  }

public:
  /**
   * @param data  [IN] Null-terminated string to encode.
   * @return char*  URL-encoded string.
   */
  char* encode(const char* data) {

    if (data == NULL) {
      return NULL;
    }
    static const char table[] = "0123456789ABCDEF";

    int len = strlen(data);

    int slen = len*3 + 1;
    char* buffer = new char[slen];
    memset(buffer, 0, slen);
  
    int n = 0;
    for (int i = 0; i < len; i++) {
      //Important
      unsigned char c = (unsigned char)data[i] & 0xff;
      if (isalnum(c)) {
        buffer[n++] = c;
      }
      else if ( c == '-' || c == '_' || c== '.' || c== '*') {
        buffer[n++] = c;  
        //Refer Java URLEncoder class.
      }
      else if (c == ' '){
        buffer[n++] = '+';
      } else {
        buffer[n++] = '%';
        buffer[n++] = table[c/16]; 
        buffer[n++] = table[c%16];
      }
      }
  
    buffer[n] = ZERO;  
    return buffer;
  }

};

}



