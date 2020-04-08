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
 *  StringT.h
 *
 *****************************************************************************/

#pragma once
#include <es++/Object.h>

namespace Es {

template <class T> class StringT :public Es::Object {

private:
  int  length;
   T*  text;
  
private:
  bool copy(const T* string)
  {
    assert(string);
    bool rc = false;
    int len = __strlen(string);

    T* temp = new T[len+1];
    if(temp) {
      __strcpy(temp, string);
      if(this->text != string && this->text !=NULL) {
        delete [] this->text;
      }
      this->text = temp;
      this->length = len;
      rc = true;
    } else {
      throw Es::IException("Memory allocation error.");
      assert(temp == NULL);
    }
    return rc;
  }

public:
  /**
   */
  StringT()
    :length(0),
    text(NULL)
  {
  }

public:
  StringT(T* array, int len)
  :length(0),
  text(NULL) 
  {   
    assert(array);
    assert(len>0); 
    deepCopy(array, len);
  }

public:
 
  StringT(const T* string, bool deepCopy=true)
  :length(0),
  text(NULL) 
  { 
    //2016/04/23
    if (string) {
    //assert(string);

      if (deepCopy) {
        copy(string);
      } else {
        shallowCopy((T*)string);
      }
    }
  }


public:
  StringT(const StringT* string)
  :length(0),
  text(NULL) 
  {   
    assert(string);
    const T*  str = string->getContents();
    assert(str);

    copy(str);
  }


public:
  StringT(const StringT& string)
  :length(0),
  text(NULL)
  {
    const T* str = string.getContents();
    if (str) {
      copy(str);
    }
  }

public:
  ~StringT() 
  { 
    clear();
  }

private:
  void clear() 
  { 
    if (this->text) {
      delete [] this->text;
    }
    this->text =NULL;
    this->length = 0;
  }


public:
  T*   getContents() const 
  { 
    return this->text; 
  }

public:
  int     getLength() const  
  { 
    return this->length; 
  }

public:
  bool isEmpty() 
 {
    bool rc = false;
    if (this->length>0 && this->text !=NULL) {
      rc = true;
    }
    return rc;
  }


public:
  operator T*() const
  {
    return this->text;
  }

public:
  operator const T*() const
  {
    return this->text;
  }

public:
  StringT& operator=(const T* string)
  { 
    if (string != NULL) {
      if(copy(string) == false) {
        throw Es::IException("Failed to substitue a string");
      }
    }
    return *this;
  }

public:
  // The string parameter can be non-NULL terminated.
  void deepCopy(T* string, int length)
  {
    if (string != NULL && length>0) {
      if (this->text != string) {
        clear();
      }
      int len = length;

      T* temp = new T[len+1];
      __memcpy(temp, string, len);
      temp[len] = ZERO;  //Put a NULL char at this len position to make a NULL terminated string
      this->text = temp;
      this->length = len;
    } else {
      clear();

      this->text   = NULL;
      this->length = 0;
    }
  }

public:
  void shallowCopy(T* string)
  { 
    // Accept a string of NULL
    if (string) {
      if (this->text != string) {
        clear();
      }
      this->text   = string;
      this->length = __strlen(string);
    }
    else {
      clear();

      this->text   = string;  //NULL
      this->length = 0;
    }    
  }

public:
  StringT& operator=(const StringT& string)
  {        
    //this->length = string.getLength();
    T* str = string.getContents();
    if (str) {
      if (copy(str) == false) {
        throw Es::IException("Failed to substitue a string");
      }
    } else {
      //str = NULL;
      clear();

      this->text = str;
      this->length = 0;
    }
    return *this;
  }

public:
  int operator==(const T* string)
  {
    int rc = 0;
    if (this->text == NULL && string ==NULL) {
      rc = 1;
    }

    //2008/07/08
    if (this->text != NULL && string != NULL) {
      if(__strcmp(this->text, string) == 0) {
        rc = 1;
      }
    }
    return rc;
  }


  //int operator==(String& string)
public:
  int operator==(StringT& string)
  {
    int rc = 0;
    const T* str = (const T*)string;
    if (this->text == NULL && str ==NULL) {
      rc = 1;
    }

    if (this->text != NULL && str != NULL) {
      if(__strcmp(this->text, str) == 0) {
        rc = 1;
      }
    }
    return rc;
  }

public:
  StringT& operator+(const T* string)
  {
    if(string == NULL) {
      return *this;
    }
    size_t orglen = this->length;
    this->length += strlen(string);
    T* temp = new T[this->length+1];
    __memset(temp, (T)0, length+1);
    __memcpy(temp, this->text, orglen);
    __memcpy(temp+orglen, string, strlen(string));

    if(this->text != string) {
      delete [] text;
    }
    text = temp;
    return *this;
  }



public:
  int compare(Es::Object* object)
  {
    int rc = 0;
    if (object==NULL) {
      return rc;
    }

    StringT* string = (StringT*)object;
    const T* p1 = this->text;

    const T* p2 = (const T*)(*string);
    if (p1 == NULL && p2 == NULL) {
      rc = 1;
    }
    if (p1 != NULL && p2 != NULL) {
      rc = __strcmp(p1, p2);
    }
    return rc;
  }


public:
  const T* find(const T* string)  
  {
    const T* ptr = NULL;
    if (this->text && string) {
      ptr = __strstr(this->text, string);
    }
    return ptr;
  }


public:
  int replace(T oc, T nc)
  {
    int  n = 0;
    if (this->text) {
      for (int i = 0; i<this->length; i++) {
        if (text[i] == oc) {
          text[i] = nc;
          n++;
        }
      }
    }
    return n;
  }

public:
  StringT substring(int position, int size)
  {
    if (this->text) {
      if (position>=0 && position<this->length) {
        if ((position+size)>=this->length) {
          size = this->length - position;
        }
        return StringT(&text[position], size);
      }
    }
    return StringT(NULL, 0);
  }

public:
  StringT& operator+(T ch)
  {
    T string[2];
    string[0] = ch;
    string[1] = (T)0;//Zero;

    return StringT::operator+(string);
  }

private:
  StringT& appendFormat(T* format,...)
  {
    T buffer[128];
    memset(buffer, (T)0, 128);

    va_list pos;
    va_start(pos, format);
    __vsprintf(buffer, format, pos);
    va_end(pos);
    return StringT::operator+(buffer);
  }

public:
  StringT& operator+(int num)
  {
    T format[3] ={(T)'%',(T)'d', (T)0};  
    return appendFormat(format, num);
  }


public:
  StringT& operator+(float num)
  {
    T format[3] ={(T)'%',(T)'f', (T)0};  
    return appendFormat(format, num);

  }

public:
  StringT& operator+(double num)
  {
    T format[3] ={(T)'%',(T)'f', (T)0};  
    return appendFormat(format, num);
  }


public:
  bool equals(const T* str) {
    bool rc = false;
    if(this->text != NULL && str != NULL) {
        if(__strcmp(this->text, str) == 0) {
             rc = true;
      }
    }
    return rc;
  }

public:
  bool equals(const StringT& str) {
    return equals((const T*)str);
  }

public:
  bool equalsIgnoreCase(const T* str) {
    bool rc = false;

    if(this->text != NULL && str != NULL) {
      //strcmpi -> stricmp
        if(__strcasecmp(this->text, str) == 0) {
             rc = true;
      }
    }
    return rc;
  }

public:
  bool equalsIgnoreCase(const StringT& str) {
    return equalsIgnoreCase((const T*)str);
  }


public:
  void trim()
  {
    T* rt = StringT::trim(this->text);

    if (rt) {
      clear();
      //Shallow copy
      this->text = rt;
      this->length = __strlen(rt);
    }
  }

public:
  void trimOnNewLine() {
    if (this->text) {
      T newLine[1] = { (T)'\n'};

      T* ptr = (T*)__strstr(this->text, newLine);
      if (ptr) {
        //Terminate this text on this ptr position by putting '\0';
        *ptr = '\0';
        this->length = __strlen(this->text);
      }
    }
  }


public:
  /**
   */  
  bool   startsWith(const T* start) {
    return StringT::startsWith(this->text, start);
  }

public:
  /**
   */  
  bool   startsWithIgnoreCase(const T* start) {
    return StringT::startsWithIgnoreCase(this->text, start);
  }

public:
  /**
   */  
  bool   endsWith(const T* end) {
    return StringT::endsWith(this->text, end);
  }

public:
  /**
   */  
  bool   endsWithIgnoreCase(const T* end) {
    return StringT::endsWithIgnoreCase(this->text, end);
  }

public:
  //2009/11/12
  const T* findLast(const T* string)
  {
    return StringT::findLast(this->text, string);
  }

public:
  //2009/11/12
  static const T* findLast(const T* text, const T* string)
  {
    const T* ptr = NULL;

    if (text && string) {
      int tlen = __strlen(text);
      int slen = __strlen(string);
      if (tlen < slen) {
        return ptr;
      } else {
        for(int i = tlen - slen; i>=0; i--) {
          int n= __strncmp(text + i, string, slen);
          if (n == 0) {
            ptr = text + i;
            break;
          }
        }
      }  
    }
    return ptr;
  }

public:
  const T* findFirst(const T* string) {
    return StringT::findFirst(this->text, string);    
  }

public:
  static const T* findFirst(const T* text, const T* string) {
    const T* ptr = NULL;
    if (text && string) {
      ptr = __strstr(text, string);
    }
    return ptr;
  }


public:
  T&    operator[](int n) 
  {
    if (this->text == NULL) {
      throw Es::IException("String::operator[] - Text is NULL");
    }
    if (n <=0 || n>this->length) {
      throw Es::IException("String::operator[] - Index is out of range");
    }
    
    //
    return text[n]; 
  }


//////////////////////////////////////////////////////////////////////
/// static methods.
public:
  static T* trim(T* string) {
    if (string == NULL) {
      return string;
    }
    
    size_t slen = __strlen(string)+1;

    T* temp = new T[slen];
    __strcpy(temp, string);
    T* p = temp;

    while (*p == (T)' ' || *p == (T)'\t' || *p ==(T)'\r' || *p==(T)'\n') {
      p++;
    }

    T* t = p + strlen(p) -1;
    while (*t == (T)' ' || *t == (T)'\t' || *t ==(T)'\r' || *t==(T)'\n') {
      if (p == t) break;
      *t = (T)'\0';
      t--;
    }

    size_t plen = __strlen(p)+1;
    T* rt = new T[plen];
    __strcpy(rt, plen, p);

    delete [] temp;

    return rt;
  }



public:
  /**
   * 
   * @param string  
   * @param start
   * @return bool
   *
   */  
  static bool  startsWith(const T* string, const T* start) {
    bool rc = false;
    if (string != NULL && start != NULL) {
      if (__strncmp(string, start, __strlen(start)) ==0) {
        rc = true;
      }
    }
    return rc;
  }

public:
  /**
   * 
   * @param string
   * @param start
   * @return bool
   */  
  static bool  startsWithIgnoreCase(const T* string, const T* start) {
    bool rc = false;
    if (string != NULL && start != NULL) {
  
      if (__strncasecmp(string, start, __strlen(start)) ==0) {
        rc = true;
      }
    }
    return rc;
  }


public:
  /**
   * 
   * @param string
   * @param end
   * @return bool
   */  

  static bool endsWith(const T* string, const T* end) {
    bool rc = false;
    if (string != NULL && end != NULL) {
      size_t tlen = __strlen(string);
      size_t slen = __strlen(end);
      if (tlen >= slen) {
        if (__strncmp(string+tlen-slen, end, slen) ==0) {
          rc = true;
        }
      }
    }
    return rc;
  }

public:
  /**
   * 
   * @param string
   * @param end  
   * @return bool
   */  
  static bool endsWithIgnoreCase(const T* string, const T* end) {
    bool rc = false;
    if (string != NULL && end != NULL) {
      size_t tlen = __strlen(string);
      size_t slen = __strlen(end);
      if (tlen >= slen) {
        if (strncasecmp(string+tlen-slen, end, slen) ==0) {
          rc = true;
        }
      }
    }
    return rc;
  }

  //2014/12/21 Printf a string without a new line
  virtual void write()
  {
    int mode = fwide(stdout, 0);
    if (sizeof(T) == sizeof(char)) {
      if (mode == 0) {
        printf("%s", (const char*)this->text);
      } else {
      	fwide(stdout, 0);
        wprintf(L"%s", (const char*)this->text);
      }
    }
    if (sizeof(T) == sizeof(wchar_t)) {
    	fwide(stdout, 1);
      wprintf(L"%ls", (const wchar_t*)this->text);
    }
  }

/*  virtual const T* toString()
  {
  	return text;
  }
*/	
  void display()
  {
    int mode = fwide(stdout, 0);
    if (sizeof(T) == sizeof(char)) {
      if (mode == 0) {
        printf("%s\n", (const char*)this->text);
      } else {
      	//fwide(stdout, 1);
        wprintf(L"%s\n", (const char*)this->text);
      }
    }
    if (sizeof(T) == sizeof(wchar_t)) {
    	fwide(stdout, 1);
      wprintf(L"%ls\n", (const wchar_t*)this->text);
    }
  }
};

}

