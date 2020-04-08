/******************************************************************************
 *
 * Copyright (c) 2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  Tree.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>

#include <fstab.h>

/*
struct fstab {
  char *fs_spec;       // block special device name
  char *fs_file;       // file system path prefix
  char *fs_vfstype;    // File system type, ufs, nfs
  char *fs_mntops;     // Mount options ala -o 
  const char *fs_type; // FSTAB_* from fs_mntops
  int fs_freq;         // dump frequency, in days 
  int fs_passno;       // pass number on parallel dump
};
*/

namespace Es {

class FileSystemTable :public Object {

public:
  FileSystemTable() 
  { 
  }

  ~FileSystemTable() 
  { 
  }

  void getEntry()
  {
    int rc = setfsent();
    if (rc == 0) {
      throw Es::IException("Failed to setfsent");
    }
    bool looping = true;
    while (looping) {
      struct fstab* entry = getfsent();
      if (entry) {
        display(entry);
      } else {
        break;
      }
    }
    endfsent();
  }

 void getSpec(const char* path)
  {
    int rc = setfsent();
    if (rc == 0) {
      throw Es::IException("Failed to setfsent");
    }
    struct fstab* entry = getfsspec(path);
    if (entry) {
      display(entry);
    }
    endfsent();
  }

 void getFile(const char* path)
  {
    int rc = setfsent();
    if (rc == 0) {
      throw Es::IException("Failed to setfsent");
    }
    struct fstab* entry = getfsfile(path);
    if (entry) {
      display(entry);
    }
    endfsent();
  }

  virtual void display(struct fstab* entry)
  {
    printf("Spec          %s\n", entry->fs_spec);
    printf("PathPrefix    %s\n", entry->fs_file);
    printf("VFSType       %s\n", entry->fs_vfstype);
    printf("MountOptions  %s\n", entry->fs_mntops);
    printf("FSTABType     %s\n", entry->fs_type);
    printf("DumpFrequency %d\n", entry->fs_freq);
    printf("PassNo        %d\n", entry->fs_passno);
  }
};

}


