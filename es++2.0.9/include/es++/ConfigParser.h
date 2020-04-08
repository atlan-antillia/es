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
 *  ConfigParser.h
 *
 *****************************************************************************/

// 2019/06/04 
// This class will parse  an ini config file, and store the information
// to std::unordered_map object.

// The configuration file will take the following format.
/*
; Comment line: Something.ini
[section1]
name1=value1
name2=value2
;;
[section2]
name1=value1
name2=value2
;;
 */

#pragma once

#include <string>
#include <list>
#include <vector>
#include <unordered_map>

#include <iostream>
#include <fstream>
#include <es++/KeyValueSplitter.h>
#include <es++/Exception.h>


namespace Es {

class ConfigParser {
private:
  KeyValueSplitter splitter;

  // The key of the following map will take a string of the format 
  // sectioned_name = section + ":" + name, and a value take a string only.
  std::unordered_map<std::string, std::string> map;

public:
  ConfigParser()
  {
  }

  void parse(const std::string& filename)
  {
    std::ifstream ifs(filename);
    if (!ifs) {
      throw IException("File not found %s", filename.c_str());
    }
    std::string line;

    //Empty section
    std::string section = "";

    while (!ifs.eof()) {
      std::getline(ifs, line);
      std::string trimmed = splitter.trim(line);
      //printf("trimmed %s¥n", trimmed.c_str());
      if (trimmed.empty()) {
        //emtpy line
        continue;
      }
      if (trimmed.find(";") == 0) { 
        //comment
        continue;
      }
      
      std::string new_section= "";
      if (is_section(trimmed, new_section)) {
        section = new_section;
      } 
      
      parse_line(section, trimmed);
    }
  }

  //If a trimmed string is of the format "[Section]", then return true else false.
  bool is_section(const std::string& string, std::string& section)
  { 
    bool found = false;
    const char& head = string.at(0);
    const char& tail = string.at(string.length()-1);
    if (head == '[' && tail== ']') {
      std::string sec = string.substr(1, string.length()-2);
      // Imagine section may have a format [  Foo ], not [Foo].
      section = splitter.trim(sec); 
      //printf("Section %s\n", section.c_str()); 
      found = true;
    }
    return found;
  }

  void parse_line(const std::string section, const std::string& trimmed)
  {
      std::string colon = ":";
      std::string name, value;
      // Split a string of the format name = value to be name and value.
      if (splitter.split(trimmed, name, value)) {
        //printf("splitted '%s'|'%s'\n", name.c_str(), value.c_str());
        std::string env;
        // This is an experimental code to handle the case
        // that a value may have a format something like 
        // dataset = $(HOME)/work/dataset/
        if (splitter.find_env_liken(value.c_str(), env)) {
          //If value contains an env_liken substring something like $(HOME) 
          //printf("env '%s'\n", env.c_str());
          std::string envv = getenv(env.c_str());
          if (!envv.empty()) {
            //printf("env '%s'\n", envv.c_str());
            std::string renv= std::string("$(") + env + std::string(")");
   
            //printf("%s to be replaced to '%s'\n", renv.c_str(), envv.c_str());
            value.find(renv);
            auto pos = value.find(renv); 
            auto len = renv.length();
            if (pos != std::string::npos) {
              value.replace(pos, len, envv);
             
              std::string sectioned_key = section + colon + name;
              /*printf("sectioned_key %s expanded value '%s'\n", 
                      sectioned_key.c_str(),
	              value.c_str());
              */
              map[sectioned_key] = value;
            }
          } else {
            //Not found by getenv() function
            std::string sectioned_key = section + colon + name;
            map[sectioned_key] = value;
          }
        } else {
          //No env_liken something like $(HOME) found.
          std::string sectioned_key = section + colon + name;
          map[sectioned_key] = value;
        }
      }
  }

  const std::string& get(const char* section, const char* name)
  {
    std::string colon=":";
    std::string sectioned_key = std::string(section) + colon
                              + std::string(name);
    return map[sectioned_key];
  }

  const std::string& get(const std::string section, const std::string& name)
  {
    std::string colon=":";
    std::string sectioned_key = section + colon + name;
    return map[sectioned_key];
  }


  const char* getString(const char* section, const char* name)
  {
    const std::string& string = get(section,  name);
    return string.c_str(); 
  }

  int getInt(const char* section, const char* name)
  {
    const std::string& string = get(section,  name);
    const char* cstring= string.c_str(); 
    return atoi(cstring);
  }

  float getFloat(const char* section, const char* name)
  {
    const std::string& string = get(section,  name);
    const char* cstring= string.c_str();
    return atof(cstring);
  }


  bool getBool(const char* section, const char* name)
  {
    bool rc = false;
    const std::string& string = get(section,  name);
    if (string == "true" || string == "True") {
      rc = true;
    }
    return rc;
  }

  void dump()
  {
    for(auto it = map.begin(); it != map.end(); it++) {
      std::cout << "key = " << it->first
                << ", val = " << it->second << "\n";
    }
  }
};

}

