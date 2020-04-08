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
 *  ListT.h
 *
 *****************************************************************************/


#pragma once

#include <es++/Object.h>
#include <es++/ListEntryT.h>

using namespace Es;

/**
 * ListT class reprents a linear singly-linked list.
 */
namespace Es {

template<class T> class ListT :public Es::Object {
  
private:
  Es::ListEntryT<T>* entry;
  bool    gc;

public:
  ListT(bool gc1=true)
    :entry(NULL), gc(gc1)
  {
  }

public:
  bool  addFirst(T* object)
  {
    bool rc = true;
    entry = new Es::ListEntryT<T>(object, entry);
    if(entry == NULL) {
      rc = false;
    }
    return rc;
  }

public:
  bool add(T* object)
  {
    Es::ListEntryT<T>* ptr  = entry;
    Es::ListEntryT<T>* prev = ptr;

    Es::ListEntryT<T>* newEntry= new Es::ListEntryT<T>(object);
    if (newEntry == NULL) {
      return false;
    }

    if (ptr == NULL) {
      entry = newEntry;
    }
    else {
      while (ptr) {
        prev = ptr;
        ptr  = ptr -> getNext();
      }
      prev -> setNext(newEntry);
    }
    return true;
  }

public:
  bool remove(T* object)
  {
    bool rc = false;
    Es::ListEntryT<T>* ptr  = entry;
    Es::ListEntryT<T>* prev = ptr;

    while (ptr) {
      T* obj = ptr -> getObject();
      if (obj == object && prev == ptr) {
        entry = ptr -> getNext();
        if (gc == false) {
          ptr ->setObject(NULL);
        }
        delete ptr;
        rc = true;
        break;
      }
      if (obj == object && prev != ptr) {
        prev -> setNext(ptr->getNext());
        if (gc == false) {
          ptr ->setObject(NULL);
        }  

        delete ptr;
        rc = true;
        break;
      }
      else {
        prev = ptr;
        ptr  = ptr -> getNext();
      }
    }
    return rc;
  }

public:
  ~ListT()
  {
    clear();
  }

public:
  bool isContained(T* obj)
  {
    bool rc = false;

    Es::ListEntryT<T>* ptr  = entry;

    while (ptr) {
      if (ptr -> getObject() == obj) {
        rc = true;
        break;
      }
      ptr = ptr -> getNext();
    }
    return rc;
  }


public:
  void clear()
  {
    Es::ListEntryT<T>* ptr  = entry;
    Es::ListEntryT<T>* prev = ptr;

    while (ptr) {
      prev = ptr;
      ptr = ptr -> getNext();
      if (gc == false) {
        prev ->setObject(NULL);
      }
      delete prev;
    }
    entry = NULL;
  }

public:
  int getLength() const
  {
    Es::ListEntryT<T>* ptr = entry;
    int n = 0;
    while(ptr) {
      ptr = ptr -> getNext();
      n++;
    }
    return n;
  }

public:
  T* getNth(int n)
  {
    int m = 0;  // Start from 0 not 1 in SOL++ 3.0

    T* object = NULL;
    Es::ListEntryT<T>* ptr = entry;

    while(ptr) {
      if(m == n) {
      	object = ptr ->getObject();
        break;
      }
      ptr = ptr -> getNext();
      m++;
    }
    return object;
  }

public:
  void reverse()
  {
    Es::ListEntryT<T>* ptr = entry;
    Es::ListEntryT<T>* rev = NULL;
        
    while (ptr) {
      Es::ListEntryT<T>* next = ptr -> getNext();
      ptr -> setNext(NULL);

      if (rev) {
        ptr -> setNext(rev); 
        rev = ptr;
      } else {
        rev = ptr;
      }
      ptr = next;
    }

    entry = rev;
  }


  bool  addLast(T* object) 
  { 
    return add(object); 
  }


  Es::ListEntryT<T>*  getEntry() const 
  { 
    return  entry; 
  } 
  

  void  enableGC() 
  {
      gc = true;
  }
  void  disableGC() 
  {
      gc = false;
  }

};

}


