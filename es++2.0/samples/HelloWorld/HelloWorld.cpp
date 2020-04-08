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
 *  HelloWorld.cpp
 *
 *****************************************************************************/

#include <es++/Object.h>
#include <es++/Exception.h>
#include <es++/CharString.h>
#include <es++/Locale.h>

namespace Es {

class HelloWorld :public Es::Object {

private:
  Es::CharString string;

public:
  HelloWorld(const char* message)
  :string(message) 
  {

  }

public:
  int compare(const char* name) 
  {
    if (name== NULL) {
      throw IException("Invalid argument");
    }
    return strcmp((const char*)string, name);
  }

public:
  void display()
  {
    string.display();
  }
};

}

int main(int argc, char** argv)
{
  Locale locale;
  try {
    HelloWorld helloWorld("Hello world");

    helloWorld.display();

    helloWorld.compare(NULL);

  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}
