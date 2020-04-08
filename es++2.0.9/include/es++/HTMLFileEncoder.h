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
 *  HTMLFileEncoder.h
 *
 *****************************************************************************/


#pragma once

#include <es++/HTMLEncoder.h>
#include <es++/Folder.h>
#include <es++/File.h>


namespace Es {

class HTMLFileEncoder: public HTMLEncoder {
private:
  bool newLineEncode; 
  bool tabEncode;

public:
  HTMLFileEncoder(bool newLineEnc=true, bool tabEnc=true) 
  :newLineEncode(newLineEnc),
   tabEncode(tabEnc)
  {
  }

public:
  /**
   * Write HTML encoded string to an output file.
   * The tab code in the string is converted to 4 space characters.
   */
  virtual bool write(const char* enc, const int len, FILE* out)
  {
    bool rc = false;

    const char* spaces_4 = "    ";
    const char* espaces_4 = "&nbsp;&nbsp;&nbsp;&nbsp;";
    int rlen = 0;
    for (int i = 0; i<len; i++) {
      if (enc[i] == '\t') {
        if (tabEncode) {
          //Repace '\t' by espace_4 of four "&nbsp;" 
          rlen += fwrite(espaces_4, 1, strlen(espaces_4), out);
        } else {
          //Replace '\t' by space_4 of for " "
          rlen += fwrite(spaces_4, 1, strlen(spaces_4), out);
        }
      } else {
        rlen +=fwrite(&enc[i], 1, 1, out);
      }

    }
  
    //nt rx = fwrite(enc, 1, len, out);
    if (rlen>0) {
      //printf("input =%d bytes: fwritten %d bytes\n", len, rlen);
      rc = true;
    }
    return rc;
  }

public:
  /**
   * HTML-encoding for the file specified by the first argument,
   * and write the encoded string to the file specified by the second argument.
   * If the output file already exists, then the contents of the file will be 
   * replaced by new encoded string.
   */
  bool encode(const char* htmlFile, const char* encodeFile) {
    bool rc = false;
    File file(htmlFile);

    if (file.exists() == false) {
      printf("HTML File not found: %s\n", htmlFile); 
      return rc;
    }
      
    File dest(encodeFile);

    FILE* in = fopen(htmlFile, "r");
    if (in) {
      if (dest.exists() == false) {
        if (strchr(encodeFile, SLASH) ) {
          //If encodeFile contains SLASH
          Folder folder(encodeFile);
          if (folder.exists() == false) {
            //Try to create a folder
            try {
              if (folder.create() == false) {
                fclose(in);
                printf("Failed to create a folder for %s\n", encodeFile);
                return rc;
              }
            } catch (Es::Exception& ex) {
              ex.display();
              fclose(in);
              return rc;
            }
          } else {
             //printf("Folder (%s) exists\n", encodeFile);
          }
        } //SLASH
      } else {
         //If File dest(encodeFile) exists, it will be overwritten.
      }
      FILE* out = fopen(encodeFile, "w");
      const int BUF_SIZE = 1024*4;
      char html[BUF_SIZE]; 
      const char* brnewLine = "<br>\n";  //Replace "\n" by "<br>\n"
      const char* newLine = "\n"; 
      if (out) {
        try {
          while( fgets(html, sizeof(html), in) ) {
            //Read a line terminaed with "\n\0"
            html[strlen(html)-1]=ZERO; //Replaec '\n' by ZERO 
			//printf("%s\n", html);
            char* enc = HTMLEncoder::encode(html);
            if (enc) {
              rc = write(enc, strlen(enc), out);
              if (newLineEncode) {
                rc += write(brnewLine, strlen(brnewLine), out);     
              } else {
                rc += write(newLine, strlen(newLine), out);     
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
    } else {
      printf("Failed to open an HTML file %s.\n", htmlFile);

    }
    return rc;
  }
};

}
 
