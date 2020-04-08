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
 *  Integer16.cpp
 *
 *****************************************************************************/


#include <es++/Locale.h>
#include <es++/Integer16.h>
#include <es++/UnsignedInteger16.h>

int main(int argc, char* argv[])
{
  Locale locale;

  try {
    int16_t MAX = SHRT_MAX;

    int16_t data[10];
    for(size_t i = 0; i<SizeOf(data); i++) {
      data[i] =  MAX - 100*i;
      Integer16 int16(data[i]);
      int16.display();
      CharString cs = int16.toCharString();
      printf("1 cs [%s]\n", (const char*)cs); 

      const char* string = int16.toString();
      printf("1 string [%s]\n", string);
      printf("array[%2d] = [%"PRId16"]\n", i, data[i]);
    }

    uint16_t udata[10];

    int i = 0;
    for (i = 0; i<10; i++) {
      udata[i] =  MAX/2 + (100*i + 10*i + i);
      UnsignedInteger16 uint16(udata[i]);
      uint16.display();
      CharString cs = uint16.toCharString();
      printf("2 cs [%s]\n", (const char*)cs); 

      const char* string = uint16.toString();
      printf("2 string [%s]\n", string);

      printf("uarray[%2d] =[%" PRIu16 "]\n", i, udata[i]);

    }
    printf("\n");

  } catch (Es::Exception& ex) {
    caught(ex);
  }

  return 0;
}

