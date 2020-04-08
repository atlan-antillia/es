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
 * Console.h
 *
 *****************************************************************************/

// es++2.0

#pragma once

#include <es++/Object.h>
#include <es++/Locale.h>

namespace Es {

class Console :public Object {
private:
  FILE*    file;
  
private:
  // Buffer size 100KB for wchar_t* buffer;
  static const int BUFFER_SIZE = 1024;


protected:
  /**
   + Constructor
   *
   */
  Console(FILE* f)
   :file(f)
  {
    assert(file);

    Locale locale;
    fwide(file, 1);
  }

public:
  ~Console()
  {
    close();
  }
  
public:
  void close() 
  {
      file = NULL;
  }
  
 public: 
  virtual int writeString(const char* string) 
  {
    int rc = 0;
    if (string) {
      rc = fwprintf(file, L"%s", string);
    }
    return rc;
  }

public:
  virtual int write(const char* format,...)
  {
    char buffer[BUFFER_SIZE];
    va_list pos;
    va_start(pos, format);

    __vsnprintf(buffer, BUFFER_SIZE, format, pos);
    va_end(pos);
      
    return fwprintf(file, L"%s", buffer);
    
  }


public:
  //Write NULL-terminated string to a file or somewhere defined in each subclass. 
  virtual int writeString(const wchar_t* string) 
  {
    int rc = 0;
    if (string) {
      rc = fwprintf(file, L"%ls", string);
    }
    return rc;
  }

public:
  virtual int write(const wchar_t* format,...)
  {
    wchar_t buffer[BUFFER_SIZE];
    va_list pos;
    va_start(pos, format);

    vswprintf(buffer, BUFFER_SIZE, format, pos);
    va_end(pos);
      
    return writeString(buffer);
  }


public:
  virtual int newLine()
  {
    return writeString(L"\n");
  }

};

}
