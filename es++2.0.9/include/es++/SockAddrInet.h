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
 *  SockAddrInet.h
 *
 *****************************************************************************/

#pragma once

#include <es++/SockAddr.h>

/*
struct sockaddr_in {
  sa_family_t    sin_family; // address family: AF_INET 
  in_port_t      sin_port;   // port in network byte order 
  struct in_addr sin_addr;   // internet address 
};

struct in_addr {
  uint32_t       s_addr;     // address in network byte order
};
*/

namespace Es {

class SockAddrInet :public Es::SockAddr {
private:
  sockaddr_in   address;
  CharString    name;

public:
  /**
   *
   */
  SockAddrInet(sockaddr* addr=NULL) 
  {
    memset(&address, 0, sizeof(address));

    if (addr) {
      if (addr->sa_family == AF_INET) {
        memcpy(&address, addr, sizeof(address));
      } else {
        throw Es::IException("Invalid argument");
      }
    }  
  }

public:
  /**
   * Given port(in host byte order) only as an argument, address is INADDR_ANY.
   */
  SockAddrInet(unsigned short port) 
  {
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);  
    //INADDR_ANY is really a 0x00000000, so htonl(INADDR_ANY)=INADDR_ANY

    address.sin_port = htons(port);
  }

public:
  /**
   * @param host  Internet host name
   * @param port  Port number(in host byte order)
   */
  SockAddrInet(const char* host, unsigned short port) 
  {
    assert(host);

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    unsigned long addr = inet_addr(host);
    if(addr == INADDR_NONE) {
      hostent* hostEnt = gethostbyname(host);
      if(hostEnt == NULL) {
        throw Es::IException("Failed to gethostbyname; host= %s", host); 
      }
      address.sin_addr = *((in_addr* )*hostEnt->h_addr_list);
    }
    address.sin_port = htons(port);
  }

public:
  // The port and addr must be in host-byte-order.
  SockAddrInet(unsigned short port, unsigned long addr) 
  {
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_port = htons(port);         //Convert the port to the network byte-order
    address.sin_addr.s_addr = htonl(addr);  //Convert the addr to the network-byte-order.
  }

  unsigned long getHostAddress() 
  {
    return address.sin_addr.s_addr;
  }

  unsigned short  getPort() 
  {
    return address.sin_port;
  }

  void  setAddress(unsigned long addr) 
  {
    address.sin_addr.s_addr = addr;
  }
  void  setPort(unsigned short port) 
  {
    address.sin_port = port;
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
    return address.sin_family;
  }

public:
  virtual  const char* toString()
  {
    char string[256];
    //The following will return the string of the format "ddd.ddd.ddd.ddd".
    if (::inet_ntop(AF_INET,
        &address.sin_addr,
             string, sizeof(string) ) == NULL) {
      throw Es::IException("Failed to inet_ntop; %s", error() );
    } else {
      name = string;
      return (const char*)name;
    }
  }

public:
  void display()
  {
    printf("Port  %d\n", address.sin_port); 
    const char* name = toString();
    printf("ipv4 inet addr = %s\n", (const char*)name);
  }
};

}

