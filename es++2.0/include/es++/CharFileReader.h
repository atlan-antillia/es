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
 *  CharFileReader.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/File.h>
#include <sys/io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 


namespace Es {

class CharFileReader :public Object {
private:
  FILE* fp;

  char*  data;
  size_t size;
  
public:
  CharFileReader(const char* filename, bool nullTerminated=true)
  :fp(NULL),
  data(NULL),
  size(0)
  {
    File file(filename);
      
    if (file.exists() == false) {
      throw Es::IException("File not found: %s.", filename);
    } else { 
      fp = fopen(filename, "r");
      if (fp == NULL) {
        throw Es::IException("Failed to open a file (%s)", filename);
      }
    }
    size = file.getFileSize();
    if (nullTerminated) {
      data = new char[size+1];
    } else {
      data = new char[size];
    }
        
    memset(data, 0, size);
  
    size_t remain = size;
    char* ptr = data;
    while (remain > 0) {
      size_t len = fread(ptr, sizeof(char), remain, fp);
      remain -= len;
      ptr += len;
    }
    if (nullTerminated) {
      data[size] = ZERO;
    }
    fclose(fp);
    fp = NULL;
  }
  

public:
  ~CharFileReader()
  {
    delete[] data;
    data = NULL;
  }  

public:
  const char* getData() const
  {
    return data;
  }
  
  const size_t getDataSize() const
  {
    return size;
  }
};

} 
   
