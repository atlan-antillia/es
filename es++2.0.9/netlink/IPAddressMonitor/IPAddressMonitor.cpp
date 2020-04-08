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
 *  IPAddressMonitor.cpp
 *
 *****************************************************************************/

#include <es++/Object.h>
#include <es++/LocalDateTime.h>
#include <es++/Exception.h>
#include <es++/netlink/SocketNetlink.h>
#include <es++/netlink/SockAddrNetlink.h>

#include <es++/netlink/NetlinkReply.h>
#include <es++/netlink/NetlinkMsgHeader.h>
#include <es++/netlink/InterfaceAddrAttribute.h>
#include <es++/netlink/InterfaceAddrMsg.h>

namespace Es {
    
class IPAddressMonitor :public Object {
private:
  int              rtmGroups;
  SocketNetlink    socketNetlink;
  NetlinkReply<ifaddrmsg> reply;

public:
  IPAddressMonitor(int group = RTMGRP_IPV4_IFADDR|RTMGRP_IPV6_IFADDR)
  :rtmGroups(group)
  {
  }
    
    
  int bind()
  {
    SockAddrNetlink  addrNetlink;
    addrNetlink.setFamily(AF_NETLINK);
    addrNetlink.setGroups(rtmGroups);
    return socketNetlink.bind(addrNetlink);
  }
    
  void recv()
  {
    while (true) {
      bzero(&reply, sizeof(reply));

      int n = socketNetlink.recv((char*)&reply, sizeof(reply), 0);
      if ( n < 0 ){
        perror("recv");
        break;
      }
      try {         

        NetlinkMsgHeader netlinkMsgHeader(&reply.head);
        if (netlinkMsgHeader.isDone() || netlinkMsgHeader.isError()) {
          break;
        }

        do {
          LocalDateTime dateTime;
          CharString now = dateTime.nowToSeconds();
          printf("%s\n", (const char*)now);
          netlinkMsgHeader.display();

          InterfaceAddrMsg addrMsg(&reply.body);
      
          InterfaceAddrAttribute attribute(netlinkMsgHeader, addrMsg);
        
          do {
            attribute.parse();
          } while (attribute.next() );

          addrMsg.display();
          printf("\n");

        } while (netlinkMsgHeader.next());
        
      } catch (Exception& ex) {
        caught(ex);   
      }
      fflush(stdout);       
    }
  }
};
    
}

int main(int argc, char* argv[])
{
  try {
    IPAddressMonitor monitor;
    //IPAddressMonitor monitor(RTMGRP_IPV4_IFADDR);
    monitor.bind();
    monitor.recv();
    
  } catch (Exception& ex) {
    caught(ex);   
  }
  return 0;
}

