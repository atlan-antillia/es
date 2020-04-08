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
 *  ZException.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Exception.h>
#include <es++/gtkmm-3.0/ErrorDialog.h>

namespace Es {
  
class ZException :public Exception {

public:
  ZException(const char* format,...)
  :Exception()
  {
    memset(this->message, 0, sizeof(this->message));
  
    va_list pos;
    va_start(pos, format);
    formatMessage(format, pos);
    va_end(pos);
  }

  ~ZException() {}

  virtual void  display()
  {
    ::printf("%s\n", toString());
  }

//2017/02/03
  //virtual void  display(const char* filename, int line, const char* function ) 
  virtual void  display(const char* filename, int line, const char* function ) 
  {
    char text[MAX_MESSAGE*2] = {0};
    snprintf(text, sizeof(text), "\nException: Caught at %s(%d) %s\nThrowed at %s",
        filename, line,  function, toString());
    ::printf("%s\n", (const char*)text);

#ifdef _GTKMM_GUI_
    std::string message = text;
    Es::ErrorDialog::show(message);
#endif
  }
};

#ifdef _GTKMM_GUI_
#undef IException
#undef caught
#define IException(format, ...) ZException("%s (%d) %s: " format, __FILE__, __LINE__,   __func__ , ##__VA_ARGS__);

#define caught(ex) ex.display(__FILE__, __LINE__,  __func__);  

#endif

}

