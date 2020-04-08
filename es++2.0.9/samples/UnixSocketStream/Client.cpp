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


#include <es++/File.h>
#include <es++/Thread.h>
#include <es++/UnixSocketStream.h>
#include <es++/Logger.h>

class ClientThread :public Thread {

private:
  UnixSocketStream socket;

public:
  ClientThread()
  {
  }

public:
  void run()
  {
    try {
      File path("/tmp/es_server");
      SockAddrUnix addr(path);
      socket.connect(addr);

      for(int i = 0; i<10; i++) {
        if (socket.isWritable()) {
          char string[1024];
          sprintf(string, "Hello world %d", i);
          socket.send(string, strlen(string));
        }
        sleep(1);      
        if (socket.isReadable()) {
          char buffer[1024];
        
          int len = socket.recv(buffer, sizeof(buffer), 0);
          if (len >0) {
            buffer[len] = '\0';
            printf("Recv=%s\n", buffer);
        
          } else if (len == 0) { 
            break;
          }
        }
      }
    } catch (Es::Exception& ex) {
      caught(ex);
    }
  }
};

//
int main(void)
{
  try {
    ClientThread client;
    client.start();

    client.wait();
  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return(0);
}
