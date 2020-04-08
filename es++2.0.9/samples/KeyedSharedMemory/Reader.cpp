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
 *  Reader.cpp
 *
 *****************************************************************************/


#include <es++/Thread.h>
#include <es++/KeyedSharedMemory.h>
#include <es++/KeyedSemaphore.h>

#include "Def.h"

namespace Es {

class Reader :public Es::Object {
private:
  KeyedSharedMemory memory;

public:
  Reader()
  {
    size_t size = 1024;
    key_t shm_key = ftok(SHM_KEY_FILENAME, SHM_KEY_ID);

    memory.create(size, shm_key);
  }

public:
  ~Reader()
  {
  }


public:
  void read()
  {
    printf("Start read method \n");
    
    void* addr = memory.attach(NULL, 0);

    Table tables[10];
    memcpy(tables, addr, sizeof(tables));

    printf("Read\n");
    for(int i = 0; i<SizeOf(tables); i++) {
  
      printf("tables[%d].value=%d\n", i, tables[i].value);
      printf("tables[%d].name =%s\n", i, tables[i].name);
      printf("\n");
    }

    printf("End read method \n");
  }
};

}


/**
 *
 */
int main(int argc, char* argv[])
{
  try {
    printf("Create an instance of Reader.\n");
    Reader reader;

    printf("Call read method.\n");
    reader.read();
  
  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;    
}
