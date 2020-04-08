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
 *  RouteAttribute.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/StringBufferT.h>
#include <es++/CharString.h>
#include <es++/Pair.h>
#include <es++/Socket.h>
#include <es++/Exception.h>
#include <es++/InetAddr.h>
#include <es++/Inet6Addr.h>
#include <es++/Bytes.h>
#include <es++/File.h>
#include <es++/netlink/SocketNetlink.h>
#include <es++/netlink/SockAddrNetlink.h>
#include <es++/netlink/NetlinkMsgHeader.h>


/*
struct rtattr {
  unsigned short rta_len;    // Length of option
  unsigned short rta_type;   // Type of option 
  // Data follows 
};
*/

namespace Es {

template <typename T>
	
class RouteAttribute :public Object {
protected:
  struct rtattr* attribute;
  int    attrlen;
  T*     msg;	
    
public:
  RouteAttribute<T>(NetlinkMsgHeader& header)
  :attribute(NULL),
  attrlen(0),
  msg(NULL)
	
  {
    this -> msg = (T*)header.data();
    if (msg) {
      //this -> attribute = FOO_RTA(msg); 
      this -> attribute = ((struct rtattr*)(((char*)(msg)) + NLMSG_ALIGN(sizeof(T))));

      this -> attrlen = header.attributeLength(sizeof(T) );
      if (this ->attribute == NULL || this -> attrlen <= 0) {
        throw IException("Message has no attribute");
      }
    }
  }

  int type()
  {
    return attribute ->rta_type;        
  }

  void* data()
  {
    return RTA_DATA(attribute);     
  }
    
  int payload()
  {
    return RTA_PAYLOAD(attribute);
  }
    
  bool ok()
  {
    return RTA_OK(attribute, attrlen);
  }
    
  bool next()
  {
    bool rc = false;
    int len = attrlen;
    this -> attribute = RTA_NEXT(this->attribute, len);
    this -> attrlen = len;
    if (ok() ) {
        rc = true;  
    }
    return rc;
  }
	
public:
  virtual void parse()
  {
  	//Please define your own parse method in the subclass of this class.
  } 

};

}

