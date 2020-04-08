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
 *  OpenCVColorFilter.h
 *
 *****************************************************************************/


#pragma once

#include <es++/gtkmm-3.0/Label.h>
#include <es++/gtkmm-3.0/TextFieldRowColumn.h>
#include <es++/opencv-4.0/OpenCVObject.h>

namespace Es {

class OpenCVColorFilter :public Gtk::VBox {
private:
  static const int  KSIZE= 3;
  cv::Mat           filter;
  float             value[KSIZE][KSIZE];
  
  Es::Label         label;

  Es::TextFieldRowColumn<KSIZE, KSIZE>
                    rowcolumn; 

public:
  OpenCVColorFilter()
  {
    label.set_label("ColorFilter");
    pack_start(label,     Gtk::PACK_SHRINK);
    pack_start(rowcolumn, Gtk::PACK_SHRINK); 

    for (int i = 0; i<KSIZE; i++) {
      for (int j = 0; j<KSIZE; j++) {
        value[i][j] = 1.0f;
        rowcolumn.set(i, j, 1.0f); 
      }
    }
  }

  void set_read_only()
  {
    for (int i = 0; i<KSIZE; i++) {
      for (int j = 0; j<KSIZE; j++) {
        rowcolumn.set_read_only(i, j);
      }
    }
  }

  void set_value(int column, float r, float g, float b)
  //void set_value(int column, double r, double g, double b)
  {
    if (column >=0 && column < KSIZE) {
      int i = 0;
      
      value[i++][column] =  b;
      value[i++][column] =  g;
      value[i  ][column] =  r;
      i = 0;
      rowcolumn.set(i++, column, b); 
      rowcolumn.set(i++, column, g);
      rowcolumn.set(i++, column, r);

    } else {
      throw IException("Invalid column parameter %d", column);
    }
  }

  cv::Mat& get_filter()
  {
    //Create a value matrix filter from three Vector3fs. 
    filter = ( cv::Mat_<float>(3,3) <<
          value[0][0], value[0][1], value[0][2],
          value[1][0], value[1][1], value[1][2],
          value[2][0], value[2][1], value[2][2] );
    return filter;
  }
};

}


