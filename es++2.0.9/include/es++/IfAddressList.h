#pragma once

#include <es++/Object.h>
#include <es++/SockAddrInet.h>
#include <es++/SockAddrInet6.h>
#include <es++/SockAddrUnix.h>

#include <ifaddrs.h>
#include <netinet/in.h>


/*
struct ifaddrs {
      struct ifaddrs  *ifa_next;    // Next item in list 
      char            *ifa_name;    // Name of interface 
      unsigned int     ifa_flags;   // Flags from SIOCGIFFLAGS 
      struct sockaddr *ifa_addr;    // Address of interface 
      struct sockaddr *ifa_netmask; // Netmask of interface 
      union {
           struct sockaddr *ifu_broadaddr;
                  // Broadcast address of interface 
           struct sockaddr *ifu_dstaddr;
                  // Point-to-point destination address
      } ifa_ifu;
     #define         ifa_broadaddr ifa_ifu.ifu_broadaddr
     #define         ifa_dstaddr   ifa_ifu.ifu_dstaddr
     void            *ifa_data;    // Address-specific data 
 };
*/

namespace Es {

class IfAddressList :public Object {

private:
  ifaddrs *addrs;
  int     count;

public:
  IfAddressList()
  :addrs(NULL),
  count(0)
  {
    int rc = getifaddrs(&addrs);
    if (rc == ERROR) {
      throw Es::IException("Failed to getifaddr; %s", error() );  
        }

    ifaddrs* ptr = addrs;
     for (ptr = addrs; ptr != NULL; ptr = ptr->ifa_next) {
      count++;
    } 
  }

public:
  ~IfAddressList() 
  {
    if (addrs) {
      freeifaddrs(addrs);
      addrs = NULL;
    }
  }

public:
  void display()
  {
    ifaddrs* ptr = addrs;
     for (ptr = addrs; ptr != NULL; ptr = ptr->ifa_next) {
          printf("ifa_name  = %s\n", ptr->ifa_name);
          printf("ifa_flags = %d\n",ptr->ifa_flags);
      if (ptr->ifa_addr->sa_family == AF_INET) {
        SockAddrInet inet(ptr->ifa_addr);
        printf("ipv4 ifa_addr = %s\n", inet.toString() );
        SockAddrInet mask(ptr->ifa_netmask);
        printf("ipv4 ifa_netmask = %s\n", mask.toString() );

      } else if (ptr->ifa_addr->sa_family == AF_INET6) {
        SockAddrInet6 inet(ptr->ifa_addr);
        printf("ipv6 ifa_addr = %s\n", inet.toString() );
        SockAddrInet6 mask(ptr->ifa_netmask);
        printf("ipv6 ifa_netmask = %s\n", mask.toString() );
      } else { 
        printf("Unsupported family :%d\n", ptr->ifa_addr->sa_family);
        }
      printf("\n");
    } 
  }
};

}

