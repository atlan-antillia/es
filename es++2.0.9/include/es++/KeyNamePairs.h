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
 *  KeyNamePairs.h
 *
 *****************************************************************************/


#pragma once
#include <es++/Object.h>
#include <es++/Exception.h>
#include <es++/Pair.h>
#include <es++/CharString.h>
#include <es++/StringBufferT.h>

namespace Es {

template <typename Key>
class KeyNamePairs :public Object {

private:
  Pair<Key, const char*>* pairs;
  size_t size;

public:
  KeyNamePairs(Pair<Key, const char*>* pairs, size_t size)
	:pairs(pairs),
	size(size)
  {
  	if (pairs == NULL || size == 0) {
  		throw Es::IException("Invalid argument");
  	}
  }
	
public:
  const char* match(Key key)
  {
  	const char* second = "";
  	for(size_t i = 0; i<size; i++) {
  	  if (pairs[i].first == key) {
  		second = pairs[i].second;
  	  	break;
  	  }
  	}
  	return second;
  }
	
public:
  CharString bitMatch(Key key)
  {
  	StringBufferT<char> buffer;
  	
  	for(size_t i = 0; i<size; i++) {
  	  if (pairs[i].first & key) {
  	  	if (buffer.getContentSize() >0) {
  	  		buffer.append("|");	
  	  	}
  	  	buffer.append(pairs[i].second);
  	  }
  	}
  	const char* string = (const char*)buffer;
  	
  	return CharString(string);
  }
};

}

