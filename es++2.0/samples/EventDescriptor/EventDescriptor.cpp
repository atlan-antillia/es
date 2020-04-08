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
 *  EventDescriptor.cpp
 *
 *****************************************************************************/


#include <es++/EventDescriptor.h>
#include <es++/Thread.h>
#include <es++/SignalSet.h>
#include <es++/SignalAction.h>

//
int main(int argc, char* argv[])
{
  try {
    EventDescriptor reader(0, 0);
    int fd = reader.getFD(); 

    int rc = fork();
    if (rc == ERROR) {
      throw IException("Failed to fork: %s", ERROR_STRING);
    }

    if (rc == 0) {
      //Child process
      EventDescriptor writer(fd);
      const uint64_t value = 123456789;
      const int count = 10;
      for (uint64_t i = value; i< (value+count); i++) {  
        sleep(1);
        printf("Child process write  %llu\n", i);
        writer.write(i);
      }
      writer.close();

    } else {

      //Parenti process
      while (true) {
        try {
          uint64_t value;
          reader.read(value);
          printf("Parent proccess read %llu\n", value); 
        } catch (Es::Exception& ex) {
          caught(ex);
        }
      }
    }
  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}
