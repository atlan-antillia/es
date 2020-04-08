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
 *  StringTokenizerT.cpp
 *
 *****************************************************************************/


#include <es++/StringTokenizerT.h>
#include <es++/Locale.h>

int main(int argc, char** argv)
{
  Locale locale;
  fwide(stdout, 1);

  try {
    const char* line = "France  USA  UK Austria China, Japan 03-1234-5678,  Hello_World";

    StringTokenizerT<char> tokenizer(line);

    while(tokenizer.hasMoreToken()) {
      char token[256];
      token[0] = (char)0;
      tokenizer.getToken(token, SizeOf(token));
      wprintf(L"Token=[%s]\n", token);
    }


    const wchar_t* wline = L"This, is, your, destiny. これが,お前の,運命だ。";

    StringTokenizerT<wchar_t> wtokenizer(wline);

    while(wtokenizer.hasMoreToken()) {
      wchar_t token[256];
      token[0] = (char)0;
      wtokenizer.getToken(token, SizeOf(token));
      wprintf(L"WToken=[%ls]\n", token);
    }
  } catch (Es::Exception& ex) {
    caught(ex);
  }

  return 0;
}

