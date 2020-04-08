/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *  notice, this list of conditions, and the following disclaimer.
 *  
 * 2. The name of the author may not be used to endorse or promote products
 *  derived from this software without specific prior written permission.
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
 *  SockAddrNetlink.h
 *
 *****************************************************************************/


#pragma once

#include <es++/SockAddr.h>

/*
struct sockaddr_nl {
  sa_family_t   nl_family;  // AF_NETLINK
  unsigned short  nl_pad;   // Zero.
    pid_t       nl_pid;   // Port ID
  __u32       nl_groups;  // Multicast groups mask
};

For more detail, see: http://linux.die.net/man/7/netlink
*/

namespace Es {

class SockAddrNetlink :public SockAddr {
private:
  sockaddr_nl  address;
  CharString  name;

public:
  /**
   * Constructor
   */
  SockAddrNetlink(const sockaddr* addr=NULL) 
  {
  memset(&address, 0, sizeof(address));
  address.nl_family = AF_NETLINK;
  if (addr) {
    if (addr->sa_family == AF_NETLINK) {
    memcpy(&address, addr, sizeof(address));
    } else {
    throw IException("Invalid argument");
    }
  }  
  }

  virtual struct sockaddr* getAddress()
  {
    return (sockaddr*)&address;
  }
    
  virtual socklen_t getSize()
  {
    return sizeof(address);
  }
    
  sa_family_t getFamily()
  {
   return address.nl_family;
  }

  pid_t  getPid() 
  {
  return address.nl_pid;
  }
    
  __u32   getGroups()
  {
  return address.nl_groups;  // Multicast groups mask
  }

  void setFamily(sa_family_t family )
  {
   address.nl_family = family;
  }

  void setPid(pid_t pid) 
  {
  address.nl_pid = pid;
  }
    
  void setGroups(__u32 groups)
  {
  address.nl_groups = groups;  // Multicast groups mask
  }

public:
  void display()
  {
    printf("nelink nl_family = %d\n", getFamily());
    printf("nelink nl_pid = %d\n",  getPid());
    
    printf("nelink nl_groups = %d\n", getGroups());
  }

};

}
  
