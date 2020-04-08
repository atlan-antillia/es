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
 *  ColoredCube.cpp
 *
 *****************************************************************************/

//

#include <es++/opengl/OpenGLApplication.h>
#include <es++/opengl/OpenGLMainView.h>
#include <es++/opengl/OpenGLView.h>
#include <es++/opengl/OpenGLGC.h>
#include <es++/opengl/OpenGLClientState.h>
#include <es++/opengl/OpenGLFace.h>

using namespace Gtk;

namespace Es {

class MainView :public Es::OpenGLMainView {
private:
  /////////////////////////////////////////////////////// 
  //Inner class starts.

  class SimpleView :public Es::OpenGLView {
  private:
    OpenGLGC gc;

  public:
    SimpleView()
    :OpenGLView()
    {
    }

    virtual void display()
    {
      gc.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      gc.loadIdentity();                 

      gc.translate(-2.0f, 0.0f, -6.0f); 
      gc.clearColor(0, 0, 0, 0);
 
      gc.begin(GL_QUADS);         

      OpenGLFace top = {
          { 1.0f, 1.0f, -1.0f},
          {-1.0f, 1.0f, -1.0f},
          {-1.0f, 1.0f,  1.0f},
          { 1.0f, 1.0f,  1.0f} };

      OpenGLFace bottom = {
          { 1.0f, -1.0f,  1.0f},
          {-1.0f, -1.0f,  1.0f},
          {-1.0f, -1.0f, -1.0f},
          { 1.0f, -1.0f, -1.0f} };

      OpenGLFace front = {
          { 1.0f,  1.0f, 1.0f},
          {-1.0f,  1.0f, 1.0f},
          {-1.0f, -1.0f, 1.0f},
          { 1.0f, -1.0f, 1.0f} };
 
      OpenGLFace back = {
        { 1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},
        { 1.0f,  1.0f, -1.0f} };
 
      OpenGLFace left = {
        {-1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f,  1.0f} };
 
      OpenGLFace right = {
        { 1.0f,  1.0f, -1.0f},
        { 1.0f,  1.0f,  1.0f},
        { 1.0f, -1.0f,  1.0f},
        { 1.0f, -1.0f, -1.0f} };

      gc.color(0.0f, 1.0f, 0.0f);     
      gc.draw(top);

      gc.color(0.5f, 0.5f, 0.0f);    
      gc.draw(bottom);

      gc.color(1.0f, 0.0f, 0.0f);  
      gc.draw(front);

      gc.color(0.5f, 0.0f, 0.5f); 
      gc.draw(back);

      gc.color(0.0f, 0.0f, 1.0f);  
      gc.draw(left);

      gc.color(0.5f, 0.5f, 1.0f);  
      gc.draw(right);
      gc.end();

    }    
 
    virtual void resize(int w, int h)
    {
      if (w == 0 || h == 0) {
        return;
      }
      gc.matrixMode(GL_PROJECTION); 
      gc.loadIdentity();
      gc.perspective(50.0f, (GLfloat)w/(GLfloat)h, 0.1f, 50.0f);
      gc.matrixMode(GL_MODELVIEW);
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

