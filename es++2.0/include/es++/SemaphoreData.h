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
 *  SemaphoreData.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>

#include <es++/Exception.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


// Es {
/*
struct semid_ds {
    struct ipc_perm sem_perm;
    time_t          sem_otime;
    time_t          sem_ctime; 
    unsigned short  sem_nsems; 
};
 
struct ipc_perm {
    key_t __key;
    uid_t uid;
    gid_t gid;
    uid_t cuid;
    gid_t cgid; 
    unsigned short mode;
    unsigned short __seq;
};
 */

namespace Es {
  
class SemaphoreData :public Object {
private:
  struct semid_ds ds;

public:
  SemaphoreData()
  {
    memset(&ds, 0, sizeof(ds));
  }

  operator semid_ds*() 
  {
    return &ds;
  }


  semid_ds* getData() 
  {
    return &ds;
  }


  void display() 
  {
    printf("ipc_perm.key = %ud\n", ds.sem_perm.__key);
    printf("ipc_perm.uid = %ud\n", ds.sem_perm.uid);
    printf("ipc_perm.gid = %ud\n", ds.sem_perm.gid);
    printf("ipc_perm.cuid = %ud\n", ds.sem_perm.cuid);
    printf("ipc_perm.cgid = %ud\n", ds.sem_perm.cgid);
    printf("ipc_perm.mode= %d\n", ds.sem_perm.mode);
    printf("ipc_perm.seq= %d\n", ds.sem_perm.__seq);
    
    printf("sem_otime    = %s\n", ctime(&ds.sem_otime));
    printf("sem_ctime    = %s\n", ctime(&ds.sem_ctime)); 
    printf("sem_nsems    = %lu\n", ds.sem_nsems); 
  }


};

}

