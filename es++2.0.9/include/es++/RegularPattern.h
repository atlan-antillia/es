/******************************************************************************
 *
 * Copyright (c) 2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  RegularPattern.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <es++/Registers.h>

/*
The folllowing 're_pattern_buffer' definition has been taken from regex.h

struct re_pattern_buffer
{
  unsigned char *buffer;

  // Number of bytes to which `buffer' points. 
  unsigned long int allocated;

  // Number of bytes actually used in `buffer'. 
  unsigned long int used;

  // Syntax setting with which the pattern was compiled. 
  reg_syntax_t syntax;

  // Pointer to a fastmap, if any, otherwise zero.  re_search uses
  // the fastmap, if there is one, to skip over impossible
  // starting points for matches.  
  char *fastmap;

  // Either a translate table to apply to all characters before
  // comparing them, or zero for no translation.  The translation
  // is applied to a pattern when it is compiled and to a string
  // when it is matched.  
  RE_TRANSLATE_TYPE translate;

  // Number of subexpressions found by the compiler.  
  size_t re_nsub;

  // Zero if this pattern cannot match the empty string, one else.
  // Well, in truth it's used only in `re_search_2', to see
  // whether or not we should use the fastmap, so we don't set
  // this absolutely perfectly; see `re_compile_fastmap' (the
  // `duplicate' case).  
  unsigned can_be_null:1;

  // If REGS_UNALLOCATED, allocate space in the `regs' structure
  // for `max (RE_NREGS, re_nsub + 1)' groups.
  // If REGS_REALLOCATE, reallocate space if necessary.
  // If REGS_FIXED, use what's there.  
  unsigned regs_allocated:2;

  // Set to zero when `regex_compile' compiles a pattern; set to one
  // by `re_compile_fastmap' if it updates the fastmap.
  unsigned fastmap_accurate:1;

  // If set, `re_match_2' does not return information about
  // subexpressions.  
  unsigned no_sub:1;

  // If set, a beginning-of-line anchor doesn't match at the
  // beginning of the string. 
  unsigned not_bol:1;

  // Similarly for an end-of-line anchor. 
  unsigned not_eol:1;

  // If true, an anchor at a newline matches. 
  unsigned newline_anchor:1;
};
 */


namespace Es {

class RegularPattern {
private:
  struct re_pattern_buffer buffer; 

public:
  RegularPattern(const char* pattern)
  {
    memset(&buffer, 0, sizeof(buffer));

    if (pattern == NULL) {
      throw Es::IException("Invalid argument. pattern is NULL");
    }

    const char* rc= re_compile_pattern(pattern, strlen(pattern), &buffer); 
    if (rc != NULL) {
      throw Es::IException("Failed to re_compile_pattern for pattern:\"%s\"",
        pattern);
    }
  }

public:
  ~RegularPattern()
  {
    regfree(&buffer);
  }

  int search(const char* string,
            int start, int range, struct re_registers* regs)
  {
    if (string == NULL) {
      throw Es::IException("Invalid argument. string is NULL.");
    }
    int rc = re_search(&buffer, string,
            strlen(string), start, range, regs);
    if (rc == -1) {
      throw Es::IException("Didn't match: \"%s\"", string);
    }

    if (rc == -2) {
      throw Es::IException("Internal error");
    }
    return rc;
  }
  

  int search_2(const char* string1,
              const char* string2,
             int start, int range, struct re_registers* regs, int stop)
  {
    if (string1 == NULL) {
      throw Es::IException("Invalid argument: string1 is NULL");
    }
    if (string2 == NULL) {
      throw Es::IException("Invalid argument: string2 is NULL");
    }
    int rc =  re_search_2(&buffer, string1,
             strlen(string1), string2, strlen(string2),
             start, range, regs, stop);
    if (rc == -1) {
      throw Es::IException("Didn't match: \"%s\" \"%s\"", string1, string2);
    }

    if (rc == -2) {
      throw Es::IException("Internal error");
    }
    return rc; 
  }

  int match(const char* string,
             int start, struct re_registers* regs)
  {
    if (string == NULL) {
      throw Es::IException("Invalid argument: string1 is NULL");
    }
    int rc =  re_match(&buffer, string,
             strlen(string), start, regs);
    if (rc == -1) {
      throw Es::IException("Didn't match: \"%s\"", string);
    }

    if (rc == -2) {
      throw Es::IException("Internal error");
    }
    return rc;
  }
  
  int match_2(const char* string1,
              const char* string2, 
             int start, struct re_registers* regs, int stop)
  {
    if (string1 == NULL) {
      throw Es::IException("Invalid argument: string1 is NULL");
    }
    if (string2 == NULL) {
      throw Es::IException("Invalid argument: string2 is NULL");
    }
    int rc = re_match_2(&buffer, string1,
             strlen(string1), string2, strlen(string2),
             start, regs, stop);
    if (rc == -1) {
      throw Es::IException("Didn't match: \"%s\" \"%s\"", string1, string2);
    }

    if (rc == -2) {
      throw Es::IException("Internal error");
    }
    return rc;
  }
  
/*
  The following comment has been taken from regex.h.
 */
  /* Set REGS to hold NUM_REGS registers, storing them in STARTS and
   ENDS.  Subsequent matches using BUFFER and REGS will use this memory
   for recording register information.  STARTS and ENDS must be
   allocated with malloc, and must+ each be at least `NUM_REGS * sizeof
   (regoff_t)' bytes long.

   If NUM_REGS == 0, then subsequent matches should allocate their own
   register data.

   Unless this function is called, the first search or match using
   PATTERN_BUFFER will allocate its own register data, without
   freeing the old data.  */
   
  void setRegisters(struct re_registers* regs,
             unsigned num_regs, regoff_t* starts, regoff_t* ends)
  {
    re_set_registers(&buffer, regs,
              num_regs, starts, ends);
  }
              

/* Typical syntax options.
#define RE_SYNTAX_EMACS
#define RE_SYNTAX_AWK
#define RE_SYNTAX_GNU_AWK
#define RE_SYNTAX_POSIX_AWK
#define RE_SYNTAX_GREP
#define RE_SYNTAX_EGREP
#define RE_SYNTAX_POSIX_EGREP
#define RE_SYNTAX_ED RE_SYNTAX_POSIX_BASIC
#define RE_SYNTAX_SED RE_SYNTAX_POSIX_BASIC
#define _RE_SYNTAX_POSIX_COMMON
#define RE_SYNTAX_POSIX_BASIC
#define RE_SYNTAX_POSIX_MINIMAL_BASIC
#define RE_SYNTAX_POSIX_EXTENDED
#define RE_SYNTAX_POSIX_MINIMAL_EXTENDED
 */
  static void setSyntaxOptions(int options=RE_SYNTAX_EGREP)
  {
    //Set the options to the global variable 're_syntax_options'.
    re_syntax_options = options;
  }
};
}

