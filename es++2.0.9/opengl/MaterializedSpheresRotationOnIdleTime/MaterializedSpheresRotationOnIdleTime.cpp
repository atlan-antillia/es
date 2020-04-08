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
 *  MaterializedSpheresRotationOnIdleTime.cpp
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
#include <es++/opengl/OpenGLWireSphere.h>


using namespace Gtk;

namespace Es {

class MainView :public Es::OpenGLMainView {
private:
  /////////////////////////////////////////////////////// 
  //Inner classes start.

  class SimpleView :public Es::OpenGLView {
  private:
    static const int        SPHERES=4;
    OpenGLGC      gc;
    SmartPtr<OpenGLGeometry>  sphere[SPHERES];
    float                   angle;

  public:
    SimpleView()
    :OpenGLView()
    {
    }

   virtual void display()
    {
      angle += 0.4f;
      
      if (sphere[0]) {
        setPerspective();

        gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gc.lookAt(4.0, 8.0, 12.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        gc.clearColor(0.0, 0.0, 0.0, 1.0);
        gc.enable(GL_CULL_FACE); 
        gc.enable(GL_LIGHTING);

        OpenGLLight light(GL_LIGHT0);
        GLfloat lightPosition[] = {10, 10, 10, 1.0};  
        light.position(lightPosition);

        for (int i = 0; i<SPHERES; i++) {
          gc.pushMatrix();
          gc.rotate(angle, 0.0f, 1.0f, 0.0f);
          sphere[i]->draw(&gc, -2.0f+1.3f*i, 0.5f,  0.0f+ 0.3f*i);
          gc.popMatrix();
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
      Color4  black         = { 0.0, 0.0, 0.0, 1.0 };
      Color4  ambient       = { 0.5, 0.5, 0.5, 1.0 };
      Color4  diffuse       = { 0.2, 0.4, 0.8, 1.0 };
      Color4  specular      = { 1.0, 1.0, 1.0, 1.0 };
      Color4  emission      = { 0.8, 0.0, 0.0, 0.0 };
      GLfloat lowShining    = { 10.0 };
      GLfloat highShining   = {100.0 };

      OpenGLMateria mat1(GL_FRONT, ambient, diffuse, specular, emission, lowShining);
      OpenGLMateria mat2(GL_FRONT, black,   diffuse, specular, emission, lowShining);
      OpenGLMateria mat3(GL_FRONT, black,   diffuse, black,    emission, highShining);
      OpenGLMateria mat4(GL_FRONT, ambient, diffuse, specular, black,    highShining);
      OpenGLMateria materias[] = {mat1, mat2, mat3, mat4};
      for (int i = 0; i<SPHERES; i++) {
        if (i%2 ==0) {
          sphere[i] = new OpenGLWireSphere(materias[i], 0.5f,  40, 40);
        } else {
          sphere[i] = new OpenGLSolidSphere(materias[i], 0.5f,  40, 40);
        }
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

    simple_view.set_size_request(400, 400);
    
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

