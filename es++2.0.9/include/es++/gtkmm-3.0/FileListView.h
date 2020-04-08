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
 *  FileListView.h
 *
 *****************************************************************************/

#pragma once

#include <es++/DirectoryScanner.h>
#include <es++/MonetaryFormatter.h>
#include <es++/FileAttributes.h>
#include <es++/LocalTime.h>
#include <es++/SmartArray.h>
#include <es++/SmartPtr.h>
#include <es++/SmartPtrs.h>
#include <es++/gtkmm-3.0/TextListView.h>

namespace Es {

class FileListView :public Es::TextListView {
private:
  static const int COLUMN_COUNT = 3; //name, size, datetime
public:
  FileListView()
  :TextListView(COLUMN_COUNT)
  {
  }

  void add_directories(const char* dir,char* items[], int num)
  {
    char path[PATH_MAX];
    setlocale(LC_NUMERIC, "");

    for (int i = 0; i<num; i++) {
      sprintf(path, "%s/%s", dir, items[i]);
      try {
        FileAttributes attr(path);

        LocalTime lt(attr.changedTime());
        std::string changed_time = lt.toCharString();
        char csize[128];
        unsigned long fs = attr.getSize();
        double kb = (double)fs/1024.0L;
        sprintf(csize, "%'0.1fKB", kb);
        const char* text[COLUMN_COUNT] = {
              items[i],
              csize,
              //(const char*)changed_time
              changed_time.c_str()
        };
        append_text(text, COLUMN_COUNT);
      } catch (...) {
      }
    } //for

    setlocale(LC_ALL, "");
  }

  void add_files(const char* dir,char* items[], int num)
  {
    char path[PATH_MAX];
    setlocale(LC_NUMERIC, "");

    for (int i = 0; i<num; i++) {
      sprintf(path, "%s/%s", dir, items[i]);
      try {
        FileAttributes attr(path);
        
        LocalTime lt(attr.changedTime());
        std::string changed_time = lt.toCharString();
        char csize[128];
        unsigned long fs = attr.getSize();
        double kb = (double)fs/1024.0L;
        sprintf(csize, "%'0.1fKB", kb);
        const char* text[COLUMN_COUNT] = {
              items[i], 
              csize, 
              changed_time.c_str()
        };
        append_text(text, COLUMN_COUNT);
      } catch (...) {
      }
    } //for
   
    setlocale(LC_ALL, "");
  }
               
  void listup_directories(const char* dir, const char* pattern)
  {

    DirectoryScanner dirScanner(dir);
    DirEntryList dirList;
    int m = dirScanner.scanDir(pattern, dirList);
    if (m > 0) {
      SmartPtrs<char*> dirNames = new char*[m];
      dirList.getNames((char**)dirNames);
      add_directories(dir, (char**)dirNames, m);
    }
  }

  void listup_files(const char* dir, const char* pattern)
  {
    DirectoryScanner fileScanner(dir);
    DirEntryList fileList;
    int n = fileScanner.scanFile(pattern, fileList);
    if (n > 0) {
      SmartPtrs<char*> fileNames = new char*[n];
      fileList.getNames((char**)fileNames);
      add_files(dir, (char**)fileNames, n);
    } 
  }

};

}


