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
 *  RoutingTableMonitor.cpp
 *
 *****************************************************************************/

#include <es++/Object.h>
#include <es++/Exception.h>
#include <es++/Locale.h>
#include <es++/netlink/SocketNetlink.h>
#include <es++/netlink/SockAddrNetlink.h>
#include <es++/netlink/RoutingTableAttribute.h>
#include <es++/netlink/NetlinkMsgHeader.h>
#include <es++/netlink/RoutingTableMsg.h>
#include <es++/netlink/NetlinkReply.h>

//Simple program to monitor IPv4 route changes through Netlink++.

namespace Es {
    
class RoutingTableMonitor :public Object {
private:
  SockAddrNetlink addr;
	
  SocketNetlink socketNetlink;

public:
  RoutingTableMonitor(unsigned char family)
  {
  }

  int bind()
  {
  	addr.setFamily(AF_NETLINK);
  	addr.setGroups(RTMGRP_IPV4_ROUTE);

    return socketNetlink.bind(addr);
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
        
      NetlinkMsgHeader replyHeader(buffer, n);
      if (replyHeader.isDone() || replyHeader.isError() ) {
        break;
      }

      try {
        do {
          replyHeader.display();
          RoutingTableMsg routingTableMsg(replyHeader.data());

          RoutingTableAttribute attr(replyHeader, routingTableMsg);
          do {
            attr.parse();
          } while (attr.next() );

          routingTableMsg.display();
          printf("\n");

        } while (replyHeader.next());

      } catch (Exception& ex) {
        caught(ex);   
      }
      fflush(stdout);  
    } //while
  }
};
    
}

//
int main(int argc, char* argv[])
{
  Locale locale;
   
  try {
    //IPv4 
    RoutingTableMonitor request(AF_INET);
  	request.bind();
    request.recv();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

