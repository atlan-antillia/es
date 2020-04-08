/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  LogWriter.cpp
 *
 *****************************************************************************/


#include <es++/Locale.h>
#include <es++/LogWriter.h>
#include <es++/StringT.h>

namespace Es {

class MbString :public StringT<char> {

public:
  MbString(const char* string)
  :StringT<char>(string)
  {
    LogWriter::debug("MbString#MbString(const char* string='%s')",
      string);  
  }


public:
  MbString(const MbString& string)
  :StringT<char>(string)
  {
    LogWriter::debug("MbString#MbString(const MbString& string='%s')",
      (const char*)string);  
  }

};

class WcString :public StringT<wchar_t> {

public:
  WcString(const wchar_t* string)
  :StringT<wchar_t>(string)
  {
    LogWriter::debug(L"WcString#WcString(const wchar_t* string='%ls')",
      string);  
  }


public:
  WcString(const WcString& string)
  :StringT<wchar_t>(string)
  {
    LogWriter::debug(L"WcString#WcString(const WcString& string='%ls')",
      (const wchar_t*)string);  
  }
};

}


int main(int argc, char* argv[])
{
  Locale locale;
  fwide(stdout, 1);

  LogWriter::debug(L"%ls", L"テスト開始");

  try {

    StringT<char>    string("OpenFlow: オープンフロー");
    MbString mbstring("NFV: ネットワーク機能仮想化");
    MbString mbstring1((const char*)string);
    MbString mbstring2(mbstring);

    wprintf(L"1 %s\n", (const char*)mbstring1);
    wprintf(L"2 %s\n", (const char*)mbstring2);


    StringT<wchar_t> wstring(L"Hello world. こんにちは世界");
    WcString wcsstring(L"Goodbye world. さようなら世界");
    WcString wcsstring1((const wchar_t*)wstring);
    WcString wcsstring2(wcsstring);
  
    wprintf(L"3 %ls\n", (const wchar_t*)wcsstring1);
    wprintf(L"4 %ls\n", (const wchar_t*)wcsstring2);

    const char* null = (char*)NULL;
    if (null != (char*)NULL) {
      StringT<char> stringx(null);
    } else {
      throw Exception("NULL string");
    }

  } catch (Es::Exception& ex) {
    LogWriter::err("%s", (const char*)ex.toString());
    wprintf(L"%s\n", (const char*)ex.toString());
  }

  LogWriter::debug(L"%ls", L"テスト終了");
  return 0;
}
