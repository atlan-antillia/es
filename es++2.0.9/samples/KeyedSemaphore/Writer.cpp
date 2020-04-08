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
 *  Writer.cpp
 *
 *****************************************************************************/


#include <es++/Thread.h>
#include <es++/KeyedSharedMemory.h>
#include <es++/KeyedSemaphore.h>

#include "Def.h"

namespace Es {

class Writer :public Es::Object {
private:
  KeyedSemaphore semaphore;

public:
  Writer()
  {
    key_t sem_key = (key_t)123456;

    semaphore.create(sem_key);
    //semaphore.setVal(0, 0);
  }

public:
  ~Writer()
  {
  }


public:
  void write()
  {
    printf("Start write method \n");
    semaphore.lock();
    FILE* fp = fopen("./test.txt", "w");
    if (fp) {
      for(int i = 0; i<10; i++) {
        fprintf(fp, "%04d Time to say 'Hello'\n", i);
        printf("%04d Time to say 'Hello'\n", i);
        sleep(1);
      }
      fclose(fp);
    }
    semaphore.unlock();
    printf("End write method \n");
  }
};

}


/**
 *
 */
int main(int argc, char* argv[])
{
  try {
    printf("Create an instance of Writer.\n");
    Writer writer;

    printf("Call write method.\n");
    writer.write();
  
  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;    
}
