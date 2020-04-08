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
 *  Attribute.cpp
 *
 *****************************************************************************/


#include <es++/SmartArray.h>
#include <es++/Attribute.h>
#include <es++/CharString.h>
#include <es++/Integer32.h>
#include <es++/UnsignedInteger32.h>
#include <es++/Integer64.h>
#include <es++/UnsignedInteger64.h>
#include <es++/Bytes.h>

using namespace Es;

int main(int argc, char* argv[])
{
  try {
    SmartArray<Attribute*> attr;
    int index = 0;
    attr[index++] = new Attribute("Name1", new CharString("Value1"));
    attr[index++] = new Attribute("Name2", new CharString("Value2"));
    attr[index++] = new Attribute("Name3", new Integer32(-1234567));
    attr[index++] = new Attribute("Name4", new UnsignedInteger32(1234567));
    attr[index++] = new Attribute("Name5", new Integer64(-1234567890123));
    attr[index++] = new Attribute("Name6", new UnsignedInteger64(12345670123));
    unsigned char data[] = {0,1,2,3,4,5,6,7,8,9};
    attr[index++] = new Attribute("Name7", new Bytes(data, sizeof(data)));

    printf("Size %ld\n", attr.getSize());
    for (int i = 0; i<index; i++) {
      CharString name  = attr[i]->getName();
      CharString value = attr[i]->getValue();
      printf("%s = %s\n", (const char*)name, (const char*)value);
    }
    printf("\n");
  } catch (Es::Exception& ex) {
    caught(ex);
  }

  return 0;
}

