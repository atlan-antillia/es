/******************************************************************************
 *
 * Copyright (c) 2015 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  OpenGLMainView.h
 *
 *****************************************************************************/

#pragma once

#include <es++/opengl/IOpenGLMainView.h>

namespace Es {

class OpenGLMainView :public Es::IOpenGLMainView {

public:
  OpenGLMainView(Es::Application& applet, std::string& name, Es::Args& args)
  :IOpenGLMainView(
           applet, name, args.set(XmNuseDefaultHelpPulldown, false) )

  {
    static Es::SigcMenuCallback help_menu_callbacks[] = {
      {"Help Info",  
             sigc::mem_fun(*this, &OpenGLMainView::help_info)},
      {"Version",   
             sigc::mem_fun(*this, &OpenGLMainView::help_version)},
      {"Context Version",   
             sigc::mem_fun(*this, &OpenGLMainView::help_context_version)},
    };
    help_pulldown_append(help_menu_callbacks, CountOf(help_menu_callbacks));
  }
  
  ~OpenGLMainView()
  {
  }

  virtual void render()
  {
    printf("render\n");
  }

  virtual void help_context_version()
  {
    Gtk::MessageDialog dialog(*this, 
                 "Copyright(C) 2017 Antillia.com ALL RIGHTS RESERVED");
    
    char message[1024];
    memset(message, 0, sizeof(message));
    const GLubyte *vendor     = glGetString(GL_VENDOR);
    const GLubyte *renderer   = glGetString(GL_RENDERER);
    const GLubyte *version    = glGetString(GL_VERSION);
    const GLubyte *glsl       = glGetString(GL_SHADING_LANGUAGE_VERSION);

    sprintf(message, 
            "Context Version\n\n" 
            "GL_VENDOR                   :%s\n"
            "GL_RENDERER                 :%s\n"
            "GL_VERSION                  :%s\n"
            "GL_SHADING_LANGUAGE_VERSION :%s\n",
             
             (const char*)vendor,
             (const char*)renderer,
             (const char*)version,
             (const char*)glsl);

    dialog.set_secondary_text(message);
    dialog.set_title("Context Version");
    dialog.run();
  }
};

}

