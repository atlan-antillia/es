/******************************************************************************
 *
 * Copyright (c) 2014   TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *      AuxDef.h
 *
 *****************************************************************************/

#pragma once

//#define True  1
//#define False  0

#define SizeOf(a)  (int)(sizeof(a)/sizeof(a[0]))
#define CountOf(a)  (int)(sizeof(a)/sizeof(a[0]))

#define SLASH  '/'
#define ZERO    '\0'
const int INVALID_SOCKET =-1;
const int INVALID_VALUE  =-1;

#define DZERO  (0.0f)

#define INVALID_PTR ((void*)(-1))

#define NEWLINE '\n'

const int ERROR =-1;
const int OK    = 0;

//2015/05/25
const int INVALID_FD =-1;

#define DEFAULT_DIR_PERMISSION    (0777)

#define RWX_FOR_ALL_PERMISSION    (0777)
#define RW_FOR_ALL_PERSMISSION    (0666)
#define DEFAULT_PERMISSION        (0666)

#define ERROR_STRING              (strerror(errno))

typedef unsigned long Key;
typedef char  Boolean;
typedef unsigned int  uint;

typedef struct {
  const char* name;
  long int value;
} NamedValue;


#define MATH_PI   3.14159265f

#ifdef DEBUG
#define dprintf printf
#else
#define dprintf
#endif