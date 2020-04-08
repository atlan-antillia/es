/******************************************************************************
 *
 * Copyright (c) 2014 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  ArrayT.cpp
 *
 *****************************************************************************/


#include <es++/ArrayT.h>

int main(int argc, char* argv[])
{
  try {
    int64_t MAX = 0x7FFFFFFFFFFFFFFF;

    int64_t data[10];
    for(size_t i = 0; i<SizeOf(data); i++) {
      data[i] =  MAX - (10000*i + 1000*i + 100*i + i);
    }

    Es::ArrayT<int64_t> array(data, SizeOf(data));

    int i = 0;
    for (i = 0; i<10; i++) {
      printf("array[%2d] = %" PRId64 "\n", i,array[i]);
    }
    printf("\n");

    for (i = 0; i<10; i++) {
      array[10+i] = -array[i];
    }

    for (i = 0; i<20; i++) {
      printf("array[%2d] = %" PRId64 "\n", i, array[i]);
    }

    printf("\n");
  } catch (Es::Exception& ex) {
    caught(ex);
  }

  return 0;
}

