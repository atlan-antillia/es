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
 *  TexturedSphereRotationOnIdleTime.cpp
 *
 *****************************************************************************/

//
// 2017/11/27 Updated.

#include <es++/SmartPtr.h>
#include <es++/opengl/OpenGLApplication.h>
#include <es++/opengl/OpenGLMainView.h>
#include <es++/opengl/OpenGLView.h>
#include <es++/opengl/OpenGLGC.h>
#include <es++/opengl/OpenGLLight.h>
#include <es++/opengl/OpenGLMaterial.h>
#include <es++/opengl/OpenGLTexturedSphere.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenGLMainView {
private:
  /////////////////////////////////////////////////////// 
  //Inner classes start.

  class SimpleView :public Es::OpenGLView {
  private:
    OpenGLGC          gc;
    SmartPtr<OpenGLTexturedSphere> texture;
    float                       angle;
    Vertex3                     axis;

  public:
    SimpleView()
    :OpenGLView()
    {
    }

    virtual void display()
    {
      angle -= 2.0f;
      setViewPort();

      if (texture) {
        gc.matrixMode(GL_PROJECTION);
        gc.loadIdentity();
        gc.perspective(16.0, (double)width() / (double)height(), 0.5, 100.0);
        gc.matrixMode(GL_MODELVIEW);

        gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gc.loadIdentity();

        gc.clearColor(0.1, 0.1, 0.2, 1.0);
        gc.enable(GL_CULL_FACE); 
        //2017/11/27 Added the following line
        gc.cullFace(GL_FRONT); 

        gc.enable(GL_LIGHTING);

        gc.enable(GL_DEPTH_TEST);
        gc.color(1.0f, 1.0f, 1.0f);
        gc.lookAt(0.0, 18.0, 4.0, 0.0, 0.0, 0.0, 0.0, 100.0, 0.0);

        gc.translate(0.0f, 0.0f, 0.0f);

        gc.rotate(angle, axis.x, axis.y, axis.z);

        OpenGLLight light(GL_LIGHT0);
        //2017/11/27 Modified lightPosition
        GLfloat lightPosition[] = {-10, -10, -10, 1.0}; 
        light.position(lightPosition);
    
        GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat blue[]  = {0.0f, 0.0f, 1.0f, 1.0f};
        GLfloat shininess[] = {100.0};
        OpenGLMaterial material(GL_FRONT);
        material.diffuse(blue);
        material.specular(white);
        material.shininess(shininess);
            
        texture -> draw(&gc);
      }
    }
  
    virtual void resize(int w, int h)
    {
      if (w == 0 || h == 0) {
        return;
      }

      gc.matrixMode(GL_PROJECTION);
      gc.loadIdentity();
      gc.perspective(16.0, (double)w/(double)h, 0.5, 100.0);

      gc.matrixMode(GL_MODELVIEW);
    }

    virtual void initialize()
    {
      //2017/11/27 Modified axis value.
      axis.x = -10.0f;
      axis.y = -50.0f;
      axis.z = 80.0f;

      gc.enable(GL_DEPTH_TEST);
      gc. enable(GL_TEXTURE_2D);
      try {
        // The 'world.topo.bathy.200412.3x860x430.jpg' used here is based on
        // 'world.topo.bathy.200412.3x5400x2700.jpg' in the following page.
        //
        //http://visibleearth.nasa.gov/view.php?id=73909
        //
        //December, Blue Marble Next Generation w/ Topography and Bathymetry
        //Credit: Reto St?ckli, NASA Earth Observatory
        const char* imageFileName = "../../images/world.topo.bathy.200412.3x860x430.jpg";

      //  const char* appName = "TexturedSpherRotationByKeyInput";
        
        texture = new OpenGLTexturedSphere(imageFileName, 2.5f, 40, 40);
        
      } catch (Exception& ex) {
        caught(ex);
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
    args.set(XmNwidth, 400);
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

