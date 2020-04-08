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

#include <es++/SocketDatagram.h>

namespace Es {

class SocketDatagramServer :public SocketDatagram {

private:
  SockAddrInet inet;

public:
  SocketDatagramServer(SockAddrInet& addr)
  :inet(addr)
  {
    bind(inet);
  }

public:
  void recvAndReplyTo() 
  {
    SockAddrInet client;
    while (1) {
      char buf[1024];
      try {
        //1 Receive a string.
        int rc = recvFrom(client, buf, sizeof(buf));
        //int rc = recvFrom(buf, sizeof(buf));
        if (rc >0) {
          printf("recvFrom: string=[%s] len=%d(bytes)\n", 
            buf, rc);
          //2 ReplyBack to a sender 
          const char* ok= "OK. Got a datagram.";
          int r = replyTo(client, ok, strlen(ok));
          //int r = replyTo(ok, strlen(ok));
          if (r >0) {
            printf("replyTo:%s\n", ok);
          }
        }
        if (rc <=0) {
          printf("Exit loop\n");
          break;
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
    SockAddrInet addr(32000);
    
    SocketDatagramServer server(addr);

    printf("Created SocketDatagramServer\n");

    server.recvAndReplyTo();

  } catch (Es::Exception& ex) {
    caught(ex);
  }
}
