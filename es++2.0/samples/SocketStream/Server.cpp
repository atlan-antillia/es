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
 *  Server.cpp
 *
 *****************************************************************************/

#include <es++/Thread.h>
#include <es++/SocketStream.h>
#include <es++/Logger.h>

class CommunicationThread :public Thread {
private:
  SocketStream* socket;

public:
  CommunicationThread(SocketStream* s)
  :socket(s)
  {
  }

public:
  void run() 
  {
    try {
      printf("CommunicationThread#run,start\n");
      int i = 0;
      while(1) {
        if (socket->isReadable()) {
          char string[1024];
          int len = socket->recv(string, sizeof(string), 0);
          if (len == 0 ) {
            break;
          } else {
            string[len] = '\0';
            printf("%s\n", string);
          }
        }
        //sleep(1);
        char msg[1024];
        sprintf(msg, "Goodbye world %d\n", i++);
        if (socket->isWritable()) {
          socket->send(msg, strlen(msg));
        }
      }
      printf("CommunicationThread#run,end\n");
    } catch (Es::Exception& ex) {
      caught(ex);
    }
    delete this;
  }  
};

class ServerThread :public Thread {
private:

public:
  void run() 
  {
    try {
      SockAddrInet inet("localhost", 8080);
      SocketStream listener;
      int on = 1;
      listener.setOption(SO_REUSEADDR, (char*) &on, sizeof(on));

      listener.bind(inet);
      listener.listen(10);
  
      while(1) {
        SockAddrInet addr;
        SocketStream* client = listener.accept(addr);
        CommunicationThread* thread= new CommunicationThread(client);
        thread->start();
      }
    } catch (Es::Exception& ex) {
      caught(ex);
    }
  }
};

//
int main(int argc, char** argv)
{
  try {

    ServerThread server;
    server.start();
    server.wait();
  
  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}
