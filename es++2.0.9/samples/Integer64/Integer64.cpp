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
 *  Integer64.cpp
 *
 *****************************************************************************/


#include <es++/Locale.h>
#include <es++/Integer64.h>
#include <es++/UnsignedInteger64.h>

int main(int argc, char* argv[])
{
  Locale locale;

  try {
    int64_t MAX = 0x7FFFFFFFFFFFFFFF;

    int64_t data[10];
    for(size_t i = 0; i<SizeOf(data); i++) {
      data[i] =  MAX - (10000*i + 1000*i + 100*i + i);
      Integer64 int64(data[i]);
      int64.display();
      CharString cs = int64.toCharString();
      printf("1 cs [%s]\n", (const char*)cs); 

      const char* string = int64.toString();
      printf("1 string [%s]\n", string);
      printf("array[%2zu] = [%" PRId64"]\n", i, data[i]);
    }

    uint64_t udata[10];

    int i = 0;
    for (i = 0; i<10; i++) {
      udata[i] =  MAX/2 + (10000*i + 1000*i + 100*i + i);
      UnsignedInteger64 uint64(udata[i]);
      uint64.display();
      CharString cs = uint64.toCharString();
      printf("2 cs [%s]\n", (const char*)cs); 

      const char* string = uint64.toString();
      printf("2 string [%s]\n", string);

      printf("uarray[%2d] =[%" PRId64 "]\n", i, udata[i]);

    }
    printf("\n");

  } catch (Es::Exception& ex) {
    caught(ex);
  }

  return 0;
}

