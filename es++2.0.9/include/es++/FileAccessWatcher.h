/******************************************************************************
 *
 * Copyright (c) 2015  TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  FileAccessWatcher.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Thread.h>
#include <es++/Exception.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <sys/fanotify.h>
#include <unistd.h>
#include <es++/FileAccessEventMetaData.h>

namespace Es {

class FileAccessWatcher :public Thread {
private:
  static const int DEFAULT_SLEEP_MILLISEC = 500; //500msec

private:
  int  fd;

protected:
  int  sleepmsec;
  uint64_t index;
  bool looping;

public:
  FileAccessWatcher(const char* mount)
  :Thread(),
  fd(INVALID_FD),
  sleepmsec(0),
  index(0),
  looping(true)
  {
    fd = fanotify_init(FAN_CLOEXEC | FAN_CLASS_CONTENT | FAN_NONBLOCK,
                        O_RDONLY | O_LARGEFILE);
    if (fd == INVALID_FD) {
      throw Es::IException("Failed to fanotify_init. This program should be run as a root user. %s", error());
    }

    if (fanotify_mark(fd, FAN_MARK_ADD | FAN_MARK_MOUNT,
                      FAN_OPEN_PERM | FAN_CLOSE_WRITE, AT_FDCWD,
                      mount)  == -1) {
      throw Es::IException("Failed to fanotify_mark: %s", error());
    }
  }

  ~FileAccessWatcher()
  {
    stop();
  }

  int getNotifierFD()
  {
    return fd;
  }

  void setSleepMilliSec(int msec)
  {
    sleepmsec = msec;
  }

  void stop()
  {
    looping = false;
    close();
  }

  void close()
  {
    if (fd != INVALID_FD) {
      ::close(fd);
      fd = INVALID_FD;
    }
  }

  virtual void run()
  {
    const nfds_t NFDS = 2;
    struct pollfd fds[NFDS];
    fds[0].fd     = fd;
    fds[0].events = POLLIN;
    fds[1].fd     = STDIN_FILENO;
    fds[1].events = POLLIN;

    while (looping) {
      sleepMilliSec(sleepmsec);
      int poll_num = poll(fds, NFDS, -1);
      if (poll_num == -1) {
        perror("poll");   
        break;
      }
     
      if (poll_num > 0) {
        if  (fds[0].revents & POLLIN) {
          try {
            FileAccessEventMetaData metaData(fd);
            metaData.parse(index);
          } catch (Es::Exception& ex) {
            ex.display();
          }
        }
      }
    } //while
  }
};

}


