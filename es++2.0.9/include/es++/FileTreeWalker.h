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
 *  FileTreeWalker.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/CharString.h>
#include <es++/Pair.h>
#include <es++/Exception.h>
#include <es++/SmartPtr.h>
#include <ftw.h>


namespace Es {

class FileTreeWalker :public Object {
private:
  CharString path;
  int        flags;
  int        openfiles;

public:
  static FileTreeWalker* getInstance(
        const char* path=NULL, 
		int flags=FTW_DEPTH|FTW_PHYS, int openfiles=20)
  {
     static FileTreeWalker* walker = NULL;

  	if (walker == NULL && path !=NULL) {
        walker = new FileTreeWalker(path, flags, openfiles); 
     }
     return walker;
  }

private:
  //Constructor
  FileTreeWalker(const char* path, 
		int flags, int openfile)
  :path(path),
   flags(flags),
   openfiles(openfiles)
  {
  }
	
public:
  ~FileTreeWalker()
  {
  	//printf("~FileTreeWalker\n");
  }
	
public:
  void start()
  {
  	//This enables to delete the singletoninstance of this class when this method ends.
  	SmartPtr<FileTreeWalker> walker(this);
  	
    if (nftw(path, &(FileTreeWalker::callback), openfiles, flags) == -1) {
    	throw Es::IException("Failed to nftw:%s", strerror(errno));
    } else {
    	printf("Finished \n");    	
    }
  }
  

public:
  //If you would like to customize the enumerate method,
  //please define a subclass of FileTreeWalker and define you own enumerate method
  virtual int enumerate(const char* filepath, const struct stat* sb,
                     int tflag, struct FTW* ftwbuf)
  {
    //Do something here on your own requirement.
    printf("%-4s %7jd %s\n", getFileType(tflag), (intmax_t)sb->st_size, filepath);
  	
    return 0;
  }

private:
  static int callback(const char* filepath, const struct stat* sb,
                     int tflag, struct FTW* ftwbuf)
  {
  	int rc = 0;
    FileTreeWalker* walker = getInstance();
  	if (walker) {
      rc = walker->enumerate(filepath, sb, tflag, ftwbuf);
  	} else {
  		printf("FileTreeWalker is NULL\n");
  	}
  	return rc;
  }
	
private:
  const char* getFileType(int type)
  {
    static const Pair<int, const char*> fileTypes[] = 
    {
  	{FTW_D,    "d"},
  	{FTW_DNR,  "dnr"},
  	{FTW_DP,   "dp"},
  	{FTW_F,    "f"},
  	{FTW_NS,   "ns"},
  	{FTW_SL,   "sl"},
  	{FTW_SLN, "sln"},
  	};
	
  	const char* string ="???";
  	for(int i = 0; i< SizeOf(fileTypes); i++) {
      if (fileTypes[i].first == type) {
       	string = fileTypes[i].second;
       	break;
      }
    }
    return string;
  }

};

}

