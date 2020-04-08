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
 *  InterfaceAddrChangeRequest.cpp
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
#include <es++/netlink/InterfaceAddrAttribute.h>
#include <es++/netlink/NetlinkRouteAttributeRequest.h>
#include <es++/netlink/NetlinkReply.h>
#include <es++/netlink/NetlinkMsgError.h>

//#include <net/if.h>

// Simple program to register a new IPv4 ip address through Netlink++ library
// Note: 
// This program should be run as the root user.

namespace Es {
    
class InterfaceAddrChangeRequest :public Object {

private:
  //Use the following class to be able to register your own route attributes.
  NetlinkRouteAttributeRequest<ifaddrmsg> request;

  //The following is a communication channel to the kernel.
  SocketNetlink socketNetlink;

  NetlinkReply<ifaddrmsg> reply;

public:
  //Constructor
  InterfaceAddrChangeRequest(const char* address)
  {
    NetlinkMsgHeader header(request.head());
    header.type(RTM_NEWADDR);
    header.flags(NLM_F_ACK|NLM_F_REQUEST|NLM_F_CREATE|NLM_F_REPLACE);
    header.pid(0); 

    InterfaceAddrMsg body(request.body());
    body.family(AF_INET); 
    body.prefixlen(24);
    body.flags(IFA_F_PERMANENT);
    body.scope(RT_SCOPE_UNIVERSE);
    body.index(2);

    header.length( NLMSG_LENGTH(body.size()) );
	struct in_addr inaddr;

    if (address == NULL || inet_pton(AF_INET, address, (void*)&inaddr) <= 0) {
      throw IException("Invaid ipaddress %s", address);
   }
   
    // Add the ipaddress of inaddr to the request as an IFA_LOCAL route attribute.
    bool r = request.addAttribute(IFA_LOCAL, (void*)&inaddr, sizeof(inaddr));
    if (r == false) {
      throw IException("Failed to add an attribiute for IFA_LOCAL"); 
    }
  }
    
 int bind()
  {
    SockAddrNetlink  addr;
    addr.setFamily(AF_NETLINK);
    addr.setGroups(0); 
    return socketNetlink.bind(addr);
  }
    
    
  int send()
  {
    RouteAttributeRequest<ifaddrmsg>& req = request.request();
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
      NetlinkMsgHeader replyMsgHeader(reply.head);
  
      if (replyMsgHeader.isError()) {
        NetlinkMsgError ack(replyMsgHeader.errorMsg());
        ack.display();
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
  Locale locale;
   
  if (argc != 2) {
    printf("Usage: %s IPv4address(*.*.*.*) \n", argv[0]);
    return 0;
  }
  try {
    const char* address = argv[1];
    InterfaceAddrChangeRequest changeRequest(address);
    changeRequest.bind();
    changeRequest.send();
    changeRequest.recv();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

