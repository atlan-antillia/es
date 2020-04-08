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
 *  DirectoryScanner.cpp
 *
 *****************************************************************************/


#include <es++/Locale.h>
#include <es++/DirectoryScanner.h>

int main(int argc, char* argv[])
{
  Locale locale;
  if (argc != 3) {
    printf("%s folder pattern\n", argv[0]);
    return 0;
  }
  try {
    const char* dir = argv[1];
    CharString pattern(argv[2]);

    DirectoryScanner scanner(dir);
    DirEntryList fileList;

    //2015/01/21 Modified to pass a skipDotFile flag.
    bool skipDotFile = true;
    int n = scanner.scanFile(pattern, fileList, &skipDotFile);
    printf("FileList n=%d\n", n);
    fileList.display();
    printf("\n");

    DirEntryList dirList;

    //2015/01/21 Modified to pass a skipDotDir flag.
    bool skipDotDir = false;
    //int m = scanner.scanDir(pattern, dirList);
    int m = scanner.scanDir(pattern, dirList, &skipDotDir);
    printf("DirList m=%d\n", m);

    dirList.display();
    
  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}
