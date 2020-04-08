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
 *  SignalParser.h
 *
 *****************************************************************************/

//2015/06/01 Modified to use Callback class.

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <sys/signalfd.h> 
#include <es++/Signal.h> 
#include <es++/Callback.h> 

namespace Es {
// Renaming signalfd_siginfo to SignalInfo.
typedef struct signalfd_siginfo SignalInfo;

// Deining to a pointer to a method for handling a catched signal. 
typedef void (Object::*Signaled)(SignalInfo&);

class SignalParser :public Object {
private:
  static const int SIGINFO_SIZE = 128;
 
private:
  //Read buffer to read signalfd_siginfo from signalfd.
  size_t      arraySize;
  SignalInfo* siginfoArray;

  //Signaled callback which can be called parse method .
  Callback<Object*, Signaled, SignalInfo> callback; 

public:
  SignalParser(const size_t size = SIGINFO_SIZE)
  :arraySize(size),
   siginfoArray(NULL)
  {
    if (size < 1) {
      throw Es::IException("Invalid argument");
    }
    siginfoArray = new SignalInfo[size];
  }

  ~SignalParser()
  {
    delete [] siginfoArray;
    siginfoArray = NULL;
  }

  void setCallback(Object* object, Signaled signaled)
  {
    callback.set(object, signaled);
  }

/*
struct signalfd_siginfo {
  uint32_t ssi_signo;   // Signal number 
  int32_t  ssi_errno;   // Error number (unused) 
  int32_t  ssi_code;    // Signal code 
  uint32_t ssi_pid;     // PID of sender 
  uint32_t ssi_uid;     // Real UID of sender 
  int32_t  ssi_fd;      // File descriptor (SIGIO) 
  uint32_t ssi_tid;     // Kernel timer ID (POSIX timers)
  uint32_t ssi_band;    // Band event (SIGIO) 
  uint32_t ssi_overrun; // POSIX timer overrun count 
  uint32_t ssi_trapno;  // Trap number that caused signal 
  int32_t  ssi_status;  // Exit status or signal (SIGCHLD) 
  int32_t  ssi_int;     // Integer sent by sigqueue(3) 
  uint64_t ssi_ptr;     // Pointer sent by sigqueue(3) 
  uint64_t ssi_utime;   // User CPU time consumed (SIGCHLD) 
  uint64_t ssi_stime;   // System CPU time consumed (SIGCHLD) 
  uint64_t ssi_addr;    // Address that generated signal
                        //   (for hardware-generated signals) 
  uint8_t  pad[X];      // Pad size to 128 bytes (allow for
                        //    additional fields in the future) 
};
 */

  virtual void display(SignalInfo info)
  {
    const char* name = Signal::toString(info.ssi_signo);
    printf("SignalFDSigInfo::display signo=%d %s\n",
             info.ssi_signo, name);  
  }

  virtual void callCallback(SignalInfo siginfo)
  {
    callback.call(siginfo);
  }

  struct signalfd_siginfo* getBuffer()
  {
    return siginfoArray;
  }

  size_t getBufferSize()
  {
    return sizeof(struct signalfd_siginfo) * arraySize;
  }

  void parse(size_t bsize) 
  {
    size_t count = bsize/sizeof(SignalInfo);
    if (count <= arraySize) {   
      for (size_t i = 0; i<count; i++) {
        SignalInfo siginfo = siginfoArray[i];
        //display(info); 
        callCallback(siginfo); 
      }
    }
  }
};
}

