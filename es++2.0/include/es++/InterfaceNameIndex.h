/******************************************************************************
 *
 * Copyright (c) 2015 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  InterfaceNameIndex.h
 *
 *****************************************************************************/

//2015/03/28 Motified not to use String class.

#pragma once
#include <es++/Object.h>
#include <es++/SocketDatagram.h>
#include <es++/CharString.h>

#include <linux/if.h> 

namespace Es {


class InterfaceNameIndex {

private:
  SocketDatagram sock;
	
public:
  InterfaceNameIndex()
  {
  } 

  ~InterfaceNameIndex()
  {
  	sock.close();
  }

  CharString name(int ifindex)
  {
    struct ifreq ifr;
  	const int len = sizeof(ifr.ifr_name);
  	
    char ifname[len + 1];
  	bzero(ifname, sizeof(ifname));
  	
    memset(&ifr, 0, sizeof(struct ifreq));
    ifr.ifr_ifindex = ifindex;

    if (sock.ioctl(SIOCGIFNAME, &ifr) >= 0) {
       strcpy(ifname, ifr.ifr_name);
    } else {
    	throw Es::IException("Invalid ifindex %d", ifindex);
    }
  	return CharString(ifname);
  }

  int index(const char* ifname) 
  {
    int index = -1;
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(struct ifreq));
  	const size_t len = sizeof(ifr.ifr_name);
  	if (ifname != NULL && strlen(ifname) < len) {
    	strcpy (ifr.ifr_name, ifname);

    	if (sock.ioctl(SIOCGIFINDEX, &ifr) >= 0) {
       	index = ifr.ifr_ifindex;
    	} else {
    		throw Es::IException("Invalid argument %s", ifname);
    	}
  	} else {
  		throw Es::IException("Invalid argument %s", ifname);	
  	}
     return index;
  }
};


}

