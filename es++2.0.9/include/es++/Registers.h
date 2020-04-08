/******************************************************************************
 *
 * Copyright (c) 2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
 *  Registers.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <es++/StringT.h>
#include <regex.h>

/*
struct re_registers
{
  unsigned num_regs;
  regoff_t *start;
  regoff_t *end;
};
 */

namespace Es {

class Registers :public Object {
private:
  re_registers regs;

public:
  Registers()
  {
    memset(&regs, 0, sizeof(regs));
  }

  ~Registers()
  {
    clear();
  }

  operator re_registers*()
  {
    return &regs;
  }  

  void clear()
  {
    if (regs.start) {
      free(regs.start);
    }
   
    if (regs.end) {
      free(regs.end);
    }
    memset(&regs, 0, sizeof(regs));
  }

  void getOffset(size_t i, int& start, int& end)
  {
    if (regs.num_regs > 0 && i < regs.num_regs) {
      start = *(regs.start + i);
      end   = *(regs.end   + i);
    } else {
      throw Es::IException("Invalid parameter: %d", i);
    }
  }

  StringT<char> getMatchedString(const char* string, int start, int end)
  {
    StringT<char> cstring(string);
    if (start > -1 && end > -1) {
      return cstring.substring(start, end - start);
    }
    else {
      throw Es::IException("Unmatched");
    }
  }

  StringT<char> getMatchedString(size_t i, const char* string)
  {
    StringT<char> cstring(string);
    int start = 0;
    int end   = 0;
    getOffset(i, start, end);
    if (start > -1 && end > -1) {
      return cstring.substring(start, end - start);
    }
    else {
      throw Es::IException("Unmatched");
    }
  }

  void displaySubstring(const char* string)
  {
    if (regs.num_regs > 0) {
     for (size_t i = 0; i< regs.num_regs; i++) {
       int start = *(regs.start + i);
       int end   = *(regs.end   + i);
       if (start > -1 && end > -1) {
         StringT<char> substring = getMatchedString(string, start, end);
         printf("start=%d,  end=%d  substring=\"%s\"\n", 
		start, end,  (const char*)substring);
       }
     }
    }
  }

 StringT<char> shortestSubstring(const char* string)
  {
    if (regs.num_regs > 0) {
      int length = 1024*1024;
      int minStart = 0;
      int minEnd  = 0;
      for (size_t i = 0; i< regs.num_regs; i++) {
        int start = *(regs.start + i);
        int end   = *(regs.end   + i);
       
        if (start > -1 && end > -1) {
          if ((end - start) <= length) {
            length = end - start;
            minStart = start;
            minEnd   = end;
          }
        }
      } //for
      
      return getMatchedString(string, minStart, minEnd);
    } else { 
      throw Es::IException("Didn't match: \"%s\"", string);
    }
  }


  void display()
  {
    if (regs.num_regs > 0) {
     for (size_t i = 0; i< regs.num_regs; i++) {
       int start = *(regs.start + i);
       int end   = *(regs.end   + i);
       if (start > -1 && end > -1) {
         printf("start=%d,  end=%d\n", start, end);
       }
     }
    }
  }
};

}

