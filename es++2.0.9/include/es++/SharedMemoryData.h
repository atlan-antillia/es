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
 *  SharedMemoryData.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


/*
struct shmid_ds {
  struct ipc_perm shm_perm;        // operation perms 
  int     shm_segsz;               // size of segment (bytes) 
  time_t  shm_atime;               // last attach time 
  time_t  shm_dtime;               // last detach time 
  time_t  shm_ctime;               // last change time 
  unsigned short  shm_cpid;        // pid of creator 
  unsigned short  shm_lpid;        // pid of last operator 
  short   shm_nattch;              // no. of current attaches 

                   // the following are private 
  unsigned short   shm_npages;     // size of segment (pages) 
  unsigned long   *shm_pages;      // array of ptrs to frames -> SHMMAX  
  struct vm_area_struct *attaches; // descriptors for attaches 
}; 
 
struct ipc_perm {
  key_t key;
  uid_t uid;
  gid_t gid;
  uid_t cuid;
  gid_t cgid; 
  unsigned short mode;
  unsigned short seq;
};
 */

namespace Es {
  
class SharedMemoryData :public Object {
private:
  shmid_ds ds;

public:

  SharedMemoryData()
  {
    memset(&ds, 0, sizeof(ds));
  }

public:
  operator shmid_ds*() {
    return &ds;
  }


public:
  shmid_ds* getData() {
    return &ds;
  }


public:
  void display() {

    printf("ipc_perm.key = %ud\n", ds.shm_perm.__key);
/*
    printf("ipc_perm.uid = %ud\m", ds.shm_perm.uid);
    printf("ipc_perm.gid = %ud\m", ds.shm_perm.gid);
    printf("ipc_perm.cuid = %ud\m", ds.shm_perm.cuid);
    printf("ipc_perm.cgid = %ud\m", ds.shm_perm.cgid);

    printf("ipc_perm.mode= %d\m", ds.sem_perm.mode);
    printf("ipc_perm.seq= %d\m", ds.sem_perm.__seq);
    
    
    printf("shm_segsz        = %d bytes\n", ds.shm_segsz);
    printf("last attach time = %s\n", ctime(ds.shm_atime));
    printf("last detach time = %s\n", ctime(ds.shm_dtime));
    printf("last change time = %s\n", ctime(ds.shm_ctime));
    printf("pid creator      = %d\n", ds.shm_cpid);
    printf("pid of operator  = %d\n", ds.shm_lpid);
    printf("no of current attaches = %d\n", ds.shm_nattch);

    // the following are private 
    //unsigned short   shm_npages;     // size of segment (pages) 
    //unsigned long   *shm_pages;   
*/

  }

};

}

