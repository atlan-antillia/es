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
 *  Logger.h
 *
 *****************************************************************************/

#pragma once

#include <es++/LogWriter.h>

namespace Es {
  
#define err(format, ...)                                  \
  do {                                                   \
    LogWriter::err(                                  \
      L"%s (%d) %s: " format, __FILE__, __LINE__,   \
      __func__ , ##__VA_ARGS__);                     \
  } while(0)

#define dump(message, data, len)                  \
  do {                                                        \
    LogWriter::dump(message, data, len,                    \
     "DUMP: %s (%d) %s: ", __FILE__, __LINE__, __func__);  \
           \
  } while(0)

#ifdef DEBUG

#define debug(format, ...)                                \
  do {                                                   \
    LogWriter::debug(                                  \
      L"%s (%d) %s: " format, __FILE__, __LINE__, \
      __func__ , ##__VA_ARGS__);                     \
  } while(0)
#else

#define debug(fmt, ...)

#endif


}


