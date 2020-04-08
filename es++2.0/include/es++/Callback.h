/******************************************************************************
 *
 * Copyright (c) 2015  TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
 *  Callback.h
 *
 *****************************************************************************/

//2015/06/01

#pragma once

#include <es++/Object.h>

namespace Es {

template <class CLASS,     //A pointer to a C++ class 
          typename METHOD, //A pointer to a method of CLASS 
          typename PARAM>  //A parameter to a callback 

class Callback :public Object {
private:
  CLASS   object;  //A pointer to an instance of CLASS
  METHOD  method;  //A pointer to a a method of CLASS 

public:
  Callback()
  :object(NULL),
  method(NULL)
  {
  }

public:
  void set(CLASS object, METHOD method)
  {
    if (object && method) {
      this -> object = object;
      this -> method = method;
    } else {
      throw Es::IException("Invalid argument");
    }
  }

  void call(PARAM param)
  {
    if (object && method) {
      (object->*method)(param);
    }
  }
};

}

