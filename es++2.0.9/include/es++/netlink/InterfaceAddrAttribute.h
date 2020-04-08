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
 *  InterfaceAddrAttribute.h
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
#include <es++/netlink/InterfaceAddrMsg.h>
#include <es++/netlink/RouteAttribute.h>


namespace Es {

class InterfaceAddrAttribute :public RouteAttribute<ifaddrmsg> {

private:
  InterfaceAddrMsg& interfaceAddrMsg;
    
public:
  InterfaceAddrAttribute(NetlinkMsgHeader& header, InterfaceAddrMsg& addrMsg)
    :RouteAttribute<ifaddrmsg>(header),
    interfaceAddrMsg(addrMsg)
  {
  }
  

  //See: if_addr.h
public:
  virtual void parse()
  {
    char buff[128];
    const char* string = (const char*)data();
    const unsigned char* byte = (const unsigned char*)data();
    switch(type()){

    case IFA_UNSPEC:
    {
        printf("IFLA_UNSPEC\n");
    }
    break;
        
    case IFA_ADDRESS:
    {
        inet_ntop(this->msg->ifa_family, byte, buff, sizeof(buff));        
        Attribute* attr = new Attribute("IFA_ADDRESS", new CharString(buff));
        interfaceAddrMsg.setAttribute(IFA_ADDRESS, attr);
    }
    break;

    case IFA_LOCAL:
    {
        inet_ntop(this->msg->ifa_family, byte, buff, sizeof(buff));
        Attribute* attr = new Attribute("IFA_LOCAL", new CharString(buff));
        interfaceAddrMsg.setAttribute(IFA_LOCAL, attr);
    }
    break;

    case IFA_LABEL:
    {
        Attribute* attr = new Attribute("IFA_LABEL", new CharString(string));
        interfaceAddrMsg.setAttribute(IFA_LABEL, attr);

    }
    break;
        
        
    case IFA_BROADCAST:
    {
        inet_ntop(this->msg->ifa_family, byte, buff, sizeof(buff));
        Attribute* attr = new Attribute("IFA_BROADCAST", new CharString(buff));
        interfaceAddrMsg.setAttribute(IFA_BROADCAST, attr);
    }
    break;

    case IFA_ANYCAST:
    {
        inet_ntop(this->msg->ifa_family, byte, buff, sizeof(buff));
        Attribute* attr = new Attribute("IFA_ANYCAST", new CharString(buff));
        interfaceAddrMsg.setAttribute(IFA_ANYCAST, attr);
    }
    break;


    case  IFA_CACHEINFO:
    {
      struct ifa_cacheinfo * cache_info = (struct ifa_cacheinfo *)byte;
      if (cache_info) {
        sprintf(buff, "valid 0x%x(sec), prefered 0x%x(sec)", cache_info->ifa_valid, cache_info->ifa_prefered);
        Attribute* attr = new Attribute("IFA_CACHEINFO", new CharString(buff));
        interfaceAddrMsg.setAttribute(IFA_CACHEINFO, attr);
      }
    }
    break;
        
    case IFA_MULTICAST:
    {
      inet_ntop(this->msg->ifa_family, byte, buff, sizeof(buff));
      Attribute* attr = new Attribute("IFA_MULTICAST", new CharString(buff));
      interfaceAddrMsg.setAttribute(IFA_MULTICAST, attr);
    }
    break;

    } //switch
  } 

    
};

}

