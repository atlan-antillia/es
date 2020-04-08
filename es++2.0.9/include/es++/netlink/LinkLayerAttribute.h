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
 *  LinkLayerAttribute.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/netlink/SocketNetlink.h>
#include <es++/netlink/SockAddrNetlink.h>
#include <es++/netlink/NetlinkMsgHeader.h>
#include <es++/netlink/LinkLayerMsg.h>

#include <es++/StringBufferT.h>
#include <es++/CharString.h>
#include <es++/Pair.h>
#include <es++/Socket.h>
#include <es++/Exception.h>
#include <es++/InetAddr.h>
#include <es++/Inet6Addr.h>
#include <es++/Bytes.h>
#include <es++/Integer.h>

#include <es++/netlink/RoutingLinkStats.h>
#include <es++/netlink/RoutingLinkStats64.h>
#include <es++/netlink/RouteAttribute.h>

namespace Es {

class LinkLayerAttribute :public RouteAttribute<ifinfomsg> {
private:
  LinkLayerMsg& linkLayerMsg;
    
public:
  LinkLayerAttribute(NetlinkMsgHeader& header, LinkLayerMsg& layerMsg)
  :RouteAttribute<ifinfomsg>(header),
  linkLayerMsg(layerMsg)
  {
  }


  //See: if_link.h
/*

enum {
    IFLA_UNSPEC,
    IFLA_ADDRESS,
    IFLA_BROADCAST,
    IFLA_IFNAME,
    IFLA_MTU,
    IFLA_LINK,
    IFLA_QDISC,
    IFLA_STATS,
    IFLA_COST,
    IFLA_PRIORITY,
    IFLA_MASTER,
    IFLA_WIRELESS,      // Wireless Extension event - see wireless.h 
    IFLA_PROTINFO,      // Protocol specific information for a link 
    IFLA_TXQLEN,
    IFLA_MAP,
    IFLA_WEIGHT,
    IFLA_OPERSTATE,
    IFLA_LINKMODE,
    IFLA_LINKINFO,
    IFLA_NET_NS_PID,
    IFLA_IFALIAS,
    IFLA_NUM_VF,        // Number of VFs if device is SR-IOV PF 
    IFLA_VFINFO_LIST,
    IFLA_STATS64,
    IFLA_VF_PORTS,
    IFLA_PORT_SELF,
    IFLA_AF_SPEC,
    IFLA_GROUP,     // Group the device belongs to 
    IFLA_NET_NS_FD,
    IFLA_EXT_MASK,      // Extended info mask, VFs, etc 
    IFLA_PROMISCUITY,   // Promiscuity count: > 0 means acts PROMISC 
    __IFLA_MAX
};
*/
public:
  virtual void parse()
  {
    int size = payload();
    void* attr = data();
    const char* string = (const char*)attr;
    int value = *(int*)attr;
    unsigned char* data = (unsigned char*)attr;
    
    switch(type()){

    case IFLA_UNSPEC:
    {
        printf("IFLA_UNSPEC\n");
    }
    break;
    
    //hardware address   interface L2 address
    case IFLA_ADDRESS:
    {
        char buff[128];
        unsigned char* a = (unsigned char*)data;
        if (size == 6) {
            sprintf(buff, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
                a[0], a[1], a[2], a[3], a[4], a[5]);
            Attribute* attr = new Attribute("IFLA_ADDRESS", new CharString(buff));
            linkLayerMsg.setAttribute(IFLA_ADDRESS, attr);
        }
    }
    break;
    
    //hardware address   L2 broadcast address.
    case IFLA_BROADCAST:
    {           
        char buff[128];
        unsigned char* a = (unsigned char*)data;
        
        if (payload() == 6) {
            sprintf(buff, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
                a[0], a[1], a[2], a[3], a[4], a[5]);
            Attribute* attr = new Attribute("IFLA_BROADCAST", new CharString(buff));
            linkLayerMsg.setAttribute(IFLA_BROADCAST, attr);

        }
    }
    break;

    case IFLA_IFNAME:
    {
        Attribute* attr = new Attribute("IFLA_IFNAME", new CharString(string));
        linkLayerMsg.setAttribute(IFLA_IFNAME, attr);
    }
    break;

    case IFLA_MTU:
    {
        Attribute* attr = new Attribute("IFLA_MTU", new Integer(value));
        linkLayerMsg.setAttribute(IFLA_MTU, attr);
    }
    break;

    case IFLA_LINK:
    {
        Attribute* attr = new Attribute("IFLA_LINK", new Integer(value));
        linkLayerMsg.setAttribute(IFLA_LINK, attr);
    }
    break;

    case IFLA_QDISC:
    {
        Attribute* attr = new Attribute("IFLA_QDISC", new CharString(string));
        linkLayerMsg.setAttribute(IFLA_QDISC, attr);
    }
    break;

    case IFLA_STATS:
    {
        struct rtnl_link_stats *st = (struct rtnl_link_stats*)data;
        Attribute* attr = new Attribute("IFLA_STATS", new RoutingLinkStats(*st));
        linkLayerMsg.setAttribute(IFLA_STATS, attr);
    }
    break;

    case IFLA_COST:
    {
        Attribute* attr = new Attribute("IFLA_COST", new Integer(value));
        linkLayerMsg.setAttribute(IFLA_COST, attr);
    }
    break;

    case IFLA_PRIORITY:
    {
        Attribute* attr = new Attribute("IFLA_PRIORITY", new Integer(value));
        linkLayerMsg.setAttribute(IFLA_PRIORITY, attr);

    }
    break;

    case IFLA_MASTER:
    {
        Attribute* attr = new Attribute("IFLA_MASTER", new Integer(value));
        linkLayerMsg.setAttribute(IFLA_MASTER, attr);
    }
    break;

    case IFLA_WIRELESS:
    {
        Attribute* attr = new Attribute("IFLA_WIRELESS", new Integer(value));
        linkLayerMsg.setAttribute(IFLA_WIRELESS, attr);
    }

    break;

    case IFLA_PROTINFO:
    {
        Attribute* attr = new Attribute("IFLA_PROTINFO", new Bytes(data, size));
        linkLayerMsg.setAttribute(IFLA_PROTINFO, attr);
    }
    break;

    case IFLA_TXQLEN:
    {
        Attribute* attr = new Attribute("IFLA_TXQLEN", new Integer(value));
        linkLayerMsg.setAttribute(IFLA_TXQLEN, attr);
    }
    break;

    case IFLA_MAP:
    {
        Attribute* attr = new Attribute("IFLA_MAP", new Bytes(data, size));
        linkLayerMsg.setAttribute(IFLA_MAP, attr);
    }
    break;

    case IFLA_WEIGHT:
    {
        Attribute* attr = new Attribute("IFLA_WEIGHT", new Integer(value));
        linkLayerMsg.setAttribute(IFLA_WEIGHT, attr);
    }
    break;

    case IFLA_OPERSTATE:
    {
        Attribute* attr = new Attribute("IFLA_OPERSTATE", new Integer(value));
        linkLayerMsg.setAttribute(IFLA_OPERSTATE, attr);
    }
    break;

    case IFLA_LINKMODE:
    {
        Attribute* attr = new Attribute("IFLA_LINKMODE", new Integer(value));
        linkLayerMsg.setAttribute(IFLA_LINKMODE, attr);
    }
    break;

    case IFLA_LINKINFO:
    {
        Attribute* attr = new Attribute("IFLA_LINKINFO", new Bytes(data, size));
        linkLayerMsg.setAttribute(IFLA_LINKINFO, attr);
    }
    break;

        
    case IFLA_NET_NS_PID:
    {
        Attribute* attr = new Attribute("IFLA_NET_NS_PID", new Integer(value));
        linkLayerMsg.setAttribute(IFLA_NET_NS_PID, attr);
    }
    break;

    case IFLA_IFALIAS:
    {
        Attribute* attr = new Attribute("IIFLA_IFALIAS", new Integer(value));
        linkLayerMsg.setAttribute(IFLA_IFALIAS, attr);          
    }
    break;

    case IFLA_NUM_VF:       // Number of VFs if device is SR-IOV PF
    {
        Attribute* attr = new Attribute("IFLA_NUM_VF", new Bytes(data, size));
        linkLayerMsg.setAttribute(IFLA_NUM_VF, attr);

    }
    break;
        
    case IFLA_VFINFO_LIST:
    {
        Attribute* attr = new Attribute("IFLA_VFINFO_LIST", new Bytes(data, size) );
        linkLayerMsg.setAttribute(IFLA_VFINFO_LIST, attr);          
    }
    break;

    case IFLA_STATS64:
    {
        struct rtnl_link_stats64 *st = (struct rtnl_link_stats64*)data; 
        Attribute* attr = new Attribute("IFLA_STATS64", new RoutingLinkStats64(*st));
        linkLayerMsg.setAttribute(IFLA_STATS64, attr);
    }
    break;

    case IFLA_VF_PORTS:
    {
        Attribute* attr = new Attribute("IFLA_VF_PORTS", new Integer(value));
        linkLayerMsg.setAttribute(IFLA_VF_PORTS, attr);
    }
    break;

    case IFLA_PORT_SELF:
    {
        Attribute* attr = new Attribute("IFLA_PORT_SELF", new Integer(value));
        linkLayerMsg.setAttribute(IFLA_PORT_SELF, attr);
    }
    break;

    
    case IFLA_AF_SPEC:
    {
        //printf("IFLA_AF_SPEC\n");
        //Bytes::dump(data, size);
    }
    break;
   
    case IFLA_GROUP:        // Group the device belongs to 
    {
        Attribute* attr = new Attribute("IFLA_GROUP", new Integer(value));
        linkLayerMsg.setAttribute(IFLA_GROUP, attr);
    }
    break;

    case IFLA_NET_NS_FD:
    {
        Attribute* attr = new Attribute("IFLA_NET_NS_FD", new Integer(value));

      linkLayerMsg.setAttribute(IFLA_NET_NS_FD, attr);
    }
    break;

    case IFLA_EXT_MASK: // Extended info mask, VFs, etc 
    {
        Attribute* attr = new Attribute("IFLA_EXT_MASK", new Bytes(data, size) );
        linkLayerMsg.setAttribute(IFLA_EXT_MASK, attr);
    }
    break;

    case IFLA_PROMISCUITY:  // Promiscuity count: > 0 means acts PROMISC 
    {
      //2015/01/17 Modified
      Attribute* attr = new Attribute("IFLA_PROMISCUITY", new Integer(value));
      linkLayerMsg.setAttribute(IFLA_PROMISCUITY, attr);
    }
    break;
    
    } //switch
  }
};

}

