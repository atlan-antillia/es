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
 *  SystemConfiguration.h
 *
 *****************************************************************************/

#pragma once

#include <es++/Object.h>
#include <es++/Exception.h>
#include <unistd.h>

namespace Es {
  
class SystemConfiguration :public Object {

public:
  SystemConfiguration()
  {
  }
  

  static long argMax()
  {
    //The maximum length of the arguments to the exec(3) family of
    //functions.  Must not be less than _POSIX_ARG_MAX (4096).
    return sysconf(_SC_ARG_MAX);
  }
 

  static long childMax()
  {
    //The maximum number of simultaneous processes per user ID.
    //Must not be less than _POSIX_CHILD_MAX (25).
    return sysconf(_SC_CHILD_MAX);
  }
  
  static long hostNameMax()
  {
    //Maximum length of a hostname, not including the terminating
    //null byte, as returned by gethostname(2).  Must not be less
    //than _POSIX_HOST_NAME_MAX (255).
    return sysconf(_SC_HOST_NAME_MAX);
  }
  

  static long loginNameMax()
  {
    //Maximum length of a login name, including the terminating null
    //byte.  Must not be less than _POSIX_LOGIN_NAME_MAX (9).
    return sysconf(_SC_LOGIN_NAME_MAX);
  }
  

  static long ngroupsMax()
  { 
    //Maximum number of supplementary group IDs.
    return sysconf(_SC_NGROUPS_MAX);
  }
  
  static long clockTicks()
  {
    //The number of clock ticks per second.  
    return sysconf(_SC_CLK_TCK);
  }
  
  static long openMax()
  {
    //The maximum number of files that a process can have open at
    //any time.  Must not be less than _POSIX_OPEN_MAX (20).
    return sysconf(_SC_OPEN_MAX);
  }

  static long pageSize()
  {
    //Size of a page in bytes.  Must not be less than 1.  (Some
    //systems use PAGE_SIZE instead.)
    return sysconf(_SC_PAGESIZE);
  }
  
  static long dupMax()
  {
    return sysconf(_SC_RE_DUP_MAX);
    //The number of repeated occurrences of a BRE permitted by
    //regexec(3) and regcomp(3). 
    return sysconf(_SC_RE_DUP_MAX);
  }
  
  static long streamMax()
  {
    //The maximum number of streams that a process can have open at
    //any time.
    return sysconf(_SC_STREAM_MAX);
  }
  
  static long symLoopMax()
  {
    //The maximum number of symbolic links seen in a pathname before
    //resolution returns ELOOP.
    return sysconf(_SC_SYMLOOP_MAX);
  }
  
  static long ttyNameMax()
  {
    //The maximum length of terminal device name, including the
    //terminating null byte.
    return sysconf(_SC_TTY_NAME_MAX);
  }
  
  static long tzNameMax()
  {
    //The maximum number of bytes in a timezone name.
    return sysconf(_SC_TZNAME_MAX);
  }
  
  static long version()
  {
    //indicates the year and month the POSIX.1 standard was approved
    //in the format YYYYMML; the value 199009L indicates the Sept.
    //1990 revision.
    return sysconf(_SC_VERSION);
  } 

  // POSIX.2 variables
  //      Next, the POSIX.2 values, giving limits for utilities.

  static long bcBaseMax()
  {
    //indicates the maximum obase value accepted by the bc(1)
    //utility.
    return sysconf(_SC_BC_BASE_MAX);
  }
  
  static long bcDimMax()
  {
    //indicates the maximum value of elements permitted in an array
    //by bc(1).
    return sysconf(_SC_BC_DIM_MAX);
  }
  
  static long bcScaleMax()
  {
    //indicates the maximum scale value allowed by bc(1).
    return sysconf(_SC_BC_SCALE_MAX);
  }
  
  static long bcStringMax()
  {
    //indicates the maximum length of a string accepted by bc(1).
    return sysconf(_SC_BC_STRING_MAX);
  }  
  static long collWeights()
  {
    //indicates the maximum numbers of weights that can be assigned
    //to an entry of the LC_COLLATE order keyword in the locale
    //definition file,
    return sysconf(_SC_COLL_WEIGHTS_MAX);
  }
  
  static long exprNestMax()
  {
    //is the maximum number of expressions which can be nested
    //within parentheses by expr(1).
    return sysconf(_SC_EXPR_NEST_MAX);
  }
  
