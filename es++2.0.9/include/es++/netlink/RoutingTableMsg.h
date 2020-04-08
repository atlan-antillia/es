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
 *  RoutingTableMsgy.h
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
#include <linux/route.h>


/*
Definitions used in routing table administration.

struct rtmsg {
    unsigned char       rtm_family;
    unsigned char       rtm_dst_len;
    unsigned char       rtm_src_len;
    unsigned char       rtm_tos;

    unsigned char       rtm_table;  // Routing table id/
    unsigned char       rtm_protocol;   // Routing protocol; see below 
    unsigned char       rtm_scope;   
    unsigned char       rtm_type;       

    unsigned        rtm_flags;
};
*/

namespace Es {

class RoutingTableMsg :public NetlinkMsg<rtmsg> {
/*private:

  struct rtmsg* message;

  Attribute* attributes[RTA_MAX +1];
 */  
public:
  RoutingTableMsg(void* msg)
  :NetlinkMsg<rtmsg>(msg, (size_t)RTA_MAX)
  {
  }

public:
  RoutingTableMsg(struct rtmsg* msg)
  :NetlinkMsg<rtmsg>(msg, (size_t)RTA_MAX)
  {
  }
    
  struct rtattr* routingAttr()
  {
    return RTM_RTA(message);  //
  }


  void family(unsigned char family)
  {
    message -> rtm_family = family; 
  }
    
  unsigned char family()
  {
    return message -> rtm_family;
  }

  void dstlen(unsigned char len)
  {
     message -> rtm_dst_len = len;
  }
    
  unsigned char dstlen()
  {
    return message -> rtm_dst_len;
  }
    
  void srclen(unsigned char len) 
  {
    message -> rtm_src_len = len;
  }
    
  unsigned char srclen() 
  {
    return message -> rtm_src_len;
  }

  void tos(unsigned char tos)
  {
    message -> rtm_tos = tos;
  }
    
  unsigned char tos()
  {
    return message -> rtm_tos;
  }
    
  void table(unsigned char t)
  {
    message -> rtm_table = t;
  }


  unsigned char table()
  {
    return message -> rtm_table;
  }

  void protocol(unsigned char protocol)
  {
    message ->  rtm_protocol = protocol;
  }

  unsigned char protocol()
  {
    return message ->   rtm_protocol;
  }

  void scope(unsigned char scope)
  {
    message -> rtm_scope = scope;
  }
    
  unsigned char scope()
  {
    return message -> rtm_scope; 
  }
    
  void type(unsigned char type)
  {
    message -> rtm_type = type;       
  }

  unsigned char type()
  {
    return message -> rtm_type;       
  }

  void flags(unsigned flags)
  {
    message -> rtm_flags = flags;
  }
    
  unsigned  flags()
  {
    return message -> rtm_flags; 
  }


