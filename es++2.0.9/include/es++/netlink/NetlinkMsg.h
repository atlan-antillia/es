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
 *  NetlinkMsg.h
 *
 *****************************************************************************/

// 2015/01/17 Modified

#pragma once

#include <es++/Object.h>
#include <es++/StringBufferT.h>
#include <es++/CharString.h>
#include <es++/Pair.h>
#include <es++/KeyNamePairs.h>

#include <es++/Socket.h>
#include <es++/Exception.h>
#include <es++/Attribute.h>

#include <es++/netlink/SocketNetlink.h>
#include <es++/netlink/SockAddrNetlink.h>

#include <linux/rtnetlink.h>
#include <linux/if_addr.h>
#include <asm/types.h>
#include <linux/rtnetlink.h>
#include <linux/if_arp.h>
#include <linux/if_addr.h>


namespace Es {

template<typename T>
    
class NetlinkMsg :public Object {
protected:
  T* message;
    
  size_t   attrSize;
    
  Attribute** attributes;

public:
  NetlinkMsg(void* msg, size_t size)
  :message((T*)msg),
  attrSize(size+1), //2015/01/17
  attributes(NULL)
  {
    if (msg == NULL || attrSize <= 0) {
        throw IException("Invalid argument");    
    }
    attributes = new Attribute*[attrSize];
    for(size_t i = 0;  i< attrSize; i++) {
        attributes[i] = NULL;
    }
  }

public:
  NetlinkMsg(T* msg, size_t size)
  :message(msg),
  attrSize(size+1), //2015/01/17
  attributes(NULL)
  {
    if (msg == NULL || attrSize <=0) {
        throw IException("Invalid argument");    
    }
    attributes = new Attribute*[attrSize];

    for(size_t i = 0;  i< attrSize; i++) {
        attributes[i] = NULL;
    }    
  }
    
public:
  ~NetlinkMsg()
  {
    for(size_t i = 0;  i< attrSize; i++) {
      if (attributes[i] != NULL) {
         delete attributes[i];
      }
    }
    delete [] attributes;  //2015/01/17
    attributes = NULL;
  }  
    
  virtual  void setAttribute(size_t index, Attribute* attr)
  {
    if (index >= 0 && index < attrSize) {
        attributes[index] = attr;
    } else {
        throw IException("Invalid attribute index %d", index);
    }
  }
    
  virtual  Attribute* getAttribute(size_t index)
  {
    Attribute* attr = NULL;
    if (index >= 0 && index < attrSize) {
        attr = attributes[index];
    } else {
        throw IException("Invalid attribute index %d", index);       
    }
    return attr;
  }
    
  virtual int size()
  {
    return sizeof(T);
  }
    
  virtual struct rtattr* routingAttr()
  {
    return NULL;
  }

  virtual void displayAttributes()
  {
     for(size_t i = 0;  i< attrSize; i++) {
        if (attributes[i] != NULL) {
            attributes[i] -> display();
        }
    }
  }
  
  //2015/01/17
  virtual void displayAttribute(size_t index)
  {
     if ( index >= 0 && index < attrSize) {
      if (attributes[index] != NULL) {
        attributes[index] -> display();
      }
     } else {
       throw IException("Invalid index (%d)", index);  
     }
  }
 
  //2015/03/28
  virtual CharString getAttributeValue(size_t index)
  {
    const char* value = "";
    if (index >= 0 && index <attrSize) {
      if (attributes[index] != NULL) {
        return attributes[index]->getValue();
      }
    }
    return CharString(value);
  }

public:
  const char* getFamily(unsigned char family)
  {
    static Pair<int, const char*> families[] = {
    {AF_UNIX,     "AF_UNIX"},
    {AF_INET,     "AF_INET"},
    {AF_INET6,    "AF_INET6"},
    {AF_IPX,      "AF_IPX"},
    {AF_NETLINK,  "AF_NETLINK"},
    {AF_X25,      "AF_X25"},
    {AF_AX25,     "AF_AX25"},
    {AF_ATMPVC,   "AF_ATMPVC"},
    {AF_APPLETALK,"AF_APPLETALK"},
    {AF_PACKET,   "AF_PACKET"},
    {AF_UNSPEC,   "AF_UNSPEC"},
    };
    KeyNamePairs<int> pairs(families, SizeOf(families));
    return pairs.match(family); 
  }
};

}

