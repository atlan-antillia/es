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
 *  Base64FileDecoder.h
 *
 *****************************************************************************/


#include <es++/Bytes.h>
#include <es++/Base64Decoder.h>

#include <es++/File.h>
#include <es++/Folder.h>


namespace Es {

class Base64FileDecoder: public Base64Decoder {

public:
  /**
   * Constructor.
   */
  Base64FileDecoder()
  {

  }


public:
  /**
   * Write Base64 decoded string to an output file.
   */
  virtual bool write(unsigned char* enc, const int len, FILE* out)
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
  bool decode(const char* filePath, const char* decodeFile) {
    bool rc = false;
    File file(filePath);
    if (file.exists() !=0) {
      printf("File not found: %s\n", filePath);
      return rc;
    }
      
    FILE* in = fopen(filePath, "r");
    if (in) {
      //printf("Opened %s\n", filePath);
      Folder folder(decodeFile);

      if (folder.exists() == false) {
        if (folder.create() == false) {
          fclose(in);
          printf("Failed to create a folder for %s\n", decodeFile);
          return rc;
        }
      }

      FILE* out = fopen(decodeFile, "wb");

      if (out) {
        try {
          char data[80];

          while (fgets(data,sizeof(data), in)) {
          
            unsigned char* dec = NULL;
            //unsigned int  dlen = 0;
            int  dlen = 0;
            int r = Base64Decoder::decode(data, strlen(data), &dec, &dlen);
            if (r>0) {
              rc = write(dec, dlen, out);
            }
            delete [] dec;  
          }
        } catch (...) {

        }
        fclose(out);
      } else {
        printf("Faild to open an output file %s\n", decodeFile);
      }
      fclose(in);
    }
    return rc;
  }
};

}
 
