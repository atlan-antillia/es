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
 *  Server.cpp
 *
 *****************************************************************************/


#include <es++/AddressInfoList.h>
#include <es++/SocketDatagram.h>
#include <es++/SockAddrInet.h>
#include <es++/NameInfo.h>

int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Usage: %s port \n", argv[0]);
    return 0; 
  }

  try {
    const char* port  = argv[1];
    const AddressInfoHints hints(UNSPEC, 
              DATAGRAM, 
	      PASSIVE);

    AddressInfoList infoList(NULL, port, hints);
    int count = infoList.getCount();
 
    SocketDatagram* socket = NULL;
    const addrinfo* info = NULL;
    for (int i = 0; i<count; i++) {
      try {
        info   = infoList.getNth(i);
        socket = new SocketDatagram(info);
 
        socket -> bind(info->ai_addr, info->ai_addrlen);
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
      throw IException("Failed to bind socket.");
    }

    bool looping = true;
    const int BUF_SIZE = 500;
    char buf[BUF_SIZE];
    while (looping) {
      try {
        SockAddrInet peer;
        size_t  nread = socket->recvFrom(peer, buf, sizeof(buf), 0);
        if (nread < 0) {
          continue; 
        }
        NameInfo nameInfo(peer);
        printf("Server: Recvfrom \"%s\" size=%zu from host=\"%s\" service=\"%s\"\n", 
		buf, nread, nameInfo.nodeName(), nameInfo.serviceName());

        printf("Server: sendto \"%s\"\n", buf);
        size_t nsent = socket-> sendTo(peer, buf, nread);
        if (nsent != nread) {
          throw IException("Failed to sendTo.");  
        }
      } catch (Es::Exception& ex) {
        caught(ex);
      } 
    }
    delete socket;

  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}

