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
 *  NetlinkMsgHeader.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <es++/StringBufferT.h>
#include <es++/Pair.h>
#include <es++/CharString.h>
#include <es++/File.h>

#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/if_arp.h>
#include <asm/types.h>

/*
struct nlmsghdr
{
    __u32     nlmsg_len; // Length of message including header
    __u16     nlmsg_type; // Message content
    __u16     nlmsg_flags;    // Additional flags
    __u32     nlmsg_seq; // Sequence number
    __u32     nlmsg_pid; // Sending process port ID 
};
*/

namespace Es {
 
class NetlinkMsgHeader :public Object {
private:
  struct nlmsghdr* header;
  int    size;

public:
  NetlinkMsgHeader(struct nlmsghdr* hdr)
  :header(hdr),
   size(0)
  {
    if (hdr) {
        size = hdr->nlmsg_len;
    } else {
      throw IException("Invalid argument");
    }   
  }
  
public:
  NetlinkMsgHeader(struct nlmsghdr& hdr)
  :header(&hdr),
   size(hdr.nlmsg_len)
  {
    if (size <=0) {
      throw IException("Invalid argument");
    }   
  }

  NetlinkMsgHeader(const char* buffer, int len)
  :header((struct nlmsghdr*)buffer),
   size(len)
  {
    if (buffer == NULL || len == 0) {
      throw IException("Invalid arguments");
    }
  }

  struct nlmsghdr* getMsgHeader()
  {
    return this->header;
  }
    
  bool isDone()
  {
    bool rc = false;
    if (header -> nlmsg_type == NLMSG_DONE) {
      rc = true;
    }
    return rc;
  }

  bool isError()
  {
    bool rc = false;
    if (header -> nlmsg_type == NLMSG_ERROR) {
      rc = true;
    }
    return rc;
  }

  __u16  type()
  {   
    return header ->nlmsg_type; 
  }
    
  void type(__u16  type)
  {   
    header ->nlmsg_type = type; 
  }


  int align()
  {
    return NLMSG_ALIGN(size);
  }

  int length()
  {
    return NLMSG_LENGTH(size);
  }

  int attributeLength(size_t msgSize )
  {
    return header ->nlmsg_len - NLMSG_LENGTH(msgSize);
  }
    
  
  void length(__u32  len)
  {
    header ->nlmsg_len =len; // Length of message including header
  }

  __u16 flags()
  {
    return header -> nlmsg_flags;
  }

  void flags(__u16  flags)
  {
    header -> nlmsg_flags = flags;  // Additional flags
  }

  __u32 seq()
  {
    return header -> nlmsg_seq; 
  }

  void seq(__u32 seq)
  {
    header -> nlmsg_seq = seq; // Sequence number
  }

  __u32 pid()
  {
    return header ->nlmsg_pid;  
  }


  void pid(__u32  pid)
  {
    header ->nlmsg_pid = pid; // Sending process port ID 
  }
    
  int space()
  {
    return NLMSG_SPACE(size);
  }

    
  void* data()
  {
     return NLMSG_DATA(header);
  }

  //2015/01/01
  struct nlmsgerr* errorMsg()
  {
    struct nlmsgerr* err = NULL;
    if (header -> nlmsg_type == NLMSG_ERROR) {
      err = (struct nlmsgerr*)NLMSG_DATA(header);
    }
    return err;
  }

  bool next()
  {
    bool rc = false;
    struct nlmsghdr* hdr = NLMSG_NEXT(this->header, size); 
    if (hdr != NULL && size >0) {
      this -> header = hdr;
      if (ok() && isDone() == false) {
        rc = true;
      }
    }
    return rc;
  }

  __u32 ok()
  {
   // size_t len = size;
    return NLMSG_OK(header, (__u32)size);
  }

