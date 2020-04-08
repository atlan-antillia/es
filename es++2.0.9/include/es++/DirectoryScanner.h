/******************************************************************************
 *
 * Copyright (c) 2014 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *  notice, this list of conditions, and the following disclaimer.
 * 
 * 2. The name of the author may not be used to endorse or promote products
 *  derived from this software without specific prior written permission.
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
 * DirectoryScanner.h
 *
 *****************************************************************************/

// 2015/01/21 Added skipDotFile and skipDotDirectory methods to skip the files or directories of the 
// name of ".something". 

#pragma once
#include <es++/Object.h>
#include <es++/Directory.h>
#include <es++/DirEntryList.h>
#include <es++/CharString.h>

namespace Es {
  
class DirectoryScanner :public Directory{

private:
  static bool skipDotFile(bool* skip = NULL)
  {
    static bool skipDotFile = true;
    if (skip) {
       skipDotFile= *skip;
    } 
    return skipDotFile;
  }

private:
  static bool skipDotDirectory(bool* skip = NULL)
  {
    static bool skipDotDir = true;
    if (skip) {
       skipDotDir= *skip;
    } 
    return skipDotDir;
  }

private:
  static const char* setFilterPattern(const char* pattern=NULL)
  {
    static CharString filterPattern = "*.*";
    if (pattern) {
      filterPattern = pattern;
    } 
    return (const char*)filterPattern;
  }
  
  static const char* setBaseDirectory(const char* dir=NULL)
  {
    static CharString baseDirectory = "*";
    if (dir) {
      baseDirectory = dir;
    } 
    return (const char*)baseDirectory;
  }

  static int select(const struct dirent* dirEntry)
  {
    const char* name = dirEntry->d_name;
    const char* pattern = setFilterPattern();
    
    if(strcmp(name, ".") == 0 ||
       strcmp(name, "..") == 0 )
    {
        return 0;
    }
    else if (::fnmatch(pattern, name, 0) == 0) {
        return 1;
    }
    return 0;
  }

  static int selectFile(const struct dirent* dirEntry)
  {
    const char* name = dirEntry->d_name;
    const char* pattern = setFilterPattern();
    const char* basedir = setBaseDirectory();
    char fullpath[PATH_MAX];
    sprintf(fullpath, "%s/%s", basedir, name);
    
    if(strcmp(name, ".") == 0 ||
       strcmp(name, "..") == 0 )
    {
        return 0;
    }
    //2015/01/21
    if (name[0] == '.' && skipDotFile()) {
      return 0;  
    }
    
    if (::fnmatch(pattern, name, 0) == 0) {
      if (isDir(fullpath) == false) {
          return 1;
      } else {
        return 0;
      }
    }
    return 0;
  }

  static int selectDir(const struct dirent* dirEntry)
  {
    const char* name    = dirEntry->d_name;
    const char* pattern = setFilterPattern();
    const char* basedir = setBaseDirectory();
    char fullpath[PATH_MAX];
    sprintf(fullpath, "%s/%s", basedir, name);
    
    if(strcmp(name, ".") == 0 ||
       strcmp(name, "..") == 0 )
    {
        return 0;
    }
    //2015/01/21
    if (name[0] == '.' && skipDotDirectory()) {
    return 0;  
    }
    if (::fnmatch(pattern, name, 0) == 0) {
      if (isDir(fullpath)) {
          return 1;
      } else {
        return 0;
      }
    }
    return 0;
  }
 
public:
  DirectoryScanner(const char* dir)
  :Directory(dir)
  {
  }

  int scan(const char* pattern, DirEntryList& list)
  {
    const char* dir = getDirectory();
    struct dirent*** dirEntry = list.getDirEntry();
    setFilterPattern(pattern);
    int count = ::scandir(dir, dirEntry, 
      select, alphasort);
    list.setCount(count);
    return count;
  }

  int scanFile(const char* pattern, DirEntryList& list, bool* skip=NULL)
  {
    const char* dir = getDirectory();
    setBaseDirectory(dir);
    //2015/01/21
    skipDotFile(skip);
    
    struct dirent*** dirEntry = list.getDirEntry();
    setFilterPattern(pattern);
    int count = ::scandir(dir, dirEntry, 
      selectFile, alphasort);
    list.setCount(count);
    return count;
  }

  int scanDir(const char* pattern, DirEntryList& list, bool* skip=NULL)
  {
    const char* dir = getDirectory();
    setBaseDirectory(dir);

    //2015/01/21
    skipDotDirectory(skip);
    
    struct dirent*** dirEntry = list.getDirEntry();
    setFilterPattern(pattern);
    int count = ::scandir(dir, dirEntry, 
      selectDir, alphasort);
    list.setCount(count);
    return count;
  }

};


}


