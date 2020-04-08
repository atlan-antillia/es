/******************************************************************************
 *
 * Copyright (c) 2015 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  JPGFile.h
 *
 *****************************************************************************/

#pragma once

#define PNG_SKIP_SETJMP_CHECK

#include <es++/Object.h>
#include <es++/Exception.h>
#include <jpeglib.h>
//2019/4/04
#include <es++/ModuleFileName.h>

namespace Es {

class JPGFile :public Object {
protected:
  typedef enum {
    READER = 0,
    WRITER = 1,
  } OPERATION;

private:
  FILE* fp;
  OPERATION operation;

private:
  JPGFile()
  {
  }

protected:
  JPGFile(OPERATION operation,const char* filename)
  :fp(NULL),
  operation(operation)
  {
    if (filename == NULL) {
      throw Es::IException("Invalid argument");
    }
    static const char* mode[] =  { "rb", "wb" };
    ModuleFileName module;
    //2019/04/05
    std::string path = filename;
    if (filename[0] != '/') {
      path = module.getPath();
      path += "/";
      path += filename;
    }

    fp = fopen(path.c_str(), mode[operation]);
    if (fp == NULL) {
      throw Es::IException("Failed to open %s", filename);
    }
  }

  ~JPGFile()
  {
  	close();
  }

  FILE* getFP()
  {
    return fp;
  }
	
  void close()
  {
    if (fp) {
     fclose(fp);
     fp = NULL;
    }
  }
	
};

}
 
