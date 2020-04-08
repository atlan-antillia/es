/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 * DirEntryList.h
 *
 *****************************************************************************/

#pragma once
#include <es++/Object.h>
#include <es++/CharString.h>
#include <es++/Exception.h>
#include <err.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <fnmatch.h>

namespace Es {
  
class DirEntryList {
private:
  struct dirent** dirEntry;

  int    count;

public:
   DirEntryList()
  :dirEntry(NULL),
   count(0)
  {
  
  }
   
  ~DirEntryList()
  {
     clear();
  }

  struct dirent*** getDirEntry()
  {
    return &dirEntry;
  }
  

  void setCount(int n)
  {
     count = n;
  }

  const char* getName(int i)
  {
    const char* name = "";
    if (dirEntry &&  (i >= 0 && i <count) ){
      name = dirEntry[i] ->d_name;
    }
    return name;
  }

  void clear()
  {
     if (dirEntry) {
        for(int i = 0; i<count; i++) {
            free(dirEntry[i]);
        }
        free(dirEntry); 
        dirEntry = NULL;
     }
  }

  void display()
  {
     if (dirEntry) {
        for(int i = 0; i<count; i++) {
      printf("%s\n", dirEntry[i]->d_name);
        }
     }
  }
  
  
  //2015/01/28  
  int getNames(char** names)
  {
    //const char** names = NULL;
    if (names) {
      if (count > 0) {
        //names = new const char*[count];
        for(int i = 0; i<count; i++) {
          //Make a shallow copy
          names[i] = dirEntry[i]->d_name;
        }
      }
    } else {
      throw Es::IException("Invalid argument");  
    }
    return count; //names;
  }
  
  //2015/01/28  
  int getNamesWithPrefix(char** names, const char* prefix)
  {
    //const char** names = NULL;
    if (names && prefix) {
      if (count > 0) {
        //names = new const char*[count];
        for(int i = 0; i<count; i++) {
       //Make a deep copy
        int len = 0;
        if (dirEntry[i]->d_name) {
          len = strlen(dirEntry[i]->d_name);
        }
        char* name = new char[len + strlen(prefix) + 1 + 1];
        sprintf(name, "%s %s", prefix, dirEntry[i]->d_name);
        names[i] = name;
        }
      }
    } else {
      throw Es::IException("Invalid argument");  
    }
    return count; //names;
  }
  
};

}