  static long lineMax()
  {
    
    //The maximum length of a utility's input line, either from
    //standard input or from a file.  This includes space for a
    //trailing newline.
    return sysconf(_SC_LINE_MAX);
  }
  
  static long reDupMax()
  {
    return sysconf(_SC_RE_DUP_MAX);
    //The maximum number of repeated occurrences of a regular
    //expression when the interval notation \{m,n\} is used.
    return sysconf(_SC_RE_DUP_MAX);
  }
  
  static long versionPosix2()
  {
    //indicates the version of the POSIX.2 standard in the format of
    //YYYYMML.
    return sysconf(_SC_2_VERSION);
  }
  
  static long cLanguagePosix2()
  {
    //indicates whether the POSIX.2 C language development
    //facilities are supported.
    return sysconf(_SC_2_C_DEV);
  }
  
  static long fortranDevPosix2()
  {
    //indicates whether the POSIX.2 FORTRAN development utilities
    //are supported.
    return sysconf(_SC_2_FORT_DEV);
  }
  
  static long fortranRuntimePosix2()
  {
    //indicates whether the POSIX.2 FORTRAN run-time utilities are
    //supported.
    return sysconf(_SC_2_FORT_RUN);
  }
   
  static long localeDefPosix2()
  {
    //indicates whether the POSIX.2 creation of locates via
    //localedef(1) is supported.
    return sysconf(_SC_2_LOCALEDEF);
  }
  
  static long softwareDevPosix2()
  {
    //indicates whether the POSIX.2 software development utilities
    //option is supported.
    return sysconf(_SC_2_SW_DEV);
  }
  
  //     These values also exist, but may not be standard.
  static long physicalPages()
  {
    //The number of pages of physical memory.  Note that it is
    //possible for the product of this value and the value of
    //_SC_PAGESIZE to overflow.
    return sysconf(_SC_PHYS_PAGES);
  }
  
  static long availablePhysicalPages()
  {
    //The number of currently available pages of physical memory.
    return sysconf(_SC_AVPHYS_PAGES);
  }   
  static long nprocessorsConf()
  {
    //The number of processors configured.
    return sysconf(_SC_NPROCESSORS_CONF);
  }
  
  static long nprocessorsOnline()
  {
    //The number of processors currently online (available).
    return sysconf(_SC_NPROCESSORS_ONLN);
  }

  ////////////////
  virtual void display()
  {
    printf("ArgMax                 %ld\n", argMax());
    printf("ChildMax               %ld\n", childMax());
    printf("HostNameMax            %ld\n", hostNameMax());
    printf("LoginNameMax           %ld\n", loginNameMax());
    printf("NgroupsMax             %ld\n", ngroupsMax());
    printf("ClockTicks             %ld\n", clockTicks());
    printf("OpenMax                %ld\n", openMax());
    printf("PageSize               %ld\n", pageSize());
    printf("DupMax                 %ld\n", dupMax());
    printf("StreamMax              %ld\n", streamMax());
    printf("SymLoopMax             %ld\n", symLoopMax());
    printf("TtyNameMax             %ld\n", ttyNameMax());
    printf("TzNameMax              %ld\n", tzNameMax());
    printf("Version                %ld\n", version());
    // POSIX.2 variables
    printf("BcBaseMax              %ld\n", bcBaseMax());
    printf("BcDimMax               %ld\n", bcDimMax());
    printf("BcScaleMax             %ld\n", bcScaleMax());
    printf("BcStringMax            %ld\n", bcStringMax());
    printf("CollWeights            %ld\n", collWeights());
    printf("ExprNestMax            %ld\n", exprNestMax());
    printf("LineMax                %ld\n", lineMax());
    printf("ReDupMax               %ld\n", reDupMax());
    printf("VersionPosix2          %ld\n", versionPosix2());
    printf("CLanguagePosix2        %ld\n", cLanguagePosix2());
    printf("FortranDevPosix2       %ld\n", fortranDevPosix2());
    printf("FortranRuntimePosix2   %ld\n", fortranRuntimePosix2());
    printf("LocaleDefPosix2        %ld\n", localeDefPosix2());
    printf("SoftwareDevPosix2      %ld\n", softwareDevPosix2());
    printf("PhysicalPages          %ld\n", physicalPages());
    printf("AvailablePhysicalPages %ld\n", availablePhysicalPages());
    printf("NprocessorsConf        %ld\n", nprocessorsConf());
    printf("NprocessorsOnline      %ld\n", nprocessorsOnline());
  }

};

}

