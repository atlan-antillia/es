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
 *  FileAccessResponse.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <sys/fanotify.h>
#include <unistd.h>

namespace Es {

class FileAccessResponse :public Es::Object {
private:
  struct fanotify_response response;
  
public:
  FileAccessResponse(const __s32 metaDataFD, bool allowChange=true) 
  {
    memset(&response, 0, sizeof(response));
    response.fd       = metaDataFD;
    if (allowChange) {
       response.response = FAN_ALLOW;
     } else {
       response.response = FAN_DENY;
     }
  }

  ssize_t write(int fd)
  {
    ssize_t len = ::write(fd, &response, sizeof(response));
    if (len <= 0) {
      throw IException("Failed to write fanotify_response");
    }
    return len;
  }
};

}

