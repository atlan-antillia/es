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
 *  TrafficControlAttribute.h
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
#include <es++/Integer.h>

#include <es++/netlink/SocketNetlink.h>
#include <es++/netlink/SockAddrNetlink.h>
#include <es++/netlink/NetlinkMsgHeader.h>
#include <es++/netlink/TrafficControlMsg.h>
#include <es++/netlink/RouteAttribute.h>


namespace Es {

class TrafficControlAttribute :public RouteAttribute<tcmsg> {
private:

  TrafficControlMsg& trafficControlMsg;
    
public:
  TrafficControlAttribute(NetlinkMsgHeader& header, TrafficControlMsg& addrMsg)
  :RouteAttribute<tcmsg>(header),
  trafficControlMsg(addrMsg)
  {
  }

 
/*
enum {
    TCA_UNSPEC,
    TCA_KIND,
    TCA_OPTIONS,
    TCA_STATS,
    TCA_XSTATS,
    TCA_RATE,
    TCA_FCNT,
    TCA_STATS2,
    TCA_STAB,
    __TCA_MAX
};
    
*/
public:
  virtual void parse()
  {
    const char* string = (const char*)data();
    int   value = *(int*)data();
    unsigned char* byte = (unsigned char*)data();
    int  len = payload();
    //printf("parse type %d\n", type());
    
    switch(type()){

    case TCA_UNSPEC:
    {
        printf("TCA_UNSPEC\n");
    }
    break;
        
    case TCA_KIND:
    {
        Attribute* attr = new Attribute("TCA_KIND", new CharString(string));
        trafficControlMsg.setAttribute(TCA_KIND, attr);
    }
    break;

    case TCA_OPTIONS:
    {
        Attribute* attr = new Attribute("TCA_OPTIONS", new Bytes(byte, len));
        trafficControlMsg.setAttribute(TCA_OPTIONS, attr);
    }
    break;

/* Generic queue statistics, available for all the elements.
   Particular schedulers may have also their private records.
 */
/*
struct tc_stats {
    __u64   bytes;          // Number of enqueued bytes
    __u32   packets;        // Number of enqueued packets/
    __u32   drops;          // Packets dropped because of lack of resources
    __u32   overlimits;     // Number of throttle events when this flow goes out of allocated bandwidth 
    __u32   bps;            // Current flow byte rate 
    __u32   pps;            // Current flow packet rate
    __u32   qlen;
    __u32   backlog;
};
*/
    case TCA_STATS:
    {
        char buff[128];
        tc_stats* st = (tc_stats*)byte;
        if (st) {
            sprintf(buff, "bytes:%" PRIu64 ", packets:%d, drops:%d, overlimits:%d, bps=%d pps:%d, qlen:%d, backlog:%d",
                st->bytes, st->packets, st->drops, st->overlimits, st->bps, st->pps, st->qlen, st->backlog);
            Attribute* attr = new Attribute("TCA_STATS", new CharString(buff));
            trafficControlMsg.setAttribute(TCA_STATS, attr);
        }
    }
    break;
        
        
    case TCA_XSTATS:
    {
        Object* object = NULL;
        
        if (len == 4) {
            object = new Integer(value);
        } else {
            object = new Bytes(byte, len);
        }
        if (object) {
            Attribute* attr = new Attribute("TC_XSTATS", object);
            trafficControlMsg.setAttribute(TCA_XSTATS, attr);
        }
    }
    break;


    case TCA_STATS2:
    {
        Attribute* attr = new Attribute("TCA_STATS2", new Bytes(byte, len));
        trafficControlMsg.setAttribute(TCA_STATS2, attr);
    }
    break;

    case TCA_RATE:
    {
        Object* object = NULL;
        
        if (len == 4) {
            object = new Integer(value);
        } else {
            object = new Bytes(byte, len);
        }
        if (object) {
            Attribute* attr = new Attribute("TC_RATE", object);
            trafficControlMsg.setAttribute(TCA_RATE, attr);
        }
    }
    break;

    case TCA_FCNT:
    {
        Object* object = NULL;
        
        if (len == 4) {
            object = new Integer(value);
        } else {
            object = new Bytes(byte, len);
        }
        if (object) {
            Attribute* attr = new Attribute("TC_FCNT", object);
            trafficControlMsg.setAttribute(TCA_FCNT, attr);
        }
    }
    break;

    case TCA_STAB:
    {
        Object* object = NULL;
        
        if (len == 4) {
            object = new Integer(value);
        } else {
            object = new Bytes(byte, len);
        }
        if (object) {
            Attribute* attr = new Attribute("TC_STAB", object);
            trafficControlMsg.setAttribute(TCA_STAB, attr);
        }
    }
    break;
    }
  } 

    
};

}

