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
 *      GrayScaleImageWriter.cpp
 *
 *****************************************************************************/

#include <es++/opencv-4.0/OpenCVObject.h>
#include <es++/Exception.h>
#include <es++/File.h>

int main(int argc, char** argv)
{
  if (argc != 3) {
    printf("Usage: %s sourceImageFile grayScaleImageFile \n", argv[0]); 
    return 1;
  }
  try {
    const char* filename = argv[1]; 
    const char* grayscalename = argv[2];

    Es::File file(filename);
    if (file.exists() == false) {
      throw Es::IException("File not found: %s", filename);
    }

    cv::Mat colorMat = cv::imread(filename, cv::IMREAD_COLOR);
    if (colorMat.empty()) {
      throw Es::IException("Failed to cv::imread file %s", filename);
    } 
    cv::Mat grayMat;
    cv::cvtColor(colorMat, grayMat, CV_BGR2GRAY);
    //cv::imwrite(grayMat, "./flower.gray.png");
    bool rc = cv::imwrite(grayscalename, grayMat);
    if (rc == false) {
      throw Es::IException("Failed to cv::imwrite: %s", grayscalename);
    } else {
      printf("Converted colorImagefile=%s to grayscaleImage=%s\n", 
           filename, grayscalename);
    }
  } catch (Es::Exception &ex) {
    caught(ex);
  }
}

