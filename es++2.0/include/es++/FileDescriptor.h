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
 *  FileDescriptor.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>

namespace Es {

class FileDescriptor :public Object {
private:
  int fd;

public:
  FileDescriptor(int fd = INVALID_FD)
  :fd(fd)
  {
  }

  ~FileDescriptor()
  {
    close();
  }

  void close()
  {
    if (fd != INVALID_FD) {
      ::close(fd);
      fd = INVALID_FD;
    }
  }

  int getFD()
  {
    return fd;
  }

  void setFD(int d)
  {
    if (d == INVALID_FD) {
      throw Es::IException("Invalid argument");
    }
    fd = d;
  }

  ssize_t read(void* buffer, size_t bsize)
  {
    if (buffer == NULL || bsize <=0) {
      throw Es::IException("Invalid argument");
    }
    return ::read(fd, buffer, bsize);
  }

  ssize_t write(void* buffer, size_t bsize)
  {
    return ::write(fd, buffer, bsize);
  }

};
}

