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
 *  RoutingTableDelRequest.cpp
 *
 *****************************************************************************/

#include <es++/Object.h>
#include <es++/Locale.h>
#include <es++/Exception.h>

#include <es++/netlink/SocketNetlink.h>
#include <es++/netlink/RoutingTableMsg.h>
#include <es++/netlink/NetlinkRouteAttributeRequest.h>
#include <es++/netlink/NetlinkReply.h>
#include <es++/netlink/NetlinkMsgError.h>

//Simple program to delete a route through Netlink++ with RTM_DELROUTE message.
//This program should be run as the root user.

namespace Es {
    
class RoutingTableDelRequest :public Object {
private:
  NetlinkRouteAttributeRequest<rtmsg> request;
  SocketNetlink              socketNetlink;
  NetlinkReply<rtmsg>        reply;
    
public:
  RoutingTableDelRequest(unsigned char family, __u32 dest, __u32 gateway)
  {     
    NetlinkMsgHeader header(request.head());
    header.type(RTM_DELROUTE);
    header.flags(NLM_F_CREATE|NLM_F_REQUEST| NLM_F_ACK);
    header.seq(100);
    header.pid(0); 

    RoutingTableMsg body(request.body());
    header.length( NLMSG_LENGTH(body.size()) );
    
    body.family(family); 
    body.table(RT_TABLE_UNSPEC);
    body.dstlen(32);  //Specifiy 32 bits for IPv4. 
    //body.srclen(32);
    body.scope(RT_SCOPE_UNIVERSE);
    body.type(RTN_UNICAST);
    body.protocol(RTPROT_UNSPEC);
    body.flags(RTM_F_NOTIFY);
    
    request.addAttribute(RTA_DST,     &dest,   4);
    request.addAttribute(RTA_GATEWAY, &gateway, 4);         
  }

  int send()
  {
    RouteAttributeRequest<rtmsg>& req = request.request();
    size_t length = request.length();
    return socketNetlink.send((const char*)&req, length, 0);   
  }
    
  void recv()
  {
    while (true) {
        
      bzero(&reply, sizeof(reply));
      int n = socketNetlink.recv((char*)&reply, sizeof(reply), 0);
      if (n < 0){
        break;;
      }
        
      NetlinkMsgHeader replyHeader(&reply.head);

      if (replyHeader.isError() ) {
        //You should check the reply from the kernel, it may return success or error.
        NetlinkMsgError error(replyHeader.errorMsg());
        error.display();
        break;
      }
      fflush(stdout);  
    } //while
  }
};
    
}


//
int main(int argc, char* argv[])
{
  if (argc != 3) {
    printf("Usage: %s destIP gatewayIP\n", argv[0]);        
    return 0;
  }
  try {
    __u32 dest    = 0;
    __u32 gateway = 0;
    unsigned char family = AF_INET;
    
    if (inet_pton(family, argv[1], (struct in_addr*)&dest) <= 0) {
        throw IException("Invalid destIP %s", argv[1]);  
    }
    if (inet_pton(family, argv[2], (struct in_addr*)&gateway) <= 0) {
        throw IException("Invalid gatewayIP %s", argv[2]);   
    }

    RoutingTableDelRequest request(family, dest, gateway);
    request.send();
    request.recv();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

