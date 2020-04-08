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
 *  Globber.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>

#include <es++/Pair.h>
#include <es++/KeyNamePairs.h>
#include <glob.h>

/*
typedef struct {
    size_t   gl_pathc;    // Count of paths matched so
    char   **gl_pathv;    // List of matched pathnames.
    size_t   gl_offs;     // Slots to reserve in gl_pathv.
} glob_t;
*/

namespace Es {

typedef  int (*GlobError)(const char*, int);

class Globber :public Object {
private:
  glob_t globber;  
  bool   firstTime;

private:
  static int globError(const char* path, int errno)
  {
    printf("Error: path(%s), errno(%d)\n", path, errno);
    return 0;
  }

public:
  Globber() 
  { 
    memset(&globber, 0, sizeof(globber));
    firstTime = true;
  }

  void match(const char* pattern, int flags=0)
  {
    if (!firstTime) {
      flags |= GLOB_APPEND;
    }
    int rc = glob(pattern, flags, (GlobError)&Globber::globError, &globber);
    firstTime = false;
    if (rc != 0) {
      const char* msg = getError(rc);
      throw Es::IException("Failed to glob error=%s", msg);
    }
  }

  ~Globber() 
  { 
    clear();
  }

  const char* getError(int err)
  {
    static Pair<int, const char*> errors[] = {
      {GLOB_NOSPACE, "Nospace"},
      {GLOB_ABORTED, "Aborted"},
      {GLOB_NOMATCH, "Nomatch"}, 
    };
    KeyNamePairs<int> pairs(errors, SizeOf(errors));
    return pairs.match(err); 
  }

  int pathCount()
  {
    return globber.gl_pathc;
  }

  const char** pathList()
  {
    return (const char**)globber.gl_pathv;
  }

  void clear()
  {
    globfree(&globber); 
    memset(&globber, 0, sizeof(globber));
    firstTime = true;
  }

  const char* getPath(int i)
  {
    int count = pathCount();
    const char** pathv = pathList();
    const char* path = NULL; 
    if (count >0 && pathv) {
      if (i >= 0 && i <count) {
        path = pathv[i];
      } else {
        throw Es::IException("Invalid argument i: %d", i);
      }
    } else {
      throw Es::IException("Pathlist is empty.");
    }
    return path;
  }

  virtual void display()
  {
    int count = pathCount();
    const char** pathv = pathList();
    if (count >0 && pathv) {
      for (int i = 0; i< count; i++) {
        printf("%s\n", pathv[i]);
      }
    }
  }
};

}


