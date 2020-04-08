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
 *  LinkLayerMsg.h
 *
 *****************************************************************************/

// 2015/01/17 Modified to inherit NetlinkMsg<T> class.

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

#include <es++/LinkedList.h>
#include <es++/Property.h>

/* struct ifinfomsg
 * passes link level specific information, not dependent
 * on network protocol.
 */
/*
From rtnetlink.h
struct ifinfomsg {
  unsigned char   ifi_family;
  unsigned char   __ifi_pad;
  unsigned short  ifi_type;       // ARPHRD_* 
  int     ifi_index;      // Link index   
  unsigned    ifi_flags;      // IFF_* flags  
  unsigned    ifi_change;     // IFF_* change mask 
};
*/


namespace Es {

class LinkLayerMsg :public NetlinkMsg<ifinfomsg> {
 
public:
  LinkLayerMsg(void* msg)
  :NetlinkMsg<ifinfomsg>(msg, (size_t)IFLA_MAX)
  {
  }

public:
  LinkLayerMsg(struct ifinfomsg* msg)
  :NetlinkMsg<ifinfomsg>(msg, (size_t)IFLA_MAX)
  {
  }
   
  struct rtattr* routingAttr()
  {
    return IFLA_RTA(message);

  }
  
  int size() 
  {
    return sizeof(struct ifinfomsg);
  }
  
  void family(unsigned char family)
  {
    message -> ifi_family = family; 
  }
  
  unsigned char family()
  {
    return message -> ifi_family;
  }

  void type(unsigned short type)
  {
    message -> ifi_type= type; 
  }

  unsigned short type()
  {
    return message -> ifi_type; 
  }
  
  void index(int index)
  {
    message -> ifi_index = index;
  }
  
  int index()
  {
    return message -> ifi_index;
  }
  
  void flags(unsigned int flags)
  {
    message -> ifi_flags = flags;
  }
  unsigned int flags()
  {
    return message -> ifi_flags; 
  }

  void change(unsigned int change)
  {
    message -> ifi_change = change;
  }
    
  unsigned int change()
  {
    return message -> ifi_change; 
  }
    
  void displayAttributes()
  {
    for(int i = 0;  i<= IFLA_MAX; i++) {
      if (attributes[i] != NULL) {
        attributes[i] -> display();
      }
    }
  }

  void displayAttributesMin()
  {
    displayAttribute(IFLA_IFNAME);
    displayAttribute(IFLA_ADDRESS);
    displayAttribute(IFLA_BROADCAST);
  }

public:
  void displayMin()
  {
    printf("IFINFO family: %s\n", getFamily(message -> ifi_family));
    printf("IFINFO type:   %s\n", getType(message -> ifi_type));
    printf("IFINFO index:  %d\n", message -> ifi_index);
    printf("IFINFO flags:  %s\n", (const char*)getFlags(message -> ifi_flags));
    printf("IFINFO change: %x\n", message -> ifi_change);
    
    displayAttributesMin();
  }
  
public:
  void getMinPropertyList(LinkedListT<Property>& list)
  {
    list.add(new Property("IFINFO family", getFamily(message -> ifi_family)) );
    list.add(new Property("IFINFO type",   getType(message -> ifi_type)) );
    
    list.add(new Property("IFINFO index",  message -> ifi_index) );
    list.add(new Property("IFINFO flags",  (const char*)getFlags(message -> ifi_flags) ) );
    list.add(new Property("IFINFO change", message -> ifi_change) );
    
    Attribute* attr = NULL;
    attr = attributes[IFLA_IFNAME];
    list.add(new Property("IFLA ifname", attr->getValue()) );
    attr = attributes[IFLA_ADDRESS];
    list.add(new Property("IFLA address", attr->getValue()) );
      
    attr = attributes[IFLA_BROADCAST];
    list.add(new Property("IFLA broadcast", attr->getValue()) );
      
  
//    displayAttributesMin();
  }

public:
  void display()
  {
    printf("IFINFO family: %s\n", getFamily(message -> ifi_family));
    printf("IFINFO type:   %s\n", getType(message -> ifi_type));
    printf("IFINFO index:  %d\n", message -> ifi_index);
    printf("IFINFO flags:  %s\n", (const char*)getFlags(message -> ifi_flags));
    printf("IFINFO change: %x\n", message -> ifi_change);
    
    displayAttributes();
  }


public:
  static const char* getType(unsigned short type)
  {
    static Pair<unsigned short, const char*> types[] =
    {
      {ARPHRD_NETROM, "ARPHRD_NETROM"},   // From KA9Q: NET/ROM pseudo.
      {ARPHRD_ETHER,  "ARPHRD_ETHER"},   // Ethernet 10/100Mbps.  
      {ARPHRD_EETHER, "ARPHRD_EETHER"},   // Experimental Ethernet. 
      {ARPHRD_AX25,   "ARPHRD_AX25"},   // AX.25 Level 2. 
      {ARPHRD_PRONET, "ARPHRD_PRONET"},  // PROnet token ring. 
      {ARPHRD_CHAOS,  "ARPHRD_CHAOS"},    // Chaosnet. 
      {ARPHRD_IEEE802,"ARPHRD_IEEE802"},    // IEEE 802.2 Ethernet/TR/TB.
      {ARPHRD_ARCNET, "ARPHRD_ARCNET"},     // ARCnet. 
      {ARPHRD_APPLETLK,"ARPHRD_APPLETLK"},  // APPLEtalk. 
      {ARPHRD_DLCI,   "ARPHRD_DLCI"},     // Frame Relay DLCI. 
      {ARPHRD_ATM,  "ARPHRD_ATM"},      // ATM.  
      {ARPHRD_METRICOM, "ARPHRD_METRICOM"},   // Metricom STRIP (new IANA id). 
      {ARPHRD_IEEE1394, "ARPHRD_IEEE1394"},   // IEEE 1394 IPv4 - RFC 2734.
      {ARPHRD_EUI64,  "ARPHRD_EUI64"},    // EUI-64.  
      {ARPHRD_INFINIBAND, "ARPHRD_INFINIBAND"},// InfiniBand.   
    };
    KeyNamePairs<unsigned short> pairs(types, SizeOf(types));
    return pairs.match(type);
  }
  
public:
  static CharString getFlags(unsigned int flags)
  {
    static Pair<unsigned int, const char*> ifFlags[] = {
      {IFF_UP,        "UP"},
      {IFF_BROADCAST,     "BROADCAST"},
      {IFF_DEBUG,       "DEBUG"},
      {IFF_LOOPBACK,      "LOOPBACK"},
      {IFF_POINTOPOINT,     "POINTOPOINT"},
      {IFF_NOTRAILERS,    "NOTRAILERS"},
      {IFF_RUNNING,       "RUNNING"},
      {IFF_NOARP,       "NOARP"},
      {IFF_PROMISC,       "PROMISC"},
      {IFF_MASTER,      "MASTER"},
      {IFF_SLAVE,       "SLAVE"},
      {IFF_MULTICAST,     "MULTICAST"},
      {IFF_PORTSEL,       "PORTSEL"},
      {IFF_AUTOMEDIA,     "AUTOMEDIA"},
      {IFF_DYNAMIC,       "DYNAMIC"},
      {IFF_LOWER_UP,      "LOWER_UP"},
      {IFF_DORMANT,       "DORMANT"},
      {IFF_ECHO,        "ECHO"},
    };
    KeyNamePairs<unsigned int> pairs(ifFlags, SizeOf(ifFlags));
    return pairs.bitMatch(flags);

  }};

}

