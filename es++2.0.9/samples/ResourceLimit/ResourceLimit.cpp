/******************************************************************************
 *
 * Copyright (c) 2017  TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
 *  ResourceLimit.cpp
 *
 *****************************************************************************/


#include <es++/ResourceLimit.h>

//
int main(int argc, char* argv[])
{
  ResourceLimit resLimit;
  printf("ResourceLimit::showAll\n");
  resLimit.showAll();
  printf("\n");

  struct rlimit limit = resLimit.coreFileSize();
  printf("Current coreFileSize \n");
  resLimit.showCoreFileSize();

  try {
    printf("Try to update coreFileSize Soft = 10000  Hard = 1000000\n");
    limit.rlim_cur = 10000;
    limit.rlim_max = 1000000; 
    resLimit.coreFileSize(limit);

    resLimit.showCoreFileSize();

  } catch (Es::Exception& ex) {
    caught(ex);
  }

  try {
    printf("Try to update coreFileSize Soft =  -1  Hard = 0\n");
    limit.rlim_cur = -1; 
    limit.rlim_max = 0; 
    resLimit.coreFileSize(limit);

    resLimit.showCoreFileSize();
     
  } catch (Es::Exception& ex) {
    caught(ex);
  }

  limit = resLimit.files();
  printf("Current files \n");
  resLimit.showFiles();

  try {
    //Increase the maximum file descriptor number that can be opened 
    limit.rlim_cur *= 2; 

    printf("Try to update files Soft =  %lu rlim  Hard = %lu\n",
      limit.rlim_cur,
      limit.rlim_max);

    resLimit.files(limit);
    resLimit.showFiles();

    for(unsigned long i = 0; i<limit.rlim_cur; i++) {
      char name[256];
      sprintf(name, "/tmp/tmp_%lu.txt", i);
      FILE* fp = fopen(name, "w");
      if (fp) {
         //printf("Opened file %s\n", name);
      } else {
        printf("Failed to open file %s\n", name);
        break;
      } 
    }
  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}
