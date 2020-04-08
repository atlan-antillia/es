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
 *  MemoryMap.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <sys/time.h>
#include <sys/resource.h> 
#include <sys/mman.h>

namespace Es {

class MemoryMap :public Object {
private:
  void*  memory;
  size_t size;

public:
  MemoryMap()
  :memory(NULL),
   length(0)
  {
  }

  MemoryMap(size_t length, int prot, int flags,
           int fd, off_t offset)
  :memory(NULL),
   length(0)
  {
    memory = map(NULL, length, prot, flags, fd,  offset);
  }

public:
  MemoryMap(void *addr, size_t length, int prot, int flags,
           int fd, off_t offset)
  :memory(NULL),
   length(0)
  {
    memory = map(addr, length, prot, flags, fd,  offset);
  }

  ~MemoryMap()
  {
    unmap();

  }

  int unmap()
  {
    int rc = -1;
    if (memory != NULL && length > 0) {
      rc = munmap(memory, length);
      memory = NULL;
      length = 0;
    }
    return rc;
  }

  void* map(void* addr, size_t length, int prot, int flags,
           int fd, off_t offset)
  {
    if (memory != NULL) {
      unmap();
    }
 
    this -> memory = NULL;
    memory = mmap(addr, length, prot, flags, fd,  offset);
    if (memory == MAP_FAILED ) {
      throw Es::IException("Failed to mmap: %s", strerror(errno));
    }

    this -> length = length; 
    return memory;
  }

  void* getMemory()
  {
    return this -> memory;
  }

  size_t getLength()
  {
    return this -> length;
  } 

};
}

