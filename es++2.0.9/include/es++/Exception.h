/******************************************************************************
 *
 * Copyright (c) 2014-2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  Exception.h
 *
 *****************************************************************************/


#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

namespace Es {
  
class Exception {

protected:
  static const int MAX_MESSAGE = 1024*2;
  char  message[MAX_MESSAGE + 256];
  int   err;

protected:
  Exception()
  :err(0)
  {
    memset(this->message, 0, sizeof(this->message));
  }

public:
  Exception(const char* format,...)
  :err(0)
  {
    memset(this->message, 0, sizeof(this->message));
    char temp[MAX_MESSAGE];
  
    va_list pos;
    va_start(pos, format);
  
    vsnprintf(temp, MAX_MESSAGE, format, pos);
    va_end(pos);
    sprintf(this->message, "Exception: %s", temp);
  }

  Exception(int e, const char* format,...)
  :err(e)
  {
    memset(this->message, 0, sizeof(this->message));
    char temp[MAX_MESSAGE];
  
    va_list pos;
    va_start(pos, format);
  
    vsnprintf(temp, MAX_MESSAGE, format, pos);
    va_end(pos);
    sprintf(this->message, "Error: %d Exception: %s", err, temp);
  }

  ~Exception() {}

  const char* toString()
  {
    return message; 
  }

  int getError()
  {
    return err; 
  }

  //2017/05/10
  void formatMessage(const char* format, va_list pos)
  {
    int bsize = sizeof(this->message);
    memset(this->message, 0, bsize);
    vsnprintf(this->message, bsize, format, pos);
  }

  virtual void  display()
  {
    ::printf("%s\n", toString());
  }

//2017/02/03
  virtual void  display(const char* filename, int line, const char* function ) 
  {
    char text[MAX_MESSAGE*2] = {0};
    snprintf(text, sizeof(text), "\n#Exception: Caught at %s(%d) %s\n#Throwed at %s",
        filename, line,  function, this->message);
    ::printf("%s\n", (const char*)text);
  }
};

#define IException(format, ...) Exception("%s (%d) %s: " format, __FILE__, __LINE__,   __func__ , ##__VA_ARGS__);

#define caught(ex) ex.display(__FILE__, __LINE__,  __func__);  
}

