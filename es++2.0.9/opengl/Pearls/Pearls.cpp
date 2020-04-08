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
 *  Pearls.cpp
 *
 *****************************************************************************/

//

#include <es++/opengl/OpenGLApplication.h>
#include <es++/opengl/OpenGLMainView.h>
#include <es++/opengl/OpenGLView.h>
#include <es++/opengl/OpenGLGC.h>
#include <es++/opengl/OpenGLLight.h>
#include <es++/opengl/OpenGLMaterial.h>
#include <es++/opengl/OpenGLSolidSphere.h>
#include <es++/opengl/OpenGLCircle.h>


using namespace Gtk;

namespace Es {

class MainView :public Es::OpenGLMainView {
private:
  /////////////////////////////////////////////////////// 
  //Inner classes start.

  class SimpleView :public Es::OpenGLView {
  private:
    static const int             CIRCLE = 360;
    
    static const int             PEARLS = 20;
    OpenGLGC                     gc;
    SmartPtr<OpenGLSolidSphere>  pearls[PEARLS];
    SmartPtr<OpenGLCircle>       circle;

  public:
    SimpleView()
    :OpenGLView()
    {
    }

    virtual void display()
    {
      if (pearls[0]) {
        setPerspective();
        gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gc.loadIdentity();
        gc.lookAt(1.0, 2.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        gc.clearColor(0.0, 0.0, 0.0, 1.0);
        gc.enable(GL_CULL_FACE); 
        gc.enable(GL_LIGHTING);

        gc.translate(0.0f, 0.0f, 0.0f);
        gc.color(1.0f, 1.0f, 1.0f); 

        circle->draw(&gc);
               
        OpenGLLight light(GL_LIGHT0);
        GLfloat lightPosition[] = {0.0f, 5.0f, 10.0f, 1.0};  
        light.position(lightPosition);

        int angle = CIRCLE/PEARLS;     
        for (int i=0; i <PEARLS; i++) {
          Vertex3 pos = circle->getOrbitPosition(angle*i);
          pearls[i]-> draw(&gc, pos.x, pos.y, pos.z);
        }
      }
    }

    virtual void resize(int w, int h)
    {
      if (w == 0 || h == 0) {
          return;
      }
      setPerspective();
    }


    virtual void initialize()
    {
      circle = new OpenGLCircle(0.0f, 0.0f, 0.0f, 0.9f);
      
      Color4  black         = { 0.0, 0.0, 0.0, 1.0 };
      Color4  ambient       = { 0.5, 0.5, 0.5, 1.0 };
      Color4  diffuse       = { 0.8, 0.8, 0.8, 1.0 };
      Color4  specular      = { 1.0, 1.0, 1.0, 1.0 };
     // Color4  emission      = { 0.8, 0.2, 0.0, 0.0 };
      GLfloat highShining   = { 80.0 };

      OpenGLMateria materia(GL_FRONT, ambient, diffuse, specular, black, highShining);

      for (int i = 0; i<PEARLS; i++) {
        Color4  diffuse       = { 0.7+0.01*i, 0.8, 0.7+0.01*i, 1.0 };
        OpenGLMateria materia(GL_FRONT, ambient, diffuse, specular, black, highShining);
        
        pearls[i] =  new OpenGLSolidSphere(materia, 0.13f, 40, 40);
      }
    }

  };

  // Inner classes end.
  /////////////////////////////////////////////////////// 

  Es::HorizontalLayout   horiz_layout;

  SimpleView             simple_view;

public:
  //////////////////////////////////////////////
  //Constructor
  //
  MainView(Es::OpenGLApplication& applet, std::string& name, Es::Args& args)
  :OpenGLMainView(applet, name, args)
  {
    int w = (int)args.get(XmNwidth);
    int h = (int)args.get(XmNheight);

    Es::MainLayout& main_layout = get_main_layout();

    main_layout.pack_start(horiz_layout);

    horiz_layout.pack_start(simple_view);

    OpenGLContext* context = applet.getOpenGLContext();

    simple_view.set_opengl_context(context);

    show_all_children();
  }

  void render()
  {
    simple_view.render();
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
    args.set(XmNheight, 400);
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

