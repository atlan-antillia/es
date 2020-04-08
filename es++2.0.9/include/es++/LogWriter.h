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
 *  LogWriter.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/LocalDateTime.h>


namespace Es {
  
class LogWriter :public Object {
  
public:
  LogWriter()
  {
  }
  

public:
  static 
  void err(const char* format,...) {    
    char buffer[LINE_MAX];
  
    va_list pos;
    va_start(pos, format);
    vsnprintf(buffer, LINE_MAX, format, pos);

    va_end(pos);

    write("ERR:", buffer);
  }
  
public:
  static 
  void debug(const char* format,...) {    
    char buffer[LINE_MAX];
  
    va_list pos;
    va_start(pos, format);
    vsnprintf(buffer, LINE_MAX, format, pos);

    va_end(pos);

    write("DEBUG:", buffer);
  }


public:
  static 
  void err(const wchar_t* format,...) {    
    wchar_t buffer[LINE_MAX];
  
    va_list pos;
    va_start(pos, format);
    vswprintf(buffer, LINE_MAX, format, pos);
    va_end(pos);

    write("ERR:", buffer);
  }
  
public:
  static 
  void debug(const wchar_t* format,...) {    
    wchar_t buffer[LINE_MAX];
  
    va_list pos;
    va_start(pos, format);
    vswprintf(buffer, LINE_MAX, format, pos);
    va_end(pos);

    write("DEBUG:", buffer);
  }
  
private:
  static
  void write(const char* level, const char* buffer) {

    char logFile[PATH_MAX];

    pid_t pid = getpid();
    
    sprintf(logFile, "pid_%d.log", pid);

    char cmdline[PATH_MAX];
    if (getCmdline(cmdline, sizeof(cmdline)) == 0) {
      sprintf(logFile, "%s.log", cmdline);
    }
    
    FILE* fp = fopen(logFile, "a+");
    if (fp != NULL) {
      //int fd = fileno(fp);
        //if (::flock(fd， LOCK_EX) == 0) {
      LocalDateTime localDateTime;
          
        char now[256];
          localDateTime.nowToSeconds(now, sizeof(now));

          fprintf(fp, "%s %s %s\n", now, level, buffer);
        //}
      //::flock(fd， LOCK_UN);
      
      fclose(fp);    
    }  
  }


private:
  static
  void write(const char* level, const wchar_t* buffer) {

    char logFile[PATH_MAX];

    pid_t pid = getpid();
    
    sprintf(logFile, "pid_%d.log", pid);

    char cmdline[PATH_MAX];
    if (getCmdline(cmdline, sizeof(cmdline)) == 0) {
      sprintf(logFile, "%s.log", cmdline);
    }
    
    FILE* fp = fopen(logFile, "a+");
    if (fp != NULL) {
      //int fd = fileno(fp);
        //if (::flock(fd， LOCK_EX) == 0) {
      LocalDateTime localDateTime;
          
        char now[256];
          localDateTime.nowToSeconds(now, sizeof(now)/sizeof(now[0]));

          fwprintf(fp, L"%s %s %ls\n", now, level, buffer);
        //}
      //::flock(fd， LOCK_UN);
      
      fclose(fp);    
    }  
  }

  
public:
  static 
  void dump(const char* message, const unsigned char* data, size_t len, 
      const char* format,...) 
  {
    char buffer[LINE_MAX];
  
    va_list pos;
    va_start(pos, format);
    vsnprintf(buffer, sizeof(buffer), format, pos);
    va_end(pos);
      
    char logFile[PATH_MAX];

    pid_t pid = getpid();
    
    sprintf(logFile, "pid_%d.log", pid);

    char cmdline[PATH_MAX];
    if (getCmdline(cmdline, sizeof(cmdline)) == 0) {
      sprintf(logFile, "%s.log", cmdline);
    }

    FILE* fp = fopen(logFile, "a+");
    if (fp != NULL) {
      LocalDateTime localDateTime;
      //int fd = fileno(fp);
        //if (::flock(fd， LOCK_EX) == 0) {
        char now[128];
        localDateTime.nowToSeconds(now, sizeof(now));

        fprintf(fp, "%s %s %s size=%zu\n", now, buffer, message, len);
        const int SIXTEEN = 16;
        int lines = len / SIXTEEN;
        int remainder = len % SIXTEEN;
        if (remainder) {
          lines++;
        }
        int  i, j, s;
        for (i = 0; i<lines; i++) {
          int MAX = SIXTEEN;
          if (remainder>0 && i == (lines-1)) {
            MAX = remainder;
          }
          for (s = 0; s<MAX; s++) {
            fprintf(fp, "%02X ", data[i*16+s]);
          }
          for(j = 0; j<SIXTEEN-MAX; j++) {
            fprintf(fp, "   ");
          }
          fprintf(fp, "    ");
          for (s = 0; s<MAX; s++) {
            char c = (char)data[i*16+s];
            if (c <' ') {
              c = '.';
            }
            fprintf(fp, "%c", c);
          }
          fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
        //}
      
      //::flock(fd， LOCK_UN);

      fclose(fp);
    }
  }

  
private:
  static 
  int getCmdline(char* line, size_t size) {
    
    static char filename[PATH_MAX] = { '\0' };
    int rc = -1;
    
    if (filename[0] == '\0') {
      
      pid_t pid = getpid();

      char cmdline[PATH_MAX];
      sprintf(cmdline, "/proc/%d/cmdline", pid);

      FILE* fp = fopen(cmdline, "r");
      if (fp) {
        if (fgets(filename, sizeof(filename), fp)) {
          char* lf = strrchr(filename, '\n');
          if (lf) {
            *lf = '\0';
          }
        }
        fclose(fp);  
      }
    } 
    
    if (filename[0] != '\0') {
      rc = 0;
      strcpy(line, filename);
    }
    return rc;
  }


};
/*
#define err(format, ...)                                  \
  do {                                                   \
    LogWriter::err(                                  \
      L"%s (%d) %s: " format, __FILE__, __LINE__,   \
      __func__ , ##__VA_ARGS__);                     \
  } while(0)

#define dump(message, data, len)                  \
  do {                                                        \
    LogWriter::dump(message, data, len,                    \
     "DUMP: %s (%d) %s: ", __FILE__, __LINE__, __func__);  \
           \
  } while(0)

#ifdef DEBUG

#define debug(format, ...)                                \
  do {                                                   \
    LogWriter::debug(                                  \
      L"%s (%d) %s: " format, __FILE__, __LINE__, \
      __func__ , ##__VA_ARGS__);                     \
  } while(0)
#else

#define debug(fmt, ...)

#endif
*/
}


