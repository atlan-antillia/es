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
 *  RoutingLinkStats64.h
 *
 *****************************************************************************/

//2018/08/20 Modified to use %llu

#pragma once

#include <es++/Object.h>
#include <linux/rtnetlink.h>
#include <linux/if_link.h>

/*
struct rtnl_link_stats64 {
    __u64   rx_packets;     // total packets received   
    __u64   tx_packets;     // total packets transmitted    
    __u64   rx_bytes;       // total bytes received     
    __u64   tx_bytes;       // total bytes transmitted  
    __u64   rx_errors;      // bad packets received     
    __u64   tx_errors;      // packet transmit problems 
    __u64   rx_dropped;     // no space in linux buffers    
    __u64   tx_dropped;     // no space available in linux  
    __u64   multicast;      // multicast packets received   
    __u64   collisions;

    // detailed rx_errors: 
    __u64   rx_length_errors;
    __u64   rx_over_errors;     // receiver ring buff overflow  
    __u64   rx_crc_errors;      // recved pkt with crc error    
    __u64   rx_frame_errors;    // recv'd frame alignment error 
    __u64   rx_fifo_errors;     // recv'r fifo overrun      
    __u64   rx_missed_errors;   // receiver missed packet   

    // detailed tx_errors 
    __u64   tx_aborted_errors;
    __u64   tx_carrier_errors;
    __u64   tx_fifo_errors;
    __u64   tx_heartbeat_errors;
    __u64   tx_window_errors;

    // for cslip etc 
    __u64   rx_compressed;
    __u64   tx_compressed;
};
*/


namespace Es {

class RoutingLinkStats64 :public Object {
private:
  struct rtnl_link_stats64 stats;

public:
  RoutingLinkStats64(struct rtnl_link_stats64 st)
  :stats(st)
  {

  }
    
  void display()
  {
    printf("STATS64: ");    
    printf("rx_packets: %llu,",   stats.rx_packets);
    printf("tx_packets: %llu,",   stats.tx_packets);
    printf("rx_bytes: %llu,",     stats.rx_bytes);
    printf("tx_bytes: %llu,",     stats.tx_bytes);
    printf("rx_errors: %llu,",    stats.rx_errors);
    printf("tx_errors: %llu,",    stats.tx_errors);
    printf("rx_dropped: %llu,",   stats.rx_dropped);
    printf("tx_dropped: %llu,",   stats.tx_dropped);
    printf("multicast: %llu,",    stats.multicast);
    printf("collisions: %llu\n",   stats.collisions);
  }

   // detailed rx_errors: 
  void rxerrors()
  {
    printf("rx_length_errors: %llu, ",   stats.rx_length_errors);
    printf("rx_over_errors: %llu, ",   stats.rx_over_errors);     // receiver ring buff overflow  
    printf("rx_crc_errors: %llu, ",   stats.rx_crc_errors);      // recved pkt with crc error    
    printf("rx_frame_errors: %llu, ",   stats.rx_frame_errors);    // recv'd frame alignment error 
    printf("rx_fifo_errors: %llu, ",   stats.rx_fifo_errors);     // recv'r fifo overrun      
    printf("rx_missed_errors: %llu\n",   stats.rx_missed_errors);   // receiver missed packet   
  }
  // detailed tx_errors 
  void txerrors()
  {
    printf("tx_aborted_errors: %llu, ",   stats.tx_aborted_errors);
    printf("tx_carrier_errors: %llu, ",   stats.tx_carrier_errors);
    printf("tx_fifo_errors: %llu, ",   stats.tx_fifo_errors);
    printf("tx_heartbeat_errors: %llu, ",   stats.tx_heartbeat_errors);
    printf("tx_window_errors: %llu\n",   stats.tx_window_errors);
  }
  // for cslip etc 
  void cslip()
  {
    printf("rx_compressed: %llu, ",   stats.rx_compressed);
    printf("tx_compressed: %llu\n",   stats.tx_compressed);
  }


};

}

