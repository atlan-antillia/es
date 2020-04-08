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
 *  Mutex.cpp
 *
 *****************************************************************************/

#include <es++/File.h>
#include <es++/Thread.h>
#include <es++/Mutex.h>

Mutex mutex;

class WriterThread : public Thread {
private:
  File file;
  int max;

public:
  WriterThread(const char* filename, int max)
  :file(filename),
  max(max)
  {
  }

public:
  ~WriterThread() 
  {
  }

public:
  void run()
  {
    printf("run\n");
    char string[PATH_MAX];
    for(int i = 0; i<max; i++) {
      mutex.lock();
      if (file.openAppend() >0) {
        sprintf(string, "Max=%d No%d Thread Id=%lx\n", max, i, getThreadId());
        file.write(string, strlen(string));
        file.close();
      } else {
        printf("Failed to open \n");
      }
      mutex.unlock();
    }
  }
};

//
int main(int argc, char* argv[])
{
  const char* name = "./writer.txt";

  try {
    File file(name);
    file.unlink();
    file.create();
    file.close();

    WriterThread thread1(name, 10);
    WriterThread thread2(name, 20);

    thread1.start();
    thread2.start();

    thread1.wait();  
    thread2.wait();  

  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}
