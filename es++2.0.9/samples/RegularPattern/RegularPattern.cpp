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
 *  RegularPattern.cpp
 *
 *****************************************************************************/

#include <es++/RegularPattern.h>

int main(int argc, char* argv[])
{
  RegularPattern::setSyntaxOptions(RE_SYNTAX_POSIX_EGREP);

  try {
    const char* expression = ".*?(Adam|Eve|Eden|fruit).*?";
    printf("Pattern : \"%s\"\n", expression); 
    RegularPattern pattern(expression);

    const char* strings[] = {
      "Adam was a vegetarian.",
      "Was Eve also a vegetarian?",
      "Did they eat forbidden fruit in the garden of Eden?",
      "The end of immortality."
    };
    for (int i = 0; i < SizeOf(strings); i++) {
      try {
        Registers regs;
        int rc = pattern.match(strings[i], 0, regs);
        printf("String \"%s\" matched rc=%d\n", strings[i], rc);
        //regs.displaySubstring(strings[i]);
        StringT<char> subs = regs.shortestSubstring(strings[i]);
        printf("ShortestSubstring=\"%s\"", (const char*)subs);

      } catch (Es::Exception& ex) {
        caught(ex);
      }
      printf("\n");
    }
  } catch (Es::Exception& ex) {
    caught(ex);
  }
  printf("----------\n");
  try {
    const char* expression = ".*(every tree|tree of the knowledge)";
    printf("Pattern : \"%s\"\n", expression); 
    RegularPattern pattern(expression);

    const char* strings[] = {
      "Of every tree of the garden thou mayest freely eat",
      ": But of the tree of the knowledge of good and evil,",
      " thou shalt not eat of it: for in the day that thou",
      " eatest thereof thou shalt surely die."
    };

    for (int i = 0; i < SizeOf(strings); i++) {
      try {
        Registers regs;
        int rc = pattern.match(strings[i], 0, regs);
        printf("String \"%s\" matched rc=%d\n", strings[i], rc);
        //regs.displaySubstring(strings[i]);
        StringT<char> subs = regs.shortestSubstring(strings[i]);
        printf("ShortestsSubstring=\"%s\"", (const char*)subs);

      } catch (Es::Exception& ex) {
        caught(ex);
      }
      printf("\n");
    }

  } catch (Es::Exception& ex) {
    caught(ex);
  }
  printf("----------\n");

  try {
    const char* expression = "2[^3]*1*6";
    printf("Pattern : \"%s\"\n", expression); 
    RegularPattern pattern(expression);

    const char* strings[] = {
      "23331", "2316", "2016", "20000116"
    }; 
     
    for (int i = 0; i < SizeOf(strings); i++) {
      try {
        Registers regs;
        int rc = pattern.match(strings[i], 0, regs);
        printf("String \"%s\" matched rc=%d\n", strings[i], rc);
        regs.displaySubstring(strings[i]); 

      } catch (Es::Exception& ex) {
        caught(ex);
      }
      printf("\n");
    }
  } catch (Es::Exception& ex) {
    caught(ex);
  }
  return 0;
}
