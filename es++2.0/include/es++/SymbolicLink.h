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
 *  SymbolicLink.h
 *
 *****************************************************************************/


#pragma once


#include <es++/Object.h>
#include <es++/CharString.h>
#include <es++/Exception.h>

namespace Es {

class SymbolicLink :public Object {
private:
  CharString path;

public:
  SymbolicLink(const char* name)
  :path("")
  {
    assert(name);
    struct stat sb;

    if (lstat(name, &sb) == 0) {
      path = name;
    } else {
      throw Es::IException("Invalid filename %s", name);
    }
  }

  int createLink(const char* newpath)
  {
    assert(newpath);
    return symlink((const char*)path, newpath); 
  }

  int readLink(CharString& link)
  {
     char buffer[PATH_MAX];
     ssize_t len = readlink((const char*)path, buffer, sizeof(buffer)-1);
     if (len > 0) {
       buffer[len] = '\0';
       link = buffer;
     } else {
       throw Es::IException("Failed to readlink %s", (const char*)path);
     }
     return len;
  }
};
}

