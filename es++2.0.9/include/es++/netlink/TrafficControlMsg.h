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
 *  TrafficControlMsg.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/StringBufferT.h>
#include <es++/CharString.h>
#include <es++/Pair.h>
#include <es++/KeyNamePairs.h>
#include <es++/Exception.h>
#include <es++/Attribute.h>

#include <es++/netlink/SocketNetlink.h>
#include <es++/netlink/SockAddrNetlink.h>
#include <es++/netlink/NetlinkMsg.h>

#include <linux/pkt_sched.h>

/*
RTM_NEWQDISC, RTM_DELQDISC, RTM_GETQDISC Add, 
remove or get a queueing discipline. The message contains a struct tcmsg 
and may be followed by a series of attributes. 
*/
/*
 Traffic control messages.

struct tcmsg {
    unsigned char   tcm_family;
    unsigned char   tcm__pad1;
    unsigned short  tcm__pad2;
    int     tcm_ifindex;
    __u32       tcm_handle;
    __u32       tcm_parent;
    __u32       tcm_info;
};
*/

namespace Es {

class TrafficControlMsg :public NetlinkMsg<tcmsg> {

public:
  TrafficControlMsg(void* msg)
  :NetlinkMsg<tcmsg>(msg, (size_t)TCA_MAX)
  {
  }

public:
  TrafficControlMsg(struct tcmsg* msg)
  :NetlinkMsg<tcmsg>(msg, (size_t)TCA_MAX)
  {
  }

  

  void family(unsigned char family)
  {
    message -> tcm_family = family; 
  }
    
  unsigned char family()
  {
    return message -> tcm_family;
  }
    
  void ifindex(int index)
  {
    message -> tcm_ifindex = index; 
  }
    
  int  ifindex()
  {
    return message -> tcm_ifindex;
  }

  void handle(__u32 h)
  {
    message -> tcm_handle = h; 
  }
    
  __u32  handle()
  {
    return message -> tcm_handle;
  }
    
  void parent(__u32 p)
  {
    message -> tcm_parent = p; 
  }


  __u32  parent()
  {
    return message -> tcm_parent;
  }
    

  void info(__u32 i)
  {
    message -> tcm_info = i; 
  }
    
  __u32  info()
  {
    return message -> tcm_info;
  }


public:
  void display()
  {
    printf("TCMSG family:  %s\n",   getFamily(message -> tcm_family));
    printf("TCMSG ifindex: %d\n",  message -> tcm_ifindex); //if_indextoname(message -> tcm_ifindex, name));
    printf("TCMSG handle:  %d\n",   message -> tcm_handle);
    printf("TCMSG parent:  %s\n",   getParent(message -> tcm_parent));
    printf("TCMSG info:    %d\n",   message -> tcm_info);

    displayAttributes();
  }


public:
  static const char* getParent(__u32 parent)
  {
    static Pair<__u32, const char*> parents[] = 
    {
      {TC_H_UNSPEC, "TC_H_UNSPEC"},
      {TC_H_ROOT,   "TC_H_ROOT"},
      {TC_H_INGRESS, "TC_H_INGRESS"},
    };
  	KeyNamePairs<__u32> pairs(parents, SizeOf(parents));
  	return pairs.match(parent);
  }

};

}

