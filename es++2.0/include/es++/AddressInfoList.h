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
 *  AddressInfoList.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/CharString.h>
#include <es++/AddressInfoHints.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
struct addrinfo {
  int              ai_flags;
  int              ai_family;
  int              ai_socktype;
  int              ai_protocol;
  socklen_t        ai_addrlen;
  struct sockaddr *ai_addr;
  char            *ai_canonname;
  struct addrinfo *ai_next;
};
 */

namespace Es {

class AddressInfoList :public Es::Object {
private:
  addrinfo* info;
  int       count;
  CharString name;
  CharString service;  

public:
  AddressInfoList(const char* node, const char* service, 
            const addrinfo* hints = NULL)
  :info(NULL),
  count(0),
  name(node),
  service(service)
  {
    /*
    Specifying hints as NULL is equivalent to setting ai_socktype and
        ai_protocol to 0; ai_family to AF_UNSPEC; and ai_flags to
        (AI_V4MAPPED | AI_ADDRCONFIG).
    */
    int rc = getaddrinfo(node, service, hints, &(this->info));
    if (rc < 0) {
      throw Es::IException("Failed to getaddrinfo node=%s, service=%s; error=%s", node, service, gai_strerror(rc) );  
    }

    //How many entries does the info contain? 
    count = 0;
    addrinfo* ptr = NULL;;
    for (ptr = this->info; ptr!=NULL; ptr=ptr->ai_next) {
      count++;
    }
  }

public:
  ~AddressInfoList() 
  {
    if (this->info) {
      freeaddrinfo(this->info);
      this->info = NULL;
    }
  }
  
  const addrinfo* getInfo() const 
  {
    return this->info;
  }

  int getCount() const
  {
    return this->count;
  }


  const addrinfo* getNth(int n)
  {
    int i = 0;
    addrinfo* ptr = NULL;
    if (n < 0 || n >= this->count) {
      throw Es::IException("Index is out of range");
    }
    for(ptr = this->info; ptr!= NULL; ptr=ptr->ai_next) {
      if (i == n) {
        break;
      }    
      i++;
    }
    return ptr;
  }  


public:
  void display()
  {
    printf("node =%s, service = %s \n", 
        (const char*)name, (const char*)service);

    int i = 0;
    addrinfo* ptr = NULL;;
    for (ptr = this->info; ptr!=NULL; ptr=ptr->ai_next) {
      printf("flags    [%d] = %d\n", i, ptr->ai_flags);
      printf("family   [%d] = %d\n", i, ptr->ai_family);
      printf("socktype [%d] = %d\n", i, ptr->ai_socktype);
      printf("protocol [%d] = %d\n", i, ptr->ai_protocol);
      printf("addrlen  [%d] = %d\n", i, ptr->ai_addrlen);
      /*
      printf("protocol = %d\n",  
               struct sockaddr *ai_addr;
      */
      printf("canonname[%d] = %s\n", i, ptr->ai_canonname);
      i++;  
    }
  }
};

}

