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
 *  OpenCVMainView.h
 *
 *****************************************************************************/

#pragma once

#include <es++/opencv-4.0/OpenCVObject.h>
#include <es++/gtkmm-3.0/Application.h>
#include <es++/gtkmm-3.0/ApplicationView.h>

using namespace Gtk;

namespace Es {

class OpenCVMainView :public Es::ApplicationView {

public:
  //////////////////////////////////////////////
  //Constructor
  //
  OpenCVMainView(
          Es::Application& applet, 
          std::string&     name,
          Es::Args&        args)

  :ApplicationView(applet, name, 
                     //We don't use the defaultHelpPulldown menu.
                     args.set(XmNuseDefaultHelpPulldown, false) )
  {
    //Define Help pulldown menu items. 
    static Es::SigcMenuCallback help_menu_callbacks[] = {
        {"Help Info",      sigc::mem_fun(*this, &OpenCVMainView::help_info)},
        {"Version",        sigc::mem_fun(*this, &OpenCVMainView::help_version)},
        {"OpenCV Version", sigc::mem_fun(*this, &OpenCVMainView::help_opencv_version)},
    };
    help_pulldown_append(help_menu_callbacks, CountOf(help_menu_callbacks));
  }

  virtual void help_opencv_version()
  {
    Gtk::MessageDialog dialog(*this, "OpenCV Version");
    char message[1024];
    sprintf(message,  "Major Version: %d  Minor Version: %d SubMinor_Version: %d",
           CV_MAJOR_VERSION,
           CV_MINOR_VERSION,
           CV_SUBMINOR_VERSION);
    dialog.set_secondary_text(message);

    dialog.set_title("OpenCV Version");
    dialog.run();
  }

  virtual bool render()
  {
    return true;
  }
};

}

