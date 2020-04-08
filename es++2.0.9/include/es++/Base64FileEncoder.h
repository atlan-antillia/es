/******************************************************************************
 *
 * Copyright (c) 2014  Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED. 
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
 *  Base64FileEncoder.h
 *
 *****************************************************************************/


#include <es++/Base64Encoder.h>
#include <es++/Folder.h>


namespace Es {

class Base64FileEncoder: public Base64Encoder {
private:
  bool insertNewLine;
public:
  /**
   * Constructor.
   */
  Base64FileEncoder(bool newLine=true)
  :insertNewLine(newLine)
  {

  }

public:
  void setInsertNewLine(bool newLine) {
    this->insertNewLine = newLine;
  }

public:
  /**
   * Write Base64 encoded string to an output file.
   * After each line of 76 characters, a new-line code (\n) is inserted.
   */
  virtual bool writeWithNewLine(const char* enc, const int len, FILE* out)
  {
    bool rc = false;
    int rx = 0;
    const int SEVENTY_SIX = 76;

    int lines   = len / SEVENTY_SIX;
    int remains = len % SEVENTY_SIX;

    const char* NEW_LINE = "\n";
    
    for (int i = 0; i<lines; i++) {
      rx += fwrite(enc, 1, SEVENTY_SIX, out);
      rx += fwrite(NEW_LINE, 1, strlen(NEW_LINE), out);
      enc += SEVENTY_SIX;
    }
    if (remains>0) {
      rx += fwrite(enc, 1, remains, out);
    }
    if (rx == (lines + len)) {
      printf("OK,File written\n");
      rc = true;
    }
    return rc;
  }

public:
  /**
   * Write Base64 encoded string to an output file.
   */
  virtual bool write(const char* enc, const int len, FILE* out)
  {
    bool rc = false;
    int rx = fwrite(enc, 1, len, out);
    if (rx == len) {
      rc = true;
    }
    return rc;
  }

public:
  /**
   * Base64-encoding for the file specified by the first argument,
   * and write the encoded string to the file specified by the second argument.
   * If the output file already exists, then the contents of the file will be 
   * replaced by new encoded string.
   */
  bool encode(const char* filePath, const char* encodeFile) {
    bool rc = false;
    File file(filePath);
    if (file.exists() != 0) {
      printf("File not found: %s\n", filePath);
      return rc;
    }
    FILE* in = fopen(filePath, "rb");

    if (in) {
      printf("Opened %s\n", filePath);
      
      Folder folder(encodeFile);

      if (folder.exists() == false) {
        if (folder.create() == false) {
          fclose(in);
          printf("Failed to create a folder for %s\n", encodeFile);
          return rc;
        }
      }
      FILE* out = fopen(encodeFile, "w");

      if (out) {
        printf("Opened write file=%s\n", encodeFile);
        try {
          //long int len = file.getFileSize();
          const int BUFFER_SIZE=1024;

          unsigned char data[BUFFER_SIZE];  // = new unsigned char[len];
          
          int rlen = 0;
          while ((rlen = fread(data, 1, BUFFER_SIZE, in)) >0) {
            printf("fread %d bytes\n", rlen);
            //data[rlen] = '\0';
            char* enc = Base64Encoder::encode(data, rlen);
            if (enc) {
              if (insertNewLine) {
                rc = writeWithNewLine(enc, strlen(enc), out);
              } else {
                rc = write(enc, strlen(enc), out);
              }
            }
            delete [] enc;  
          }
        } catch (...) {

        }
        fclose(out);
      } else {
        printf("Faild to open an output file %s\n", encodeFile);
      }
      fclose(in);
    }
    return rc;
  }
};

}
 
