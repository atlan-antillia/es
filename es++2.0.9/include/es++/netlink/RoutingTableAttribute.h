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
 *  RoutingTableAttribute.h
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
#include <es++/Integer.h>

#include <es++/netlink/NetlinkMsgHeader.h>
#include <es++/netlink/RoutingTableMsg.h>
#include <es++/netlink/RouteAttribute.h>


namespace Es {

class RoutingTableAttribute :public RouteAttribute<rtmsg> {
private:
    
  RoutingTableMsg& routingTableMsg;
    
public:
  RoutingTableAttribute(NetlinkMsgHeader& header, RoutingTableMsg& routeMsg)
  :RouteAttribute<rtmsg>(header),
  routingTableMsg(routeMsg)
  {
  }
  

//See: rtnetlink.h

/*
num rtattr_type_t {
    RTA_UNSPEC,
    RTA_DST,
    RTA_SRC,
    RTA_IIF,
    RTA_OIF,
    RTA_GATEWAY,
    RTA_PRIORITY,
    RTA_PREFSRC,
    RTA_METRICS,
    RTA_MULTIPATH,
    RTA_PROTOINFO, // no longer used
    RTA_FLOW,
    RTA_CACHEINFO,
    RTA_SESSION, // no longer used
    RTA_MP_ALGO, // no longer used
    RTA_TABLE,
    RTA_MARK,
    __RTA_MAX
};
*/
  
public:
  virtual void parse()
  {
    //int size = payload();
    void* attr = data();
 
    int value = *(int*)attr;
    unsigned char* byte = (unsigned char*)attr;
    
    switch(type()){

    case RTA_UNSPEC:
    {
        printf("RTA_UNSPEC\n");
    }
    break;
    
    case RTA_DST:
    {
        char buff[128];
        inet_ntop(this->msg->rtm_family, byte, buff, sizeof(buff));
        Attribute* attr = new Attribute("RTA_DST", new CharString(buff));
        routingTableMsg.setAttribute(RTA_DST, attr);

    }
    break;
    
    case RTA_SRC:
    {           
        char buff[128];
        inet_ntop(this->msg->rtm_family, byte, buff, sizeof(buff));
        Attribute* attr = new Attribute("RTA_SRC", new CharString(buff));
        routingTableMsg.setAttribute(RTA_SRC, attr);
    }
    break;

    case RTA_IIF:
    {
        Attribute* attr = new Attribute("RTA_IIF", new Integer(value));
        routingTableMsg.setAttribute(RTA_IIF, attr);
    }
    break;

    case RTA_OIF:
    {
        Attribute* attr = new Attribute("RTA_OIF", new Integer(value));
        routingTableMsg.setAttribute(RTA_OIF, attr);
    }
    break;

    case RTA_GATEWAY:
    {
        char buff[128];
        inet_ntop(this->msg->rtm_family, byte, buff, sizeof(buff));
        Attribute* attr = new Attribute("RTA_GATEWAY", new CharString(buff));
        routingTableMsg.setAttribute(RTA_GATEWAY, attr);
    }
    break;

    case RTA_PRIORITY:
    {
        Attribute* attr = new Attribute("RTA_PRIORITY", new Integer(value));
        routingTableMsg.setAttribute(RTA_PRIORITY, attr);
    }
    break;

    case RTA_PREFSRC:
    {
        char buff[128];
        inet_ntop(this->msg->rtm_family, byte, buff, sizeof(buff));
        Attribute* attr = new Attribute("RTA_PREFSRC", new CharString(buff));
        routingTableMsg.setAttribute(RTA_PREFSRC, attr);
    }
    break;

    case RTA_METRICS:
    {
        Attribute* attr = new Attribute("RTA_METRICS", new Integer(value));
        routingTableMsg.setAttribute(RTA_METRICS, attr);
    }
    break;

/*
struct rtnexthop
{
    unsigned short rtnh_len;
    unsigned char rtnh_flags;
    unsigned char rtnh_hops;
    int rtnh_ifindex;
};
  */
    case RTA_MULTIPATH:
    {
        char buff[128];
        struct rtnexthop* hop = (struct rtnexthop*)attr;
        sprintf(buff, "len=%u, flags=%u hops=%u ifindex=%d",
            hop -> rtnh_len, hop -> rtnh_flags, hop -> rtnh_hops, hop -> rtnh_ifindex);
        Attribute* attr = new Attribute("RTA_MULTIPATH", new CharString(buff));
        routingTableMsg.setAttribute(RTA_MULTIPATH, attr);      }
    break;

    case RTA_FLOW:
    {
        Attribute* attr = new Attribute("RTA_FLOW", new Integer(value));
        routingTableMsg.setAttribute(RTA_FLOW, attr);
    }
    break;

    case RTA_CACHEINFO:
    {
/*
  struct rta_cacheinfo {
 __u32  rta_clntref 
 __u32  rta_lastuse 
 __s32  rta_expires 
 __u32  rta_error 
 __u32  rta_used 
 __u32  rta_id 
 __u32  rta_ts 
 __u32  rta_tsage 
 };         
  */
        struct rta_cacheinfo* cache = (struct rta_cacheinfo*)attr;
        char buff[128];
        sprintf(buff, "clntref=%d lastuse=%d expires=%d"
                      " error=%d used=%d id=%d ts=%d tsage=%d",
                    cache->rta_clntref, cache->rta_lastuse, cache->rta_expires, cache->rta_error,
                    cache->rta_used,    cache->rta_id,      cache->rta_ts,      cache->rta_tsage);

        Attribute* attr = new Attribute("RTA_CACHEINFO", new CharString(buff));
        routingTableMsg.setAttribute(RTA_CACHEINFO, attr);
    }

    break;

    case RTA_TABLE:
    {
        Attribute* attr = new Attribute("RTA_TABLE", new Integer(value));
        routingTableMsg.setAttribute(RTA_TABLE, attr);
    }
    break;

    case RTA_MARK:
    {
        Attribute* attr = new Attribute("RTA_MARK", new Integer(value));
        routingTableMsg.setAttribute(RTA_MARK, attr);
    }
    break;
    
    } //switch
  }
};

}

