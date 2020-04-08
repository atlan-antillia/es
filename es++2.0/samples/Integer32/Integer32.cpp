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
 *  Integer32.cpp
 *
 *****************************************************************************/


#include <es++/Locale.h>
#include <es++/Integer32.h>
#include <es++/UnsignedInteger32.h>

int main(int argc, char* argv[])
{
  Locale locale;

  try {
    int32_t MAX = INT_MAX;

    int32_t data[10];
    for(size_t i = 0; i<SizeOf(data); i++) {
      data[i] =  MAX - 10000*i;
      Integer32 int32(data[i]);
      int32.display();
      CharString cs = int32.toCharString();
      printf("1 cs [%s]\n", (const char*)cs); 

      const char* string = int32.toString();
      printf("1 string [%s]\n", string);
      printf("array[%2d] = [%"PRId32"]\n", i, data[i]);
    }

    uint32_t udata[10];

    int i = 0;
    for (i = 0; i<10; i++) {
      udata[i] =  MAX/2 + (10000*i + 1000*i + 100*i + i);
      UnsignedInteger32 uint32(udata[i]);
      uint32.display();
      CharString cs = uint32.toCharString();
      printf("2 cs [%s]\n", (const char*)cs); 

      const char* string = uint32.toString();
      printf("2 string [%s]\n", string);

      printf("uarray[%2d] =[%" PRIu32 "]\n", i, udata[i]);

    }
    printf("\n");

  } catch (Es::Exception& ex) {
    caught(ex);
  }

  return 0;
}

