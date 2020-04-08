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
 *  BufferedColorBox.cpp
 *
 *****************************************************************************/

//
#include <es++/opengl/OpenGLApplication.h>
#include <es++/opengl/OpenGLMainView.h>
#include <es++/opengl/OpenGLView.h>
#include <es++/opengl/OpenGLGC.h>
#include <es++/opengl/OpenGLArrayBuffer.h>

#include <es++/opengl/OpenGLVertex3.h>
#include <es++/opengl/OpenGLClientState.h>

#include <es++/opengl2/OpenGLVertexShader.h>
#include <es++/opengl2/OpenGLFragmentShader.h>
#include <es++/opengl2/OpenGLProgram.h>
#include <es++/opengl2/OpenGLVertexAttribute.h>
#include <es++/opengl2/OpenGLUniform.h>

#include <es++/gtkmm-3.0/FileOpenDialog.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenGLMainView {
private:
  /////////////////////////////////////////////////////// 
  //Inner class starts.

  class SimpleView :public Es::OpenGLView {
  private:
    OpenGLGC gc;
    SmartPtr<OpenGLProgram>    program;

  public:
    SimpleView()
    :OpenGLView()
    {
    }

  virtual void display()
  {
    if (program) {
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.loadIdentity();
      gc.clearColor(1.0f, 1.0f, 1.0f, 1.0f);
      
      try {
        program -> use();
    
        OpenGLClientState clientState(GL_VERTEX_ARRAY);
        
        struct Coord2Color3 {
          GLfloat coord[2];
          GLfloat color[3];
        };
        Coord2Color3 triangles[] = {
          {{ 0.0,  0.5},  { 1.0, 1.0, 0.0}},
          {{-0.5, -0.5},  { 0.0, 0.0, 1.0}},
          {{ 0.5, -0.5},  { 1.0, 0.0, 0.0}},
        };
        
        OpenGLArrayBuffer buffer;
        buffer.bind();
        buffer.data(sizeof(triangles), triangles, GL_STATIC_DRAW);

 
        OpenGLVertexAttribute coord(program->getAttributeLocation("coord2d"));
        OpenGLVertexAttribute vcolor(program->getAttributeLocation("v_color"));
        coord.setPointer(2, GL_FLOAT, GL_FALSE, sizeof(Coord2Color3), 0); 
        
        vcolor.setPointer(3, GL_FLOAT, GL_FALSE,  sizeof(Coord2Color3), (GLvoid*) (2 * sizeof(GLfloat)) );

        gc.drawArrays(GL_TRIANGLES, 0, SizeOf(triangles));
          
        buffer.unbind();
          
      } catch (Exception& ex) {
        caught(ex);
      }
    }
  }
 
  virtual void resize(int w, int h)
  {
    if (w == 0 || h == 0) {
      return;
    }
    gc.matrixMode(GL_PROJECTION);
    gc.loadIdentity();
    gc.perspective(16.0, (double)w / (double)h, 0.5, 40.0); 

    gc.matrixMode(GL_MODELVIEW);
  }

  virtual void initialize()
  {
    gc.matrixMode(GL_PROJECTION);
    gc.loadIdentity();

    gc.enable( GL_DEPTH_TEST );
    gc.frustum(1 , -1 , -1 , 1 , 1 , 10);
    
    try { 
      OpenGLVertexShader vertexShader("vertexShader.glsl");
      
      OpenGLFragmentShader fragmentShader("fragmentShader.glsl");      
      
      program = new OpenGLProgram();
      program -> attachShader(vertexShader);
      program -> attachShader(fragmentShader);

      program -> link();
      
      program -> detachShader(vertexShader);
      program -> detachShader(fragmentShader);
      
    } catch (Exception& ex) {
      caught(ex);      
    }
  }

  };

  // Inner class ends.
  /////////////////////////////////////////////////////// 

  Es::HorizontalLayout   horiz_layout;

  Es::VerticalLayout     control_pane;

  SimpleView             simple_view;

  Es::FileOpenDialog     file_dialog;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::OpenGLApplication& applet, std::string& name, Es::Args& args)
  :OpenGLMainView(applet, name, args)
  ,file_dialog(*this, Es::FileOpenDialog::IMAGE_FILES)
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);

    Es::MainLayout& main_layout = get_main_layout();

    main_layout.pack_start(horiz_layout);

    horiz_layout.pack_start(simple_view);

    OpenGLContext* context = applet.getOpenGLContext();

    simple_view.set_opacity (0.5);

    simple_view.set_opengl_context(context);

    simple_view.set_size_request(400, 400);
    
    show_all_children();
  }

  void render()
  {
    simple_view.render();
  }

  void file_open()
  {
    int rc = file_dialog.popup();

    if (rc == Gtk::RESPONSE_OK) {
      const std::string filename = file_dialog.get_filename();
    }
  }
};
}


int main(int argc, char** argv)
{
  Es::Environment env;

  try {
    std::string name = argv[0];

    Es::OpenGLApplication applet(argc, argv);

    Es::Args args;
    args.set(XmNx, 20);
    args.set(XmNy, 40);
    args.set(XmNwidth, 500);
    args.set(XmNheight,400);
    Es::MainView mainv(applet, name, args);

    mainv.show();

    applet.run(mainv);

  } catch (Es::Exception& ex) {
    caught(ex);

  } catch (...) {
    printf("Get exception \n");
  }
  return 0;
}