  void displayAttributes()
  {
    for(int i = 0;  i<= RTA_MAX ; i++) {
        if (attributes[i] != NULL) {
            attributes[i] -> display();
        }
    }
  }
    
public:
  void display()
  {
    printf("RTMSG family:   %s\n",   getFamily(message -> rtm_family));
    printf("RTMSG dst_len:  %d\n",   message -> rtm_dst_len);
    printf("RTMSG src_len:  %d\n",   message -> rtm_src_len);
    printf("RTMSG tos:      %d\n",   message -> rtm_tos);
    printf("RTMSG table:    %s\n",   getTable(message -> rtm_table));
    printf("RTMSG protocol: %s\n",   getProtocol(message -> rtm_protocol) );
    printf("RTMSG scope:    %s\n",   getScope(message -> rtm_scope) );
    printf("RTMSG type:     %s\n",   getType(message -> rtm_type) );
    printf("RTMSG flags:    %s\n",   (const char*)getFlags(message -> rtm_flags));
    displayAttributes();
  }

public:
  void getPropertyList(LinkedListT<Property>& list)
  {
  	list.add(new Property("RTMSG family",   getFamily(message -> rtm_family)) );
    list.add(new Property("RTMSG dst_len",   message -> rtm_dst_len) );
  	list.add(new Property("RTMSG src_len",   message -> rtm_src_len) );
  	list.add(new Property("RTMSG tos",   message -> rtm_tos) );
    list.add(new Property("RTMSG table",   getTable(message -> rtm_table)) );

 
  	list.add(new Property("RTMSG protocol",   getProtocol(message -> rtm_protocol))  );
  	list.add(new Property("RTMSG scope",   getScope(message -> rtm_scope) ) );
  	list.add(new Property("RTMSG type",   getType(message -> rtm_type) ) );
  	list.add(new Property("RTMSG flags",   (const char*)getFlags(message -> rtm_flags)) );
  	
  	//
  	list.add(new Property("RTA_DST", getAttributeValue(RTA_DST) ) );
  	list.add(new Property("RTA_SRC", getAttributeValue(RTA_SRC) ) );
  	
  	list.add(new Property("RTA_IIF", getAttributeValue(RTA_IIF) ) );
  	
  	list.add(new Property("RTA_OIF", getAttributeValue(RTA_OIF) ) );
  	list.add(new Property("RTA_GATEWAY", getAttributeValue(RTA_GATEWAY) ) );
  	list.add(new Property("RTA_PRIORITY", getAttributeValue(RTA_PRIORITY) ) );
  	list.add(new Property("RTA_PREFSRC", getAttributeValue(RTA_PREFSRC) ) );
  	list.add(new Property("RTA_METRICS", getAttributeValue(RTA_METRICS) ) );
  	list.add(new Property("RTA_MULTIPATH", getAttributeValue(RTA_MULTIPATH) ) );
  	list.add(new Property("RTA_FLOW", getAttributeValue(RTA_FLOW) ) );
  	list.add(new Property("RTA_CACHEINFO", getAttributeValue(RTA_CACHEINFO) ) );
  	list.add(new Property("RTA_TABLE", getAttributeValue(RTA_TABLE) ) );
  	list.add(new Property("RTA_MARK", getAttributeValue(RTA_MARK) ) );
  	
  }
	
public:
 static const char* getScope(int scope)
  {
    static Pair<int, const char*> scopes[] = {
        {RT_SCOPE_UNIVERSE, "RT_SCOPE_UNIVERSE"},
        {RT_SCOPE_SITE,     "RT_SCOPE_SITE"},
        {RT_SCOPE_LINK,     "RT_SCOPE_LINK"},
        {RT_SCOPE_HOST,     "RT_SCOPE_HOST"},
        {RT_SCOPE_NOWHERE,  "RT_SCOPE_NOWHERE"},
    };
  	KeyNamePairs<int> pairs(scopes, SizeOf(scopes));
  	return pairs.match(scope);
  }

public:
 static const char* getTable(int table)
  {
    static Pair<int, const char*> tables[] = {
	{RT_TABLE_UNSPEC	,"RT_TABLE_UNSPEC"},
	{RT_TABLE_DEFAULT	,"RT_TABLE_DEFAULT"},
	{RT_TABLE_MAIN		,"RT_TABLE_MAIN"},
	{RT_TABLE_LOCAL		,"RT_TABLE_LOCAL"},
    };
  	KeyNamePairs<int> pairs(tables, SizeOf(tables));
  	return pairs.match(table);
  }



public:
  static const char* getType(unsigned short type)
  {
 //rtm_type
    static Pair<unsigned, const char*> types[] = { 
        {RTN_UNSPEC,      "RTN_UNSPEC"},
        {RTN_UNICAST,     "RTN_UNICAST"},        // Gateway or direct route  
        {RTN_LOCAL,       "RTN_LOCAL"},    // Accept locally       
        {RTN_BROADCAST,   "RTN_BROADCAST"},     // Accept locally as broadcast, send as broadcast 
        {RTN_ANYCAST,     "RTN_ANYCAST"},      // Accept locally as broadcast, but send as unicast 
        {RTN_MULTICAST,   "RTN_MULTICAST"},     // Multicast route      
        {RTN_BLACKHOLE,   "RTN_BLACKHOLE"},     // Drop             
        {RTN_UNREACHABLE, "RTN_UNREACHABLE"},   // Destination is unreachable   
        {RTN_PROHIBIT,    "RTN_PROHIBIT"},      // Administratively prohibited  
        {RTN_THROW,       "RTN_THROW"},     // Not in this table        
        {RTN_NAT,         "RTN_NAT"},      // Translate this address   
        {RTN_XRESOLVE,    "RTN_XRESOLVE"},
    };
  	KeyNamePairs<unsigned> pairs(types, SizeOf(types));
  	return pairs.match(type);
  }
  
// rtm_protocol 

public:
  static const char* getProtocol(unsigned char protocol)
  {
    static Pair<unsigned char, const char*> protocols[] =
     {
        {RTPROT_UNSPEC,     "RTPROT_UNSPEC"},
        {RTPROT_REDIRECT,   "RTPROT_REDIRECT"},
        {RTPROT_KERNEL,     "RTPROT_KERNEL"},
        {RTPROT_BOOT,       "RTPROT_BOOT"},
        {RTPROT_STATIC,     "RTPROT_STATIC"},
        {RTPROT_GATED,      "RTPROT_GATED"},
        {RTPROT_RA,         "RTPROT_RA"},
        {RTPROT_MRT,        "RTPROT_MRT"},
        {RTPROT_ZEBRA,      "RTPROT_ZEBRA"},
        {RTPROT_BIRD,       "RTPROT_BIRD"},
        {RTPROT_DNROUTED,   "RTPROT_DNROUTED"},
        {RTPROT_XORP,       "RTPROT_XORP"},
        {RTPROT_NTK,        "RTPROT_NTK"},
        {RTPROT_DHCP,       "RTPROT_DHCP"}
    };
 	KeyNamePairs<unsigned char> pairs(protocols, SizeOf(protocols));
  	return pairs.match(protocol);
  }

public:
// rtm_flags 

  static CharString getFlags(unsigned  flags)
  {
    static Pair<unsigned, const char*> rtmFlags[] = {
        {RTM_F_NOTIFY,    "RTM_F_NOTIFY"},
        {RTM_F_CLONED,    "RTM_F_CLONED"},
        {RTM_F_EQUALIZE,  "RTM_F_EQUALIZE"},
        {RTM_F_PREFIX,    "RTM_F_PREFIX"},
    };
    KeyNamePairs<unsigned> pairs(rtmFlags, SizeOf(rtmFlags));
  	return pairs.bitMatch(flags);
  }
  
};

}

