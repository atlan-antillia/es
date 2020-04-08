/******************************************************************************
 *
 * Copyright (c) 2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  Matches.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <es++/StringT.h>
#include <es++/SockAddr.h>

namespace Es {

class NameInfo: public Object {

private:
  StringT<char> node;	//host-name
  StringT<char> service;
 
public:
  NameInfo(SockAddr& sockAddr, int flags=NI_NUMERICSERV)
  :node(""),
   service("")
  {
    //The getnameinfo() function shall translate a socket address to a node name 
    //and service location, all of which are defined as in getaddrinfo().

    char hname[NI_MAXHOST];
    char sname[NI_MAXSERV];
    memset(hname, 0, sizeof(hname));
    memset(sname, 0, sizeof(sname));
    struct sockaddr* sa = sockAddr.getAddress();
    socklen_t     salen = sockAddr.getSize();
 
    int rc = getnameinfo(sa, salen,
        hname, sizeof(hname), sname, sizeof(sname),
        flags);
    if (rc == 0) {
      node    = hname;
      service = sname;
    } else {
      throw Es::IException("Failed to getnameinfo: %s", gai_strerror(rc))
    }
  }

public:
  NameInfo(const struct sockaddr& sa, socklen_t salen, int flags=NI_NUMERICSERV)
  :node(""),
   service("")
  {
    char hname[NI_MAXHOST];
    char sname[NI_MAXSERV];
    memset(hname, 0, sizeof(hname));
    memset(sname, 0, sizeof(sname));
    int rc = getnameinfo(&sa, salen,
        hname, sizeof(hname), sname, sizeof(sname),
        flags);
    if (rc == 0) {
      node    = hname;
      service = sname;
    } else {
      throw Es::IException("Failed to getnameinfo: %s", gai_strerror(rc))
    }
  }

  const char* nodeName()
  {
    return (const char*)node;
  }
  const char* serviceName()
  {
    return (const char*)service;
  }

  virtual void display()
  {
    printf("NodeName   = %s\n", nodeName());
    printf("ServiceName= %s\n", serviceName());
  }
};

}

