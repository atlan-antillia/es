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
 *  Bytes.h
 *
 *****************************************************************************/

// 2015/03/17 Added toHexString method.
// Array ->Bytes

#pragma once

#include <es++/ArrayT.h>
#include <es++/StringBufferT.h>
#include <es++/CharString.h>
#include <es++/Exception.h>

/**
 * Bytes class
 */
namespace Es {

class Bytes :public ArrayT<unsigned char> {
private:
  CharString string;

public:
  /**
   * Constructor
   */
  Bytes()
    :ArrayT<unsigned char>(0)
  {
  }

public:
  /**
   * Constructor
   */
  Bytes(unsigned char* data, size_t size, char delim=':')
  :ArrayT<unsigned char>(data, size)
  {
  	string = toCharString(delim);
  }

public:
  Bytes(const Bytes& bytes, char delim = ';')
  :ArrayT<unsigned char>(bytes)
  {
  	string = toCharString(delim);
  }
  


public:
  /**
   * Destructor
   */
  ~Bytes() 
  {
  }

  void display()
  {
    dump(getArray(), getSize());
  }

  void dump()
  {
    dump(getArray(), getSize());
  }

  //2015/03/17
  CharString toHexString()
  {
    const unsigned char* array = getArray();
    size_t   size        = getSize();
    StringBufferT<char> buffer;
    for (size_t i = 0; i<size; i++) {
      char string[20];
      if (i > 0) {
        buffer.append(":");
      }
      sprintf(string, "%02x", array[i]);
        
      buffer.append(string);
    }
    return CharString(buffer.getBuffer());
  }
    
  //2015/03/25
  CharString toCharString(char delim=':')
  {
    const unsigned char* array = getArray();
    size_t   size        = getSize();
    StringBufferT<char> buffer;
    for (size_t i = 0; i<size; i++) {
      char string[20];
      if (i > 0) {
        buffer.append(delim);
      }
      sprintf(string, "%02x", array[i]);
        
      buffer.append(string);
    }
    return CharString(buffer.getBuffer());
  }
	
  //2015/03/25
  const char* toString()
  {
  	return (const char*)string;
  }
	
  static void dump(const unsigned char* array, size_t size)
  {
    char format[128];
    sprintf(format, "%%0%ldx ", sizeof(unsigned char)*2);
   
    printf("Size = %ld(Bytes)\n", size);
    for (size_t i = 0; i<size; i++) {
      if (i >0 && (i %16) == 0) {
        printf("\n");
      }
      printf(format, array[i]);
    }
    printf("\n");
  }
};

}


