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
 *  Client.cpp
 *
 *****************************************************************************/


#include <es++/Thread.h>
#include <es++/SocketStream.h>
#include <es++/Logger.h>

class ClientThread :public Thread {

private:
  SocketStream stream;
  SockAddrInet inet;

public:
  ClientThread(SockAddrInet& addr)
  :inet(addr)
  {
  }

public:
  void run()
  {
    try {
    if(stream.connect(inet) ==0) {
      for(int i = 0; i<10; i++) {
        if (stream.isWritable()) {
        char string[1024];
        sprintf(string, "PID=%d Hello world %d", getpid(), i);
        stream.send(string, strlen(string));
        }
        sleep(1);      
        if (stream.isReadable()) {
        char buffer[1024];
        
        int len = stream.recv(buffer, sizeof(buffer), 0);
        if (len >0) {
          buffer[len] = '\0';
          printf("Recv=%s\n", buffer);
        
        } else if (len == 0) break;
        }
      }
    }
    } catch (Es::Exception& ex) {
      caught(ex);
    }
  }
};


int main(int argc, char** argv)
{
  try {
    SockAddrInet addr("localhost", 8080);
    ClientThread client(addr);
    client.start();

    client.wait();
  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}
