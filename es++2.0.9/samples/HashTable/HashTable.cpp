/******************************************************************************
 *
 * Copyright (c) 2017   TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  HashTable.cpp
 *
 *****************************************************************************/


#include <es++/HashTable.h>
#include <es++/Item.h>


int main(int argc, char** argv)
{
  HashTable* hashTable = new HashTable(113);
  Item*   item1 = new Item("New", 1);
  Item*   item2 = new Item("Open",2);
  Item*   item3 = new Item("Save",3);
  Item*   item4 = new Item("Exit",4);

  hashTable -> add(item1->getText(), item1);
  hashTable -> add(item2->getText(), item2);
  hashTable -> add(item3->getText(), item3);
  hashTable -> add(item4->getText(), item4);

  Item* it1 = (Item*)hashTable -> lookup("Open");
  printf("%s %lu\n", it1-> getText(), it1->getId());

  Item* it2 = (Item*)hashTable -> lookup("Exit");
  printf("%s %lu\n", it2-> getText(), it2->getId());

  return 0;
}

