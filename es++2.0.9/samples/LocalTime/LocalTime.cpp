/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *  notice, this list of conditions, and the following disclaimer.
 * 
 * 2. The name of the author may not be used to endorse or promote products
 *  derived from this software without specific prior written permission.
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
 * LocalTime.cpp
 *
 *****************************************************************************/


#include <es++/Locale.h>
#include <es++/LocalTime.h>


int main(int argc, char* argv[])
{
 Locale locale;

 try {
  LocalTime time1;
  time1.display();
  
  LocalTime time2 = time1.add(6, 1, 0, 0, 0, 0);
  time2.display();
  
  LocalTime time3 = time1.sub(1, 1, 0, 0, 0, 0);
  time3.display();
  
  LocalTime t1;
  LocalTime t2(3000);
  t1.display();
  t2.display();
 	
  LocalTime t3 = t1 + t2;
  t3.display();
 
  LocalTime t4 = t1 - t2;
  t4.display();
 	
 } catch (Es::Exception& ex) {
  caught(ex);
 }

 return 0;
}
