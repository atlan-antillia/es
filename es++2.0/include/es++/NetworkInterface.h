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
 *  NetworkInterface.h
 *
 *****************************************************************************/

#pragma once
#include <es++/Object.h>
#include <es++/InterfaceNameIndex.h>
#include <es++/SocketDatagram.h>
#include <es++/CharString.h>
#include <es++/StringBufferT.h>
#include <es++/Bytes.h>
#include <es++/Pair.h>
#include <linux/if.h> 

/*
struct ifreq {
#define IFHWADDRLEN 6
    union
    {
        char    ifrn_name[IFNAMSIZ];   
    } ifr_ifrn;
   
    union {
        struct  sockaddr ifru_addr;
        struct  sockaddr ifru_dstaddr;
        struct  sockaddr ifru_broadaddr;
        struct  sockaddr ifru_netmask;
        struct  sockaddr ifru_hwaddr;
        short   ifru_flags;
        int ifru_ivalue;    //ifindex
        int ifru_mtu;
        struct  ifmap ifru_map;
        char    ifru_slave[IFNAMSIZ];  
        char    ifru_newname[IFNAMSIZ];
        void *  ifru_data;
        struct  if_settings ifru_settings;
    } ifr_ifru;
};


*/

namespace Es {
//AF_INET IPv4
struct InterfaceRecord {    
   unsigned char family;
   char          ifname[IFNAMSIZ+1];        //

   struct  sockaddr ifaddr;
   struct  sockaddr dstaddr;
   struct  sockaddr broadaddr;
   struct  sockaddr netmask;
   struct  sockaddr hwaddr;
   short   flags;
   int     ifindex;           //ifindex returned by SIOCGIFINDEX
   int     mtu;
   struct  ifmap map;
   char    slave[IFNAMSIZ];   //
   char    newname[IFNAMSIZ];
   void *  data;
   struct  if_settings settings;
}; 


//IPv4
class NetworkInterface :public Object {

private:
  struct ifreq ifr;

  struct InterfaceRecord record;

  SocketDatagram sock;

public:
  NetworkInterface(int index)
  {
    clear();
  	
  	record.family  = AF_INET;
    InterfaceNameIndex nameIndex;
    const char* name = nameIndex.name(index);
  	
  	size_t namelen = sizeof(record.ifname);
    if (name && strlen(name) <= namelen) {
      strcpy(record.ifname, name);	
    } else {
      throw Es::IException("Invalid argument %d", index);	
    }
    //struct ifreq ifr;
  	memset(&ifr, 0, sizeof(ifr));
  	strncpy(ifr.ifr_name, record.ifname, sizeof(ifr.ifr_name));
  	ifr.ifr_addr.sa_family = record.family;

  	getAll();
  }

public:
  NetworkInterface(const char* name)
  {
    clear();
  	
  	record.family  = AF_INET;
 
  	size_t namelen = sizeof(record.ifname);
    if (name && strlen(name) < namelen) {
      strcpy(record.ifname, name);	
    } else {
      throw Es::IException("Invalid argument %s", name);	
    }
  	memset(&ifr, 0, sizeof(ifr));
  	strncpy(ifr.ifr_name, record.ifname, sizeof(ifr.ifr_name));
  	ifr.ifr_addr.sa_family = record.family;

  	getAll();
  }

private:
  void clear()
  {
  	 memset(&record, 0, sizeof(record)); 
  }

  sockaddr getIfAddr()
  {
  	if(sock.ioctl(SIOCGIFADDR, &ifr) == 0) {
  		record.ifaddr = ifr.ifr_addr;
  	} else {
  		throw Es::IException("Failed to get address");
  	}
  	return record.ifaddr;
  }

  sockaddr getDstAddr()
  {
 	if(sock.ioctl(SIOCGIFDSTADDR, &ifr) == 0) {
  		record.dstaddr = ifr.ifr_dstaddr;
  	} else {
  		throw Es::IException("Failed to get address");
  	}
  	return record.dstaddr;
  }

  sockaddr getBroadAddr()
  {
  	if(sock.ioctl(SIOCGIFBRDADDR, &ifr) == 0) {
  		record.broadaddr = ifr.ifr_broadaddr;
  	} else {
  		throw Es::IException("Failed to get address");
  	}
  	return record.broadaddr;
  }

  sockaddr getNetmask()
  {
  	if(sock.ioctl(SIOCGIFNETMASK, &ifr) == 0) {
  		record.netmask = ifr.ifr_netmask;
  	} else {
  		throw Es::IException("Failed to get address");
  	}
  	return record.netmask;
  }
	
  sockaddr getHardwareAddr()
  {
	if (sock.ioctl(SIOCGIFHWADDR, &ifr) == 0) {
		record.hwaddr = ifr.ifr_hwaddr;
  	} else {
  		throw Es::IException("Failed to get hardware address");
  	}
  	return record.hwaddr;
  }

