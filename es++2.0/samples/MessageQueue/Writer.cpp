/******************************************************************************
 *
 * Copyright (c) 2014  TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
#include <es++/MessageQueue.h>

namespace Es {

class Writer :public Thread {
private:
  MessageQueue queue;

public:
  Writer(const char* qname)
  :Thread()
  {
    queue.create(qname);
  }

public:
  void run()
  {
    int i = 0;
    //You have to know the size of a message in your queue.
    const long int size = queue.messageSize();
    printf("Message Size:%ld\n", size); //Default size is 8192 bytes. 
    while(i < 100) {
      if (queue.isWritable()) {
                char message[size];
        sprintf(message, "Hello world %d", i);
        try {
          //You may specify the real message size 
          // to the second argument of the send method of 
          // MessageQueue.
          queue.send(message, strlen(message), 0);
          printf("Sent: %s\n", message);
          i++;
        } catch (Es::Exception& ex) {
          caught(ex);
        }
      } else {
        printf("Your message queue seems to be full\n");
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
    Writer writer(qname);
    writer.start();
    writer.wait();

  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;    
}
