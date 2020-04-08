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
 *  NetlinkMsgError.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include <asm/types.h>
#include <es++/netlink/NetlinkMsgHeader.h>

namespace Es {
 
class NetlinkMsgError :public Object {
private:
  struct nlmsgerr* _error;

public:
  NetlinkMsgError(struct nlmsgerr* err)
  :_error(err)
  {
  }

public:
  void display()
  {
    if (_error) {
      printf("Message :%s\n", strerror( -1 * _error->error));
      printf("NLMSGERR errno:%d\n", _error -> error);
      printf("NLMSGERR len:   %u\n",  _error -> msg.nlmsg_len);
      printf("NLMSGERR type:  %s\n",  NetlinkMsgHeader::getType(_error -> msg.nlmsg_type ));
      printf("NLMSGERR flags: %s\n", (const char*)NetlinkMsgHeader::getFlags(_error -> msg.nlmsg_flags));
      printf("NLMSGERR seq:   %u\n", _error -> msg.nlmsg_seq);
      printf("NLMSGERR pid:   %u\n", _error -> msg.nlmsg_pid);
    }
  }
};

}