  int getFlags()
  {
   	if (sock.ioctl(SIOCGIFFLAGS, &ifr) == 0) {
  		record.flags = ifr.ifr_flags;
  	} else {
  		throw Es::IException("Failed to get ifflags address");
  	}
  	return record.flags;
  }

  int getIfindex()
	{
   	if (sock.ioctl(SIOCGIFINDEX, &ifr) == 0) {
  		record.ifindex = ifr.ifr_ifindex;
  	} else {
  		throw Es::IException("Failed to get ivalue");
  	}
	return record.ifindex;
  }
	
  int getMtu()
  {
  	if (sock.ioctl(SIOCGIFMTU, &ifr) == 0) {
  		record.mtu = ifr.ifr_mtu;
  	} else {
  		throw Es::IException("Failed to get mtu");
  	}
  	return record.mtu;
  }
	
private:
  void getAll()
  {
	getIfAddr();
	getDstAddr();
  	getBroadAddr();
  	
	getNetmask();
	getHardwareAddr();
  	
 	getFlags();
 	getIfindex();
	getMtu();
  	
  }

public:
  static CharString getFlags(int flag)
  {
     Pair<int, const char*> flags[] = 
  	{
	{IFF_UP 		,"IFF_UP"}, 
	{IFF_BROADCAST ,"IFF_BROADCAST"},
	{IFF_DEBUG     ,"IFF_DEBUG"},
	{IFF_LOOPBACK  ,"IFF_LOOPBACK"},
	{IFF_POINTOPOINT ,"IFF_POINTOPOINT"},
	{IFF_RUNNING     ,"IFF_RUNNING"},
	{IFF_NOARP       ,"IFF_NOARP"},
	{IFF_PROMISC     ,"IFF_PROMISC"},
	{IFF_NOTRAILERS   ,"IFF_NOTRAILERS"},
	{IFF_ALLMULTI	,"IFF_ALLMULTI"},
	{IFF_MASTER		,"IFF_MASTER"},
	{IFF_SLAVE 		,"IFF_SLAVE"},
	{IFF_MULTICAST	,"IFF_MULTICAST"},
	{IFF_PORTSEL	,"IFF_PORTSEL"},
	{IFF_AUTOMEDIA	,"IFF_AUTOMEDI"}, 
	{IFF_DYNAMIC 	,"IFF_DYNAMIC"},
	{IFF_LOWER_UP	,"IFF_LOWER_UP"}, 
	{IFF_DORMANT	,"IFF_DORMANT"},
	{IFF_ECHO 		,"IFF_ECHO"},	
  	};
    StringBufferT<char> buffer;
    for (int i = 0; i<SizeOf(flags); i++) {
      if (flag & flags[i].first) {
        if (buffer.getContentSize() > 0) {
          buffer.append("|");
        }
        buffer.append(flags[i].second);
      }
    }
    const char* string = (const char*)buffer;
  	return CharString(string);
  }
	
public:
  struct sockaddr ifaddr()
  {
  	return record.ifaddr;
  }
	
  CharString ifAddrString()
  {
  	char buff[20];
  	struct sockaddr_in* addrin = (struct sockaddr_in*)&record.ifaddr;
  	::inet_ntop(record.family,  &addrin->sin_addr, buff, sizeof(buff));
  	return CharString(buff);
  }
	
  CharString dstAddrString()
  {
  	char buff[20];
  	struct sockaddr_in* addrin = (struct sockaddr_in*)&record.dstaddr;
  	::inet_ntop(record.family,  &addrin->sin_addr, buff, sizeof(buff));
  	return CharString(buff);
  }
	
  CharString broadAddrString()
  {
  	char buff[20];
  	struct sockaddr_in* addrin = (struct sockaddr_in*)&record.broadaddr;
  	::inet_ntop(record.family,  &addrin->sin_addr, buff, sizeof(buff));
  	return CharString(buff);
  }
	
  Bytes hardwareAddr()
  {
  	size_t size = sizeof(record.hwaddr.sa_data);
  	if (record.family == AF_INET) {
  		size = 6;
  	}
  	return Bytes((unsigned char*)record.hwaddr.sa_data, size);
  }

  CharString flags()
  {
  	return getFlags(record.flags);
  }

  int ifindex()
  {
  	return record.ifindex;
  }
	
  int mtu()
  {
  	return record.mtu;
  }
	
public:
  void display()
  {
  	printf("Ifaddr:  %s\n", (const char*)ifAddrString() );
	
  	printf("Dstaddr: %s\n", (const char*)dstAddrString() );
  	printf("BroadAddr: %s\n", (const char*)broadAddrString() );
  	
  	printf("HardwareAddr:");
  	hardwareAddr().display();
  	printf("Flags: %s\n", (const char*)flags() );
  	printf("Ifindex: %d\n", ifindex() );
  	printf("Mtu:     %d\n", mtu() );
  }
	
};



}

