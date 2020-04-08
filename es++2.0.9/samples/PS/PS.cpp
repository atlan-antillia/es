/******************************************************************************
 *
 * Copyright (c) 2017  TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
 *  PS.cpp
 *
 *****************************************************************************/


#include <es++/Locale.h>
#include <es++/Directory.h>

int main(int argc, char* argv[])
{
  Locale locale;
  fwide(stdout, 0);
  try {
    const char* dir = "/proc";
    CharString pattern("*");
    Directory directory(dir);

    LinkedListT<FileAttributes> dirAttrList;

    int m = directory.getDirList(pattern, dirAttrList);
    printf("DirAttrList m=%d\n", m);
    dirAttrList.sort(Sortable::DESCENDING);


    ListEntryT<FileAttributes>* entry = dirAttrList.getEntry();
     while (entry) {
       FileAttributes* attr = entry -> getObject();
      if (attr) {
        const char* fullpath = (const char*)attr->getPath();
        const char* name = strrchr(fullpath, '/');
        if (name) {
          name++;
        } 
        //printf("fullpath :%s  %s\n", fullpath, pid);
        pid_t pid = atoi(name);

        if (pid > 0) {
          char cmdline[PATH_MAX];
          sprintf(cmdline, "%s/%s", fullpath, "cmdline");
          FILE* fp = fopen(cmdline, "rt");
          if (fp) {
            char process[PATH_MAX];
            int r = fscanf(fp, "%s", process);
            if (r > 0) {
              printf("PID=%d Name=%s\n", pid, process);
            }
            fclose(fp);
          }
        }
      }
      entry = entry -> getNext();
 
     }

  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}
