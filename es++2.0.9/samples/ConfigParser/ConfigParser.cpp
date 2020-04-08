/******************************************************************************
 *
 * Copyright (c) 2019  TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
 *  ConfigParser.cpp
 *
 *****************************************************************************/


#include <es++/Locale.h>
#include <es++/ConfigParser.h>

int main(int argc, char* argv[])
{
  Locale locale;
  try {
    const std::string filename ="./test.ini";

    ConfigParser parser;
    parser.parse(filename);

    parser.dump();

    int x = parser.getInt("Window", "x");
    int y = parser.getInt("Window", "y");
    int w = parser.getInt("Window", "width");
    int h = parser.getInt("Window", "height");

    printf("Window:  x=%d, y=%d, width=%d, height=%d\n", x, y, w, h);

    float min  = parser.getFloat("Score", "minimum");
    float max  = parser.getFloat("Score", "maximum");
    bool valid = parser.getBool("Score", "validation");

    printf("Score: minimum=%1.3f, maximum=%1.3f, validation=%d\n", min, max, (int)valid);

    const char* value = nullptr;
    value = parser.getString("CFG_FILE", "filename");
    printf("CFG_FILE:    filename=%s\n", value);

    value = parser.getString("WEIGHT_FILE", "filename");
    printf("WEIGHT_FILE: filename%s\n", value);

    value = parser.getString("COCO_FILE", "filename");
    printf("COCO_FILE:   filename=%s\n", value);

  } catch (Exception& ex) {
    caught(ex);
  } 
  return 0;
}

