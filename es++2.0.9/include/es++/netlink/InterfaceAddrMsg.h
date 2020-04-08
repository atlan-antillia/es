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
 *  InterfaceAddrMsg.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/LinkedListT.h>
#include <es++/Property.h>
#include <es++/StringBufferT.h>
#include <es++/CharString.h>
#include <es++/Pair.h>
#include <es++/KeyNamePairs.h>
#include <es++/Exception.h>
#include <es++/Attribute.h>

#include <es++/netlink/SocketNetlink.h>
#include <es++/netlink/SockAddrNetlink.h>
#include <es++/netlink/NetlinkMsg.h>

/*
struct ifaddrmsg {
    __u8        ifa_family;
    __u8        ifa_prefixlen;  // The prefix length   
    __u8        ifa_flags;  // Flags            
    __u8        ifa_scope;  // Address scope    
    __u32       ifa_index;  // Link index       
};
*/

namespace Es {

class InterfaceAddrMsg :public NetlinkMsg<ifaddrmsg> {
 
public:
  InterfaceAddrMsg(void* msg)
  :NetlinkMsg<ifaddrmsg>(msg, (size_t)IFA_MAX)
  {
  }

public:
  InterfaceAddrMsg(struct ifaddrmsg* msg)
  :NetlinkMsg<ifaddrmsg>(msg, (size_t)IFA_MAX)
  {
  }
    
  struct rtattr* routingAttr()
  {
    return IFA_RTA(message);
  }
    

  void family(unsigned char family)
  {
    message -> ifa_family = family; 
  }
    
  unsigned char family()
  {
    return message -> ifa_family;
  }

  void prefixlen(unsigned char len)
  {
    message -> ifa_prefixlen= len; 
  }

  unsigned char prefixlen()
  {
    return message -> ifa_prefixlen; 
  }
    
    
  void index(int index)
  {
    message -> ifa_index = index;
  }
    
  int index()
  {
    return message -> ifa_index;
  }
    
  void flags(unsigned char flags)
  {
    message -> ifa_flags = flags;
  }
    
  unsigned char flags()
  {
    return message -> ifa_flags; 
  }

  void scope(unsigned char scope)
  {
    message -> ifa_scope = scope;
  }
    
  unsigned char scope()
  {
    return message -> ifa_scope; 
  }
  
public:
  void display()
  {
    printf("IFADDR family:    %s\n", getFamily(message -> ifa_family));
    printf("IFADDR prefixlen: %d\n", prefixlen() );  //2015/01/01
    printf("IFADDR flags:     %s\n", (const char*)getFlags(message -> ifa_flags));
    printf("IFADDR scope:     %s\n", getScope(message -> ifa_scope) );

    printf("IFADDR index:  %d\n", message -> ifa_index);

    displayAttributes();
  }

public:
  void getPropertyList(LinkedListT<Property>& list)
  {
    list.add(new Property("IFADDR family", getFamily(message -> ifa_family)) );
    list.add(new Property("IFADDR prefixlen", prefixlen()) );
    list.add(new Property("IFADDR flags", (const char*)getFlags(message -> ifa_flags)) );
    list.add(new Property("IFADDR scope", getScope(message -> ifa_scope) ) );
    list.add(new Property("IFADDR index", message -> ifa_index) );

 
    list.add(new Property("IFA_ADDRESS", getAttributeValue(IFA_ADDRESS)) );
    list.add(new Property("IFA_LOCAL", getAttributeValue(IFA_LOCAL) ) );
    list.add(new Property("IFA_LABEL", getAttributeValue(IFA_LABEL) ) );
    list.add(new Property("IFA_BROADCAST", getAttributeValue(IFA_BROADCAST) ) );
    list.add(new Property("IFA_ANYCAST", getAttributeValue(IFA_ANYCAST) ) );
    list.add(new Property("IFA_CACHEINFO", getAttributeValue(IFA_CACHEINFO) ) );
    list.add(new Property("IFA_MULTICAST", getAttributeValue(IFA_MULTICAST) ) );
  }
  
public:
  static const char* getScope(int scope)
  {
     static Pair<int, const char*> scopes[] = {
      {RT_SCOPE_UNIVERSE, "SCOPE_UNIVERSE"},
      {RT_SCOPE_SITE,     "SCOPE_SITE"},
      {RT_SCOPE_LINK,     "SCOPE_LINK"},
      {RT_SCOPE_HOST,     "SCOPE_HOST"},
      {RT_SCOPE_NOWHERE,  "SCOPE_NOWHERE"},
    };
    KeyNamePairs<int> pairs(scopes, SizeOf(scopes));
    return pairs.match(scope);    
  }

public:
  static const char* getType(unsigned short type)
  {
    static Pair<unsigned short, const char*> types[] =
    {
    {ARPHRD_NETROM,    "ARPHRD_NETROM"},   // From KA9Q: NET/ROM pseudo.
    {ARPHRD_ETHER,     "ARPHRD_ETHER"},   // Ethernet 10/100Mbps.  
    {ARPHRD_EETHER,    "ARPHRD_EETHER"},   // Experimental Ethernet. 
    {ARPHRD_AX25,      "ARPHRD_AX25"},   // AX.25 Level 2. 
    {ARPHRD_PRONET,    "ARPHRD_PRONET"},  // PROnet token ring. 
    {ARPHRD_CHAOS,     "ARPHRD_CHAOS"},    // Chaosnet. 
    {ARPHRD_IEEE802,   "ARPHRD_IEEE802"},    // IEEE 802.2 Ethernet/TR/TB.
    {ARPHRD_ARCNET,    "ARPHRD_ARCNET"},     // ARCnet. 
    {ARPHRD_APPLETLK,  "ARPHRD_APPLETLK"},  // APPLEtalk. 
    {ARPHRD_DLCI,      "ARPHRD_DLCI"},     // Frame Relay DLCI. 
    {ARPHRD_ATM,       "ARPHRD_ATM"},      // ATM.  
    {ARPHRD_METRICOM,  "ARPHRD_METRICOM"},   // Metricom STRIP (new IANA id). 
    {ARPHRD_IEEE1394,  "ARPHRD_IEEE1394"},   // IEEE 1394 IPv4 - RFC 2734.
    {ARPHRD_EUI64,     "ARPHRD_EUI64"},    // EUI-64.  
    {ARPHRD_INFINIBAND,"ARPHRD_INFINIBAND"},// InfiniBand.   
    };
    KeyNamePairs<unsigned short> pairs(types, SizeOf(types));
    return pairs.match(type);
  }
  
public:
  CharString getFlags(unsigned int flags)
  {
    static Pair<unsigned int, const char*> ifFlags[] = {
    {IFA_F_TEMPORARY,   "IFA_F_TEMPORARY"},     
    {IFA_F_NODAD,       "IFA_F_NODAD"}, 
    {IFA_F_OPTIMISTIC,  "IFA_F_OPTIMISTIC"}, 
    {IFA_F_DADFAILED,   "IFA_F_DADFAILED"  }, 
    {IFA_F_HOMEADDRESS, "IFA_F_HOMEADDRESS"},
    {IFA_F_DEPRECATED,  "IFA_F_DEPRECATED"}, 
    {IFA_F_TENTATIVE,   "IFA_F_TENTATIVE"}, 
    {IFA_F_PERMANENT,   "IFA_F_PERMANENT"},
    };

    KeyNamePairs<unsigned int> pairs(ifFlags, SizeOf(ifFlags));
    
    return pairs.bitMatch(flags);
  }  
};

}

