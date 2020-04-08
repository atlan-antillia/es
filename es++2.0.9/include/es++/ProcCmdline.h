/******************************************************************************
 *
 * Copyright (c) 2014  TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
 *  ProcCmdline.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/CharString.h>

namespace Es {
	
class ProcCmdline :public Object {
private:	
	pid_t pid;
	CharString fullpath;
    CharString cmdline;
public:
	ProcCmdline(pid_t id, const char* cmdlineFile) //  cmdlineFile = "/proc/[pid]/cmdline"
	:pid(id),
	fullpath(cmdlineFile),
	cmdline("")
	{
		read();
	}

public:
	~ProcCmdline()
	{
	}
	
public:
	CharString getPath()
	{
		return fullpath;
	}

	
public:
	void read() {
		
		//Open and read the /proc/[pid]/cmdline file.
		FILE* fp = fopen((const char*)fullpath, "r");
		if (fp) {
           char process[PATH_MAX];
            int r = fscanf(fp, "%s", process);
            fclose(fp);
			if (r) {
				cmdline = process;
			}
		} else {
			printf("Failed to open a file %s\n", (const char*)fullpath);
		}
	}
	
  //2014/12/08
public:
  int compare(Object* object)
  {
    int rc = 0;
    if (object==NULL) {
      return rc;
    }
  	
  	//compare this->fullpath and object->fullpath
  	const char* p1 = (const char*)this->fullpath;

    ProcCmdline* cmd = (ProcCmdline*)object;
    const char* p2 = (const char*)cmd->getPath();
  	
    if (p1 == NULL && p2 == NULL) {
      rc = 1;
    }
    if (p1 != NULL && p2 != NULL) {
      rc = __strcmp(p1, p2);
    }
    return rc;
  }
	
public:
	void display()
	{
		wprintf(L"%d %s\n", pid, (const char*)cmdline);		
	}
	
	
	pid_t getPid() {
		return pid;
	}
	
	const char* getCmdline()
	{
		return (const char*)cmdline;
	}
	

};

}

