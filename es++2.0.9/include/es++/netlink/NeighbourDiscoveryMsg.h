/******************************************************************************
 *
 * Copyright (c) 2014 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  NeighbourDiscoveryMsg.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/StringBufferT.h>
#include <es++/CharString.h>
#include <es++/Pair.h>
#include <es++/KeyNamePairs.h>
#include <es++/Exception.h>
#include <es++/LinkedListT.h>
#include <es++/Property.h>

#include <es++/netlink/SocketNetlink.h>
#include <es++/netlink/SockAddrNetlink.h>
#include <es++/netlink/RoutingTableMsg.h>
#include <es++/netlink/NetlinkMsg.h>


/*
struct ndmsg
{
    unsigned char ndm_family;
    unsigned char ndm_pad1;
    unsigned short ndm_pad2;
    int ndm_ifindex; // Link index  
    __u16 ndm_state;
    __u8 ndm_flags;
    __u8 ndm_type;
};
*/

namespace Es {

class NeighbourDiscoveryMsg :public NetlinkMsg<ndmsg> {
    
public:
  NeighbourDiscoveryMsg(void* msg)
  :NetlinkMsg<ndmsg>(msg, (size_t)NDA_MAX)
  {
  }

public:
  NeighbourDiscoveryMsg(struct ndmsg* msg)
  :NetlinkMsg<ndmsg>(msg, (size_t)NDA_MAX)
  {
  }
  
    
  struct rtattr* routingAttr()
  {
    return IFA_RTA(message);
  }
    

  void family(unsigned char family)
  {
    message -> ndm_family = family; 
  }
    
  unsigned char family()
  {
    return message -> ndm_family;
  }

    
  void ifindex(int index)
  {
    message -> ndm_ifindex = index;
  }
    
  int ifindex()
  {
    return message -> ndm_ifindex;
  }

  void flags(unsigned char flags)
  {
    message -> ndm_flags = flags;
  }
    
  unsigned char flags()
  {
    return message -> ndm_flags; 
  }

  void type(unsigned char t)
  {
    message -> ndm_type = t;
  }
    
  unsigned char type()
  {
    return message -> ndm_type; 
  }

public:
  void display()
  {
    printf("NDMSG family:   %s\n", getFamily(message -> ndm_family));
    printf("NDMSG flags:    %s\n", (const char*)getFlags(message -> ndm_flags));
    printf("NDMSG type:     %s\n", getType(message -> ndm_type) );
    printf("NDMSG state:    %s\n", getState(message -> ndm_state ));
    printf("NDMSG ifindex:  %d\n", message -> ndm_ifindex);

    displayAttributes();
  }

public:
  void getPropertyList(LinkedListT<Property>& list)
  {
    list.add(new Property("NDMSG family", getFamily(message -> ndm_family)) );
    list.add(new Property("NDMSG flags", (const char*)getFlags(message -> ndm_flags)) );
    list.add(new Property("NDMSG type",  getType(message -> ndm_type) ) );
    list.add(new Property("NDMSG state", getState(message -> ndm_state )) );
    list.add(new Property("NDMSG ifindex", message -> ndm_ifindex) );

 
    list.add(new Property("NDA_DST", getAttributeValue(NDA_DST)) );
    list.add(new Property("NDA_LLADDR", getAttributeValue(NDA_LLADDR) ) );
    list.add(new Property("NDA_CACHEINFO", getAttributeValue(NDA_CACHEINFO) ) );
  }
  
public:
  static const char* getState(int state)
  {
    static Pair<__u16, const char*> states[] = {
        {NUD_INCOMPLETE, "INCOMPLETE"},
        {NUD_REACHABLE,  "REACHABLE"},
        {NUD_STALE,  "STALE"},
        {NUD_DELAY,  "DELAY"},
        {NUD_PROBE,   "PROBE"},
        {NUD_FAILED,  "FAILED"},
        {NUD_NOARP,  "NOARP"},
        {NUD_PERMANENT,  "PERMANENT"},
        };
    KeyNamePairs<__u16> pairs(states, SizeOf(states));
    return pairs.match(state);    
  }

public:
  static const char* getType(unsigned short type)
  {
    return RoutingTableMsg::getType(type);
 
  }
  
public:
  CharString getFlags(unsigned int flags)
  {
    static Pair<unsigned int, const char*> ifFlags[] = {
        {NTF_USE,"USE"},
        {NTF_PROXY,"PROXY"},
        {NTF_ROUTER,"ROUTER"},
        {NTF_SELF,"SELF"},
        {NTF_MASTER,"MASTER"},
    };
    
    KeyNamePairs<unsigned int> pairs(ifFlags, SizeOf(ifFlags));
    return pairs.bitMatch(flags);    
  }

  
};

}

