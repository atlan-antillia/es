/******************************************************************************
 *
 * Copyright (c) 2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  SystemInformation.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>

#include <sys/sysinfo.h>


/*
On Linux 2.3.23 (i386)、2.3.48 
struct sysinfo {
  long uptime;		        // Seconds since boot 
  unsigned long loads[3];	// 1, 5, and 15 minute load averages 
  unsigned long totalram;	// Total usable main memory size
  unsigned long freeram;	// Available memory size 
  unsigned long sharedram;      // Amount of shared memory 
  unsigned long bufferram;      // Memory used by buffers 
  unsigned long totalswap;      // Total swap space size 
  unsigned long freeswap;	// swap space still available 
  unsigned short procs;	        // Number of current processes 
  unsigned long totalhigh;      // Total high memory size 
  unsigned long freehigh;	// Available high memory size
  unsigned int mem_unit;	// Memory unit size in bytes 
  char _f[20-2*sizeof(long)-sizeof(int)]; // Padding for libc5 
 };

 */

namespace Es {

class SystemInformation :public Object {
private:
  struct sysinfo info;

public:
  SystemInformation()
  {
    memset(&info, 0, sizeof(info));
    int rc = sysinfo(&info);
    if (rc != 0) {
      throw Es::IException("Failed to sysinfo");
    }
  }

  long upTime()
  {
    return info.uptime;                  // Seconds since boot
  } 
  
  unsigned long loadAverage1Minute()
  {
    return info.loads[0];       // 1, 5, and 15 minute load averages
  }

  unsigned long loadAverage5Minute()
  {
    return info.loads[1];       // 1, 5, and 15 minute load averages
  }

  unsigned long loadAverage15Minute()
  {
    return info.loads[2];       // 1, 5, and 15 minute load averages
  }
  unsigned long totalRam()
  {
    return info.totalram;       // Total usable main memory size
  }

  unsigned long freeRam()
  {
    return info.freeram;        // Available memory size
  }

  unsigned long sharedRam()
  {
    return info.sharedram;      // Amount of shared memory
  }

  unsigned long bufferRam()
  {
    return info.bufferram;      // Memory used by buffers
  }

  unsigned long totalSwap()
  {
    return info.totalswap;      // Total swap space size
  }

  unsigned long freeSwap()
  {
    return info.freeswap;       // swap space still available
  }

  unsigned short numberOfProcesses()
  {
    return info.procs;         // Number of current processes
  }

  unsigned long totalHighMemory()
  {
    return info.totalhigh;      // Total high memory size
  }

  unsigned long freeHighMemory()
  {
    return info.freehigh;       // Available high memory size
  }

  unsigned int memoryUnit()
  {
     return info.mem_unit;        // Memory unit size in bytes
  }

  virtual void display()
  {
    printf("UpTime              %lu\n", upTime());
    printf("LoadAverage1Minute  %lu\n", loadAverage1Minute());
    printf("LoadAverage5Minute  %lu\n", loadAverage5Minute());
    printf("LoadAverage15Minute %lu\n", loadAverage15Minute());
    printf("TotalRam            %lu\n", totalRam());
    printf("FreeRam             %lu\n",  freeRam());
    printf("SharedRam           %lu\n",  sharedRam());
    printf("BufferRam           %lu\n",  bufferRam());
    printf("TotalSwap           %lu\n", totalSwap());
    printf("FreeSwap            %lu\n", freeSwap());
    printf("NumberOfProcesses   %u\n", numberOfProcesses());
    printf("TotalHighMemory     %lu\n", totalHighMemory());
    printf("FreeHighMemory      %lu\n", freeHighMemory());
    printf("MemoryUnit          %du\n",  memoryUnit());
  }
};

}

