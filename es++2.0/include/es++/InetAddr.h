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
 *  InetAddr.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/CharString.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <sys/un.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


namespace Es {

class InetAddr :public Es::Object {

private:
  in_addr     address;
  Es::CharString  string;

public:
  InetAddr(const in_addr addr)
  :address(addr),
  string("")
  {
    string = toCharString(address);
  }

public:
  InetAddr(const Es::CharString& str)
  :string(str)
  {
    int rc = inet_pton(AF_INET, (const char*)string, (void*)&address);
    if (rc == -1) {
      throw Es::IException("Faild in inet_pton; %s", error());
    }
  }

public:
  InetAddr(const char* str)
  :string(str)
  {
    int rc = inet_pton(AF_INET, (const char*)string, (void*)&address);
    if (rc == -1) {
      throw Es::IException("Faild in inet_pton: %s", error());
    }
  }
  
public:
  in_addr getAddress()
  {
    return address;
  }
  
  const char* toString() {
    return (const char*)string;
  }

  static Es::CharString toCharString(in_addr& inaddr)
  {
    char buffer[INET_ADDRSTRLEN + 1];
    memset(buffer, 0, sizeof(buffer)); 
    const char* ptr = inet_ntop(AF_INET,  &inaddr,
                buffer, sizeof(buffer));
    if (ptr) {
      return CharString(buffer); 
    } else {
      throw Es::IException("Failed to inet_ntop %s", strerror(errno));
    }
  } 
};

}

