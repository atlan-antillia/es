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
 *  InterfaceAddrRequest.cpp
 *
 *****************************************************************************/

#include <es++/Object.h>
#include <es++/Locale.h>
#include <es++/Exception.h>

#include <es++/netlink/SocketNetlink.h>
#include <es++/netlink/SockAddrNetlink.h>
#include <es++/netlink/InterfaceAddrAttribute.h>
#include <es++/netlink/NetlinkMsgHeader.h>
#include <es++/netlink/InterfaceAddrMsg.h>
#include <es++/netlink/NetlinkRequest.h>

namespace Es {
    
class InterfaceAddrRequest :public Object {
private:
  NetlinkRequest<ifaddrmsg> request;
  SocketNetlink socketNetlink;

public:
  InterfaceAddrRequest(unsigned char family)
  {
    NetlinkMsgHeader header(&request.head);
    header.type(RTM_GETADDR);
    header.flags(NLM_F_ROOT|NLM_F_REQUEST);
    header.pid(getpid()); 

    InterfaceAddrMsg body(&request.body);
    body.family(family); 

    header.length( NLMSG_LENGTH(body.size()) );
  }
    
  ~InterfaceAddrRequest()
  {
    socketNetlink.close();      
  }
    
  int send()
  {
    return socketNetlink.send((const char*)&request, sizeof(request), 0);   
  }
    
  void recv()
  {
    while (true) {
      char buffer[4096];
      bzero(buffer, sizeof(buffer));
      int n = socketNetlink.recv(buffer, sizeof(buffer), 0);
      if (n < 0){
        break;;
      }
        
      NetlinkMsgHeader netlinkMsgHeader(buffer, n);
      if (netlinkMsgHeader.isDone() || netlinkMsgHeader.isError()) {
        break;
      }
      try {
        do {
          netlinkMsgHeader.display();
          InterfaceAddrMsg addrMsg(netlinkMsgHeader.data());

          InterfaceAddrAttribute attr(netlinkMsgHeader, addrMsg);
          do {
            attr.parse();
          } while (attr.next() );

          addrMsg.display();
          printf("\n");

        } while (netlinkMsgHeader.next());
      } catch (Exception& ex) {
        caught(ex);   
      }
      fflush(stdout);  
    } //while
  }
};
    
}


int main(int argc, char* argv[])
{
  Locale locale;
   
  try {
    InterfaceAddrRequest inetRequest(AF_INET);
    inetRequest.send();
    inetRequest.recv();
    
    InterfaceAddrRequest inet6Request(AF_INET6);
    inet6Request.send();
    inet6Request.recv();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

