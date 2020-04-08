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
 *  Barrier.cpp
 *
 *****************************************************************************/


#include <es++/Thread.h>
#include <es++/Barrier.h>

namespace Es {

class ComputationThread : public Es::Thread {
private:
  int max;

  Es::Barrier& barrier;
public:
  ComputationThread(int max, Es::Barrier& barrier)
  :max(max),
   barrier(barrier)
  {
  }

public:
  void run() 
  {
    pthread_t id = getThreadId();
    printf("run::start threadId=%lu\n", id);
    int i = 0;
    while(i<max) {
      int value = i*3;
      printf("threadId=%lu %d \n", id, value); 
      sleep(1);
      i++;
    }  
    barrier.wait();      
    printf("run::exit threadId=%lu\n", id);
  }
};
}

//
int main(int argc, char* argv[])
{
  try {
    int count = 2;
    Es::Barrier barrier(count);

    ComputationThread thread1(20, barrier);
    ComputationThread thread2(10, barrier);
    thread1.start();
    thread2.start();

    thread1.wait();
    thread2.wait();

  } catch (Es::Exception& ex) {
    caught(ex);
  }

  return 0;
}
