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
 *  Node.h
 *
 *****************************************************************************/

#pragma once

#include <es++/LinkedList.h>

namespace Es {

class Node :public Object {
private:
  float   x;
  float   y;
  char*   name;
  void*   data;
  Node*  parent;
  LinkedList*  children;
  
public:
  Node(const char* name1, int nid)
  {
    setId(nid);

    parent = NULL;
    x = (float)0.0;
    y = (float)0.0;

    name = NULL;

    if(name1) {
      name = __strdup(name1);
    }

    data     = NULL;
    children = new LinkedList();
  }

public:
  Node(Node* parent1, const char* name1, int nid)
  {
    setId(nid);

    parent = parent1;
    x = (float)0.0;
    y = (float)0.0;

    name = NULL;
    if (name1) {
      name = __strdup(name1);
    }

    data     = NULL;
    children = new LinkedList();
  }

public:
  ~Node()
  {
    if (name) {
        delete [] name;
      name = NULL;
    }
    if (data) {
        //delete data;
      data = NULL;
    }
    if (children) {
      delete children;
      children = NULL;
    }
  }

public:
  int configure(int nx, int ny)
  {
    x = (float)nx;
    y = (float)ny;

    nx++;
    int num = 0;
    int n = ny;
    ListEntry* ptr = children -> getEntry();
    while (ptr) {
      Node* node = (Node*) ptr -> getObject();
//    n++;
      n = node -> configure(nx, n);
      n++;
      num++;
      ptr = ptr -> getNext();
    }
    if (num > 0) n--;
    return n;
  }

public:
  float reconfigure()
  {
    int   num = 0;
    float sum = (float)0.0;

    ListEntry* ptr = children -> getEntry();
    while (ptr) {
      Node* node = (Node*)ptr -> getObject();
      sum = sum + node ->reconfigure();
      num++;
      ptr = ptr -> getNext();
    }
    if (num > 0) y = sum/(float)num;
    return y;
  }

public:
  Node* search(const char* name1)
  {
    //
    if (name1 == NULL || *name1 == ZERO) {
      return NULL;
    }

    if (strcmp(name, name1) == 0) {
      return this;
    }

    ListEntry* ptr = children -> getEntry();
    while (ptr) {
      Node* node  = (Node*) ptr -> getObject();
      Node* found = node -> search(name1);
      if (found) return found;
      ptr = ptr -> getNext();
    }
    return NULL;
  }

public:
  Node* search(int nid)
  {
    int id = getId();

    if(id == nid) {
      return this;
    }

    ListEntry* ptr = children -> getEntry();
    while (ptr) {
      Node* node  = (Node*) ptr -> getObject();
      Node* found = node -> search(nid);
      if(found) {
        return found;
      }
      ptr = ptr -> getNext();
    }
    return NULL;
  }


  void   add(Node* node) { 
    children -> add(node); 
  }

//  int    configure(int x, int y);

  LinkedList*   getChildren() { 
    return children; 
  }

  void*   getData() { 
    return data; 
  }

  char*   getName() { 
    return name;
  }

  Node*  getParent() {
    return parent; 
  }

  float  getX() { 
    return x; 
  }

  float   getY() { 
    return y; 
  }

  void  setData(void* data1) { 
    data = data1; 
  }

  void  setParent(Node* parent1) { 
    parent = parent1; 
  }
};

}

