/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  File.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>

namespace Es {

class File :public Es::Object {

protected:
  int fd;
  std::string filename;

public:
  File(const char* name)
  :fd(INVALID_VALUE) 
  {
    assert(name);
    filename = name;
  }

  File(const std::string& name)
  :fd(INVALID_VALUE)
  {
    filename = name;
  }

  int getFD() 
  {
    return fd;  
  }
    
  const char* getFilename() const
  {
    return filename.c_str();
  }

  int create(int mode=S_IRUSR|S_IWUSR) 
  {
    fd = ::open(filename.c_str(), O_CREAT|O_RDONLY|O_WRONLY, mode);
    if (fd == ERROR) {
      throw Es::IException("Failed to open a file(%s); %s", filename.c_str(), error() );
    }
    return fd;
  }


  int open(int oflag=O_RDONLY|O_WRONLY) 
  {
    fd = ::open(filename.c_str(), oflag);
    if (fd == ERROR) {
      throw Es::IException("Failed to open a file(%s); %s", filename.c_str(), error() );
    }
    return fd;
  }

  int openReadOnly() 
  {
    fd = ::open(filename.c_str(), O_RDONLY);
    if (fd == ERROR) {
      throw Es::IException("Failed to open a file(%s); %s", filename.c_str(), error() );
    }
    return fd;
  }


  int openAppend() 
  {
    fd = ::open(filename.c_str(), O_WRONLY|O_APPEND);
    if (fd == ERROR) {
      throw Es::IException("Failed to open a file(%s); %s", filename.c_str(), error() );
    }
    return fd;
  }

  int openWriteOnly() 
  {
    fd = ::open(filename.c_str(), O_WRONLY);
    if (fd == ERROR) {
      throw Es::IException("Failed to open a file(%s); %s", filename.c_str(), error() );
    }
    return fd;
  }

  int write(const char* bytes, size_t len) 
  {
    assert(bytes);
    assert(len>0);
    return ::write(fd, bytes, len);  
  }


  int write(const unsigned char* bytes, size_t len) 
  {
    assert(bytes);
    assert(len>0);
    return ::write(fd, bytes, len);  
  }
  

  int read(char* bytes, size_t len) 
  {
    assert(bytes);
    assert(len>0);
    return ::read(fd, bytes, len);  
  }

  int read(unsigned char* bytes, size_t len) 
  {
    assert(bytes);
    assert(len>0);
    return ::read(fd, bytes, len);  
  }

  void close() 
  {
    if (fd !=INVALID_VALUE) {
      ::close(fd);
      fd = INVALID_VALUE;
    }
  }

  int unlink() 
  {
    int rc = ERROR;
    if (exists()) {
      rc = ::unlink(filename.c_str());
      if (rc == ERROR) {
        throw Es::IException("Failed to unlink (%s); %s", filename.c_str(), error() );
      }
    }
    return rc;
  }

public:
  operator const char*() const
  {
    return filename.c_str();
  }

public:
  int chmod(int mode) 
  {
    int rc = ::chmod(filename.c_str(), mode);
    if (rc == ERROR) {
      throw Es::IException("Failed to chmod (%s); %s", filename.c_str(), error() );
    }
    return rc;
  }

public:
  bool isWritable()
  {
    bool rc = false;
    
    fd_set writeFDs;
    
    FD_ZERO(&writeFDs);
    FD_SET(fd, &writeFDs);
    if (::select(fd+1, NULL, &writeFDs, NULL, NULL)) {
      if (FD_ISSET(fd, &writeFDs)) {
        rc = true;
      }
    }
    return rc;
  }

public:
  bool isReadable()
  {
    bool rc = false;
    fd_set readFDs;
    
    FD_ZERO(&readFDs);
    FD_SET(fd, &readFDs);
    if (::select(fd+1, &readFDs, NULL, NULL, NULL)) {
      if (FD_ISSET(fd, &readFDs)) {
        rc = true;
      }
    }
    return rc;
  }

public:
  bool exists()
  {
    bool rc = false;
    struct stat sts;
    int r = stat(filename.c_str(), &sts);
    if (r ==-1 && errno == ENOENT) {
        //printf ("The file %s doesn't exist...\n", name);
        rc = false;
    } else {
      rc = true;
    }
    return rc;
  }
  
public:
  long int getFileSize()
  {
    struct stat sts;
    int rc = stat(filename.c_str(), &sts);
    if (rc == ERROR) {
      throw Es::IException("Failed to stat %s; %s", filename.c_str(), error() );
    } 
    return sts.st_size;
  }
};

}

