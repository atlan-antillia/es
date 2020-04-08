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
 *  Matches.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <es++/StringT.h>
#include <regex.h>

namespace Es {

class Matches : public Object {
private:
  regmatch_t* matches;
  size_t      subExpression;  //The number of subexpessions in compiled 
                              //re_regex_buffer.

public:
  Matches(size_t nsub)
  :matches(NULL),
   subExpression(nsub+1)
  {
    matches = new regmatch_t[subExpression];
  }
  
  ~Matches()
  {
    delete [] matches;
    matches = NULL;
  }
  
  operator regmatch_t*()
  {
    return matches;
  }
  
  operator size_t()
  {
    return subExpression;
  }

  size_t numberOfMatched()
  {
    size_t matched = 0;
    for (size_t i = 0; i < subExpression; i++) {
       if (matches[i].rm_so > -1 && matches[i].rm_eo > -1 ) {
         matched++;
       } else {
         break;
       }
    }
    return matched;
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

  StringT<char> getSubstring(size_t i, const char* string)
  {
  
    StringT<char> cstring(string);
    if (i >= 0 && i < subExpression) {

      int start = matches[i].rm_so;
      int end   = matches[i].rm_eo;
      if (start > -1 && end > -1) {
        return cstring.substring(start, end-start);
      } else {
        throw Es::IException("Unmatched \"%s\"", string); 
      }
    } else {
      throw Es::IException("Invalid argument: %d", i);
    }
  }

  StringT<char> shortestSubstring(const char* string)
  {
    size_t matched = numberOfMatched();
    if (matched > 0) {
      int length = 1024*1024;
      int minStart = 0;
      int minEnd  = 0;
      for (size_t i = 0; i< matched; i++) {
        int start = matches[i].rm_so;
        int end   = matches[i].rm_eo;

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

  void displaySubstring(const char* string)
  {
    size_t matched = numberOfMatched();
    printf("displaySubstring count=%d\n", matched);

    try {
      for (size_t i = 0; i < matched; i++) {
        StringT<char> sub = getSubstring(i, string);
        printf("Substring: \"%s\"\n", (const char*)sub);
      }
    } catch (Es::Exception& ex) {
      ex.display(); 
    }
  }
};

}


