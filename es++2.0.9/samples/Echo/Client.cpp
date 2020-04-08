/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  Client.h
 *
 *****************************************************************************/

//2016/04/23 EchoClient

#include <es++/AddressInfoList.h>
#include <es++/Socket.h>

int main(int argc, char *argv[])
{
  if (argc < 3) {
    printf("Usage: %s host port msg...\n", argv[0]);
    return 0; 
  }

  try {
    const char* host    = argv[1];
    const char* service = argv[2];
    AddressInfoHints hints(UNSPEC, DATAGRAM);

    AddressInfoList infoList(host, service, hints);
    int count = infoList.getCount();
 
    Socket* socket = NULL;
    for (int i = 0; i<count; i++) {
      try {
        const addrinfo* info = infoList.getNth(i);
        socket = new Socket(info);
 
        socket->connect(info->ai_addr, info->ai_addrlen);
        break;
        
      } catch (Es::Exception& ex) {
        if (socket) {
          delete socket;
          socket = NULL;
        }
        caught(ex);
      }
    }

    if (socket == NULL) {
      throw IException("Failed to create/connect socket");
    }

    const int BUF_SIZE = 500;
    char buf[BUF_SIZE];

    for (int i = 3; i < argc; i++) {
        int len = strlen(argv[i]) + 1;

        if (len + 1 > BUF_SIZE) {
            //Ignore long message.
            continue;
        }

        printf("Client: write %s\n", argv[i]);
        if (socket -> write( argv[i], len) != len) {
          throw IException("Failed to write.");
        } 

        size_t nread = socket-> read( buf, BUF_SIZE);
        if (nread < 0) {
          throw IException("Failed to read.");  
        }
        printf("Client: Received %zu bytes: \"%s\"\n", nread, buf);
    }
    delete socket;
     
  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}

