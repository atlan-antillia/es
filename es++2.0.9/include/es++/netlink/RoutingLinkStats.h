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
 *  RoutingLinkStats.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <linux/rtnetlink.h>
#include <linux/if_link.h>

/*
struct rtnl_link_stats
{
    __u32   rx_packets;     // total packets received   
    __u32   tx_packets;     // total packets transmitted    
    __u32   rx_bytes;       // total bytes received     
    __u32   tx_bytes;       // total bytes transmitted  
    __u32   rx_errors;      // bad packets received     
    __u32   tx_errors;      // packet transmit problems 
    __u32   rx_dropped;     // no space in linux buffers    
    __u32   tx_dropped;     // no space available in linux  
    __u32   multicast;      // multicast packets received   
    __u32   collisions;

    // detailed rx_errors: 
    __u32   rx_length_errors;
    __u32   rx_over_errors;     // receiver ring buff overflow  
    __u32   rx_crc_errors;      // recved pkt with crc error    
    __u32   rx_frame_errors;    // recv'd frame alignment error 
    __u32   rx_fifo_errors;     // recv'r fifo overrun      
    __u32   rx_missed_errors;   // receiver missed packet   

    // detailed tx_errors 
    __u32   tx_aborted_errors;
    __u32   tx_carrier_errors;
    __u32   tx_fifo_errors;
    __u32   tx_heartbeat_errors;
    __u32   tx_window_errors;

    // for cslip etc 
    __u32   rx_compressed;
    __u32   tx_compressed;
};
*/

namespace Es {

class RoutingLinkStats :public Object {
private:
  struct rtnl_link_stats stats;

public:
  RoutingLinkStats(struct rtnl_link_stats st)
  :stats(st)
  {

  }
    
  void display()
  {
    printf("STATS:");   

    printf("rx_packets: %d, ",   stats.rx_packets); 
    printf("tx_packets: %d, ",   stats.tx_packets); 
    printf("rx_bytes: %d, ",   stats.rx_bytes);   
    printf("tx_bytes: %d, ",   stats.tx_bytes);   
    printf("rx_errors: %d, ",   stats.rx_errors);      
    printf("tx_errors: %d, ",   stats.tx_errors);  
    printf("rx_dropped: %d, ",   stats.rx_dropped); 
    printf("tx_dropped: %d, ",   stats.tx_dropped); 
    printf("multicast: %d, ",   stats.multicast);
    printf("collisions: %d\n",   stats.collisions);
    
  }

    // detailed rx_errors: 
  void rxerrors()
  {
    printf("rx_length_errors: %d, ",   stats.rx_length_errors);
    printf("rx_over_errors: %d, ",   stats.rx_over_errors);     // receiver ring buff overflow  
    printf("rx_crc_errors: %d, ",   stats.rx_crc_errors);      // recved pkt with crc error    
    printf("rx_frame_errors: %d, ",   stats.rx_frame_errors);    // recv'd frame alignment error 
    printf("rx_fifo_errors: %d, ",   stats.rx_fifo_errors);     // recv'r fifo overrun      
    printf("rx_missed_errors: %d\n",   stats.rx_missed_errors);   // receiver missed packet   
  }
    // detailed tx_errors 
  void txerrors()
  {
    printf("tx_aborted_errors: %d, ",   stats.tx_aborted_errors);
    printf("tx_carrier_errors: %d, ",   stats.tx_carrier_errors);
    printf("tx_fifo_errors: %d, ",   stats.tx_fifo_errors);
    printf("tx_heartbeat_errors: %d, ",   stats.tx_heartbeat_errors);
    printf("tx_window_errors: %d\n",   stats.tx_window_errors);
  }
    // for cslip etc 
  void cslip()
  {
    printf("rx_compressed: %d, ",   stats.rx_compressed);
    printf("tx_compressed: %d\n",   stats.tx_compressed);
  }
};

}

