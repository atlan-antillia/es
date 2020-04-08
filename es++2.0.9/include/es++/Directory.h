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
 *  Directory.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>

#include <es++/CharString.h>
#include <es++/LinkedList.h>
#include <es++/LinkedListT.h>

#include <es++/FileAttributes.h>

#include <fnmatch.h>
#include <err.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

namespace Es {

class Directory :public Es::Object {

private:
  DIR* dirp;
  CharString directory;

public:
  Directory(const char* dir)
  :dirp(NULL),
  directory(dir)
  {
  }

public:
  ~Directory()
  {
    close();
  }

public:
 const char* getDirectory()
 {
    return (const char*)directory;
 }
  
public:
  void close() 
  {
    if (dirp) {
      closedir(dirp);
      dirp = NULL;
    }
  }

public:
  int open()
  {
    int rc = 0;
    
    dirp = opendir((const char*)directory);
    if (dirp == NULL) {
       throw Es::IException("Failed to open a dir(%s): errno", (const char*)directory, errno);
    }
    return rc;
  }
  
public:
  struct dirent* readDir()
  {
    return readdir(dirp);
  }

public:
  int getFileList(CharString pattern, LinkedList& list)  
  {
    int n = 0;
    open();
     
    struct dirent *dirEntry = NULL;

    while ((dirEntry = readdir(dirp)) != NULL) {
      if (strcmp(dirEntry->d_name, ".") ==0 || strcmp(dirEntry->d_name, "..") == 0) {
        continue;
      }
      char path[PATH_MAX];
      sprintf(path, "%s/%s", (const char*)directory, dirEntry->d_name);

      if (isDir(path) == false ) {

        if (fnmatch((const char*)pattern, dirEntry->d_name, 0
    //FNM_NOESCAPE|FNM_PATHNAME|FNM_PERIOD|FNM_PATHNAME|
    //FNM_FILE_NAME|FNM_LEADING_DIR|FNM_CASEFOLD 
      ) == 0) {
          CharString* name = new CharString(dirEntry->d_name);
          list.addLast(name);
          n++;
         }
       }
    }
    close();
    
    return n;
  }
  
public:
  int getFileList(CharString pattern, LinkedListT<CharString>& list)  
  {
    int n = 0;
    open();
     
    struct dirent *dirEntry = NULL;

    while ((dirEntry = readdir(dirp)) != NULL) {
      if (strcmp(dirEntry->d_name, ".") ==0 || 
          strcmp(dirEntry->d_name, "..") == 0) {
        continue;
      }
      char path[PATH_MAX];
      sprintf(path, "%s/%s", (const char*)directory, dirEntry->d_name);

      if (isDir(path) == false ) {

        if (fnmatch((const char*)pattern, dirEntry->d_name, 0
    //FNM_NOESCAPE|FNM_PATHNAME|FNM_PERIOD|FNM_PATHNAME|
    //FNM_FILE_NAME|FNM_LEADING_DIR|FNM_CASEFOLD 
      ) == 0) {
          CharString* name = new CharString(dirEntry->d_name);
          list.addLast(name);
          n++;
         }
       }
    }
    close();
   
    return n;
  }

public:
  int getFileList(CharString pattern, LinkedListT<FileAttributes>& list)  
  {
    int n = 0;
    open();
     
    struct dirent *dirEntry = NULL;

    while ((dirEntry = readdir(dirp)) != NULL) {
      if (strcmp(dirEntry->d_name, ".") ==0 || 
          strcmp(dirEntry->d_name, "..") == 0) {
        continue;
      }
      char path[PATH_MAX];
      sprintf(path, "%s/%s", (const char*)directory, dirEntry->d_name);

      if (isDir(path) == false ) {

        if (fnmatch((const char*)pattern, dirEntry->d_name, 0
    //FNM_NOESCAPE|FNM_PATHNAME|FNM_PERIOD|FNM_PATHNAME|
    //FNM_FILE_NAME|FNM_LEADING_DIR|FNM_CASEFOLD 
      ) == 0) {
      
          FileAttributes* attr = new FileAttributes(path);
          list.addLast(attr);
          n++;
         }
       }
    }
    close();
    
    return n;
  }

public:
  int getDirList(CharString pattern, LinkedList& list)  
  {
    int n = 0;
    open();   
    struct dirent *dirEntry;

    while ((dirEntry = readdir(dirp)) != NULL) {
      if (strcmp(dirEntry->d_name, ".") ==0 || 
          strcmp(dirEntry->d_name, "..") == 0) {
        continue;
      }
      char path[PATH_MAX];
      sprintf(path, "%s/%s", (const char*)directory, dirEntry->d_name);
      
      if (isDir(path) ) {
        if (fnmatch((const char*)pattern, dirEntry->d_name, 0
    //FNM_NOESCAPE|FNM_PATHNAME|FNM_PERIOD|FNM_PATHNAME|
    //FNM_FILE_NAME|FNM_LEADING_DIR|FNM_CASEFOLD 
    ) == 0) {
          CharString* name = new CharString(dirEntry->d_name);
          list.addLast(name);
          n++;
         }
       }
    }
    close();
    return n;
  }

public:
  int getDirList(CharString pattern, LinkedListT<FileAttributes>& list)  
  {
    int n = 0;
    open();   
    struct dirent *dirEntry;

    while ((dirEntry = readdir(dirp)) != NULL) {
      if (strcmp(dirEntry->d_name, ".") ==0 || 
          strcmp(dirEntry->d_name, "..") == 0) {
        continue;
      }
      char path[PATH_MAX];
      sprintf(path, "%s/%s", (const char*)directory, dirEntry->d_name);
      
      if (isDir(path) ) {
        if (fnmatch((const char*)pattern, dirEntry->d_name, 0
    //FNM_NOESCAPE|FNM_PATHNAME|FNM_PERIOD|FNM_PATHNAME|
    //FNM_FILE_NAME|FNM_LEADING_DIR|FNM_CASEFOLD 
    ) == 0) {
          FileAttributes* attr = new FileAttributes(path);
          list.addLast(attr);
          n++;
         }
       }
    }
    close();
    return n;
  }

public:
  static bool isDir(const char* path)
  {
    bool rc = false;
    struct stat st;
    if(stat(path, &st) == 0) {
      if ((st.st_mode & S_IFMT) == S_IFDIR) {
        //rc = S_ISDIR(st.st_mode);
        rc = true;
      }
    }
    return rc;
  }

};

}

