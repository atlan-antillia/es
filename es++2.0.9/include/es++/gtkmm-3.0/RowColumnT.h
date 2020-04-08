/******************************************************************************
 *
 * Copyright (c) 2017  TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
 *  RowColumnT.h
 *
 *****************************************************************************/

#pragma once

#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <es++/gtkmm-3.0/Label.h>
#include <es++/gtkmm-3.0/TextField.h>

namespace Es {

template <class T, int ROW, int COLUMN> 
    class RowColumnT :public Gtk::Grid {

private: 
  T           rowcolumn[ROW][COLUMN];
  
public:
  RowColumnT()
  {
    if (ROW <1 || COLUMN <1 ) {
      throw IException("Invalid ROW or COLUMN parameter.");
    }
    
    set_row_homogeneous(true);
    set_column_homogeneous(true) ;
    for (int i = 0; i<ROW; i++) {
      for (int j = 0; j<COLUMN; j++) {
        //rowcolumn[i][j].set_width_chars(4);
        attach(rowcolumn[i][j], i, j, 1, 1);
      }
    }    
  }

  bool validateIndex(int i, int j)
  {
    if (i >= 0 && i <=ROW &&
        j >= 0 && j <=COLUMN) {
       return true;
    } else {
      throw IException("Invalid index parameter (%d, %d)", i, j);
    }
  }

  //This applys when T is TextField 
  void set_width_chars(int i, int j, int chars_count)
  {
    validateIndex(i, j);
    rowcolumn[i][j].set_width_chars(chars_count);
  }

  void set(int i, int j, double value)
  {
    validateIndex(i, j);
    char label[20];
    sprintf(label, "%.2f", value);
    rowcolumn[i][j].set_label(label); 
  }

  void set_label(int i, int j, const char* label)
  {
    validateIndex(i, j);
    rowcolumn[i][j].set_label(label); 
  }

  void set_label(int i, int j, std::string& label)
  {
    validateIndex(i, j);
    rowcolumn[i][j].set_label(label); 
  }

  std::string get_label(int i, int j)
  {
    validateIndex(i, j);
    return rowcolumn[i][j].get_label();
  }
};
}

