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
 *  Folder.h
 *
 *****************************************************************************/


#pragma once


#include <es++/Object.h>

namespace Es {

#define SLASH  '/'

class Folder :public Es::Object {

private:
  char path[PATH_MAX+1];

public:
  /*
   * Folder name is expected to end with '/';
   *
   */
  Folder(const char* name)
  {
    assert(name);
    memset(path, 0, sizeof(path));
    int len = strlen(name);
    if (len > 0 && len < PATH_MAX) {
        
      strcpy(path, name);
      checkPath(path);
    }
  }
  
public:
  //This returns true if path exists
  bool exists()
  {
    bool rc = false;
    struct stat sts;
    int r = stat(path, &sts);
    if (r ==-1 && errno == ENOENT) {
        rc = false;
    } else {
      rc = true;
    }
    return rc;
  }

public:
  //This returns true if path folder is created successfully
  bool  create(mode_t mode=DEFAULT_DIR_PERMISSION) 
  {
    char* dir = strdup(path);
    
    char* slash;
    bool status = true;
    char* ptr = dir;
    ptr++;
    while ((slash = strchr(ptr, SLASH)) !=NULL ) {
      *slash = '\0';
      dprintf("%s\n", dir);
      struct stat st;
      if (stat(dir, &st) != 0) {
        if (mkdir(dir, mode) != 0 && errno != EEXIST) {
          dprintf("failed to mkdir %s\n", dir);
          throw Es::IException("Failed to mkdir %s; %s", dir, error() );
          status = false;
          break;
        } else {
           dprintf("Created a folder %s\n", dir);
           status = true;
        }
      } else {
        dprintf("Dir(%s) alredy exists\n", dir);
      }
      *slash = SLASH;
      ptr = slash + 1;
    }

    free(dir);
    return status;
  }
  
private:
  int checkPath(char* path)
  {
    int rc = -1;
    int len = strlen(path);
    char* slash = strrchr(path, SLASH);
    if (slash) {
      if (path[len-1] != SLASH) {
        slash++;
        *slash = '\0';
        rc = 0;
      }
    }
    return rc;
  }
};

}


