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
 *  InodeWatcher.h
 *
 *****************************************************************************/

//2015/06/10 Modified to use inotify_init1 instead of inotify_init 
// in InodeWatcher Constructor.
// To create a nonblocking instance of inofity, you may write:
// InodeWatcher watcher(IN_NONBLOCKING);

 
#pragma once

#include <es++/Thread.h>
#include <es++/ArrayT.h>
#include <es++/HashTable.h>
#include <es++/CharString.h>
#include <es++/LocalDateTime.h>
#include <es++/Pair.h>

#include <sys/inotify.h>
#include <limits.h>
#include <sys/ioctl.h>

namespace Es {
 
//Inode's change watcher thread based on Linux specific system calls (inotify_*).

class InodeWatcher :public Thread {
private:
  static const int DEFAULT_SLEEP_MILLISEC = 500; //500msec

private:
  int         flag;
  int         notifier;
  bool        looping;
  int         sleepmsec;

  int         watchCount;
  ArrayT<int> watchFDs;
  HashTable*  hashTable;

private:
  static const int BUF_LEN  = (256 * (sizeof(struct inotify_event) + NAME_MAX + 1));

public:
  InodeWatcher(int flag = 0 )
  :Thread(),
  flag(flag),
  notifier(INVALID_FD),
  looping(true),
  sleepmsec(DEFAULT_SLEEP_MILLISEC),
  watchCount(0),
  hashTable(NULL)
  {
    //Modified to use inotify_init1 
    notifier = inotify_init1(flag);
    if (notifier == INVALID_FD) {
      throw Es::IException("Failed to inotiy_init");
    }
    hashTable = new HashTable(13, true);
  }

  ~InodeWatcher()
  {
    close();
    delete hashTable;
  }

  void setSleepMilliSec(int msec)
  {
    sleepmsec = msec;
  }

  void close()
  {
    removeAllWatch();
    if (notifier != INVALID_FD) {
       ::close(notifier);
       notifier = INVALID_FD;
    }
  }

  void removeAllWatch()
  {
    for (int i = 0; i< watchCount; i++) {
      int wd = watchFDs[i];
      inotify_rm_watch(notifier, wd); 
    }
    watchCount = 0;
  }

  int addWatch(const char* path, int events = IN_ALL_EVENTS)
  {
    int wd = inotify_add_watch(notifier, path, events);
    if (wd == INVALID_FD) {
      throw Es::IException("Failed to inotify_add_watch");
    }
    watchFDs[watchCount++] = wd;
    hashTable->add((Key)wd, new CharString(path), 0);

    return wd;
  } 
 
  int removeWatch(int wd)
  {
    int rc = -1;
    if (wd != INVALID_FD) {
      //printf("inotify_rm_watch %d\n", wd);
      rc = inotify_rm_watch(notifier, wd); 
      int i = 0;
      for (i = 0; i<watchCount; i++) {
        if (watchFDs[i] == wd) {
          break;
        }
      } 
      for (int n = i; n<watchCount-1; n++) {
          watchFDs[n] = watchFDs[n+1];  
      }
      hashTable -> remove((Key)wd);
      if (watchCount> 0) {
        watchCount--;
      }
    }
    return rc; 
  }

  const char* maskToString(int mask)
  {
     static const Pair<int, const char*> pairs[] = {
       {IN_ACCESS,        "IN_ACCESS       "},
       {IN_ATTRIB,        "IN_ATTRIB       "},
       {IN_CLOSE_NOWRITE, "IN_CLOSE_NOWRITE"},
       {IN_CLOSE_WRITE,   "IN_CLOSE_WRITE  "},
       {IN_CREATE,        "IN_CREATE       "},
       {IN_DELETE,        "IN_DELETE       "},
       {IN_DELETE_SELF,   "IN_DELETE_SELF  "},
       {IN_IGNORED,       "IN_IGNORED      "},
       {IN_ISDIR,         "IN_ISDIR        "},
       {IN_MODIFY,        "IN_MODIFY       "},
       {IN_MOVE_SELF,     "IN_MOVE_SELF    "},
       {IN_MOVED_FROM,    "IN_MOVED_FROM   "},
       {IN_MOVED_TO,      "IN_MOVED_TO     "},
       {IN_OPEN,          "IN_OPEN         "},
       {IN_Q_OVERFLOW,    "IN_Q_OVERFLOW   "},
       {IN_UNMOUNT,       "IN_UNMOUNT      "} };
     const char* name = "";
     for (int i = 0; i < SizeOf(pairs); i++) {
        if (pairs[i].first & mask) {
          name = pairs[i].second;
          break;
        }
     } 
     return name;
  }

  //2015/06/05
  CharString* lookupFolderName(int fd)
  {
    return (CharString*)hashTable->lookup(fd);
  }

  virtual void display(struct inotify_event* event)
  {
     const char* name = "";
     if  (event->len >0) {
         name = event->name;
     }
     LocalDateTime ldt;
     CharString now = ldt.nowToSeconds();
     
     const char* path = "";
     CharString* folder = (CharString*)hashTable->lookup(event->wd);
     if (folder) { 
       path = (const char*)(*folder);
     }
     const char* mask = maskToString(event->mask);
     printf("%s %s %s/%s\n", (const char*)now, mask, path, name);
  }

  //Thread procedure, which will be called by Thread::start method. 
  virtual void run()
  {
    printf("InodeWatcher::run start\n");
    
    char buf[BUF_LEN] __attribute__ ((aligned(8)));

    while (looping && watchCount >0) {
      //2015/06/10
      if (flag & IN_NONBLOCK) {
        sleepMilliSec(sleepmsec);
      }

      int len = read(notifier, buf, BUF_LEN);
      if (len == -1 && errno != EAGAIN) {
        printf("Closed inotifier file descriptor\n");
        break;
      }

      if (len > 0) {
        for (char* p = buf; p < buf + len; ) {
          struct inotify_event* event = (struct inotify_event *) p;
          display(event);
 
          p += sizeof(struct inotify_event) + event->len;
        }
        //printf("len %d errno=%d\n", len, errno);
      }
    }
    printf("InodeWatcher::run end\n");
  }

  void stop()
  {
    looping = false;
    close();
  }
};

}


