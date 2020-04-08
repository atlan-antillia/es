/******************************************************************************
 *
 * Copyright (c) 2014  TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
 *  ProcStat.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/CharString.h>
#include <es++/StringTokenizerT.h>
#include <es++/Pair.h>

namespace Es {
    
class ProcStat :public Object {

//See: http://man7.org/linux/man-pages/man5/proc.5.html
enum STAT {
    PID,            //pid %d 
    COMM,           //comm %s
    STATE,          //state  %c 
    PPID,           // ppid %d
    PGRP,           // pgrp  %d
    SESSION,        //session  %d
    TTY_NR,         //tty_nr  %d
    TPGID,          //tpgid  %d 
    FLAGS,          //flags  %u
    MINFLT,         //minflt  %lu
    CMINFLAT,       // cminflt  %lu
    MAJFLT,         //majflt  %lu
    CMAJFLT,        //cmajflt  %lu
    UTIME,          //utime  %lu
    STIME,          //stime  %lu
    CUTIME,         //cutime  %ld
    CSTIME,         //cstime  %ld
    PRIORITY,       //priority  %ld
    NICE,           //  %ld
    NUM_THREADS,    //num_threads  %ld
    ITRAVELVALUE,   //itrealvalue  %ld
    STARTTIME,      //starttime  %llu
    VSIZE,          //vsize  %lu
    RSS,            //rss  %ld
    RSSLIM,         //rsslim  %lu
    STARTCODE,      //startcode  %lu
    ENDCODE,        //endcode  %lu
    STARTSTACK,     //startstack  %lu
    KSTKESP,        //kstkesp  %lu
    KSTKEIP,        //kstkeip  %lu
    SIGNAL,         //signal  %lu
    BLOCKED,        //blocked  %lu
    SIGIGNORE,      //sigignore  %lu
    SIGCATCH,       //sigcatch  %lu
    WCHAN,          //wchan  %lu
    NSWAP,          //nswap  %lu
    CNSWAP,         //cnswap  %lu
    EXIT_SIGNAL,    //exit_signal  %d
    PROCESSOR,      //processor  %d
    RT_PRIORITY,    //rt_priority  %u
    POLICY,         //policy  %u
    DELAYACCT_BLKIO_TICKS, //delayacct_blkio_ticks  %llu
    GUEST_TIME,     //guest_time  %lu
    CGUEST_TIME,    //cguest_time %ld

    START_DATA,     //start_data  %lu  
    END_DATA,       //end_data   %lu
    START_BRK,      //start_brk  %lu
    ARG_START,      //arg_start  %lu
    ARG_END,        //arg_end  %lu  
    ENV_START,      //env_start  %lu  
    ENV_END,        //env_end  %lu
    EXIT_CODE,      //exit_code  %d
    
};
    
static const int STAT_SIZE = 52; //44;
    
  CharString fullpath;
  CharString* stat[STAT_SIZE];
    
public:
    ProcStat(const char* statFile) //  statFile = "/proc/[pid]/stat"
    {
        fullpath = statFile;
        
        read();
    }

public:
    ~ProcStat()
    {
        for (int i = 0; i<STAT_SIZE; i++) {
            if (stat[i]) {
                delete stat[i];
                stat[i] = NULL;
            }
        }
    }
    
public:
    CharString getPath()
    {
        return fullpath;
    }
    
public:
    void read() {
        //Nullify stat array.
        for (int i = 0; i<STAT_SIZE; i++) {
            stat[i] = NULL; 
        }
        
        //Open and read the /proc/[pid]/stat file.
        FILE* fp = fopen((const char*)fullpath, "r");
        if (fp) {
            const int BUFFER_SIZE = 1024*2;
            const int TOKEN_SIZE  = 512;
            char line[BUFFER_SIZE];
            memset(line, 0, sizeof(line));
            
            if (fgets(line, sizeof(line), fp)) {
                line[strlen(line)-1] = '\0';
            }
            fclose(fp);
            
            StringTokenizerT<char> tokenizer(line);
            int n = 0;
            while(tokenizer.hasMoreToken()) {
                char token[TOKEN_SIZE];
                token[0] = (char)0;
                tokenizer.getToken(token, SizeOf(token));
                if (n <STAT_SIZE) {
                    stat[n++] = new CharString(token);
                } else {
                    break;
                }
            }
            
        } else {
            printf("Failed to open a file %s\n", (const char*)fullpath);
        }
    }
    
  //2014/12/08
public:
  int compare(Object* object)
  {
    int rc = 0;
    if (object==NULL) {
      return rc;
    }
    
    //compare this->fullpath and object->fullpath
    const char* p1 = (const char*)this->fullpath;

    ProcStat* attr = (ProcStat*)object;
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
        for (int i = 0; i<STAT_SIZE; i++) {
            const char* name = indexToName(i);
            wprintf(L"%s : %s\n", name, (const char*)(*stat[i]) );
        }
        wprintf(L"\n");
    }
    
    
    pid_t getPid() {
        const char* string = (const char*)(*stat[PID]);
        return atoi(string);
    }
    
    const char* getComm()
    {
        return (const char*)(*stat[COMM]);
    }
    
    pid_t getPPid()
    {
        const char* string = (const char*)(*stat[PPID]);
        return atoi(string);
    }
    
    pid_t getPgrp()
    {
        const char* string = (const char*)(*stat[PGRP]);
        return atoi(string);
    }
    
    int getSession()
    {
        const char* string = (const char*)(*stat[SESSION]);
        return atoi(string);
    }
    
    int getTTyNr() 
    {
        const char* string = (const char*)(*stat[TTY_NR]);
        return atoi(string);        
    }
    
    int getTPGid() 
    {
        const char* string = (const char*)(*stat[TPGID]);
        return atoi(string);        
    }
    
    unsigned int getFlags() 
    {
        const char* string = (const char*)(*stat[FLAGS]);
        return atoi(string);        
    }
    
    unsigned long getMinflt() 
    {
        const char* string = (const char*)(*stat[MINFLT]);
        return atoi(string);        
    }
    
public:
    const char* indexToName(int n)
    {
    Pair<int, const char*> pairs[] = {
        {PID,           "pid"},
        {COMM,          "comm"}, 
        {STATE,         "state"},
        {PPID,          "ppid"},
        {PGRP,          "pgrp"},
        {SESSION,       "session"},
        {TTY_NR,        "tty_nr"},
        {TPGID,         "tpgid"},
        {FLAGS,         "flags"},
        {MINFLT,        "minflt"},
        {CMINFLAT,      "cminflt"},
        {MAJFLT,        "majflt"},
        {CMAJFLT,       "cmajflt"},
        {UTIME,         "utime"},
        {STIME,         "stime"}, 
        {CUTIME,        "cutime"},
        {CSTIME,        "cstime"},
        {PRIORITY,      "priority"}, 
        {NICE,          "nice"}, 
        {NUM_THREADS,   "num_threads"}, 
        {ITRAVELVALUE,  "itravelvalue"},
        {STARTTIME,     "starttime"},
        {VSIZE,         "vsize"},
        {RSS,           "rss"},
        {RSSLIM,        "rsslim"},
        {STARTCODE,     "startcode"},
        {ENDCODE,       "endcode"},
        {STARTSTACK,    "startstack"},
        {KSTKESP,       "kstkesp"},
        {KSTKEIP,       "kstkeip"},
        {SIGNAL,        "signal"},
        {BLOCKED,       "blocked"},
        {SIGIGNORE,     "sigignore"},
        {SIGCATCH,      "sigcatch"},
        {WCHAN,         "wchan"},
        {NSWAP,         "nswap"},
        {CNSWAP,        "cnswap"},
        {EXIT_SIGNAL,   "exit_signal"},
        {PROCESSOR,     "processor"},
        {RT_PRIORITY,   "rt_priority"},
        {POLICY,        "policy"}, 
        {DELAYACCT_BLKIO_TICKS, "delayacct_blkio_ticks"}, 
        {GUEST_TIME,    "guest_time"},
        {CGUEST_TIME,   "cguest_time"},
        
        {START_DATA,     "start_data"},
        {END_DATA,       "end_data"}, 
        {START_BRK,      "start_brk"},
        {ARG_START,      "arg_start"},
        {ARG_END,        "arg_end"},
        {ENV_START,      "env_start"},
        {ENV_END,        "env_end"}, 
        {EXIT_CODE,      "exit_code"},

    };
        
        const char* name = "";
        int size = SizeOf(pairs);
        
        for (int i = 0; i<size; i++) {
            if (pairs[i].first == n) {
                name = pairs[i].second;
                break;
            }
        }
        return name;
    }
};

}

