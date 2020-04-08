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
 *  Directory.cpp
 *
 *****************************************************************************/


#include <es++/Locale.h>
#include <es++/Directory.h>

int main(int argc, char* argv[])
{
  Locale locale;
  fwide(stdout, 0);
  if (argc != 3) {
    printf("%s folder pattern\n", argv[0]);
    return 0;
  }
  try {
    const char* dir = argv[1];
    CharString pattern(argv[2]);
    Directory directory(dir);

    LinkedList fileList;

    int n = directory.getFileList(pattern, fileList);
    printf("FileList n=%d\n", n);
    fileList.sort(Sortable::ASCENDING);
    fileList.display();

    LinkedList dirList;

    int m = directory.getDirList(pattern, dirList);
    printf("DirList m=%d\n", m);
    dirList.sort(Sortable::DESCENDING);

    dirList.display();

    LinkedListT<FileAttributes> fileAttrList;

    n = directory.getFileList(pattern, fileAttrList);
    printf("FileAttrList n=%d\n", n);

    fileAttrList.sort(Sortable::ASCENDING);
    fileAttrList.display();

    LinkedListT<FileAttributes> dirAttrList;

    m = directory.getDirList(pattern, dirAttrList);
    printf("DirAttrList m=%d\n", m);
    dirAttrList.sort(Sortable::DESCENDING);

    dirAttrList.display();

  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}
