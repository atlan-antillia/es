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
 *  Socket.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <time.h>
#include <sys/types.h>		
#include <sys/socket.h>
#include <netdb.h>
#include <sys/ioctl.h>

#include <sys/un.h>

#include <es++/SockAddrInet.h>
#include <es++/SockAddrInet6.h>
#include <es++/SockAddrUnix.h>


/**
 * Socketclass
 */
namespace Es {

enum ADDRESS_FAMILY {
  UNIX      = AF_UNIX,
  INET      = AF_INET,
  INET6     = AF_INET6,
  IPX       = AF_IPX,
  NETLINK   = AF_NETLINK,
  X25       = AF_X25,
  AX25      = AF_AX25,
  ATMVC     = AF_ATMPVC,
  APPLETALK = AF_APPLETALK,
  PACKET    = AF_PACKET,
  UNSPEC    = AF_UNSPEC //2016/04/23
};

enum SOCKET_TYPE {
  STREAM    = SOCK_STREAM,
  DATAGRAM  = SOCK_DGRAM,
  SEQPACKET = SOCK_SEQPACKET,
  RAW       = SOCK_RAW,
  RDM        = SOCK_RDM
};

class Socket :public Es::Object {

private:
  int  address_family;
  int  socket_type;
  int  protocol;
  int  fd;

  bool autoClose;  //2015/06/02

public:
  /**
   * This constructor doesn't create a socket. 
   *You have to call 'create' method explicitly to get it.
   */
  Socket()
    :address_family(INET),
    socket_type(STREAM),
    protocol(0),
    fd(INVALID_SOCKET),
    autoClose(true)
  {
    //You have to call int::create method to get a valid socket file descriptor.
  }


public:
  /**
   */
  Socket(ADDRESS_FAMILY family, SOCKET_TYPE type,  int proto)
    :address_family(family),
    socket_type(type),
    protocol(proto),
    fd(INVALID_SOCKET),
    autoClose(true)
  {
    this->fd = ::socket(address_family, socket_type, protocol);
    if(this->fd == INVALID_SOCKET) {
      throw Es::IException("Failed to create a socket: address_family=%d, socket_type=%d, protocol=%d",
          family, type, proto);
    }
  }

public:
  /*
   * Constructor to take addrinfo& argment.
   */
  Socket(const addrinfo& info)
    :address_family(info.ai_family),
    socket_type(info.ai_socktype),
    protocol(info.ai_protocol),
    fd(INVALID_SOCKET),
    autoClose(true)
  {
    this->fd = ::socket(address_family, socket_type, protocol);
    if(this->fd == INVALID_SOCKET) {
      throw Es::IException("Failed to create a socket: address_family=%d, socket_type=%d, protocol=%d",
          address_family, socket_type, protocol);
    }
  }

public:
  /*
   * Constructor to take addrinfo* argment.
   */
  Socket(const addrinfo* info)
    :address_family(0),
    socket_type(0),
    protocol(0),
    fd(INVALID_SOCKET),
    autoClose(true)
  {
    assert(info);
    address_family = info->ai_family;
    socket_type    = info->ai_socktype;
    protocol       = info->ai_protocol;

    this->fd = ::socket(address_family, socket_type, protocol);
    if(this->fd == INVALID_SOCKET) {
      throw Es::IException("Failed to create a socket: address_family=%d, socket_type=%d, protocol=%d",
          address_family, socket_type, protocol);
    }
  }

public:
  /**
   * Destructor
   */
  ~Socket()
  {
    if (autoClose) {
      close();
    }
  }

  //accept for sockaddr
  int accept(sockaddr* addr, socklen_t* size)
  {
    assert(addr);
    assert(size);

    int newfd = ::accept(getSocket(), addr, size);
    if (newfd == ERROR) {
      throw Es::IException("Failed to accept; %s", error());
    }
    return newfd;
  }

  //accept for SockAddr and its subclasses SockAddrInet, SocketAddrInet6 and SockAddrUnix
  int accept(SockAddr& addr)
  {
    socklen_t size = addr.getSize();
    return Socket::accept(addr.getAddress(), &size);
  }


  //bind 2014/12/19
  int bind(SockAddr* addr) 
  {
    int rc = 0;
    if (addr) {
       rc = Socket::bind(addr -> getAddress(), addr -> getSize());
    } else {
      throw Es::IException("Invalid argument");
    }
    return rc;
  }
	
  //bind
  int bind(sockaddr* addr, socklen_t size)
  {
    assert(addr);
    assert(size>0);

    int rc = 0;;

    int soc = getSocket();
    rc  = ::bind(soc, addr, size);
    if (rc < 0) {
      throw Es::IException("Faile to bind: %s", error());
    }
    return rc;
}

