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
 *  FileSystemStatistics.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <sys/statfs.h>


/*
struct statfs {
  __SWORD_TYPE f_type;    // type of file system (see below) 
  __SWORD_TYPE f_bsize;   // optimal transfer block size 
  fsblkcnt_t f_blocks;    // total data blocks in file system 
  fsblkcnt_t f_bfree;     // free blocks in fs 
  fsblkcnt_t f_bavail;    // free blocks available to unprivileged user 
  fsfilcnt_t f_files;     // total file nodes in file system 
  fsfilcnt_t f_ffree;     // free file nodes in fs 
  fsid_t f_fsid;          // file system id 
  __SWORD_TYPE f_namelen; // maximum length of filenames 
  __SWORD_TYPE f_frsize;  // fragment size (since Linux 2.6) 
  __SWORD_TYPE f_spare[5];
}; 
The types fsblkcnt_t and fsfilcnt_t are defined in <sys/types.h>. Both used to be unsigned long. 
 */

namespace Es {

class FileSystemStatistics :public Object {
private:
  struct statfs fs;

public:
  FileSystemStatistics(const char* path="/")
  {
    if (path == NULL) {
      throw Es::IException("Invalid argument. path is NULL");
    }
    int rc = statfs(path, &fs);
    if (rc != 0) {
      throw Es::IException("Failed to statfs for %s", path);
    }
  }

  FileSystemStatistics(int fd)
  {
    int rc = fstatfs(fd, &fs); 
    if (rc != 0) {
      throw Es::IException("Failed to fstatfs for %d", fd);
    }
  }

  __SWORD_TYPE fileSystemType()
  {
    return fs.f_type;    // type of file system (see below)
  }
  __SWORD_TYPE transferBlockSize()
  {
    return fs.f_bsize;   // optimal transfer block size
  }
  fsblkcnt_t totalDataBlocks()
  {
    return fs.f_blocks;    // total data blocks in file system
  }
  
  fsblkcnt_t freeBlocks()
  {
    return fs.f_bfree;     // free blocks in fs
  }
  fsblkcnt_t freeBlocksToUnprivilegedUser()
  {
    return fs.f_bavail;    // free blocks available to unprivileged user
  }
  
  fsfilcnt_t totalFileNodes()
  {
    return fs.f_files;     // total file nodes in file system
  } 
  
  fsfilcnt_t freeFileNodes()
  {
    return fs.f_ffree;     // free file nodes in fs
  }
  
  fsid_t fileSystemID()
  {
    return fs.f_fsid;          // file system id
  }
  
  __SWORD_TYPE maximumFilenamesLength()
  {
    return fs.f_namelen; // maximum length of filenames
  }
  
  __SWORD_TYPE fragmentSize()
  {
    return fs.f_frsize;  // fragment size (since Linux 2.6
  }
  
  virtual void display()
  {
    printf("FileSystemType                %d\n", fileSystemType());
    printf("TransferBlockSize             %d\n", transferBlockSize());
    printf("TotalDataBlocks               %lu\n", totalDataBlocks());
    printf("FreeBlocks                    %lu\n", freeBlocks());
    printf("FreeBlocksToUnprivilegedUser  %lu\n", freeBlocksToUnprivilegedUser());
    printf("TotalFileNodes                %lu\n", totalFileNodes());
    printf("FreeFileNodes                 %lu\n", freeFileNodes());
    fsid_t id = fileSystemID();
    printf("FileSystemID                  %d %d\n", id.__val[0], id.__val[1]);
    printf("MaximumFilenamesLength        %d\n", maximumFilenamesLength());
    printf("FragmentSize                  %d\n", fragmentSize());
  }
};

}


