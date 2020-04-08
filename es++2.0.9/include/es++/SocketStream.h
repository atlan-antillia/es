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
 *  SocketStream.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Socket.h>
#include <es++/SockAddrInet.h>
#include <es++/SockAddrInet6.h>
#include <es++/CharString.h>
#include <es++/CharStringBuffer.h>

/**
 * SocketStream class
 */
namespace Es {

class SocketStream :public Es::Socket {
private:
  char*  buffer;

  static const int BUFFER_SIZE = 1024*8;

  
public:
  /**
   * Constructor
   */  
  SocketStream(ADDRESS_FAMILY family=INET) 
    :Es::Socket(family, STREAM, 0),
    buffer(new char[BUFFER_SIZE])
  {
  }
  

public:
  /**
   * Constructor
   */
  SocketStream(int soc) 
    :Es::Socket(),
  buffer(new char[BUFFER_SIZE])
  {
    setSocket(soc);
  }

public:
  /**
   * Constructor
   */
  ~SocketStream() 
  {
    if (this->buffer) {
      delete [] this->buffer; 
      this->buffer = NULL;
    }
  }


public:
  SocketStream* accept(SockAddrInet& address) 
  {
    int fd = Socket::accept(address);
    return new SocketStream(fd);
  }


public:
  SocketStream* accept(SockAddrInet6& address) 
  {
    int fd = Socket::accept(address);
    return new SocketStream(fd);
  }

public:
  /**
   *
   */
  int create(ADDRESS_FAMILY family=INET, int protocol=0) 
  {
    return Socket::create(family, STREAM, protocol);
  }


public:
  int getPeerName(sockaddr_in* addr) 
  {
    memset(addr, 0, sizeof(sockaddr_in));
    int soc = getSocket();
    socklen_t addrlen = sizeof(sockaddr_in);
    return ::getpeername(soc, (sockaddr*)addr, &addrlen);
  }


public:
  //
  bool isReadable(int timeout=100)
  {
    bool rc = false;

    int fd = getSocket();
    
    fd_set  readFD, writeFD;
  
    timeval tv;
    memset(&tv, 0, sizeof(tv));
    tv.tv_usec = timeout;
    
    FD_ZERO(&readFD);
    FD_ZERO(&writeFD);
  
    FD_SET(fd, &readFD);
    FD_SET(fd, &writeFD);

    if (::select(FD_SETSIZE, &readFD, &writeFD, 
        NULL,  &tv) ) {
  
      if (FD_ISSET(fd, &readFD)) {
        //OK. Readable
        rc = true;
      }
    }
    return rc;
  }

public:
  //
  bool isWritable(int timeout=100)
  {
    bool rc = false;
  
    int fd = getSocket();
    
    fd_set  readFD, writeFD;
    timeval tv;
    memset(&tv, 0, sizeof(tv));
    tv.tv_usec = timeout;

    FD_ZERO(&readFD);
    FD_ZERO(&writeFD);

    FD_SET(fd, &readFD);
    FD_SET(fd, &writeFD);

    if (::select(FD_SETSIZE, &readFD, &writeFD, 
        NULL,  &tv) ) {
      if (FD_ISSET(fd, &writeFD)) {
        //OK. Writable
        rc = true;
      }
    }
    return rc;
  }

public:
  int printf(const char* format,...)
  {
    va_list pos;
    va_start(pos, format);
    vsnprintf(buffer, BUFFER_SIZE, format, pos);
    va_end(pos);

    return sendAll(buffer);
  }


public:
  int sendAll(const char* buff, int len, int flag=0, long timeout=30)
  {
    
    int sentBytes = 0;
    
    int soc = getSocket();

    const char* ptr = buff;
    int  orglen = len;
    time_t startTime = time(NULL);
    
    const int RETRY_COUNT_MAX = 10;
    int   retryCount = 0;

    while (len >0) {
      
      time_t currentTime = time(NULL);

      if ((currentTime - startTime) > timeout) {
        if (sentBytes < orglen) { 
          // timeout;
          break;
        }
        if (sentBytes == orglen) {
          break;
        }
      }

      //Check if fd is writable
      if (isWritable() == false) {
        continue;
      }

      int size = ::send(soc, ptr, len, flag);
      
      if (size <0 && (errno== EWOULDBLOCK  || errno ==EAGAIN) ) {
        //Socket error has happened, and if it were caused by blocking,
        //retry to send the buff data.
        if (retryCount < RETRY_COUNT_MAX) {
          sleep(1);
          retryCount++;
          continue;
        } else {
          break;
        }
      }

      if (size < 0 && len <=0) {
        break;
      }
  
      if (size >0) {
        sentBytes += size;
        ptr += size;
        len -= size;
      }

      retryCount = 0;
    }
    return sentBytes;
  }

public:
  int sendAll(const char* string) 
  {
    int rc = 0;
    if (string) {
      rc = sendAll(string, strlen(string), 0);
    }
    return rc;
  }

public:    
  int sendAll(CharString& string) 
  {
    const char* text = (const char*)string;
    int rc = 0;
    if (text) {
      rc = sendAll(text, strlen(text), 0);
    }
    return rc;
  }
  
public:
  int sendAll(CharStringBuffer& buffer) 
  {
    const char* text = buffer.getBuffer();
    int rc = 0;
    if (text) {
      rc = sendAll(text, strlen(text), 0);
    }
    return rc;
  }

};

}



