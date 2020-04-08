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
 *  SocketDatagram.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Socket.h>

namespace Es {

class SocketDatagram :public Es::Socket {

public:
  /**
   *
   */
  SocketDatagram(ADDRESS_FAMILY family=INET) 
    :Es::Socket(family, DATAGRAM, IPPROTO_UDP)
  {
  }

  SocketDatagram(const addrinfo& info)
    :Es::Socket(info)
  {
  }

  SocketDatagram(const addrinfo* info)
    :Es::Socket(info)
  {
  }

public:
  /**
   * Given socket from caller, and simply set it to a member of this class
   */
  SocketDatagram(int soc) 
    :Es::Socket()
  {
    setSocket(soc);
  }

public:
  /**
   *
   */
  ~SocketDatagram() 
  {
  }

  
public:
  /**
   *
   */
  int enableBroadcast() {
    int enable = 1;
    return setOption(SO_BROADCAST, to_voidptr(enable), sizeof(enable));
  }

public:
  /**
   *
   */
  int sendTo(SockAddrInet& to, const char* buf, int len, int flag=0) {
    int rc = 0;
    if (buf && len>0) {

      int soc = getSocket();
      rc = ::sendto(soc, buf, len, flag, to.getAddress(), to.getSize());
    }
    return rc;  
  }
  

public:
  int recvFrom(SockAddrInet& from, char* buf, int len, int flag=0) {
    int rc =0;
    if (buf && len>0) {
      int soc = getSocket();
      
      socklen_t fromLen= from.getSize();
      rc = ::recvfrom(soc, buf, len, flag, 
        from.getAddress(), &fromLen);

      buf[len-1] = '\0';
      if (rc >0 && rc<len) {
        buf[rc] = '\0';  //Terminate by NULL.
      }
    }
    return rc;
  }


public:
  int recvFrom(char* buf, int len, int flag=0) {
    int rc =0;
    if (buf && len >0) {
      int soc = getSocket();
      rc = ::recvfrom(soc,  buf, len, flag, NULL, NULL);
      buf[len-1] = '\0';
      if (rc >0 && rc<len) {
        buf[rc] = '\0';  //Terminate by NULL.
      }
    }
    return rc;
  }

public:
  int replyTo(SockAddrInet& to, const char* buf, int len, int flag=0) {
    int rc = 0;
    if (buf && len>0) {

      int soc = getSocket();
      rc = ::sendto(soc, buf, len, flag, 
        to.getAddress(), to.getSize());
    }
    return rc;  
  }

public:
  int replyTo(const char* buf, int len, int flag=0) {
    int rc = 0;
    if (buf && len>0) {

      int soc = getSocket();
      rc = ::sendto(soc, buf, len, flag, 
        NULL, 0);
    }
    return rc;  
  }

};

}


