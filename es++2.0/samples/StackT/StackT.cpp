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
 *  StackT.cpp
 *
 *****************************************************************************/


#include <es++/Locale.h>
#include <es++/StackT.h>
#include <es++/CharString.h>

int main(int argc, char* argv[])
{
  Locale locale;
  fwide(stdout, 1);
 
  try {

    StackT<CharString> stack;

    CharString string1("1 OpenFlow: オープンフロー");
    CharString string2("2 NFV: ネットワーク機能仮想化");

    CharString string3("3 Hello world. こんにちは世界");
    CharString string4("4 Goodbye world. さようなら世界");

    CharString string5("5 Happy birthday. 誕生日おめでとう");
    CharString string6("6 The end of eternity.  永遠の終わり");
  
    stack.push(&string1);
    stack.push(&string2);
    stack.push(&string3);
    stack.push(&string4);
    stack.push(&string5);
    stack.push(&string6);
    
    stack.display();
    
    Es::Object* object = NULL;
    while((object = stack.pop()) != NULL) {
      object->display();
    }

  } catch (Es::Exception& ex) {
    caught(ex);
  }

  return 0;
}
