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
 *  SignalDescriptor.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/FileDescriptor.h>
#include <es++/Exception.h>
#include <sys/signalfd.h> 
#include <es++/SignalSet.h>
#include <es++/SignalParser.h>

namespace Es {

class SignalDescriptor :public FileDescriptor {
 
public:
  SignalDescriptor(int fd, SignalSet& set, int flags)
  :FileDescriptor()
  {
    //The fd can take -1.
    sigset_t mask = set.getSet();

    int sfd = signalfd(fd, &mask, flags);
    if (sfd == INVALID_FD) {
      throw Es::IException("Failed signalfd :%s", strerror(errno));
    } else {
      setFD(sfd);
    }
  }

public:
  SignalDescriptor(int fd, const sigset_t* mask, int flags)
  :FileDescriptor()
  {
    //The fd can take -1. 
    int sfd = signalfd(fd, mask, flags);
    if (sfd == INVALID_FD) {
      throw Es::IException("Failed signalfd :%s", strerror(errno));
    } else {
      setFD(sfd);
    }
  }

  void read(SignalParser& parser)
  {
    ssize_t size = FileDescriptor::read((void*)parser.getBuffer(),
                                               parser.getBufferSize());
    parser.parse(size);
  }
};
}

