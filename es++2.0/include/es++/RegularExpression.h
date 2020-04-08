/******************************************************************************
 *
 * Copyright (c) 2015  TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  RegularExpression.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <es++/Matches.h>

namespace Es {

class RegularExpression :public Object {
private:
  regex_t expression;

public:
  RegularExpression(const char* regex, int cflags=REG_EXTENDED)
  {
    if (regex == NULL) {
      throw Es::IException("Invalid argument. regex is NULL.");
    }
    memset(&expression, 0, sizeof(expression));
    int rc = regcomp(&expression, regex, cflags);
    if (rc != 0) {
      char message[1024];
      memset(message, 0, sizeof(message));
      regerror(rc, &expression, message, sizeof(message));
      throw Es::IException("Failed to regcompile for \"%s\": error \"%s\"", 
		regex, message);
    }
  }

  ~RegularExpression()
  {
    regfree(&expression);
  }

  size_t numberOfSubexpressions()
  {
    return expression.re_nsub;
  }

  int match(const char* string, regmatch_t& match, int eflags=0)
  {
    if (string == NULL) {
      throw Es::IException("Invalid argument. string is NULL.");
    }
    int rc = regexec(&expression, string, 1, &match, eflags);
    if (rc != 0) {
      char message[1024];
      memset(message, 0, sizeof(message));
      regerror(rc, &expression, message, sizeof(message));
      throw Es::IException("Failed to regexec for \"%s\": error \"%s\"", 
                string, message);
    }
    return rc;
  } 

  int match(const char* string, size_t nmatch, regmatch_t pmatch[], int eflags=0)
  {
    if (string == NULL) {
      throw Es::IException("Invalid argument. string is NULL.");
    }
    int rc = regexec(&expression, string, nmatch, pmatch, eflags);
    if (rc != 0) {
      char message[1024];
      memset(message, 0, sizeof(message));
      regerror(rc, &expression, message, sizeof(message));
      throw Es::IException("Failed to regexec for \"%s\": error \"%s\"", 
                string, message);
    }
    return rc;
  }
};

}

