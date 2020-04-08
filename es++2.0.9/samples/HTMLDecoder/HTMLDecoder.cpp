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
 *  HTMLDecoder.cpp
 *
 *****************************************************************************/


#include <es++/HTMLEncoder.h>
#include <es++/HTMLDecoder.h>
#include <es++/Locale.h>

int main(int argc, char* argv[])
{
  Locale locale;

  const char* string = 
    "<html>\n"
    "<body>\n"
    "<h1>HTMLEncoder/HTMLDecoder test!</h1>\n"
    "<h2>1234567890</h2>\n"
    "<h3>ABCDEFGHIJKLMNOPQRSTUVWXYZ</h3>\n"
    "<h3>abcdefghijklmnopqrstuvwxyz</h3>\n"
    "<h3>!\"#$%&'()=~|0\\`{[]}+*<>?/_</h3>\n"
    "</body>\n"
    "</html>\n";

  printf("String=[%s]\n", string);
  
  // Encoding
  HTMLEncoder encoder;
  char* encoded = encoder.encode(string);
  printf("HTML Encoded1=[%s]\n", encoded);

  CharString enc;
  
  encoder.encode(string, enc);
  printf("HTML Encoded2=[%s]\n", (const char*)enc);

  // Decoding
  HTMLDecoder decoder;

  char* decoded = decoder.decode(encoded);

  printf("HTML Decoded1=[%s]\n", decoded);
  CharString dec;
  decoder.decode(encoded, dec);

  printf("HTML Decoded2=[%s]\n", (const char*)dec);
  

  if (strcmp(string, decoded) ==0) {
    printf("OK.HTML-encode and HTML-decode created the same original string \n");
  } else {
    printf("Failed to HTML-encode or HTML-decode.\n");

  }
  delete [] encoded;
  delete [] decoded;

  const CharString bstring = string;

  CharString bencoded = encoder.encode(bstring);
  printf("String Encoded = %s\n", (const char*)bencoded);

  CharString bdecoded = decoder.decode(bencoded);
  printf("String Decode = %s\n", (const char*)bdecoded);

  return 0;
}

