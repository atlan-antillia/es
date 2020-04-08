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
 *  SmartArray.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>

namespace Es {
// T may take char*, wchar_t*, and so on.
// For example, SmartArray<char*, 100> means 100 array of char*.

template <typename T>

class SmartArray :public Object {
private:
  T*      array;
  size_t  size;

  static  const int AUTO_INCREMENT = 100;
  static  const int DEFAULT_SIZE   = 100;

public:
  SmartArray(size_t s=DEFAULT_SIZE)
  :array(NULL),
  size(s)	
  {
  	if (size > 0) {
  	  array = new T[size];
  	  for (size_t n = 0; n<size; n++) {
  	    array[n] = NULL;	
  	  }
  	} else {
  		throw Es::IException("Invalid argument %d", size);	
  	}
  }

  ~SmartArray() 
  { 
  	clear();
  }

  void removeAllItems()
  {
  	clear();
  	alloc();
  }
	
  void alloc()
  {
  	size = DEFAULT_SIZE;
  	if (size > 0) {
  	  array = new T[size];
  	  for (size_t n = 0; n<size; n++) {
  	    array[n] = NULL;	
  	  }
  	} else {
  		throw Es::IException("Invalid argument %d", size);	
  	}

  }
	
  void clear()
  {
  	if (array) {
  	  for (size_t i = 0; i < size; i++) {
  	    delete array[i];
  	  }
  	  delete [] array;
  	  array = NULL;
  	}
  	size = 0;
  }

  T* getArray()
  {
    return array;
  }
	

  void put(size_t pos, T t) 
  {
    if (pos >= 0 && pos < size) {
      
      if (array[pos] != NULL && t != array[pos] ) {
    	  delete array[pos];
      }
      //printf("put %d\n", pos);
      array[pos] = t;
    } else if (pos >= size) {
      resize( pos + AUTO_INCREMENT);
    	
      array[pos] = t;	
    } else {
      throw Es::IException("Index:out of range, %d", pos);
    }
  }
	
  size_t getSize()
  {
    return size;
  }
	
  T get(size_t pos) 
  {
    if (pos >= 0 && pos < size) {
      return array[pos];
    } else {
      throw Es::IException("Index:out of range, %d", pos);
    }
  }
	
  bool resize(size_t len)
  {
    bool rc = false;
    if (len <= 0) {
      throw Es::IException("Invalid argument");
    }
    size_t nlen =  len;
    T* temp = new T[nlen];

    if(temp) {
     	for (size_t i = 0; i<nlen; i++) {
     		temp[i] = (T)NULL;
    	}
    	for (size_t n = 0; n< this->size; n++) {
    		temp[n] = array[n];
    	}
    } else {
      return rc;
    }
    this -> size = nlen;
  	
    delete [] this->array;
    this -> array = temp;
    rc = true;

    return rc;
  }

  T&  operator[](size_t pos)
  {
    if (pos>=0 && pos < size) {
      return array[pos];
    } else if (pos >= size) {
      resize( pos + AUTO_INCREMENT);
      return array[pos];
    } else {
      throw Es::IException("Index:out of range, %d", pos);
    }
  }
	
	void dump()
	{
		for(size_t i = 0; i<size; i++) {
			printf("%d %ld\n", i, (long)array[i]);	
		}
	}
};

}


