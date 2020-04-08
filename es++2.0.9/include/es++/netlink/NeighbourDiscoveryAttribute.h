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
 *  NeighbourDiscoveryAttribute.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/StringBufferT.h>
#include <es++/CharString.h>
#include <es++/Pair.h>
#include <es++/Socket.h>
#include <es++/Exception.h>
#include <es++/InetAddr.h>
#include <es++/Inet6Addr.h>
#include <es++/Bytes.h>

#include <es++/netlink/SocketNetlink.h>
#include <es++/netlink/SockAddrNetlink.h>
#include <es++/netlink/NetlinkMsgHeader.h>
#include <es++/netlink/NeighbourDiscoveryMsg.h>
#include <es++/netlink/RouteAttribute.h>


namespace Es {

class NeighbourDiscoveryAttribute :public RouteAttribute<ndmsg> {
private:

  NeighbourDiscoveryMsg& neighbourDiscoveryMsg;
    
public:
  NeighbourDiscoveryAttribute(NetlinkMsgHeader& header, NeighbourDiscoveryMsg& discoveryMsg)
  :RouteAttribute<ndmsg>(header),
    neighbourDiscoveryMsg(discoveryMsg)
  {
  }

 
/*
    NDA_UNSPEC,
    NDA_DST,
    NDA_LLADDR,
    NDA_CACHEINFO
 */
public:   
  virtual void parse()
  {
    //const char* string = (const char*)data();
    const unsigned char* byte = (const unsigned char*)data();
    int size = payload();
    switch(type()){

    case NDA_UNSPEC:
    {
        printf("NDA_UNSPEC\n");
    }
    break;
        
    case NDA_DST:
    {
        char buff[128];
        inet_ntop(this->msg->ndm_family, byte, buff, sizeof(buff));
        
        Attribute* attr = new Attribute("NDA_DST", new CharString(buff));
        neighbourDiscoveryMsg.setAttribute(NDA_DST, attr);
    }
    break;

    case NDA_LLADDR:
    {
        char buff[128];
        memset(buff, 0, sizeof(buff));
        
        //Bytes::dump(byte, size);
        //printf("NDA_LLADDR size(%d\n", size);
            
        if (size == 4 || size == 16) {
          inet_ntop(this->msg->ndm_family, byte, buff, sizeof(buff));
          Attribute* attr = new Attribute("NDA_LLADDR", new CharString(buff));
          neighbourDiscoveryMsg.setAttribute(NDA_LLADDR, attr);
        } else if (size == 6){
                
           sprintf(buff, "%02x:%02x:%02x:%02x:%02x:%02x",
              byte[0], byte[1], byte[2], byte[3], byte[4], byte[5]);
            
            Attribute* attr = new Attribute("NDA_LLADDR", new CharString(buff));
            neighbourDiscoveryMsg.setAttribute(NDA_LLADDR, attr);
        } else {
            
        }
    }
    break;

/*
struct nda_cacheinfo
{
    __u32 ndm_confirmed;
    __u32 ndm_used;
    __u32 ndm_updated;
    __u32 ndm_refcnt;
};


 */
    case NDA_CACHEINFO:
    {
      char buff[128];
      struct nda_cacheinfo * cache_info = (struct nda_cacheinfo *)byte;
      if (cache_info) {
        sprintf(buff, "confirmed=%d, used=%d, updated=%d, refcnt=%d", cache_info->ndm_confirmed,
          cache_info->ndm_used, cache_info->ndm_updated, cache_info->ndm_refcnt);
        Attribute* attr = new Attribute("NDA_CACHEINFO", new CharString(buff));
        neighbourDiscoveryMsg.setAttribute(NDA_CACHEINFO, attr);
      }
    }
    break;
        
    }
  } 

};

}

