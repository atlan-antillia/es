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
 *  LinkLayerRequest.cpp
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

namespace Es {

class LinkLayerRequest :public Object {
private:
  NetlinkRequest<ifinfomsg> request;
  SocketNetlink  socketNetlink;
    
public:
  LinkLayerRequest(unsigned char family)
  {
    NetlinkMsgHeader header(&request.head);

    header.type(RTM_GETLINK);
    header.flags(NLM_F_ROOT|NLM_F_REQUEST);
    header.seq(1);
    header.pid(0);

    LinkLayerMsg body(&request.body);
    body.family(family); 

    header.length( NLMSG_LENGTH(body.size()) );
  }

  ~LinkLayerRequest()
  {
    socketNetlink.close();
  }
    
  int send()
  {
     return socketNetlink.send((const char*)&request, sizeof(request), 0);
  }
    
  void recv()
  {
    while (1) {
      char buffer[4096];
      bzero(buffer, sizeof(buffer));
      int n = socketNetlink.recv(buffer, sizeof(buffer), 0);
      if (n < 0){
        break;;
      }
        
      NetlinkMsgHeader netlinkMsgHeader(buffer, n);
      if ( netlinkMsgHeader.isDone() ) {
        break;
      }
      try {
        do {
          netlinkMsgHeader.display();
          LinkLayerMsg linkLayerMsg(netlinkMsgHeader.data());

          LinkLayerAttribute attr(netlinkMsgHeader, linkLayerMsg); //
          do {
            attr.parse();
          } while (attr.next() );

          //linkLayerMsg.displayMin();
          linkLayerMsg.display();
          printf("\n");

        } while ( netlinkMsgHeader.next());
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
    LinkLayerRequest request(ARPHRD_ETHER);
    request.send();
    request.recv();
    
  } catch (Exception& ex) {
    caught(ex);   
  }
  return 0; 
}

