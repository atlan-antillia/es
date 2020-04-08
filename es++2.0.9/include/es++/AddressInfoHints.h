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
 *  AddressInfoHints.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/Socket.h>

/*
struct addrinfo {
  int     ai_flags;
  int     ai_family;
  int     ai_socktype;
  int     ai_protocol;
  size_t  ai_addrlen;
  struct sockaddr *ai_addr;
  char   *ai_canonname;
  struct addrinfo *ai_next;
};
 */

namespace Es {

enum AI_FLAGS {
  PASSIVE                  = AI_PASSIVE,
  CANONNAME                = AI_CANONNAME,
  NUMERICHOST              = AI_NUMERICHOST,
  NUMERICSERV              = AI_NUMERICSERV,
  V4MAPPED                 = AI_V4MAPPED,
  ALL                      = AI_ALL,
  ADDRCONFIG               = AI_ADDRCONFIG, 

  IDN                      = AI_IDN,
  CANONIDN                 = AI_CANONIDN,
  IDN_ALLOW_UNASSIGNED     = AI_IDN_ALLOW_UNASSIGNED,
  IDN_USE_STD3_ASCII_RULES = AI_IDN_USE_STD3_ASCII_RULES
};


class AddressInfoHints :public Es::Object {
private:
  struct addrinfo hints;

public:
  AddressInfoHints()
  {
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = UNSPEC;
    hints.ai_socktype = STREAM;
    hints.ai_flags    = (V4MAPPED | ADDRCONFIG);
    hints.ai_protocol = 0;
  }

public:
  AddressInfoHints(ADDRESS_FAMILY family, 
        SOCKET_TYPE sockType, 
        int flags =  (V4MAPPED | ADDRCONFIG),
        int protocol = 0 /*Any protocol */ )
  {
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = family;   
    hints.ai_socktype = sockType; 
    hints.ai_flags    = flags;
    hints.ai_protocol = protocol;
  }

  void setFamily(ADDRESS_FAMILY family)
  {
    hints.ai_family = family;
  }

  void setSocketType(SOCKET_TYPE type)
  {
    hints.ai_socktype = type;
  }

  void setFlags(int flags)
  {
    hints.ai_flags = flags;
  }

  void setProtocol(int protocol)
  {
    hints.ai_protocol = protocol;
  }

  operator const struct addrinfo*() const
  {
    return &hints;
  }
};

}

