/******************************************************************************
 *
 * Copyright (c) 2014   TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  MessageQueue.h
 *
 *****************************************************************************/

// You have to specify -lrt option compiling an application using this class

#pragma once

#include <es++/Object.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

//#include <es++/MessageQueueAttr.h>
#include <es++/CharString.h>

namespace Es {

class MessageQueue :public Object {
private:
  mqd_t qid;
  CharString qname;

public:
  MessageQueue()
  :qid(INVALID_VALUE)
  {
  }


  int create(const char* name,
    mode_t mode = DEFAULT_PERMISSION, mq_attr* attr = NULL)
  {

    assert(name);
    this -> qid = ::mq_open(name, O_RDWR | O_CREAT, mode, attr);
    if (this->qid == INVALID_VALUE) {
      throw Es::IException("Failed to mq_open; %s", error() );
    }
    this->qname = name;
    return this->qid;
  }


  int openWriteOnly(const char* name)
  {
    assert(name);
    this -> qid = ::mq_open(name, O_WRONLY);
    if (this->qid == INVALID_VALUE) {
      throw Es::IException("Failed to mq_open; %s", error() );
    }
    this -> qname = name;
    return this->qid;
  }

  int openReadOnly(const char* name)
  {
    assert(name);
    this -> qid = ::mq_open(name, O_RDONLY);
    if (this->qid == INVALID_VALUE) {
      throw Es::IException("Failed to mq_open; %s", error() );
    }
    this -> qname = name;
    return this->qid;
  }


  ~MessageQueue() 
  {
    if (this->qid != INVALID_VALUE) {
      ::mq_close(this->qid);
      this->qid = INVALID_VALUE;
    }
  }


  int unlink()
  {
    int rc = ::mq_unlink((const char*)qname);
    if (rc == ERROR) {
      throw Es::IException("Failed to mq_unlink; &s", error() );
    }  
    return rc;
  }


  mqd_t getQueue()
  {
    if (this -> qid == INVALID_VALUE) {
      throw Es::IException("Invalid queue id");
    }
    return this -> qid;
  }


  int receive(char *message, size_t size, unsigned *priority)
  {
    assert(message);
    assert(size>0);
    int rc = ::mq_receive(getQueue(), message, size, priority);
    if (rc == ERROR) {
      throw Es::IException("Failed to mq_receive; &s", error() );
    }
    return rc;
  }


  int timedreceive(char* message, size_t size,
    unsigned int* priority, timespec* ts)
  {
    assert(message);
    assert(size>0);
    int rc = ::mq_timedreceive(getQueue(), message, size, priority, ts);
    if (rc == ERROR) {
      throw Es::IException("Failed to mq_timedreceive; &s", error() );
    }
    return rc;
  }


  int send(const char* message, size_t size, unsigned priority)
  {
    assert(message);
    assert(size>0);
    
    int rc = ::mq_send(getQueue(), message, size, priority);
    if (rc == ERROR) {
      throw Es::IException("Failed to mq_send; %s", error());
    }
    return rc;
  }


  int timedsend(const char *message, size_t size, unsigned priority,
    const timespec *timeout)
  {
    assert(message);
    assert(size>0);
    int rc = ::mq_timedsend(getQueue(), message, size, priority, timeout);
    if (rc == ERROR) {
      throw Es::IException("Failed to mq_timedsend; %s", error());
    }
    return rc;
  }


  int setBlocking(bool flag)
  {
    mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_flags = (int)flag;        // blocking read/write
    mq_attr rattr;
    int rc = ::mq_setattr(getQueue(), &attr, &rattr);
    if (rc == ERROR) {
      throw Es::IException("Failed to mq_setattr; %s", error());
    }
    return rc;
  }


  int getAttributes(mq_attr& attr)
  {
    memset(&attr, 0, sizeof(attr));
    int rc = ::mq_getattr(getQueue(), &attr);
    if (rc == ERROR) {
      throw Es::IException("Failed to mq_getattr; %s", error());
    }
    return rc;
  }


  int blockingFlags() 
  {
    mq_attr attr;
    getAttributes(attr);
    return attr.mq_flags;
  }
  

  long int maxMessage() 
  {
    mq_attr attr;
    getAttributes(attr);
    return attr.mq_maxmsg;
  }

  long int messageSize() 
  {
    mq_attr attr;
    getAttributes(attr);
    return attr.mq_msgsize;
  }

  long int currentMessage() 
  {
    mq_attr attr;
    getAttributes(attr);
    return attr.mq_curmsgs;
  }

  bool isWritable()
  {
    bool rc = false;
    mq_attr attr;
    getAttributes(attr);

    long int space = attr.mq_maxmsg - attr.mq_curmsgs;
    if (space > 0) {
      rc = true;
    }
    return rc;
  }

  bool isReadable()
  {
    bool rc = false;
    mq_attr attr;
    getAttributes(attr);

    if (attr.mq_curmsgs > 0) {
      rc = true;
    }
    return rc;
  }

  void display()
  {
    try {
      mq_attr attr;
      getAttributes(attr);
      printf("blockingflags = %ld\n", attr.mq_flags);
      printf("max_msg       = %ld\n", attr.mq_maxmsg);
      printf("msg_size      = %ld\n", attr.mq_msgsize);
      printf("current_msg   = %ld\n", attr.mq_curmsgs);

    } catch (Es::Exception ex) {
      ex.display();
    }
  }

};


}
