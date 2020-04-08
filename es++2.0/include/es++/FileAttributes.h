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
 *  FileAttributes.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <es++/StringT.h>
#include <es++/CharString.h>

#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>


/*
struct stat {
  dev_t     st_dev;     // ID of device containing file 
  ino_t     st_ino;     // inode number 
  mode_t    st_mode;    // protection 
  nlink_t   st_nlink;   // number of hard links 
  uid_t     st_uid;     // user ID of owner 
  gid_t     st_gid;     // group ID of owner 
  dev_t     st_rdev;    // device ID (if special file) 
  off_t     st_size;    // total size, in bytes 
  blksize_t st_blksize; // blocksize for file system I/O 
  blkcnt_t  st_blocks;  // number of 512B blocks allocated 
  time_t    st_atime;   // time of last access
  time_t    st_mtime;   // time of last modification 
  time_t    st_ctime;   // time of last status change 
 };
*/

namespace Es {

class FileAttributes :public Es::Object {
public:
  static enum {
  	DEV     = (1 << 0), 
  	INO     = (1 << 1),
  	MODE    = (1 << 2), 
  	NLINK   = (1 << 3),  
  	UID     = (1 << 4), 
  	GID     = (1 << 5), 
  	RDEV    = (1 << 6), 
  	SIZE    = (1 << 7 ),  
  	BLKSIZE = (1 << 8),
  	BLOCKS  = (1 << 9),
  	ATIME   = (1 << 10),
  	MTIME   = (1 << 11),
  	CTIME   = (1 << 12), 
  } ATTRIBUTES;
	
  static const int MAX = (DEV |INO | MODE |NLINK |\
         UID | GID | RDEV |SIZE | BLKSIZE | BLOCKS |ATIME |MTIME|CTIME); 

  static const int MIN = (MODE |UID |GID | SIZE | MTIME);

  static int displayItems(int items = 0)
  {
    static int dispItems = MIN;
    if (items >0) {
      dispItems = items;	
    }
    return dispItems;
  }
	
private:
  struct stat st;
  //2014/12/05
  CharString fullpath;

public:
  FileAttributes(const struct stat& st, const char* path)
  :st(st),
  fullpath(path)
  {
  }

public:
  FileAttributes(const char* path)
  {
    fullpath = path;
    if( stat(path, &st) != 0) {
      throw Exception("Failed to stat(%s); %s", path, error() );
    }
  }

public:
  CharString& getPath() 
  {
    return fullpath;
  }

public:
  dev_t getDeviceID() 
  {
    return st.st_dev;
  }
  
  ino_t getINode() 
  {
    return st.st_ino;
  }
  
  
  mode_t getMode() 
  {
    return st.st_mode;
  }
	
  //2014/12/05
  bool isRegularFile() 
  {
    return S_ISREG(st.st_mode);
  } 

  bool isDirectory() 
  {
    return S_ISDIR(st.st_mode);
  }
   
  bool isCharcterDevice()
  {
    return S_ISCHR(st.st_mode);
  }
	
  
  bool isBlockDevice()
  {
    return S_ISBLK(st.st_mode);
  }
   
  bool isFIFO()
  {
    return S_ISFIFO(st.st_mode);
  }
   
  bool isSymbolicLink()
  {
    return S_ISLNK(st.st_mode);
  }

  bool isSocket()
  {
    return S_ISSOCK(st.st_mode);
  }

  bool isExecutable()
  {
    bool rc = false;
    if ((st.st_mode & S_IXUSR) ||
        (st.st_mode & S_IXGRP) ||
        (st.st_mode & S_IXOTH) ) {
      rc = true;
    }
    return rc;
  }


  //
  CharString modeToString( )
  {
    char string[11];
    memset(string, '-', 10);
    string[10] = '\0';

    if (S_ISDIR(st.st_mode)) string[0] = 'd';    
    if (S_ISCHR(st.st_mode)) string[0] = 'c';    
    if (S_ISBLK(st.st_mode)) string[0] = 'b';    
  	
    if (st.st_mode & S_IRUSR) string[1] = 'r';    
    if (st.st_mode & S_IWUSR) string[2] = 'w';
    if (st.st_mode & S_IXUSR) string[3] = 'x';

    if (st.st_mode & S_IRGRP) string[4] = 'r';    
    if (st.st_mode & S_IWGRP) string[5] = 'w';
    if (st.st_mode & S_IXGRP) string[6] = 'x';

    if (st.st_mode & S_IROTH) string[7] = 'r';   
    if (st.st_mode & S_IWOTH) string[8] = 'w';
    if (st.st_mode & S_IXOTH) string[9] = 'x';

    return CharString(string);
  }


