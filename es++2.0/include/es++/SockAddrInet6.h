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
 *  SockAddrInet6.h
 *
 *****************************************************************************/


#pragma once

#include <es++/SockAddr.h>
#include <es++/Inet6Addr.h>

/*
struct sockaddr_in6 {
  sa_family_t     sin6_family;   // AF_INET6
  in_port_t       sin6_port;     // port number
  uint32_t        sin6_flowinfo; // IPv6 flow information 
  struct in6_addr sin6_addr;     // IPv6 address 
  uint32_t        sin6_scope_id; // Scope ID (new in 2.4) 
};

struct in6_addr {
  unsigned char   s6_addr[16];   // IPv6 address 
};
*/

namespace Es {

class SockAddrInet6 :public Es::SockAddr {
private:
  sockaddr_in6  address;
  CharString    name;

public:
  /**
   *
   */
  SockAddrInet6(sockaddr* addr=NULL) 
  {
    memset(&address, 0, sizeof(address));

    if (addr) {
      if (addr->sa_family == AF_INET6) {
        memcpy(&address, addr, sizeof(address));
      } else {
        throw Es::IException("Invalid argument");
      }
    }  
  }

public:
  SockAddrInet6(unsigned short port) 
  {
    memset(&address, 0, sizeof(address));

    address.sin6_family = AF_INET6;
    address.sin6_addr = in6addr_any;  

    address.sin6_port = htons(port);
  }


public:
  // The port and addr must be in host-byte-order.
  SockAddrInet6(unsigned short port, const Es::CharString& addr) 
  {
    Inet6Addr inet6(addr);
    
    memset(&address, 0, sizeof(address));

    address.sin6_family = AF_INET6;
    address.sin6_port = htons(port);         //Convert the port to the network byte-order
    address.sin6_addr = inet6.getAddress();  
  }

public:
  // The port and addr must be in host-byte-order.
  SockAddrInet6(unsigned short port, Es::Inet6Addr addr) 
  {
    memset(&address, 0, sizeof(address));

    address.sin6_family = AF_INET6;
    address.sin6_port = htons(port);         //Convert the port to the network byte-order
    address.sin6_addr = addr.getAddress();  
  }

public:
  // The port and addr must be in host-byte-order.
  SockAddrInet6(unsigned short port, in6_addr addr) 
  {
    memset(&address, 0, sizeof(address));

    address.sin6_family = AF_INET6;
    address.sin6_port = htons(port);         //Convert the port to the network byte-order
    address.sin6_addr = addr;  
  }

  in6_addr getHostAddress() 
  {
      return address.sin6_addr;
  }

  const char*     toString() 
  {
    char string[INET6_ADDRSTRLEN];
    if (::inet_ntop(AF_INET6, &address.sin6_addr, 
      string, sizeof(string))==NULL) {
      throw Es::IException("Failed to inet_ntop; %s", error() );
    } else {
      name = string;
      return (const char*)name;
    }
  }

  unsigned short  getPort() 
  {
    return address.sin6_port;
  }

  void  setAddress(in6_addr addr) 
  {
    address.sin6_addr = addr;
  }

  void  setPort(unsigned short port) 
  {
    address.sin6_port = htons(port);
  }


public:
  virtual sockaddr* getAddress() 
  {
    return (sockaddr*)&address;
  }

  virtual socklen_t getSize() 
  {
    return sizeof(address);
  } 

  virtual sa_family_t getFamily()
  {
    return address.sin6_family;
  }

public:
  void display()
  {
    printf("Port  %d\n", address.sin6_port);
    const char* name = toString();
    printf("ipv6 inet_addr = %s\n", name);
  }

};

}
  
