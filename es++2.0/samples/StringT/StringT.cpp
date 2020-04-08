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
 *  StringT.cpp
 *
 *****************************************************************************/


#include <es++/Locale.h>
#include <es++/StringT.h>

class WString :public StringT<wchar_t> {

public:
  WString(const wchar_t* string)
  :StringT<wchar_t>(string)
  {

  }

public:
  WString(const WString* string)
  :StringT<wchar_t>(string)
  {
  }

public:
  WString(const WString& string)
  :StringT<wchar_t>(string)
  {
  }
};


int main(int argc, char* argv[])
{
  Locale locale;
  fwide(stdout, 1);

  StringT<char> string("Good morning.");
  wprintf(L"%s\n", (const char*)string);

  StringT<wchar_t> wstring(L"Hello world.こんにちは世界。");

  WString wxstring(L"Goodbye world. さようなら世界。");

  WString wxstring1(wstring);
  WString wxstring2(wxstring);


  wprintf(L"%ls\n", (const wchar_t*)wxstring);
  wprintf(L"%ls\n", (const wchar_t*)wxstring1);
  wprintf(L"%ls\n", (const wchar_t*)wxstring2);

  return 0;
}

