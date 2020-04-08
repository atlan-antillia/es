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
#include <es++/MessageQueue.h>

namespace Es {

class  Reader :public Thread {
private:
  MessageQueue queue;

public:
  Reader(const char* qname)
  :Thread()
  {
    queue.create(qname);
  }

public:
  void run()
  {
    int i = 0;
    //You have to know the size of a message int your queue.
    const int size = queue.messageSize();

    while(i < 100) {
      if (queue.isReadable()) {
        char message[size];
        try {
          queue.receive(message, size, 0);
          printf("Received: %s\n", message);
          i++;
        } catch (Es::Exception& ex) {
          caught(ex);
        }
      } else {
        printf("Your message queue seems to be empty\n");
        sleep(1);
      }
    } //while
  }
};
}


/**
 *
 */
int main(int argc, char* argv[])
{
  const char* qname = "/es++_queue";

  try {
    Reader reader(qname);
    reader.start();
    reader.wait();

  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;    
}

