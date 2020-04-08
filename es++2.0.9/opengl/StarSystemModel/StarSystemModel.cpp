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
 *  StarSystemModel.cpp
 *
 *****************************************************************************/

//
#include <es++/SmartPtr.h>
#include <es++/Mutex.h>
#include <es++/opengl/OpenGLRenderingTimer.h>

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
  static const int NUMBER_OF_PLANETS = 4;

private:
  /////////////////////////////////////////////////////// 
  //Inner class starts.

  class SimpleView :public Es::OpenGLView {
  private:
    OpenGLGC      gc;
    SmartPtr<OpenGLSolidSphere>  sun;    
    SmartPtr<OpenGLSolidSphere>  planets[NUMBER_OF_PLANETS];
    SmartPtr<OpenGLCircle>  orbits[NUMBER_OF_PLANETS];
    float                   angles[NUMBER_OF_PLANETS];
    SmartPtr<OpenGLRenderingTimer> timerThread;
    int                     renderingInterval;
    float angle;
    static const int CIRCLE_ANGLE = 360;
    static const int INCREMENT    = 1;
    Mutex    mutex;

  public:
    SimpleView()
    :OpenGLView()
    ,renderingInterval(40)
    {
       for (int i = 0; i < NUMBER_OF_PLANETS; i++) {
        angles[i] = 30.0f * i;
      }
    }

    virtual void display()
    {
      mutex.lock();
      //setViewPort();
      
      if (angle <(CIRCLE_ANGLE - INCREMENT) ) {
          angle += INCREMENT;
      } else {
        angle = INCREMENT;
      }
        
      static const float INCREMENT[NUMBER_OF_PLANETS] = { 1.0f, 0.7f, 0.5f, 0.2f };
      for (int i = 0; i < NUMBER_OF_PLANETS; i++) {
          if (angles[i] < CIRCLE_ANGLE - INCREMENT[i]) {
            angles[i] += INCREMENT[i];
          }
          else {
            angles[i] = INCREMENT[i];
          }
      }
      
      if (sun) {
        setPerspective();

        gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gc.loadIdentity();
        gc.lookAt(-1.0, 10.0, 19.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        gc.clearColor(0.0, 0.0, 0.0, 1.0);
        gc.enable(GL_CULL_FACE); 
        gc.enable(GL_LIGHTING);

        gc.color(1.0f, 1.0f, 1.0f); 

        sun-> draw(&gc, 0.0f, 0.0f, 0.0f);
        
        OpenGLLight light(GL_LIGHT0);
        GLfloat lightPosition[] = {-2.0f, 0.0f, -1.0f, 1.0};  
        light.position(lightPosition);

        for (int i = 0; i<NUMBER_OF_PLANETS; i++) {
          Vertex3 pos = orbits[i]->getOrbitPosition(angles[i]);
          planets[i]-> draw(&gc, pos.x, pos.y, pos.z);
        }
      }
      mutex.unlock();
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
      Color4  greendiffuse  = { 0.0, 1.0, 0.0, 1.0 };
      Color4  reddiffuse    = { 1.0, 0.0, 0.0, 1.0 };
      Color4  bluediffuse   = { 0.0, 0.0, 1.0, 1.0 };
      Color4  silverdiffuse = { 0.8, 0.8, 0.8, 1.0 };

      Color4  specular      = { 1.0, 1.0, 1.0, 1.0 };
      Color4  emission      = { 0.8, 0.2, 0.0, 0.0 };
      GLfloat lowShining    = { 10.0 };
      GLfloat highShining   = {100.0 };
 
      OpenGLMateria sunMateria(GL_FRONT, black,   diffuse, black, emission, highShining);
      
      sun = new OpenGLSolidSphere(sunMateria, 0.5f,  40, 40);

      int index = 0;
      orbits[index++] = new OpenGLCircle(0.0f, 0.0f, 0.0f, 1.0);
      orbits[index++] = new OpenGLCircle(0.0f, 0.0f, 0.0f, 1.6f);
      orbits[index++] = new OpenGLCircle(0.0f, 0.0f, 0.0f, 2.4f);
      orbits[index]   = new OpenGLCircle(0.0f, 0.0f, 0.0f, 2.9f);

      OpenGLMateria planetMateria1(GL_FRONT, ambient, bluediffuse, specular, black, lowShining);
      OpenGLMateria planetMateria2(GL_FRONT, ambient, silverdiffuse, specular, black, lowShining);
      OpenGLMateria planetMateria3(GL_FRONT, ambient, greendiffuse, specular, black, lowShining);
      OpenGLMateria planetMateria4(GL_FRONT, ambient, reddiffuse, specular, black, lowShining);
      
      index = 0;
      planets[index++] = new OpenGLSolidSphere(planetMateria1, 0.10f, 20, 20);
      planets[index++] = new OpenGLSolidSphere(planetMateria2, 0.18f, 20, 20);
      planets[index++] = new OpenGLSolidSphere(planetMateria3, 0.18f, 20, 20);
      planets[index]   = new OpenGLSolidSphere(planetMateria4, 0.12f, 20, 20);

      timerThread  = new OpenGLRenderingTimer(*this, renderingInterval);
      timerThread->start();
    }

  };

  // Inner class ends.
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

  XInitThreads();

  try {
    std::string name = argv[0];

    Es::OpenGLApplication applet(argc, argv);

    Es::Args args;
    args.set(XmNx, 20);
    args.set(XmNy, 40);
    args.set(XmNwidth, 600);
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

