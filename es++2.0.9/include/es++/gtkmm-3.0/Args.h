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
 *      Args.h
 *
 *****************************************************************************/

//2017/07/02

#pragma once

#include <es++/Object.h>
#include <es++/gtkmm-3.0/StrDef.h>

namespace Es {

typedef int64_t ArgVal;

typedef struct {
  double doublet;
  ArgVal value;
} Value;

typedef struct {
  const char* name;
  Value value;
} Arg;

typedef Arg* ArgList ;

class Args :public Es::Object {

private:
  static const int ARG_MAX_COUNT = 100; 
  int       MAX_COUNT;
  int       num;
  Es::Arg*  args;
 
public:
  Args(int max = ARG_MAX_COUNT)
  :MAX_COUNT(max)
  {  
    if (max < 1) {
      throw IException("Invalid argument: %d", max);
    }
    num = 0;
    args = new Es::Arg[MAX_COUNT];
  }

  ~Args() 
  { 
    delete [] args;
    args = NULL;
  }
 
  Args&   reset() 
  {
    num = 0;
    return *this;
  }
  
  Args& clear()
  {
    num = 0;
    for (int i = 0; i<MAX_COUNT; i++) {
      args[i].name = NULL;
      args[i].value.doublet = DZERO;
      args[i].value.value = (ArgVal)0;
    }
    return *this;
  }

  ArgList  getArgList() 
  { 
    return args; 
  }
  
  int   count() 
  { 
    return num; 
  }

