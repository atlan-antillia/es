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
 *  PyramidWithColorEyeLightPositioner.cpp
 *
 *****************************************************************************/

//

#include <es++/opengl/OpenGLApplication.h>
#include <es++/opengl/OpenGLMainView.h>
#include <es++/opengl/OpenGLView.h>
#include <es++/opengl/OpenGLGC.h>
#include <es++/opengl/OpenGLLight.h>
#include <es++/opengl/OpenGLMaterial.h>
#include <es++/opengl/OpenGLSphere.h>
#include <es++/opengl/Normal3.h>
#include <es++/opengl/FaceIndices.h>

#include <es++/opengl/OpenGLClientState.h>
#include <es++/opengl/OpenGLTriangleSurfaces.h>
#include <es++/gtkmm-3.0/ColorEyeLightPositioner.h>


using namespace Gtk;

namespace Es {

class MainView :public Es::OpenGLMainView {
private:
  /////////////////////////////////////////////////////// 
  //Inner classes start.

  class SimpleView :public Es::OpenGLView {
  private:
    OpenGLGC gc;
    SmartPtr<OpenGLTriangleSurfaces> surfaces;
    Normal3* normals; //Shallow copy
    size_t numNormals;
 
    Es::ColorEyeLightPositioner* positioner;

  public:
    SimpleView()
    :OpenGLView()
    {
    }

  virtual void display()
  {
    static Vertex<3> vertices[] = {
      { -1.0, -1.0,  1.0},
      {  1.0, -1.0,  1.0},
      {  1.0, -1.0, -1.0},
      { -1.0, -1.0, -1.0},
      {  0.0,  1.0,  0.0},
    };
              
    static FaceIndices<3> indices[] = {
      {0, 1, 4}, //Triangle: face1
      {1, 2, 4}, //Triangle: face2
      {2, 3, 4}, //Triangle: face3
      {3, 0, 4}, //Triangle] face4
    };
       
    if (surfaces == NULL) {
      surfaces = new OpenGLTriangleSurfaces(vertices, SizeOf(vertices), indices, SizeOf(indices));
      normals = surfaces->calculateSurfaceNormals(numNormals);
      //surfaces->displayNormals();
    }
    
    if (normals && positioner) { 
      gc.frustum(1 , -1 , -1 , 1 , 1 , 10);

      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.loadIdentity();
      gc.matrixMode(GL_MODELVIEW);
      gc.clearColor(0.0, 1.0, 1.0, 1.0);
      
      gc.translate(0.0f,0.0f,-1.0f);
      double eyeX, eyeY, eyeZ;
      positioner->get_eye_position(eyeX, eyeY, eyeZ); 
      gc.lookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
      gc.rotate(0.0, 0.0, 1.0, 0.0);
      
      try {
        gc.enable(GL_LIGHTING);
 
        OpenGLLight light(GL_LIGHT0);
        double lightX, lightY, lightZ;
        positioner->get_light_position(lightX, lightY, lightZ);

        light.position(lightX, lightY, lightZ, 0.0);  

        OpenGLMaterial material(GL_FRONT);
        double r, g, b;
        positioner->get_color_position(r, g, b);
        material.diffuse (r, g, b, 0.0f);
        material.specular(1.0f, 1.0f, 1.0f, 0.0f);
        material.shininess(100.0f);
  
        gc.frontFace(GL_CCW);

        gc.enable(GL_CULL_FACE);
        gc.cullFace(GL_BACK);
        gc.enable(GL_NORMALIZE);
        
        for (int i = 0; i< SizeOf(indices); i++) {
          gc.begin(GL_TRIANGLES);
          FaceIndices<3> tri = indices[i];
          gc.draw( normals[i], 
                    vertices[ tri.index[0] ], 
                    vertices[ tri.index[1] ], 
                    vertices[ tri.index[2] ]);
          gc.flush();
          gc.end();
        }
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
      gc.enable(GL_DEPTH_TEST);
    }

    void set_positioner(Es::ColorEyeLightPositioner* apositioner)
    {
      positioner = apositioner;
    }
  };

  // Inner classes end.
  /////////////////////////////////////////////////////// 

  Es::HorizontalLayout   horiz_layout;

  Es::VerticalLayout     control_pane;

  SimpleView             simple_view;

  Es::ColorEyeLightPositioner   positioner;

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

    int pwidth = positioner.get_preferred_width();

    horiz_layout.pack_start(simple_view);

    horiz_layout.pack_start(positioner, Gtk::PACK_SHRINK);

    simple_view.set_positioner(&positioner);

    simple_view.set_size_request(w - pwidth-4, h);
    positioner.set_size_request(pwidth, h);

    positioner.set_color_range(0.0, 1.0);
    positioner.set_eye_range(-30.0, 30.0);
    positioner.set_light_range(-20.0, 20.0);

    positioner.set_color_position(180.0, 0.0, 160.0);
    positioner.set_eye_position(-20.0, 4.0, 30.0);
    positioner.set_light_position(-5.0, 18.0, 12.0);

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
    args.set(XmNwidth, 600);
    args.set(XmNheight, 400);
    Es::MainView mainv(applet, name, args);

    mainv.show();

    applet.run(mainv, 40);

  } catch (Es::Exception& ex) {
    caught(ex);

  } catch (...) {
    printf("Get exception \n");
  }
  return 0;
}

