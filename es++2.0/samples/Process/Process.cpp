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
 *  Process.cpp
 *
 *****************************************************************************/


#include <es++/Process.h>
#include <es++/Exception.h>
#include <es++/ArrayT.h>

int main(int argc, char* argv[])
{
  if (argc < 2) {
    printf("Usage: %s path arg1\n", argv[0]);
    return 0;
  }
  try {
    ArrayT<char*> args( &argv[1], argc-1);
    args[argc-1] = NULL;

    for(int i = 0; i< argc; i++) {
      printf("args[%d]= %s\n", i, args[i]);
    }
    
    const char* path = argv[1];

    Process process(path);

    printf("Process.spawn()\n");
    process.spawn(args.getArray());

    printf("Process.pause()\n");
    process.pause();

    printf("sleep(4)\n");
    sleep(4);
    
    printf("Process.resume()\n");
    process.resume();
    
    printf("Process.wait()\n");
    process.wait();

/*
    Process proc(path);
    proc.start(args.getArray());
    proc.wait();
*/

  } catch (Es::Exception& ex) {
    caught(ex);
  }

  return 0;
}

