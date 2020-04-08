/******************************************************************************
 *
 * Copyright (c) 2019 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  KeyValueSplitter.h
 *
 *****************************************************************************/

#pragma once

#include <string>

namespace Es {

class KeyValueSplitter {

private:
  std::string separator; 

public:
  KeyValueSplitter(const std::string& sep="=")
  :separator(sep)
  {
  }

  //Split string to key and value by separator.
  bool split(const std::string& string, 
             std::string& key, std::string& value)
  {
    bool found = false;
 
    std::string::size_type  offset = std::string::size_type(0);
    std::string::size_type  pos    = string.find(separator, offset);
    
    if (pos != std::string::npos) {
      std::string first  = string.substr(0, pos);
      std::string second = string.substr(pos+1, string.length());
      key   = trim(first);
      value = trim(second);
      found = true;
      //printf("%s|%s¥n¥n", key.c_str(), value.c_str());
    }
    
    return found;
  }
  

  //Trim string by removing trimChars something like  " \t\v\r\n".
  std::string trim(const std::string& string, 
        const char* trimchars = " \t\v\r\n")

  {
    std::string trimmed;

    std::string::size_type left = string.find_first_not_of(trimchars);

    if (left != std::string::npos) {
      std::string::size_type right = string.find_last_not_of(trimchars);
      trimmed = string.substr(left, right - left + 1);
    }
    return trimmed;
  }
  
  // Find env_liken string $(HOME).
  // Return a string something like "HOME" from string something like 
  // "$(HOME)/hello/world".
  bool find_env_liken(const std::string& string, std::string& value)
  {
    bool found = false;
    std::string::size_type left  = string.find("$(");
    if (left != std::string::npos) {
      std::string::size_type right = string.find(")");
      if (right != std::string::npos) {
        value = string.substr(left+2, right - (left + 2) );
        found = true;
      }
    }
    return found;
  }
};

}

