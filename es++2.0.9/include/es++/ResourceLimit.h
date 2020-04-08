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
 *  ResourceLimit.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <sys/time.h>
#include <sys/resource.h> 

namespace Es {

/*
struct rlimit {
    rlim_t rlim_cur;  // Soft limit 
    rlim_t rlim_max;  // Hard limit (ceiling for rlim_cur)
};
 */

class ResourceLimit :public Object {
private:

public:
  ResourceLimit()
  {
  }

  //The maximum size of the process's virtual memory in bytes.
  struct rlimit addressSpace()
  {
    struct rlimit rl;
    getrlimit(RLIMIT_AS, &rl);
    return rl;
  }

  void showAddressSpace()
  {
    struct rlimit rl = addressSpace();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "AS", rl.rlim_cur, rl.rlim_max); 
  }

  int addressSpace(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_AS, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }

  //Maximum size of core file
  struct rlimit coreFileSize()
  {
    struct rlimit rl;
    getrlimit(RLIMIT_CORE, &rl);
    return rl;
  }

  void showCoreFileSize()
  {
    struct rlimit rl = coreFileSize();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "CORE", rl.rlim_cur, rl.rlim_max); 
  }

  int coreFileSize(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_CORE, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }


  //CPU time limit in seconds.
  struct rlimit cpuTime()
  {
    struct rlimit rl;
    getrlimit(RLIMIT_CPU, &rl);
    return rl;
  }

  void showCPUTime()
  {
    struct rlimit rl = cpuTime();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "CPU", rl.rlim_cur, rl.rlim_max); 
  }

  int cpuTime(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_CPU, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }


  //The maximum size of the process's data segment
  struct rlimit dataSegmentSize()
  { 
    struct rlimit rl; 
    getrlimit(RLIMIT_DATA, &rl);
    return rl;
  }

  void showDataSegmentSize()
  {
    struct rlimit rl = dataSegmentSize();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "DATA", rl.rlim_cur, rl.rlim_max); 
  }

  int dataSegmentSize(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_DATA, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }

  //The maximum size of files that the process may create.
  struct rlimit fileSize()
  {
    struct rlimit rl;
    getrlimit(RLIMIT_FSIZE, &rl);
    return rl;
  }

  void showFileSize()
  {
    struct rlimit rl = fileSize();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "FSIZE", rl.rlim_cur, rl.rlim_max); 
  }

  int fileSize(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_FSIZE, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }

  //A limit on the combined number of flock locks and fcntl.
  struct rlimit locks()
  {
    struct rlimit rl;
    getrlimit(RLIMIT_LOCKS, &rl);
    return rl;
  }

  void showLocks()
  {
    struct rlimit rl = locks();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "LOCKS", rl.rlim_cur, rl.rlim_max); 
  }

  int locks(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_LOCKS, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }

  //The maximum number of bytes of memory that may be locked into RAM.
  struct rlimit memLock()
  {
    struct rlimit rl;
    getrlimit(RLIMIT_MEMLOCK, &rl);
    return rl;
  }

  void showMemLock()
  {
    struct rlimit rl = memLock();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "MEMLOCK", rl.rlim_cur, rl.rlim_max); 
  }

  int memLock(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_MEMLOCK, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }


  //The number of bytes that can be allocated for POSIX message queues
  struct rlimit msgQueue()
  {
    struct rlimit rl;
    getrlimit(RLIMIT_MSGQUEUE, &rl);
    return rl;
  }

  void showMsgQueue()
  {
    struct rlimit rl = msgQueue();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "MSGQUEUE", rl.rlim_cur, rl.rlim_max); 
  }

  int msgQueue(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_MSGQUEUE, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }

  //A ceiling to which the process's nice value can be raised. 
  struct rlimit nice()
  {
    struct rlimit rl;
    getrlimit(RLIMIT_NICE, &rl);
    return rl;
  }

  void showNice()
  {
    struct rlimit rl = nice();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "NICE", rl.rlim_cur, rl.rlim_max); 
  }

  int nice(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_NICE, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }

  //The maximum file descriptor number that can be opened by a process.
  struct rlimit files()
  {
    struct rlimit rl;
    getrlimit(RLIMIT_NOFILE, &rl);
    return rl;
  }

  void showFiles()
  {
    struct rlimit rl = files();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "NOFILE", rl.rlim_cur, rl.rlim_max); 
  }

  int files(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_NOFILE, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }

  //The maximum number of processes that can be created 
  struct rlimit processes()
  {
    struct rlimit rl;
    getrlimit(RLIMIT_NPROC, &rl);
    return rl;
  }

  void showProcesses()
  {
    struct rlimit rl = processes();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "NPROC", rl.rlim_cur, rl.rlim_max); 
  }

  int processes(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_NPROC, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }

  //The limit (in pages) of the process's resident set. 
  struct rlimit rss()
  {
    struct rlimit rl;
    getrlimit(RLIMIT_RSS, &rl);
    return rl;
  }

  void showRSS()
  {
    struct rlimit rl = rss();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "RSS", rl.rlim_cur, rl.rlim_max); 
  }

  int rss(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_RSS, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }

  struct rlimit rtPriority()
  {
    struct rlimit rl;
    getrlimit(RLIMIT_RTPRIO, &rl);
    return rl;
  }

  void showRTPriority()
  {
    struct rlimit rl = rtPriority();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "RTPRIO", rl.rlim_cur, rl.rlim_max); 
  }

  int rtPriority(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_RTPRIO, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }

  struct rlimit rtTime()
  {
    struct rlimit rl;
    getrlimit(RLIMIT_RTTIME, &rl);
    return rl;
  }

  void showRTTime()
  {
    struct rlimit rl = rtTime();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "RTTIME", rl.rlim_cur, rl.rlim_max); 
  }

  int rtTime(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_RTTIME, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }


  //A limit on the number of signals. 
  struct rlimit sigPending()
  {
    struct rlimit rl;
    getrlimit(RLIMIT_SIGPENDING, &rl);
    return rl;
  }

  void showSigPending()
  {
    struct rlimit rl = sigPending();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "SIGPENDING", rl.rlim_cur, rl.rlim_max); 
  }

  int sigPending(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_SIGPENDING, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }

  //The maximum size of the process stack, in bytes.
  struct rlimit stack()
  {
    struct rlimit rl;
    getrlimit(RLIMIT_STACK, &rl);
    return rl;
  }

  void showStack()
  {
    struct rlimit rl = stack();
    printf("%-12s Soft: %10lu Hard: %10lu\n", "STACK", rl.rlim_cur, rl.rlim_max); 
  }

  int stack(struct rlimit& limit)
  {
    int rc = setrlimit(RLIMIT_STACK, &limit);
    if (rc < 0) {
      throw Es::IException("Failed to setrlimit: rlim_cur=%10lu rlim_max=%10lu,  %s",
        limit.rlim_cur, limit.rlim_max, strerror(errno));
    }
    return rc;
  }


  void showAll()
  {
    showAddressSpace();
    showCoreFileSize();
    showCPUTime();
    showDataSegmentSize();
    showFileSize();
    showLocks();
    showMemLock();
    showMsgQueue();
    showNice();

    showFiles();

    showProcesses();
    showRSS();
    showRTPriority();
    showRTTime();
    showSigPending();
    showStack();

  }
};

}

