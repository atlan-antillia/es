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
 *  Base64Decoder.cpp
 *
 *****************************************************************************/


#include <es++/Base64Encoder.h>
#include <es++/Base64Decoder.h>
#include <es++/Bytes.h>

using namespace Es;

int main(int argc, char* argv[])
{
  try {
    unsigned char bytes[64];
    for(size_t i = 0; i<sizeof(bytes); i++) {
      bytes[i] = i%10;
    }

    Bytes::dump(bytes, sizeof(bytes));

    Base64Encoder encoder;
  
    char* encoded = encoder.encode(bytes, sizeof(bytes)); 
    printf("encoded=%s\n", encoded);

    Base64Decoder decoder;

    Bytes decoded;
    decoder.decode(encoded, decoded);

    decoded.dump();
  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;

}

