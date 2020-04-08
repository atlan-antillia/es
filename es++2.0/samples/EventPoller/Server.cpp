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
 *  Server.cpp
 *
 *****************************************************************************/

#include <es++/Thread.h>
#include <es++/SocketStream.h>
#include <es++/Logger.h>
#include <es++/EventPoller.h>
#include <es++/LocalDateTime.h>

namespace Es {

class ServerThread :public EventPoller {
private:
  bool looping;
  EventPollerEvents events;

public:
  ServerThread()
  :looping(true)
  {
  }

  virtual void accept(Socket& listener)
  {
    SockAddrInet addr;
    int newFD = listener.accept(addr);
    if (newFD != INVALID_FD) {
      printf("Accepted newFD %d\n", newFD);
      EventPollerEvent newEvent(EPOLLIN, newFD);

      this -> add(newFD, newEvent);
    } else {
      printf("Failed to accept %s\n", error());
    }
  }

  virtual void read(SocketStream& stream)
  {
    char buffer[1024];
    int len = stream.recv(buffer, sizeof(buffer));
    if (len > 0) {
      buffer[len] = '\0';
      printf("%s\n", buffer);
    }
  }

  virtual void write(SocketStream& stream)
  {
    char msg[1024];
    LocalDateTime ldt;
    CharString now = ldt.nowToSeconds();

    sprintf(msg, "Goodbye world %s", (const char*)now);
    stream.send(msg, strlen(msg));
  }

public:
  virtual void run() 
  {
    try {
      SockAddrInet inet("localhost", 8080);
      Socket listener(INET, STREAM, 0);

      int on = 1;
      listener.setOption(SO_REUSEADDR, (char*) &on, sizeof(on));

      listener.bind(inet);
      listener.listen(10);
      int listenerFD = listener.getSocket();

      EventPollerEvent ev(EPOLLIN, listenerFD);
      this -> add(listenerFD, ev);
  
      while (looping) {
        int num = this -> wait(events);
        epoll_event* ev = events.getEvents();
 
        for (int i = 0; i<num; i++) {
          EventPollerEvent polledEvent(ev[i]);
          int fd = polledEvent.fd();

          if (fd == listenerFD && polledEvent.readable()) {
            accept(listener);
            break;
          } 

          if (fd != listenerFD && polledEvent.readable() ) {
            SocketStream stream(fd);              
            stream.noAutoClose();

            read(stream);
            write(stream); 
          }
        } 
      }
    } catch (Es::Exception& ex) {
      caught(ex);
    }
  }
};
}

//
int main(int argc, char** argv)
{
  try {
    sigignore(SIGPIPE);

    ServerThread server;
    server.start();
    server.wait();
  
  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}
