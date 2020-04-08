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
 *  FileWriterT.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/StringT.h>
#include <es++/Folder.h>


namespace Es {

template <class T> class FileWriterT :public Object {

private:
  FILE* fp;
  char* filename;

public:
  /**
   * Constructor
   *
   * fileName is a name to save a text string.
   *     If this parameter NULL, simply display a string on the console(stdout).
   */
  FileWriterT(const char* name)
  :fp(NULL),
  filename(NULL)
  {
  assert(name);
    filename = __strdup(name);
  }

public:
  int create()
  {  
    int rc = OK;
    File file(filename);
      
    if (file.exists() ) {
      //If already exists, try to delete it.
      file.unlink();
      //Don't care success or failure of this deleteion operation.
    } else {
      //If not existent, check a folder and try to create it
      Folder folder(filename);
          
      if (folder.exists() == false) {
        if (folder.create() == false) {
          printf("Failed to create a folderfor filePath:\"%s\"\n",
              filename);
          return ERROR;
        }
      }
    }
  
    fp = fopen(filename, "ab");
    if (fp == NULL) {
      throw Es::IException("Failed to open a file:%s",  filename, error() );
    }
    else {
      //unsigned char bom[2] = {0xff, 0xfe}; 
      //writeBytes(bom, 2);
    }
  }

public:
  ~FileWriterT()
  {
    close();

    if (filename) {
      delete [] filename;
      filename = NULL;
    }
  }

public:
  void close()
  {
    if (fp) {
      fclose(fp);
      fp = NULL;
    }
  }

/*
public:
  int writeBytes(unsigned char* bytes, size_t len)
  {
    int rc = 0;
    if (bytes && len>0) {
      if (fp) {
        rc = fwrite(bytes, sizeof(unsigned char), len, fp);
      } else {
        fwrite(bytes, sizeof(unsigned char), len, stdout);
      }
    }
    return rc;
  }
*/

public:
  // string parameter is a null-terminated T type string
  //
  int writeString(const T* string)
  {
    int rc = 0;
    if (string) {
      size_t slen = __strlen(string);
      if (fp) {
        rc = fwrite(string, sizeof(T), slen, fp);
      } else {
        rc = fwrite(string, sizeof(T), slen, stdout);
      }
    }
    return rc;
  }
};

}