  int payload()
  {
    return NLMSG_PAYLOAD(header,  size);
  }

public:
  //2015/01/01 
  static CharString getFlags(__u16 flags)
  {
    static Pair< __u16 , const char*> hdrFlags[] = 
    {
      {NLM_F_REQUEST,"NLM_F_REQUEST"}, 
      {NLM_F_MULTI,"NLM_F_MULTI"}, 
      {NLM_F_ACK,"NLM_F_ACK"}, 
      {NLM_F_ECHO,"NLM_F_ECHO"}, 
      {NLM_F_DUMP_INTR,"NLM_F_DUMP_INTR"}, 
      {NLM_F_ROOT,"NLM_F_ROOT"}, 
      {NLM_F_MATCH,"NLM_F_MATCH"}, 
      {NLM_F_ATOMIC,"NLM_F_ATOMIC"}, 
      {NLM_F_DUMP,"NLM_F_DUMP"}, 
      {NLM_F_REPLACE,"NLM_F_REPLACE"},
      {NLM_F_EXCL,"NLM_F_EXCL"}, 
      {NLM_F_CREATE,"NLM_F_CREATE"}, 
      {NLM_F_APPEND, "NLM_F_APPEND"},
    };
    
    StringBufferT<char> buffer;
     for (int i = 0; i<SizeOf(hdrFlags); i++) {
        if (flags & hdrFlags[i].first) {
            if (buffer.getContentSize() > 0) {
                buffer.append("|");
            }
            buffer.append(hdrFlags[i].second);
        }
    }
    const char* string = (const char*)buffer;
    return CharString(string);
    
  }

public:
  //2014/12/26
  static const char* getType(int type)
 {
//  Routing/neighbour discovery messages.

    static Pair<int, const char*> types[] = {
  {RTM_BASE, "RTM_BASE"},

  {RTM_NEWLINK, "RTM_NEWLINK"},
  {RTM_DELLINK, "RTM_DELLINK"},
  {RTM_GETLINK, "RTM_GETLINK"},
  {RTM_SETLINK,  "RTM_SETLINK"},

  {RTM_NEWADDR,  "RTM_NEWADDR"},
  {RTM_DELADDR,  "RTM_DELADDR"},
  {RTM_GETADDR,  "RTM_GETADDR"},

  {RTM_NEWROUTE,  "RTM_NEWROUTE"},
  {RTM_DELROUTE,  "RTM_DELROUTE"},
  {RTM_GETROUTE,  "RTM_GETROUTE"},

  {RTM_NEWNEIGH,  "RTM_NEWNEIGH"},
  {RTM_DELNEIGH,  "RTM_DELNEIGH"},
  {RTM_GETNEIGH,  "RTM_GETNEIGH"},

  {RTM_NEWRULE,  "RTM_NEWRULE"},
  {RTM_DELRULE,  "RTM_DELRULE"},
  {RTM_GETRULE,  "RTM_GETRULE"},

  {RTM_NEWQDISC,  "RTM_NEWQDISC"},
  {RTM_DELQDISC,  "RTM_DELQDISC"},
  {RTM_GETQDISC,  "RTM_GETQDISC"},

  {RTM_NEWTCLASS,  "RTM_NEWTCLASS"},
  {RTM_DELTCLASS,  "RTM_DELTCLASS"},
  {RTM_GETTCLASS,  "RTM_GETTCLASS"},

  {RTM_NEWTFILTER,  "RTM_NEWTFILTER"},
  {RTM_DELTFILTER,  "RTM_DELTFILTER"},
  {RTM_GETTFILTER,  "RTM_GETTFILTER"},

  {RTM_NEWACTION,  "RTM_NEWACTION"},
  {RTM_DELACTION,  "RTM_DELACTION"},
  {RTM_GETACTION,  "RTM_GETACTION"},

  {RTM_NEWPREFIX,  "RTM_NEWPREFIX"},

  {RTM_GETMULTICAST,  "RTM_GETMULTICAST"},

  {RTM_GETANYCAST,  "RTM_GETANYCAST"},

  {RTM_NEWNEIGHTBL,  "RTM_NEWNEIGHTBL"},
  {RTM_GETNEIGHTBL,  "RTM_GETNEIGHTBL"},
  {RTM_SETNEIGHTBL,  "RTM_SETNEIGHTBL"},

  {RTM_NEWNDUSEROPT,  "RTM_NEWNDUSEROPT"},

  {RTM_NEWADDRLABEL,  "RTM_NEWADDRLABEL"},
  {RTM_DELADDRLABEL,  "RTM_DELADDRLABEL"},
  {RTM_GETADDRLABEL,  "RTM_GETADDRLABEL"},

  {RTM_GETDCB ,   "RTM_GETDCB"},
  {RTM_SETDCB,    "RTM_SETDCB"},

  };
  const char* name = "???";
  for (int i = 0; i<SizeOf(types); i++) {
    if (types[i].first == type) {
      name = types[i].second;
      break;
    }
  }
  
  return name;
  }

public:
  void save(const char* filename)
  {
    File file(filename);
    file.create();
    size_t size = length();
    printf("Size %zd\n", size);
    unsigned char* data = (unsigned char*) this -> header;
    while (size > 0) {
      int written = file.write(data, size);
      data += written;
      //printf("Written %d\n", written); 
      if (written <= 0) {
        break;
      }
      size -= written;
    }
    file.close();
  }


public:
  void display()
  {
     printf("NLMSGHDR len:   %u\n",  length());
     printf("NLMSGHDR type:  %s\n",  getType(type() ));
     printf("NLMSGHDR flags: %s\n", (const char*)getFlags( flags() ));
     printf("NLMSGHDR seq:   %u\n", seq());
     printf("NLMSGHDR pid:   %u\n", pid());

  }
};

}

