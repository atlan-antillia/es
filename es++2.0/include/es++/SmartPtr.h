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
 *  SmartPtr.h
 *
 *****************************************************************************/

//2016/12/26 Updated.

#pragma once

#include <es++/Exception.h>

namespace Es {

/**
 *
 * SmatPtr<char> buff(new char[100]);
 * char* b = (char*)buff;
 * 
 */

  
template <class T> class SmartPtr {

private:
  T* ptr;

public:
  /**
   * Constructor
   */
  SmartPtr()
  :ptr(NULL)  
  { 
  }

  /**
   * Constructor
   */
  SmartPtr(T* t)
  :ptr(t) 
  {

  }

  /**
   * Destructor
   */
  ~SmartPtr()  
  {
    if (ptr) {
      delete ptr;
      ptr = NULL;
    }
  }

  operator T*() 
  { 
    return ptr;
  }

  
  T&  operator*()  
  { 
    return *ptr;
  }

  T**  operator&() 
  {
    return &ptr;
  }

  T*  operator->() 
  {
    if (ptr) {
      return ptr;
    } else {
      throw Es::IException("Invalid pointer.");
    }
  }

  T* operator=(T* t) 
  {
    if (ptr != t) {      
      delete ptr;
      ptr = t;
    }
    return ptr;
  }

  bool operator!=(T* t) 
  {
    if (ptr != t) {      
      return true;
    } else {
      return false;
    }
  }

  bool operator==(T* t) 
  {
    if (ptr != t) {      
      return false;
    } else {
      return true;
    }
  }

  bool null()
  {
    bool rc = true;
    if (ptr) {
      rc = false;
    }
    return rc;
  }

  bool notNull()
  {
    bool rc = false;
    if (ptr) {
      rc = true;
    }
    return rc;
  }

  void nullify() 
  {
    if (ptr) {      
      delete ptr;
      ptr = NULL;
    }
  }
};
}

