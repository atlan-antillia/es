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
 *  SemaphoreInfo.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


/*
struct  seminfo {
    int semmap;  // # of entries in semaphore map; unused 
    int semmni;  // Max. # of semaphore sets 
    int semmns;  // Max. # of semaphores in all semaphore sets 
    int semmnu;  // System-wide max. # of undo structures; unused 
    int semmsl;  // Max. # of semaphores in a set 
    int semopm;  // Max. # of operations for semop() 
    int semume;  // Max. # of undo entries per process; unused 
    int semusz;  // size of struct sem_undo 
    int semvmx;  // Maximum semaphore value 
    int semaem;  // Max. value that can be recorded for
                 //   semaphore adjustment (SEM_UNDO) 
};
 */

namespace Es {
  
class SemaphoreInfo :public Object {
private:
  struct seminfo info;

public:

  SemaphoreInfo()
  {
    memset(&info, 0, sizeof(info));
  }

public:
  operator seminfo*() {
    return &info;
  }


public:
  seminfo* getInfo() {
    return &info;
  }


public:
  void display() {
  }

};

}

