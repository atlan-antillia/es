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
 *  Process.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/CharString.h>
#include <es++/Exception.h>

#include <signal.h>
#include <spawn.h>
#include <sys/wait.h>

extern char **environ;

namespace Es {

class Process :public Object {
private:
  CharString path;
  pid_t pid;
  int  status;
	
public:
  Process(const char* path) 
  :path(path),
	pid(0),
	status(-1)
  {
  }
	
  int start(char* const* argv = NULL)
  {
  	pid = fork();
  	if ( pid == -1 ) { 
       throw Es::IException("Process(%s) fialed to folk: %s", (const char*)path, strerror(errno));
    }
  	if (pid == 0) {
  	  execv((const char*)path, (char**)argv);
  	}
    return pid;
  }

  int spawn(char* const* argv=NULL, 
    const posix_spawn_file_actions_t* file_actions=NULL,
    const posix_spawnattr_t* attrp=NULL)

  {
    status = ::posix_spawnp(&pid, (const char*)path, file_actions, attrp, (char**)argv, environ);
    if (status != 0) {
        throw Es::IException("posix_spawn failed: %s", strerror(status));
    }
    return status;
  } 

  int wait() 
  {
    int rc = -1;
  	if (pid > 0) {
      if (::waitpid(pid, &rc, 0) != -1) {
      	if (rc != 0) {
        	throw Es::IException("Process(%s) exited with status %s\n", (const char*)path, strerror(rc));
      	}      	
      } else {
        perror("waitpid");
      }
  	}
    return rc;
  }

  int kill()
  {
  	int rc = -1;
  	if (pid > 0) {
      rc = ::kill(pid, SIGKILL);
  	}
  	return rc;
  }

  int pause()
  {
  	int rc = -1;
  	if (pid > 0) {
      rc = ::kill(pid, SIGSTOP);
  	}
  	return rc;
  }

  int resume()
  {
  	int rc = -1;
  	if (pid > 0) {
      rc = ::kill(pid, SIGCONT);
  	}
  	return rc;
  }
};

}

