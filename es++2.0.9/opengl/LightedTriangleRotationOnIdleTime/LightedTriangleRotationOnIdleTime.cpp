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
 *  LightedTriangleRotationOnIdleTime.cpp
 *
 *****************************************************************************/

//

#include <es++/opengl/OpenGLApplication.h>
#include <es++/opengl/OpenGLMainView.h>
#include <es++/opengl/OpenGLView.h>
#include <es++/opengl/OpenGLGC.h>
#include <es++/opengl/OpenGLLight.h>
#include <es++/opengl/OpenGLMaterial.h>

#include <es++/opengl/Vertex3.h>
#include <es++/opengl/Color4.h>
#include <es++/opengl/FaceIndices.h>

#include <es++/opengl/OpenGLClientState.h>
#include <es++/opengl/OpenGLTriangleSurfaces.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenGLMainView {
private:
  /////////////////////////////////////////////////////// 
  //Inner class starts.

  class SimpleView :public Es::OpenGLView {
  private:
    OpenGLGC gc;
    SmartPtr<OpenGLTriangleSurfaces> surfaces;

    float            angle;

    Normal3*         normals; 
    size_t           numNormals;

  public:
    SimpleView()
    :OpenGLView()
    {
    }


  virtual void display()
  {
    angle += 0.6f;
    
    static Vertex<3> vertices[] = {
      //Face 1:
      {-1.0,-1.0, 1.0 }, //0
      { 1.0,-1.0, 1.0 }, //1
      { 0.0, 1.0, 1.0 }, //2      
    };
        
    static Color4 colors[] = {
      {1.0f, 0.4f, 0.2f, 1.0f}, 
    };
       
    static FaceIndices<3> indices[] = {
      {0, 1, 2}   //Triangle
    };
   
    if (surfaces == NULL) {
      surfaces = new OpenGLTriangleSurfaces(vertices, SizeOf(vertices), indices, SizeOf(indices));
      normals = surfaces->calculateSurfaceNormals(numNormals);
    }
    
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gc.loadIdentity();

      try {        
        gc.clearColor(1.0, 1.0, 1.0, 1.0);

        gc.translate(0.0f,0.0f, 0.0f); 
        gc.lookAt(0.0, 6.0, 12.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 

        gc.rotate(-angle, 0.0, 1.0, 0.0);

        OpenGLClientState state1(GL_VERTEX_ARRAY); 
        OpenGLClientState state2(GL_COLOR_ARRAY);
        OpenGLClientState state3(GL_NORMAL_ARRAY);
        gc.vertexPointer(3, GL_FLOAT, 0, vertices);
        gc.colorPointer (4, GL_FLOAT, 0, colors);
        gc.normalPointer(GL_FLOAT,  0,   normals);
        gc.frontFace(GL_CCW);
        gc.enable(GL_CULL_FACE);

        gc.enable(GL_LIGHTING);

        OpenGLLight light(GL_LIGHT0);
        light.position(0, 0.0, 1.0, 0.0);  

        OpenGLMaterial material(GL_FRONT);
        material.specular(1.0f, 1.0f, 1.0f, 1.0f); //white);
        material.shininess(50.0); 

        material.diffuse(0.0f, 1.0f, 0.0f, 1.0f);  //green;
     
        gc.cullFace(GL_BACK);
        gc.drawElements(GL_TRIANGLES, SizeOf(indices)*3, GL_UNSIGNED_INT, indices);
         
        gc.cullFace(GL_FRONT);
        gc.drawElements(GL_TRIANGLES, SizeOf(indices)*3, GL_UNSIGNED_INT, indices);
      
      } catch (Exception& ex) {
        caught(ex); 
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
    gc.enable( GL_DEPTH_TEST );
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

