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
 *  Property.h
 *
 *****************************************************************************/

//2015/03/24

#pragma once

#include <es++/Object.h>
#include <es++/CharString.h>

namespace Es {

class Property :public Object {
private:
  CharString  _name;
  CharString  _value;

public:
  Property()
    :_name(""),
     _value("")
  { 
  }

public:
  Property(const char* name, const char* value)
    :_name(name),
     _value(value)
  { 
  }
/*
public:
  Property(const char* name, int value)
    :_name(name),
     _value("")
  { 
  	char string[128];
  	sprintf(string, "%d", value);
  	_value = string;
  }

public:
  Property(const char* name, unsigned int value)
    :_name(name),
     _value("")
  { 
  	char string[128];
  	sprintf(string, "%u", value);
  	_value = string;
  }
*/	
public:
  Property(const char* name, unsigned long value)
    :_name(name),
     _value("")
  { 
  	char string[128];
  	sprintf(string, "%lu", value); //unsigned long int
  	_value = string;
  }
	
public:
  Property(const char* name, int16_t value)
    :_name(name),
     _value("")
  { 
  	char string[128];
  	sprintf(string, "%" PRId16"", value); //unsigned long int
  	_value = string;
  }

public:
  Property(const char* name, uint16_t value)
    :_name(name),
     _value("")
  { 
  	char string[128];
  	sprintf(string, "%" PRIu16"", value); //unsigned long int
  	_value = string;
  }

public:
  Property(const char* name, int32_t value)
    :_name(name),
     _value("")
  { 
  	char string[128];
  	sprintf(string, "%" PRId32"", value); //unsigned long int
  	_value = string;
  }

public:
  Property(const char* name, uint32_t value)
    :_name(name),
     _value("")
  { 
  	char string[128];
  	sprintf(string, "%" PRIu32"", value); //unsigned long int
  	_value = string;
  }
	
public:
  Property(const char* name, int64_t value)
    :_name(name),
     _value("")
  { 
  	char string[128];
  	sprintf(string, "%" PRId64"", value); //unsigned long int
  	_value = string;
  }

public:
  Property(const char* name, uint64_t value)
    :_name(name),
     _value("")
  { 
  	char string[128];
  	sprintf(string, "%" PRIu64"", value); //unsigned long int
  	_value = string;
  }

public:
  Property(const char* name, double value)
    :_name(name),
     _value("")
  { 
  	char string[128];
  	sprintf(string, "%f", value);
  	_value = string;
  }

  ~Property() 
  { 
  }

  void set(const char* n, const char* v)
  {
    if (n) {
      _name = n;
    }
    if (v) {
      _value = v;
    }
  }

  CharString&  name() 
  {
    return this->_name;
  }

  CharString&  value() 
  {
    return this->_value;
  }

  void name(const char*  n) 
  {
    this -> _name = n;
  }

  void value(const char*  v) 
  {
    this -> _value = v;
  }
	
  void display()
  {
  	printf("%s = %s\n", (const char*) name(), (const char*)value());		
  }
	
/*
  void display()
  {
    if (_value) {
      _name.write();
       String delim(": ");
       delim.write();
      _value->display();
    }
  }
*/
};

}