  Args&  set(const char* name, unsigned int value)
  {
    if(num >= 0 && num < MAX_COUNT) {
      args[num].name  = name;
      args[num].value.value = (ArgVal)value;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, ArgVal value)
  {
    if(num >= 0 && num < MAX_COUNT) {
      args[num].name  = name;
      args[num].value.value = value;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, double doublet)
  {
    if(num >= 0 && num < MAX_COUNT) {
      args[num].name  = name;
      args[num].value.doublet = doublet;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, char value)
  {
    if(num >= 0 && num < MAX_COUNT) {
      args[num].name  = name;
      args[num].value.value = (ArgVal)value;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, int value)
  {
    if(num >= 0 && num < MAX_COUNT) {
      args[num].name  = name;
      args[num].value.doublet  = DZERO;
      args[num].value.value = (ArgVal)value;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, int8_t value)
  {
    if(num >= 0 && num < MAX_COUNT) {
      args[num].name  = name;
      args[num].value.doublet  = DZERO;
      args[num].value.value = (ArgVal)value;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, int16_t value)
  {
    if(num >= 0 && num < MAX_COUNT) {
      args[num].name  = name;
      args[num].value.doublet  = DZERO;
      args[num].value.value = (ArgVal)value;
      num++;
    }
    return *this;
  }
/*
  Args&  set(const char* name, int32_t value)
  {
    if(num >= 0 && num < MAX_COUNT) {
      args[num].name  = name;
      args[num].value.doublet  = DZERO;
      args[num].value.value = (ArgVal)value;
      num++;
    }
    return *this;
  }
*/
/*
  Args&  set(const char* name, Atom atom)
  {
    if(num >= 0 && num < MAX_COUNT) {
      args[num].name  = name;
      args[num].value.doublet  = DZERO;
      args[num].value.value = (ArgVal)atom;
      num++;
      }
    return *this;
  }
*/
  Args&  set(const char* name, const char* string)
  {
    if(num >= 0 && num < MAX_COUNT) {
      args[num].name  = name;
      args[num].value.doublet  = DZERO;
      args[num].value.value = (ArgVal)string;
      num++;
    }
    return *this;
  }
    
  Args&  set(const char* name, unsigned short value)
  {
    if(num >= 0 && num < MAX_COUNT) {
      args[num].name  = name;
      args[num].value.doublet  = DZERO;
      args[num].value.value = (ArgVal)value;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, bool value)
  {
    if(num >= 0 && num < MAX_COUNT) {
      args[num].name  = name;
      args[num].value.doublet  = DZERO;
      args[num].value.value = (ArgVal)value;
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, GtkWidget* w)
  {
    if(num >= 0 && num < MAX_COUNT) {
      args[num].name  = name;
      args[num].value.doublet  = DZERO;
      args[num].value.value = (ArgVal)w;
      num++;
    }
    return *this;
  }

  Args&  setIfNotSet(const char* name, ArgVal value)
  {
    if (has(name) == false) {
      set(name, value); 
    }
    return *this; 
  }

  Args&  set(const char* name, std::string& string)
  {
    if(num >= 0 && num < MAX_COUNT) {
      args[num].name  = name;
      args[num].value.doublet  = DZERO;
      args[num].value.value = (ArgVal)string.c_str();
      num++;
    }
    return *this;
  }

  Args&  set(const char* name, Gtk::Window* window)
  {
    if(num >= 0 && num < MAX_COUNT && window) {
      args[num].name  = name;
      args[num].value.doublet  = DZERO;
      args[num].value.value = (ArgVal)window;
      num++;
    }
    return *this;
  }

  void  copy(Args* args1)
  {
    Arg* ar = args1->getArgList();
    num = args1->count();

    for(int i= 0; i<num; i++) {
      args[i].name  = ar[i].name;
      args[i].value.doublet = ar[i].value.doublet;
      args[i].value.value = ar[i].value.value;
    }
  }


  ArgVal get(const char* name)
  {
    for(int i = 0; i<num; i++) {
      if(strcmp(args[i].name, name) == 0) {
        return args[i].value.value;
      }
    }
    return (ArgVal)NULL;
  }

  double getDouble(const char* name)
  {
    for(int i = 0; i<num; i++) {
      if(strcmp(args[i].name, name) == 0) {
        return (double)args[i].value.doublet;
      }
    }
    return 0.0f;
  }

  bool get(const char* name, ArgVal& value)
  {
    bool rc = false;
    for(int i = 0; i<num; i++) {
      if(strcmp(args[i].name, name) == 0) {
        value = args[i].value.value;
        rc = true;
        break;
      }
    }
    return rc;
  }

  bool get(const char* name, bool& value)
  {
    bool rc = false;
    for(int i = 0; i<num; i++) {
      if(strcmp(args[i].name, name) == 0) {
        value = (bool)args[i].value.value;
        rc = true;
        break;
      }
    }
    return rc;
  }

  bool get(const char* name, double& doublet)
  {
    bool rc = false;
    for(int i = 0; i<num; i++) {
      if(strcmp(args[i].name, name) == 0) {
        doublet = args[i].value.doublet;
        rc = true;
        break;
      }
    }
    return rc;
  }

  bool get(const char* name, float& fvalue)
  {
    bool rc = false;
    for(int i = 0; i<num; i++) {
      if(strcmp(args[i].name, name) == 0) {
        fvalue = (float)args[i].value.doublet;
        rc = true;
        break;
      }
    }
    return rc;
  }

  bool get(const char* name, int& value)
  {
    bool rc = false;
    for(int i = 0; i<num; i++) {
      if(strcmp(args[i].name, name) == 0) {
        value = (int)args[i].value.value;
        rc = true;
        break;
      }
    }
    return rc;
  }

  bool get(const char* name, int8_t& value)
  {
    bool rc = false;
    for(int i = 0; i<num; i++) {
      if(strcmp(args[i].name, name) == 0) {
        value = (int8_t)args[i].value.value;
        rc = true;
        break;
      }
    }
    return rc;
  }

  bool get(const char* name, int16_t& value)
  {
    bool rc = false;
    for(int i = 0; i<num; i++) {
      if(strcmp(args[i].name, name) == 0) {
        value = (int16_t)args[i].value.value;
        rc = true;
        break;
      }
    }
    return rc;
  }
/*
  bool get(const char* name, int32_t& value)
  {
    bool rc = false;
    for(int i = 0; i<num; i++) {
      if(strcmp(args[i].name, name) == 0) {
        value = (int32_t)args[i].value.value;
        rc = true;
        break;
      }
    }
    return rc;
  }
*/

  bool get(const char* name, std::string& value)
  {
    bool rc = false;
    for(int i = 0; i<num; i++) {
      if(strcmp(args[i].name, name) == 0) {
        value = (const char*)args[i].value.value;
        rc = true;
        break;
      }
    }
    return rc;
  }

  bool has(const char* name)
  {
    bool rc = false;
    for(int i = 0; i<num; i++) {
      if(strcmp(args[i].name, name) == 0) {
        rc = true;
        break;
      }
    }
    return rc;
  }
  
  Args&  put(const char* name, ArgVal value)
  {
    for(int i = 0; i<num; i++) {
      if(strcmp(args[i].name, name) == 0) {
        args[i].value.doublet  = DZERO;
        args[i].value.value = value;
        return *this;
      }
    }
    return set(name, value);  
  }
};

}

