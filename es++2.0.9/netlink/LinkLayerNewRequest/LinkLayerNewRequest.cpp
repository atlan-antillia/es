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
 *  LinkLayerNewRequest.cpp
 *
 *****************************************************************************/

#include <es++/Object.h>
#include <es++/Exception.h>
#include <es++/Locale.h>

#include <es++/netlink/SocketNetlink.h>
#include <es++/netlink/SockAddrNetlink.h>
#include <es++/netlink/LinkLayerAttribute.h>
#include <es++/netlink/NetlinkMsgHeader.h>
#include <es++/netlink/LinkLayerMsg.h>
#include <es++/netlink/NetlinkRequest.h>
#include <es++/netlink/NetlinkReply.h>
#include <es++/netlink/NetlinkMsgError.h>

namespace Es {

class LinkLayerNewRequest :public Object {
private:
  NetlinkRequest<ifinfomsg> request;

  SocketNetlink  socketNetlink;

  NetlinkReply<ifinfomsg> reply;
    
public:
  LinkLayerNewRequest(const char* updown, int index)
  {
    NetlinkMsgHeader header(&request.head);

    header.type(RTM_NEWLINK);
    header.flags(NLM_F_ACK|NLM_F_REQUEST);
    header.seq(1);
    header.pid(0);

    LinkLayerMsg body(&request.body);
    body.family(AF_UNSPEC); 
    unsigned change = body.change()|IFF_UP;
    body.change(change);
    unsigned flags = body.flags(); 
    if (strcasecmp(updown, "up") == 0) {
      flags |= IFF_UP;
    }
    if (strcasecmp(updown, "down") == 0) {
      flags &=  ~IFF_UP;
    }
    body.flags(flags);

    body.index(index);

    header.length( NLMSG_LENGTH(body.size()) );
  }


  int bind()
  {
    SockAddrNetlink addr;
    addr.setFamily(AF_NETLINK);
    addr.setGroups(0);

    return socketNetlink.bind(&addr);
  }

  int send()
  {
     return socketNetlink.send((const char*)&request, sizeof(request), 0);
  }
    
  void recv()
  {
    while (1) {
      bzero(&reply, sizeof(reply));
      int n = socketNetlink.recv((char*)&reply, sizeof(reply), 0);
      if (n < 0){
        break;;
      }
      NetlinkMsgHeader replyHeader(reply.head);
  
      if ( replyHeader.isError() ) {
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
//  Locale locale;
  if (argc != 3) {
    printf("Usage: %s \"up\"|\"down\" ifindex\n", argv[0]);
    printf(" %s down 2\n", argv[0]);
    return 0;
  }
  try {
    const char* updown = argv[1];
    if (!(strcasecmp(updown, "up") == 0 ||
        strcasecmp(updown, "down") == 0) ) {
       throw IException("Invalid argument %s\n", updown);
    }
    int index = atoi(argv[2]);

    LinkLayerNewRequest request(updown, index);
    request.bind();
    request.send();
    request.recv();
    
  } catch (Exception& ex) {
    caught(ex);   
  }
  return 0; 
}

