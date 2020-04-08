/******************************************************************************
 *
 * Copyright (c) 2014  TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
 *  StringBufferT.cpp
 *
 *****************************************************************************/


#include <es++/StringBufferT.h>
#include <es++/Locale.h>
#include <es++/CharString.h>
#include <es++/WcharString.h>

namespace Es {

class MbStringBuffer :public StringBufferT<char> {

public:
  MbStringBuffer()
  :StringBufferT<char>()
  {
  }  
};

class WcStringBuffer :public StringBufferT<wchar_t> {

public:
  WcStringBuffer()
  :StringBufferT<wchar_t>()
  {
  }  
};

}


int main(int argc, char** argv)
{
  Locale locale;
  fwide(stdout, 1);

  MbStringBuffer mbuffer;
  mbuffer.append("これは実験的かつ初歩的な");

  mbuffer.append(" C++ クラス for Linux");

  mbuffer.append(" Version: ");
  mbuffer.append(2.102);
  mbuffer.append("です");

  CharString mstring = mbuffer.getBuffer();
  mstring.display();

   
  WcStringBuffer wbuffer;
  wbuffer.append(L"これは");

  wbuffer.append(L" Es StringBuffer クラス");

  wbuffer.append(L" Version: ");
  wbuffer.append(2);
  wbuffer.append(L"です");

  WcharString wstring = wbuffer.getBuffer();
  wstring.display();

  return 0;
}
