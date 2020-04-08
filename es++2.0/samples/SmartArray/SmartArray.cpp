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
 *  SmartArray.cpp
 *
 *****************************************************************************/

#include <es++/Locale.h>
#include <es++/SmartArray.h>
#include <es++/CharString.h>

int main(int argc, char* argv[])
{
  Locale locale();
  fwide(stdout, 0);
  try {
    //Array of CharString*  
  	SmartArray<CharString*> array(100);
    char buffer[128];
 
    for (int i = 0; i<100; i++) {
      sprintf(buffer, "Hello world. こんちは世界 %d", i);
      array[i] = new CharString(buffer);
    }
    // array[100] ~ array[199] are empty
    for (int i = 200; i<300; i++) {
      sprintf(buffer, "Goodbye world. さよなら世界 %d", i);
      array[i] = new CharString(buffer);
    }
  	
    for (int i = 0; i<300; i++) {
      CharString* string = array[i]; 
      if (string) {
        string->display();
      }
    }

  } catch (Es::Exception& ex) {
    caught(ex);
  }

  return 0;
}