  nlink_t getNLink() 
  {
    return st.st_nlink;
  }
  
  uid_t getUid() 
  {
    return st.st_uid;
  }
	
  CharString uidToName()
  {
    const char* name = "";
    struct passwd* pw = getpwuid(st.st_uid);
    if (pw) {
      name = pw->pw_name;
    }
    return CharString(name);
  }
	
  gid_t getGid() 
  {
    return  st.st_gid;
  }
  
  CharString gidToName()
  {
    const char* name = "";
    struct group* gr = getgrgid(st.st_gid);
    if (gr) {
      name = gr->gr_name;
    }
    return CharString(name);
  }
	
  dev_t getRDev()
  {
    return st.st_rdev;
  }
  
  off_t getSize() 
  {
    return st.st_size;
  }
  
  long getSizeKB()
  {
    long size = getSize();
    long kbsize = size/1024;
    int  remain = (int)(size % 1024);
    if (kbsize == 0 && remain > 0) {
      kbsize = 1;
    }
    return kbsize;
  }


  blksize_t getBlokSize()
  {
    return st.st_blksize;
  }
  
  blkcnt_t getBlocks()
  {
    return  st.st_blocks;
  }
  
  void localTime(time_t& timer, StringT<char>& string)
  {
    struct tm tm;
    localtime_r(&timer, &tm);

    char buff[80];
    memset(buff, 0, sizeof(buff));

    snprintf(buff, sizeof(buff),
      "%04d/%02d/%02d %02d:%02d:%02d",
      tm.tm_year+1900,
      tm.tm_mon+1,
      tm.tm_mday,
      tm.tm_hour,
      tm.tm_min,
      tm.tm_sec);
    string = buff;
  }

  const char* getAccessTime()
  {
    return ctime(&st.st_atime);
  }
  
  void getAccessTime(StringT<char>& string)
  {
    time_t timer = st.st_atime;
    localTime(timer, string);
  }

  const char* getModifiedTime()
  {
    return ctime(&st.st_mtime);
  }
  
  void getModifiedTime(StringT<char>& string)
  {
    time_t timer = st.st_mtime;
    localTime(timer, string);
  }
  
  const char*  getCurrentTime()
  {
    return ctime(&st.st_ctime);
  }
	
  void getCurrentTime(StringT<char>& string)
  {
    time_t timer = st.st_ctime;
    localTime(timer, string);
  }
  
  time_t accessesTime()
  {
    return st.st_atime;   // time of last access
  }

  time_t modifiedTime()
  {
    return st.st_mtime;   // time of last modification 
  }
	
  time_t   changedTime()
  {
    return st.st_ctime;   // time of last status change 
  }
		
  //2014/12/05
public:
  int compare(Es::Object* object)
  {
    int rc = 0;
    if (object==NULL) {
      return rc;
    }
    const char* p1 = (const char*)this->fullpath;

    FileAttributes* attr = (FileAttributes*)object;
    const char* p2 = (const char*)attr->getPath();
  	
    if (p1 == NULL && p2 == NULL) {
      rc = 1;
    }
    if (p1 != NULL && p2 != NULL) {
      rc = __strcmp(p1, p2);
    }
    return rc;
  }
	
public:
  void display()
  {
    int items = displayItems();
  	
    printf("path : %s\n",    (const char*)fullpath);
    if (items & DEV) {
      printf("device_id : %llu\n",    (long long unsigned int)st.st_dev);
    }
    if (items & INO) {
      printf("inode: %ld\n",      st.st_ino);
    }
    if (items & MODE) {
      printf("mode: %s\n",    (const char*)modeToString());
    }
    if (items & NLINK) {
      printf("nlink: %d\n",  (int)st.st_nlink);
    }
    if (items & UID) {
      printf("uid :  %s\n",  (const char*)uidToName());
    }
    if (items & GID) {
      printf("gid:   %s\n",  (const char*)gidToName());
    }
    if (items & RDEV) {
      printf("rdev:  %llu\n",  (long long unsigned int)st.st_rdev);
    }
    if (items & SIZE) {
      printf("size:  %ld\n",  st.st_size);
    }
    if (items & BLKSIZE) {
      printf("block_size: %ld\n", st.st_blksize);
    }
    if (items & BLOCKS) {
      printf("blocks: %ld\n",  st.st_blocks);
    }
    if (items & CTIME) {
      printf("access_time:   %s",ctime(&st.st_atime));
    }
    if (items & MTIME) {
      printf("modified_time: %s",ctime(&st.st_mtime));
    }
    if (items & CTIME) {
      printf("current_time:  %s",ctime(&st.st_ctime));
    }
    printf("\n");
  }
};
}

