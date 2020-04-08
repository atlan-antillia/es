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
 *  Semaphore.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <es++/SemaphoreData.h>
#include <es++/SemaphoreInfo.h>

// Please see: http://www.tutorialspoint.com/unix_system_calls/semctl.htm

namespace Es {
  
union semun {
  int val;                // value for SETVAL 
  struct semid_ds *buf;   // buffer for IPC_STAT & IPC_SET 
  ushort *array;          // array for GETALL & SETALL 
  struct seminfo *__buf;  // buffer for IPC_INFO. Linux only
  void *__pad;
};
 
class Semaphore :public Object {
private:
  key_t key;
  int semid;
  int numsems;

public:
  Semaphore()
  :key(0),
   semid(INVALID_VALUE),
   numsems(0)
  {
  }

public:
  Semaphore(key_t key, int num=1, int semflg=DEFAULT_PERMISSION)
  :key(0),
   semid(INVALID_VALUE),
   numsems(0)
  {
    assert(num > 0);
    this -> key = key;
    this -> numsems = num;
    this -> semid = ::semget(key, num, semflg);
    if(this->semid == ERROR) {
      Exception("Failed to semget; %s", error()).display();
      assert(this->semid == ERROR);
    }  
  }

public:
  int create(key_t key=IPC_PRIVATE, int num=1, int semflg=DEFAULT_PERMISSION)
  {
    if (this->semid != INVALID_VALUE) {
      throw Es::IException("Already exists key=%ld", key);
    }
    assert(num > 0);
    this -> key = key;
    this -> numsems = num;
    this -> semid = ::semget(key, num, semflg);
    if(this->semid == ERROR) {
      throw Es::IException("Failed to semget; %s", error());
    }  
    return this->semid;
  }


  ~Semaphore() 
  {
  }

  int lock(int num=0) 
  {
    if (num <0 || num >=numsems) {
      throw Es::IException("Invalid argument; %d", num);
    }
    
    struct sembuf sop[1];
    sop[0].sem_num = num; 
    sop[0].sem_op = 1; 
    sop[0].sem_flg = 0; 

    int rc = semop(semid, sop, 1);
    if (rc  == ERROR) {
      throw Es::IException("Failed to semop; %s", error());
    }
    return rc;
  }


  int unlock(int num=0) {
    if (num <0 || num >=numsems) {
      throw Es::IException("Invalid argument; %d", num);
    }

    struct sembuf sop[1];
    sop[0].sem_num = num; 
    sop[0].sem_op = -1; 
    sop[0].sem_flg = 0; 

    int rc = semop(semid, sop, 1);
    if (rc  == ERROR) {
      throw Es::IException("Failed to semop; %s", error());
    }
    return rc;
  }


  int stat(int num, SemaphoreData& ds) 
  {
    if (num <0 || num >=numsems) {
      throw Es::IException("Invalid argument; %d", num);
    }
    semun arg;
    arg.buf = (semid_ds*)ds;
    int rc = ::semctl(this->semid, num, IPC_STAT, arg);
    if (rc == ERROR) {
      throw Es::IException("Faield to semcctl; %s", error());
    }
    return rc;
  }


  int info(int num, SemaphoreInfo& info) 
  {
    if (num <0 || num >=numsems) {
      throw Es::IException("Invalid argument; %d", num);
    }
  
    semun arg;
    arg.__buf = (seminfo*)info;
    int rc = ::semctl(this->semid, num, IPC_INFO, arg);
    if (rc == ERROR) {
      throw Es::IException("Faield to semcctl; %s", error());
    }
    return 0;
  }



  int setVal(int num, int value) {
    if (num <0 || num >=numsems) {
      throw Es::IException("Invalid argument; %d", num);
    }

    semun arg;    
    arg.val = value;

    int rc = ::semctl(this->semid, num, SETVAL, arg);
    if (rc == ERROR) {
      throw Es::IException("Faield to semcctl; %s", error());
    }
    return 0;
  }



  int getVal(int num, int value) {
    if (num <0 || num >=numsems) {
      throw Es::IException("Invalid argument; %d", num);
    }
  
    int rc = ::semctl(this->semid, num, GETVAL, 0);
    if (rc == ERROR) {
      throw Es::IException("Faield to semcctl; %s", error());
    }
    return rc;
  }


  int getPid(int num) 
  {
    if (num <0 || num >=numsems) {
      throw Es::IException("Invalid argument; %d", num);
    }
    int rc = ::semctl(this->semid, num, GETPID, 0);
    if (rc == ERROR) {
      throw Es::IException("Faield to semcctl; %s", error());
    }
    return rc;
  }


  int setAll(ushort* values) 
  {
    assert(values);
    semun arg;
    arg.array = values;
    int rc = ::semctl(this -> semid, 0, SETALL, arg);
    if (rc == ERROR) {
      throw Es::IException("Failed to semctl; %s", error());
    }
    return rc;
  }


  int getAll(ushort* values, int num) 
  {
    assert(values);
    if (num != this->numsems) {
      throw Es::IException("Invalid argument; %d", num);
    }
    semun arg;
    arg.array = values;
    // The second argment semnums to semctl will be ignored 
    int rc = ::semctl(this -> semid, 0, GETALL, arg);
    if (rc == ERROR) {
      throw Es::IException("Failed to semctl; %s", error());
    }
    return rc;
  }

};

}
