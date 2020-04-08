/******************************************************************************
 *
 * Copyright (c) 1999-2008 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  HashEntry.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Entry.h>


namespace Es {

class HashEntry :public Es::Entry {
private:
  Key     intKey;
  char*  charKey;
  int     subKey;
  Es::HashEntry* next;
  
public:
  HashEntry(Key key, Es::Object* object)
  :Entry(object, NULL) 
  {
    intKey  = key;
    charKey = NULL;
    subKey  = 0;
    next    = NULL;
  }
        
  HashEntry(Key key, Es::Object* object, int id)
  :Entry(object, NULL) 
  {
    intKey  = key;
    charKey = NULL;
    subKey  = id;
    next    = NULL;
  }

  HashEntry(const char* key, Es::Object* object)
  :Entry(object, NULL) 
  {
    intKey  = (Key)NULL;
    charKey = __strdup(key);
    subKey  = 0;
    next    = NULL;
  }

  ~HashEntry() 
  {
    if(charKey) {
      delete charKey;
    }     
  }
        
  char*     getCharKey() 
  {
    return charKey;
  }
        
  Key        getIntKey() 
  {
    return intKey;
  }
        
  Es::HashEntry* getNext() 
  {
    return next;
  }
        
  void  setNext(Es::HashEntry* next1) 
  {
    next = next1;;
  }
        
  int     getSubKey() 
  {
    return subKey;
  }
};
  
}

