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
 *  StringBufferT.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/StringT.h>
#include <es++/Exception.h>

namespace Es {

template <class T> class StringBufferT :public Object{

private:
  int  size;
  int  increment;
  T*  buffer;

  int  position;

public:

  StringBufferT(int s =1024, int inc = 128)
    :size(s), increment(inc), buffer(NULL),
    position(0)
  { 
    assert(size>0);
    assert(increment>0);

    this->buffer = new T[size+1];
    __memset(this->buffer, (T)0, size+1);
  }

public:
  ~StringBufferT()
  {
    if (this->buffer) {
      delete [] this->buffer;
      this->buffer = NULL;
    }
  }

public:
  void clear()
  {
    this->position = 0;
    if (this->buffer) {
      __memset(this->buffer, (T)0, size+1);
    }
  }

public:
  bool append(T ch)
  {  
    if (position >=size) {
      size += increment;
      T* temp = new T[size+1];
      if (temp == NULL) {
        return false;
      }
      __memset(temp, (T)0, size+1);
      __memcpy(temp, buffer, position);
      delete [] buffer;
      buffer = temp;
    }

    *(buffer+position) = ch;
    position++;
    *(buffer+position) = (T)0;

    return true;
  }

public:
  bool append(const T* string)
  {
    bool rc = true;
    if (string) {
      int len = __strlen(string);
     
      for (int i = 0; i<len; i++) {
        rc = append(*string++);
      }
    }
    return rc;
  }


private:
  bool appendFormat(T* format,...)
  {
    T buffer[128];
    __memset(buffer, (T)0, 128);

    va_list pos;
    va_start(pos, format);
    __vsnprintf(buffer, sizeof(buffer)/sizeof(buffer[0]), format, pos);
    va_end(pos);
    return append(buffer);
  }

public:
  bool append(int num)
  {
    T format[8] ={(T)'%',(T)'d', (T)0};  
    return appendFormat(format, num);
  }


public:
  bool append(float num)
  {
    T format[8] ={(T)'%',(T)'f', (T)0};  
    return appendFormat(format, num);

  }

public:
  bool append(double num)
  {
    T format[8] ={(T)'%',(T)'f', (T)0};  
    return appendFormat(format, num);
  }


public:
  bool append(StringT<T>& string)
  {
    return append((const T*)string);
  }

public:
  StringT<T>* newString() 
  {
    //buffer should be NULL terminated
    return new StringT<T>(this->buffer);  
  }

public:
  bool getString(StringT<T>& string)
  {
    T empty[1] = {(T)0};
    string = empty;

    bool rc = false;
    if (buffer) {
      string = this->buffer;
      rc = true;
    }
    return rc;
  }

public:
  const T* find(const T* string)
  {
    const T* ptr = NULL;
    if (buffer && string) {
      ptr = __strstr(buffer, string);
    }
    return ptr;
  }

public:
  const T* getBuffer() const 
  { 
    return this->buffer; 
  }

public:
  operator const T*() const
  {
    return this->buffer;
  }

public:
  int  getBufferSize() const  
  { 
    return this->size; 
  }

public:
  int  getContentSize() const 
  { 
    return this->position; 
  }

};

}


