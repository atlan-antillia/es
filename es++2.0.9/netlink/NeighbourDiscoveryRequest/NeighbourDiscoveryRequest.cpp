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
 *  NeighbourDiscoveryRequest.cpp
 *
 *****************************************************************************/

#include <es++/Object.h>
#include <es++/netlink/SocketNetlink.h>
#include <es++/netlink/SockAddrNetlink.h>

#include <es++/Exception.h>

#include <es++/netlink/NeighbourDiscoveryAttribute.h>
#include <es++/netlink/NetlinkMsgHeader.h>
#include <es++/netlink/NeighbourDiscoveryMsg.h>
#include <es++/netlink/NetlinkRequest.h>
#include <es++/Locale.h>

namespace Es {
    
class NeighbourDiscoveryRequest :public Object {
private:
  NetlinkRequest<ndmsg> request;
  SocketNetlink socketNetlink;

public:
  NeighbourDiscoveryRequest(unsigned char family)
 {
    NetlinkMsgHeader header(&request.head);
    header.type(RTM_GETNEIGH);
    header.flags(NLM_F_REQUEST | NLM_F_DUMP); ////NLM_F_ROOT|NLM_F_REQUEST);
    header.seq(100);
    header.pid(getpid()); 

    NeighbourDiscoveryMsg body(&request.body);
    body.family(family); 

    header.length( NLMSG_LENGTH(body.size()) );
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
        
      NetlinkMsgHeader replyHeader(buffer, n);
      if (replyHeader.isDone() || replyHeader.isError() ) {
        break;
      }

      try {
        do {
          replyHeader.display();
          NeighbourDiscoveryMsg discoveryMsg(replyHeader.data());

          NeighbourDiscoveryAttribute attr(replyHeader, discoveryMsg);
          do {
            attr.parse();
          } while (attr.next() );

          discoveryMsg.display();
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


int main(int argc, char* argv[])
{
  Locale locale;
  try {

    NeighbourDiscoveryRequest request(AF_INET);

    //request.bind();
    request.send();
    request.recv();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

