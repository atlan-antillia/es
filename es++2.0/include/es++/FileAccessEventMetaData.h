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
 *  FileAccessEventMetaData.h
 *
 *****************************************************************************/

//2015/06/10 Modified to use display method in parse method.

#pragma once

#include <es++/Object.h>
#include <es++/LocalDateTime.h>
#include <es++/SymbolicLink.h>
#include <es++/Pair.h>
#include <es++/FileAccessResponse.h>
#include <es++/UnsignedInteger64.h>
#include <es++/Exception.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/fanotify.h>
#include <unistd.h>

namespace Es {

class FileAccessEventMetaData :public Es::Object {
private:
  int fd;
  
public:
  FileAccessEventMetaData(int fd) 
  :fd(fd)
  {
    if (fd < 0) {
      throw Es::IException("Invalid argument");
    }
  }

  const char* maskToString(uint64_t mask)
  {
    static const Pair<uint64_t, const char*> pairs[] = {
      {FAN_ACCESS,        "FAN_ACCESS           "},
      {FAN_MODIFY,        "FAN_MODIFY           "}, 
      {FAN_CLOSE_WRITE,   "FAN_CLOSE_WRITE      "},
      {FAN_CLOSE_NOWRITE, "FAN_CLOSE_NOWRITE    "},
      {FAN_OPEN,          "FAN_OPEN             "},
      {FAN_OPEN_PERM,     "FAN_OPEN_PERMISSION  "},
      {FAN_ACCESS_PERM,   "FAN_ACCESS_PERMISSION"},
    };
    const char* name = "";
    for (int i = 0; i<SizeOf(pairs); i++) {
      if (pairs[i].first & mask) {
        name = pairs[i].second;
        break;
      }
    }
    return name;
  }

  //2016/06/10
  virtual void display(const char* path)
  {
    printf("%s\n", path); 
  }

  virtual void parse(uint64_t& index)
  {
    struct fanotify_event_metadata buf[128];

    ssize_t length = read(fd, (void *)&buf, sizeof(buf));
    if (length == -1 && errno != EAGAIN) {
      throw Es::IException("read error");
    }

    if (length <= 0) {
      throw Es::IException("read error");
    }

    const struct fanotify_event_metadata* metadata = buf;
    while (FAN_EVENT_OK(metadata, length)) {

      if (metadata -> vers != FANOTIFY_METADATA_VERSION) {
        throw Es::IException("Mismatch of fanotify metadata version.");
      }

      if (metadata -> fd >= 0) {
        const char* name = maskToString(metadata -> mask);
        //printf("%s ", name);
        if ( (metadata -> mask & FAN_OPEN_PERM)  ||
             (metadata -> mask & FAN_ACCESS_PERM) ) { 
          try {
            bool allowChange = true;
            FileAccessResponse response(metadata -> fd, allowChange);
            response.write(fd);
          } catch (Es::Exception& ex) {
            ex.display();
          }
        }

        char procSelfFD[PATH_MAX];
        snprintf(procSelfFD, sizeof(procSelfFD),
                         "/proc/self/fd/%d", metadata->fd);
        try {
          CharString path;
          SymbolicLink slink(procSelfFD);
          slink.readLink(path);
          //2016/06/10
          UnsignedInteger64 u64(index);
          char item[PATH_MAX];
          sprintf(item, "%10s %s %s", u64.toString(), 
              name, (const char*)path); 
          display(item);
          index++;
        } catch (Es::Exception& ex) {
          ex.display();
        }
        close(metadata->fd);
      }
      metadata = FAN_EVENT_NEXT(metadata, length);
    }
  }
};
}


