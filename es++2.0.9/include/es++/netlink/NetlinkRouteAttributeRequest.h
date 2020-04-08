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
 *  NetlinkRouteAttributeRequest.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/netlink/NetlinkMsgHeader.h>
#include <es++/netlink/RouteAttributeRequest.h>


namespace Es {

template <typename T>

class NetlinkRouteAttributeRequest :public Object {  
private:
  RouteAttributeRequest<T> rta_request;
  size_t buffer_size;
  size_t buffer_pos;

public:
  NetlinkRouteAttributeRequest()
  :buffer_size(sizeof(rta_request.attribute)),
   buffer_pos(0)
  {
    bzero(&rta_request, sizeof(rta_request));
  }
  
public:
  struct nlmsghdr* head()
  {
    return &rta_request.head;
  }
  
  T* body()
  {
    return &rta_request.body;
  }
  
  RouteAttributeRequest<T>& request()
  {
    return this -> rta_request;
  }
  
  size_t length()
  {
    return this -> rta_request.head.nlmsg_len;
  }
 
  char*  buffer(size_t size)
  {
    char* buff = NULL;
    if ((this -> buffer_pos + size) < this -> buffer_size) {
      buff  =  this -> rta_request.attribute;
      buff +=  this -> buffer_pos;
    }
    return buff;
  }
  
  
  bool addAttribute(unsigned short type, void* data, unsigned short data_size)
  {
    bool rc = false;

    struct rtattr attr;
    memset(&attr, 0, sizeof(attr));
    attr.rta_type = type;
    //2015/01/02attr.rta_len  = RTA_LENGTH(data_size);
    attr.rta_len  = RTA_SPACE(data_size);
    
    int buffsize = attr.rta_len; 
    
    char* buff = buffer(buffsize);
    if (buff) {
        memcpy(buff, &attr, sizeof(attr));
        memcpy(buff + sizeof(attr), data, data_size);
        this -> buffer_pos += buffsize;
        
        this -> rta_request.head.nlmsg_len += buffsize;
        rc = true;
    }
    return rc;
  }
 
};

}

