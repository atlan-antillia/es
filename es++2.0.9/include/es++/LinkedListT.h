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
 *  LinkedListT.h
 *
 *****************************************************************************/


#pragma once

#include <es++/List.h>
#include <es++/ListEntryT.h>

using namespace Es;

/**
 * LinkedListT class reprents a linear singly-linked list.
 */
namespace Es {

template <class T> class LinkedListT :public Es::List {
  
private:
  Es::ListEntryT<T>* entry;
  bool    gc;

public:
  LinkedListT(bool gc1=true)
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
  ~LinkedListT()
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


// Simple selection sort
public:
  void sortAscending()
  {
    sort(ASCENDING);
  }
  void sortDescending()
  {
    sort(DESCENDING);
  }
  void sort(SortDirection dir)
  {
    int length = getLength();
    int i = 0;
    Es::ListEntryT<T>* ith = entry;
   
    while (i<length-1) {  
      Es::ListEntryT<T>* cth = ith;
      Es::ListEntryT<T>* jth = ith -> getNext();
      T*    obj = ith -> getObject();

      while (jth) {
      	T* jthObj = jth->getObject();
        if (dir == ASCENDING) {
          if (jthObj -> compare(obj) > 0) {
            cth = jth;
            obj = jth -> getObject();
          }  
        }
        if (dir == DESCENDING) {
          if (jthObj -> compare(obj) < 0) {
            cth = jth;
            obj = jth -> getObject();
          }  
        }
        jth = jth -> getNext();
      }

      cth -> setObject(ith->getObject());
      ith -> setObject(obj);

      ith = ith -> getNext();
      i++;
    }  // wile
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

public:
  void display()
  {
    Es::ListEntryT<T>* ptr  = entry;

    while (ptr) {
      T* object = ptr -> getObject();
      if (object) {
        object -> display();	
      }
      ptr = ptr -> getNext();
    }
  }

};

}