  //bind for IPv4
  int bind(unsigned short port, unsigned long address)
  {
    //This is for IPv4, domain is AF_INET
    sockaddr_in inet;
    memset(&inet, 0, sizeof(inet));
    inet.sin_family = address_family;
    inet.sin_port   = htons(port);
    inet.sin_addr.s_addr = htonl(address);
    return  Socket::bind((sockaddr*)&inet, sizeof(inet));
  }

  //bind for SockAddr and its subclasses SockAddrInet, SockAddrInet6, SockAddrUnix
  int bind(SockAddr& addr) {
    return  Socket::bind(addr.getAddress(), addr.getSize());
  }


  void noAutoClose()
  {
    autoClose = false;
  }

  bool close()
  {
    bool rc = false;
    if(this->fd != INVALID_SOCKET) {
      ::close(this->fd);
      this->fd = INVALID_SOCKET;
      rc= true;
    }
    return rc;
  }


  int create(ADDRESS_FAMILY family, SOCKET_TYPE type, int protocol)
  {
    if(this->fd != INVALID_SOCKET) {
    //If already created, return it.
      return this->fd;
    }

    this -> address_family = family;
    this -> socket_type   = type;
    this -> protocol = protocol;
    this -> fd = ::socket(address_family, socket_type, protocol);

    if(this->fd == INVALID_SOCKET) {
      throw Es::IException("Failed to create a socket: address_family=%d, socket_type=%d, protocol=%d",
          family, type, protocol);
    }
    return this->fd;

  }

  //connect
  int connect(const sockaddr* addr, size_t size)
  {
    assert(addr);
    assert(size>0);

    int rc = ::connect(getSocket(), addr, size);
    if (rc < 0) {
      throw Es::IException("Failed to connect: %s", strerror(errno));
    }
    return rc;
  }

  //To connect to the SockAddr and its subclasses SockAddrInet, 
  //SocketAddrInet6 and SockAddrUnix
  int connect(SockAddr& addr)
  {
    return Socket::connect(addr.getAddress(), addr.getSize());
  }

  int listen(int backlog)
  {
    int rc = ::listen(getSocket(), backlog);
    if (rc < 0) {
      throw Es::IException("Failed to listen: %s", strerror(errno));
    }
    return rc;
  }

  int recv(char* buff, int len, int flag=0)
  {
    assert(buff);
    assert(len>0);
    return ::recv(getSocket(), buff, len, flag);
  }

  int read(char* buff, int len)
  {
    assert(buff);
    assert(len>0);
    return ::read(getSocket(), buff, len);
  }


  int send(const char* buff, int len, int flag=0)
  {
    assert(buff);
    assert(len>0);
    return ::send(getSocket(), buff, len, flag);
  }

  int write(const char* buff, int len)
  {
    assert(buff);
    assert(len>0);
    return ::write(getSocket(), buff, len);
  }

  int getOption(int optname, char* optval, socklen_t* optlen)
  {
    assert(optval);
    assert(optlen);
    return ::getsockopt(getSocket(), SOL_SOCKET, optname, optval, optlen);
  }

  int setOption(int optname, char* optval, int optlen)
  {
    assert(optval);
    return::setsockopt(getSocket(), SOL_SOCKET, optname, optval, optlen);
  }

  int getSockName(sockaddr* addr, socklen_t* size)
  {
    assert(addr);
    assert(size);
    return ::getsockname(getSocket(), (sockaddr*)addr, size);
  }

  int getSockName(SockAddr& addr)
  {
    socklen_t size = addr.getSize();
    return Socket::getSockName(addr.getAddress(), &size);
  }


  int shutdown(int how=SHUT_WR|SHUT_RD)
  {
    return ::shutdown(getSocket(), how);
  }

  int  getAddressFamily()
  {
    return address_family;
  }


  void setSocket(int soc)
  {
    this->fd = soc;
  }

  int  getSocket()
  {
    if(this->fd == INVALID_SOCKET) {
      throw Es::IException("Invalid socket");
    }
    return this->fd;
  }

  int getSocketType()
  {
    return socket_type;
  }


  int ioctl(long cmd, int* argp)
  {
    return ::ioctl(getSocket(), cmd, argp);
  }

  int ioctl(long cmd, void* argp)
  {
    return ::ioctl(getSocket(), cmd, argp);
  }


  int nonBlocking()
  {
    int block = 1;
    return ::ioctl(getSocket(), FIONBIO, &block);
  }


  int isWritable()
  {
    fd_set writeFDs;

    FD_ZERO(&writeFDs);
    FD_SET(fd, &writeFDs);
    int rc = ::select(fd+1, NULL, &writeFDs, NULL, NULL);
    if (rc == 1) {
      rc = FD_ISSET(fd, &writeFDs);
    }
    return rc;
  }


  int isReadable()
  {
    fd_set readFDs;

    FD_ZERO(&readFDs);
    FD_SET(fd, &readFDs);
    int rc = ::select(fd+1, &readFDs, NULL, NULL, NULL);
    if (rc ==1 ) {
      rc = FD_ISSET(fd, &readFDs);
    }
    return rc;
  }
};

}

