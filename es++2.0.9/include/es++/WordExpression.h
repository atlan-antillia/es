/******************************************************************************
 *
 * Copyright (c) 2017  TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  WordExpression.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/Pair.h>
#include <es++/KeyNamePairs.h>
//#include <es++/Exception.h>
#include <wordexp.h>

namespace Es {

class WordExpression :public Es::Object {
private:
  wordexp_t expression;

public:
  WordExpression(const char* word, int cflags=0)
  {
    if (word == NULL) {
      throw IException("Invalid argument. word is NULL.");
    }
    memset(&expression, 0, sizeof(expression));
    int rc = wordexp(word, &expression, cflags);
    if (rc != 0) {
      const char* error = getError(rc);
      throw IException("Failed to wordexp for %s: error:%s", 
		word, error);
    }
    //show();
  }

  ~WordExpression()
  {
    wordfree(&expression);
  }

  void show() 
  {
    char** wordv = expression.we_wordv;
    size_t count = expression.we_wordc;
    if (wordv && count >0) {
      for (size_t i = 0; i<count; i++) {
        printf("%s\n", wordv[i]);
      }
    }
  } 

  size_t getWordCount()
  {
    size_t count = expression.we_wordc;
    return count;
  }

  char** getWordVector()
  {
    char** wordv = expression.we_wordv;
    if (wordv == NULL) {
      throw IException("WordVector is NULL.")
    }
    return wordv; 
  } 

private:
  static const char* getError(int error)
  {
    static Pair<int, const char*> errors[] = {
        {WRDE_BADCHAR, "WRDE_BADCHAR"},
        {WRDE_BADVAL,  "WRDE_BADVAL"},
        {WRDE_CMDSUB,  "WRDE_CMDSUB"},
        {WRDE_NOSPACE,  "WRDE_NOSPACE"},
        {WRDE_SYNTAX,   "WRDE_SYNTAX"},
        };
    KeyNamePairs<int> pairs(errors, SizeOf(errors));
    return pairs.match(error);    
  }
};

}

