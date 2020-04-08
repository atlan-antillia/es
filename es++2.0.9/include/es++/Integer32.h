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
 *	Integer32.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/CharString.h>

namespace Es {

class Integer32 :public Object {
private:
  int32_t      _value;
  CharString   _string;
	
public:
  Integer32(int32_t val) 
  :_value(val) 
  {
    _string = toCharString();
  }

  int32_t   getContents() const 
  { 
    return _value; 
  }

  void value(int32_t v)
  {
    this -> _value = v;
  } 

  int32_t  value() 
  {
     return this -> _value;
  } 

  operator int32_t() 
  { 
    return _value; 
  }

  void write()
  {
     printf("%4" PRId32 "", _value);
  }
	
  //2015/03/24
  CharString toCharString()
  {
    char string[128];
    sprintf(string, "%4" PRId32 "", _value);
    return CharString(string);
  }

  const char* toString()
  {
    return (const char*)_string;
  }
	
  void display()
  {
     printf("%4" PRId32 "\n", _value);
  }
};

}

