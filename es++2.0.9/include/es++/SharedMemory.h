/******************************************************************************
 *
 * Copyright (c) 2014 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  SharedMemorye.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <sys/types.h>
#include <sys/shm.h>


namespace Es {

class SharedMemory :public Object {
private:
  int  segmentId;

  void* shmaddr;
public:
  SharedMemory()
  :segmentId(INVALID_VALUE),
  shmaddr(NULL)
  {

  }

public:
  int create(int size, key_t key = IPC_PRIVATE, 
      int shmflg=S_IRUSR|S_IWUSR)
  {
    this->segmentId = ::shmget(key, size, shmflg);
    if (this->segmentId == ERROR) {
      throw Es::IException("Failed to shmget; %s", error() );  
    }
    return this->segmentId;
  }

public:
  ~SharedMemory() 
  {
    detach();
  }

public:
  int getSegmentId()
  {
    if (this->segmentId == INVALID_VALUE) {
      throw Es::IException("Invalid segmentId");
    } else {
      return  this->segmentId;
    }
  }

public:
  
  bool remove(struct shmid_ds* buf = NULL) 
  {
    bool rc = true;
    int r = ::shmctl(getSegmentId(), IPC_RMID, buf);
    if (r == ERROR) {
      throw Es::IException("Failed to shmctl; %s", error() );
    }
    
    return rc;
  }
  
public:
  void* attach(void* addr=NULL, int shmflg=SHM_RDONLY)
  {
    this-> shmaddr = ::shmat(getSegmentId(), addr, shmflg);

    if (this->shmaddr == INVALID_PTR) {
      throw Es::IException("Failed to shmat; %s", error() );  
    }
    return this->shmaddr;
  }
  
public:
  int detach() 
  {
    int rc = ERROR;
    if (this->shmaddr) {
      rc = ::shmdt(this->shmaddr);
      if (rc == ERROR) {
        throw Es::IException("Failed to shmdt; %s", error() );  
      }
    } 
    return rc; 
  }
};

}
