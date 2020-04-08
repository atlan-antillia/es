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
 *  VFileSystemStatistics.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <sys/statvfs.h>


/*
struct statvfs {
  unsigned long  f_bsize;    // file system block size 
  unsigned long  f_frsize;   // fragment size 
  fsblkcnt_t     f_blocks;   // size of fs in f_frsize units 
  fsblkcnt_t     f_bfree;    // # free blocks 
  fsblkcnt_t     f_bavail;   // # free blocks for unprivileged users 
  fsfilcnt_t     f_files;    // # inodes 
  fsfilcnt_t     f_ffree;    // # free inodes 
  fsfilcnt_t     f_favail;   // # free inodes for unprivileged users 
  unsigned long  f_fsid;     // file system ID 
  unsigned long  f_flag;     // mount flags 
  unsigned long  f_namemax;  // maximum filename length 
};
The types fsblkcnt_t and fsfilcnt_t are defined in <sys/types.h>. Both used to be unsigned long. 
 */

namespace Es {

class VFileSystemStatistics :public Object {
private:
  struct statvfs vfs;

public:
  VFileSystemStatistics(const char* path="/")
  {
    if (path == NULL) {
      throw Es::IException("Invalid argument. path is NULL");
    }
    int rc = statvfs(path, &vfs);
    if (rc != 0) {
      throw Es::IException("Failed to statvfs for %s", path);
    }
  }

  VFileSystemStatistics(int fd)
  {
    int rc = fstatvfs(fd, &vfs); 
    if (rc != 0) {
      throw Es::IException("Failed to statvfs for %d", fd);
    }
  }

  unsigned long  systemBlockSize()
  {
    return vfs.f_bsize;    // file system block size 
  }

  unsigned long  fragmentSize()
  {
    return vfs.f_frsize;   // fragment size 
  }

  fsblkcnt_t     frsizeUnits()
  {
    return vfs.f_blocks;   // size of fs in f_frsize units 
  }

  fsblkcnt_t     freeBlocks()
  {
    return vfs.f_bfree;    // # free blocks 
  }

  fsblkcnt_t     freeBlocksForUnprivilegedUsers()
  {
    return vfs.f_bavail;   // # free blocks for unprivileged users 
  }

  fsfilcnt_t     inodes()
  {
    return vfs.f_files;    // # inodes 
  }

  fsfilcnt_t     freeInoodes()
  {
    return vfs.f_ffree;    // # free inodes 
  }

  fsfilcnt_t     freeInodesForUnpriviledUsers()
  {
    return vfs.f_favail;   // # free inodes for unprivileged users 
  }

  unsigned long  fileSystemID()
  {
    return vfs.f_fsid;     // file system ID 
  }

  unsigned long  mountFlags()
  {
    return vfs.f_flag;     // mount flags 
  }

  unsigned long  maximumFileNameLength()
  {
    return vfs.f_namemax;  // maximum filename length 
  }

  virtual void display()
  {
    printf("SystemBlockSize                %lu\n",  systemBlockSize());
    printf("FragmentSize                   %lu\n",  fragmentSize());
    printf("FrSizeUnits                    %lu\n", frsizeUnits());
    printf("FreeBlocks                     %lu\n",  freeBlocks());
    printf("FreeBlocksForUnprivilegedUsers %lu\n", freeBlocksForUnprivilegedUsers());
    printf("Inodes                         %lu\n", inodes());
    printf("FreeInodes                     %lu\n", freeInoodes());
    printf("FreeInodesForUnpriviledUsers   %lu\n", freeInodesForUnpriviledUsers());
    printf("FileSystemiD                   %lu\n", fileSystemID());
    printf("MountFlags                     %lu\n", mountFlags());
    printf("MaxFileNameLength              %lu\n", maximumFileNameLength());
  }
};

}


