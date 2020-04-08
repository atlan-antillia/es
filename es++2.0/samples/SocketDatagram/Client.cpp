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
 *  Client.cpp
 *
 *****************************************************************************/


#include <es++/SocketDatagram.h>

namespace Es {

class  SocketDatagramClient :public SocketDatagram {
private:
  SockAddrInet inet;

public:
  SocketDatagramClient(SockAddrInet& addr)
  {  
    inet = addr;
  }

public:
  void sendAndRecv() 
  {
    char buf[1024];

    for (int i=0; i<10; i++) {
      sprintf(buf, "%d Message from SocketDatagramClient.\n'Hello world!\n Goodbye world.'", i); 
      int slen = strlen(buf);
      try {
        //1 Send a string.
        int rc = sendTo(inet, buf, slen);
        printf("sendTo: string=[%s] len=%d(bytes) sent=%d(bytes)\n", 
            buf, slen, rc);

        sleep(1);

        if (rc >0) {
          char reply[1024];
          //2 Receive a string.
          int r = recvFrom(reply, sizeof(reply));
          if (r >0) {
            printf("recvFrom:[%s]\n", reply);
          }
        }
      } catch (Es::Exception& ex) {
        caught(ex);
      }
    }
  }  
};
}

/**
 *
 */
int main(int argc, char* argv[])
{

  try {
    printf("Create SocketDatagramSender\n");

    SockAddrInet inet("127.0.0.1", 32000);

    SocketDatagramClient client(inet);

    client.sendAndRecv();

    client.close();

  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}
