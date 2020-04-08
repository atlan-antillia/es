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
 *  TrafficControlRequest.cpp
 *
 *****************************************************************************/

#include <es++/Object.h>
#include <es++/Exception.h>
#include <es++/Locale.h>

#include <es++/netlink/SocketNetlink.h>
#include <es++/netlink/SockAddrNetlink.h>
#include <es++/netlink/TrafficControlMsg.h>
#include <es++/netlink/TrafficControlAttribute.h>
#include <es++/netlink/NetlinkMsgHeader.h>
#include <es++/netlink/NetlinkRequest.h>
#include <es++/netlink/NetlinkReply.h>

namespace Es {
    
class TrafficControlRequest :public Object {
private:
  NetlinkRequest<tcmsg> request;

  SocketNetlink socketNetlink;
  
  NetlinkReply<tcmsg> reply;

public:
  TrafficControlRequest(int parent = TC_H_ROOT)
  {
    NetlinkMsgHeader header(&request.head);
    header.type(RTM_GETQDISC);
    header.flags(NLM_F_REQUEST |NLM_F_DUMP);
    header.pid(getpid()); 

    TrafficControlMsg body(&request.body);
    body.family(AF_UNSPEC); 
  	body.ifindex(0);
  	body.parent(parent);
  	
    header.length(NLMSG_LENGTH(body.size()) );
  }
    

  int bind()
  {
    SockAddrNetlink  local;
  	local.setFamily(AF_NETLINK);
    local.setPid(getpid());
    return socketNetlink.bind(&local);
  }
	
  int connect()
  {
    SockAddrNetlink  remote;
  	remote.setFamily(AF_NETLINK);
    remote.setPid(0);
    return socketNetlink.connect(remote);
  }
	
  int send()
  {
    return socketNetlink.send((const char*)&request, sizeof(request), 0);   
  }
    
  void recv()
  {
    int count = 1;
    char filename[256];
    while (true) {
      bzero(&reply, sizeof(reply));
      int n = socketNetlink.recv((char*)&reply, sizeof(reply), 0);
      if (n < 0){
        break;;
      }
      sprintf(filename, "./reply_%d.bin",  count++); 
      NetlinkMsgHeader replyMsgHeader(&reply.head);
      if (replyMsgHeader.isDone() || replyMsgHeader.isError()) {
        break;
      }
      try {
        do {
          replyMsgHeader.display();

          replyMsgHeader.save(filename);

          TrafficControlMsg replyTrafficControlMsg(&reply.body);

          TrafficControlAttribute attr(replyMsgHeader, replyTrafficControlMsg);
          do {
            attr.parse();
          } while (attr.next() );

          replyTrafficControlMsg.display();
          printf("\n");

        } while (replyMsgHeader.next());
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

    TrafficControlRequest request;
    request.send();
  	request.bind();
    request.recv();
    
  } catch (Exception& ex) {
    caught(ex);
  }
  return 0;
}

