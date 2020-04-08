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
 *  NetworkInterface.cpp
 *
 *****************************************************************************/


#include <es++/Locale.h>
#include <es++/NetworkInterface.h>

int main(int argc, char* argv[])
{
  try {
    NetworkInterface interface("eth0");
    CharString ifaddr = interface.ifAddrString();
    printf("IfAddress: %s\n", (const char*)ifaddr);

    CharString dstaddr = interface.dstAddrString();
    printf("DstAddr:   %s\n", (const char*)dstaddr);

    CharString broadaddr = interface.broadAddrString();
    printf("BroadAddr: %s\n", (const char*)broadaddr);

    Bytes hwaddr = interface.hardwareAddr();
    printf("HwAddr:    ");
    hwaddr.display();

    CharString flags = interface.flags();
    printf("Flags:     %s\n", (const char*)flags);

    printf("Ifindex:   %d\n", interface.ifindex());
    printf("Mtu:       %d\n", interface.mtu());

  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}
