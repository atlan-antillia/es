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
 *  ProcDirectory.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Directory.h>
#include <es++/ProcCmdline.h>

#include <es++/ProcStat.h>

namespace Es {

class ProcDirectory :public Directory {

public:
  ProcDirectory()
	:Directory("/proc")
  {
  }

public:
  ~ProcDirectory()
  {
  }


public:
  int getCmdlineList(LinkedListT<ProcCmdline>& list)  
  {
    int n = 0;
  	open();   
    struct dirent *dirEntry;

    while ((dirEntry = readDir()) != NULL) {
      if (strcmp(dirEntry->d_name, ".") ==0 || strcmp(dirEntry->d_name, "..") == 0) {
        continue;
      }
      char path[PATH_MAX];
      sprintf(path, "%s/%s", (const char*)getDirectory(), dirEntry->d_name);
      pid_t pid = atoi(dirEntry->d_name);
      if (isDir(path) &&  pid >0 ) {
 	 	char cmdlineFile[PATH_MAX];
      	sprintf(cmdlineFile, "/proc/%s/cmdline", dirEntry->d_name);
           	
          ProcCmdline* cmdline = new ProcCmdline(pid, cmdlineFile);
          list.addLast(cmdline);
          n++;
       }
    }
  	close();
    return n;
  }


public:
  int getStatList(LinkedListT<ProcStat>& list)  
  {
    int n = 0;
  	open();   
    struct dirent *dirEntry;
  	printf("getStatList\n");
  	
    while ((dirEntry = readDir()) != NULL) {
      if (strcmp(dirEntry->d_name, ".") ==0 || strcmp(dirEntry->d_name, "..") == 0) {
        continue;
      }
      char path[PATH_MAX];
      sprintf(path, "%s/%s", (const char*)getDirectory(), dirEntry->d_name);
    	printf("path %s\n", path);
    	
      if (isDir(path) && atoi(dirEntry->d_name) >0 ) {
      	pid_t currentPid = getpid();
      	pid_t targetPid  = atoi(dirEntry->d_name);
      	if (currentPid != targetPid) {
      	  char statFile[PATH_MAX];
      	  sprintf(statFile, "/proc/%s/stat", dirEntry->d_name);
      	 //printf("...%s\n", statdir);
      	  ProcStat* stat = new ProcStat(statFile);
      	
          list.addLast(stat);
          n++;
      	}
       }
    }
  	close();
    return n;
  }



};


}

