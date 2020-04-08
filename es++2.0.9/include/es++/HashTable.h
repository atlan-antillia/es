/******************************************************************************
 *
 * Copyright (c) 2014 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  HashTable.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/HashEntry.h>

namespace Es {

class HashTable  :public Es::Object {
private:
  int   size;
  Es::HashEntry** table;
  bool    gc;//2015/02/15
	
private:
  Key   hash(const Key key) 
  { 
    return (key % size); 
  }


private:
  Key hash(const char* key)
  {
    long sum = 0;
    int len  = strlen(key);

    for(int i = 0; i<len; i++) {
      sum = sum + ((int)*key % size);
      key++;
    }
    return (Key)(sum % size);
  }

public:
  HashTable(int size1, bool doGC=false)
  {
    gc   = doGC; //2015/0215
    size = size1;
    table = new Es::HashEntry*[size];

    for (int i =0 ; i<size; i++) {
      table[i] = NULL;
    }
  }

public:
  int add(Key key, Es::Object* object, int id)
  {
    int n = hash(key);

    Es::HashEntry* ptr  = table[n];
    Es::HashEntry* prev = ptr;
    int rc = true;
    if(ptr == NULL) {
      table[n] = new Es::HashEntry(key, object, id);
    } else {
      while(ptr) {
         prev = ptr;
         ptr  = ptr -> getNext();
         if(prev->getIntKey() == key &&
           prev->getSubKey() == id) {
           rc = false;
           break;
         }
       }
       if(rc == true) {
         prev -> setNext(new Es::HashEntry(key, object, id));
       }
     }
    return rc;
  }

public:
  int  add(const char* key, Es::Object* object)
  {
    int n = hash(key);

    Es::HashEntry* ptr  = table[n];
    Es::HashEntry* prev = ptr;
    int rc = true;
    if(ptr == NULL) {
       table[n] = new Es::HashEntry(key, object);
    } else {
      while(ptr) {
        prev = ptr;
        ptr  = ptr -> getNext();
        if(prev->getCharKey() == key) {
          rc = false;
          break;
        }
      }
      if(rc == true) {
        prev -> setNext(new Es::HashEntry(key, object));
      }
    }
    return rc;
  }

public:
  void enableGC()
  {
    gc = true;
  }

  void disableGC()
  {
    gc = false;
  }

public:
  Es::Object* lookup(Key key)
  {
    Es::Object* object = NULL;
    Key  n = hash(key);
    Es::HashEntry* ptr = table[n];

    while (ptr) {
      if (ptr -> getIntKey() == key) {
        object = ptr -> getObject();
        break;
      }
      ptr = ptr -> getNext();
    }
    return object;
  }

public:
  Es::Object* lookup(Key key, int id)
  {
     Es::Object* object = NULL;
     int n = hash(key);
     Es::HashEntry* ptr = table[n];
     while (ptr) {
        if(ptr -> getIntKey() == key &&
           ptr -> getSubKey() == id){
              object = ptr -> getObject();
              break;
         }
         ptr = ptr -> getNext();
     }
     return object;
  }

public:
  Es::Object* lookup(const char* key)
  {
    Key n = hash(key);
    Es::HashEntry* ptr = table[n];
    Es::Object* object = NULL;

    while (ptr) {
      if(strcmp(ptr -> getCharKey(), key)==0) {
        object = ptr->getObject();
        break;
      }
      ptr = ptr -> getNext();
    }
    return object;
  }

public:
  void remove(Es::HashEntry* prev, Es::HashEntry* ptr, int n)
  {
    if(prev == ptr) {
      table[n] = ptr -> getNext();
    } else {
      prev -> setNext(ptr -> getNext());
    }
    delete ptr;
  }

public:
  bool remove(Key key)
  {
    Key n = hash(key);
    Es::HashEntry* ptr  = table[n];
    Es::HashEntry* prev = ptr;

    while(ptr) {
      if(ptr -> getIntKey() == key) {
        remove(prev, ptr, n);
        return true;
      }
      prev = ptr;
      ptr  = ptr -> getNext();
    }
    return false;
  }

public:
  bool remove(const char* key)
  {
    Key n = hash(key);
    Es::HashEntry* ptr  = table[n];
    Es::HashEntry* prev = ptr;

    while (ptr) {
      if(strcmp(ptr -> getCharKey(), key)==0 ) {
        remove(prev, ptr, n);
        return true;
      }
      prev = ptr;
      ptr  = ptr -> getNext();
    }
    return false;
  }

public:
  ~HashTable()
  {
    clear();
  }
	
  void clear()
  {
    if (table) {
      for(int i = 0; i < size; i++) {
        Es::HashEntry* ptr  = table[i];
        Es::HashEntry* prev = ptr;
        while(ptr) {
          prev = ptr;
          ptr  = ptr -> getNext();
          //2015/02/15
          if (gc) {
            Es::Object* object = prev->getObject();
            if (object) {
              //printf("HashTable::~HashTable gc\n");
              delete object;
              prev -> setObject(NULL);
            }
          }
          //
          delete prev;
        }
      }
      delete [] table;
    }

    table = NULL;
  }

public:
  //2014/12/20
  void display()
  {
    for(int i = 0; i < size; i++) {
      Es::HashEntry* ptr  = table[i];
      while(ptr) {
      	Es::Object* object =  ptr -> getObject();
      	if (object) {
      	  object -> display();
      	}
        ptr  = ptr -> getNext();
      }
    }
  }

};

}



