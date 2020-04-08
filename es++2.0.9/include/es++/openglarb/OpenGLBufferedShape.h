/******************************************************************************
 *
 * Copyright (c) 2016 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
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
 *  OpenGLBufferedShape.h
 *
 *****************************************************************************/

#pragma once

#include <es++/opengl/OpenGLGC.h>

#include <es++/opengl/OpenGLIndexedVertices.h>

#include <es++/openglarb/OpenGLVertexBufferARB.h>
#include <es++/openglarb/OpenGLIndexBufferARB.h>
//#include <es++/opengl2/OpenGLVertexAttribute.h>



namespace Es {

class OpenGLBufferedShape : OpenGLObject {
private:
  OpenGLIndexedVertices* vertices;
  OpenGLVertexBufferARB* vertexBuffer;
  OpenGLIndexBufferARB*  indexBuffer;
  
public:
  OpenGLBufferedShape(OpenGLIndexedVertices* vertices1)
  :OpenGLObject(),
  vertices(vertices1),
  vertexBuffer(NULL),
  indexBuffer(NULL)
  {
    if (vertices) {
      vertexBuffer = new OpenGLVertexBufferARB();
      vertexBuffer -> bind();
      vertexBuffer -> data(vertices->getVerticesDataSize(),  vertices->getVertices(), GL_STATIC_DRAW_ARB);
      vertexBuffer -> unbind();
              
      indexBuffer   = new OpenGLIndexBufferARB();
      indexBuffer  -> bind();
      indexBuffer  -> data(vertices->getIndicesDataSize(), vertices->getIndices(), GL_STATIC_DRAW_ARB);
      indexBuffer  -> unbind();
    } else {
      throw IException("Invalid vertices parameter.");
    }
  }
  
  ~OpenGLBufferedShape()
  {
    delete vertexBuffer;
    delete indexBuffer;
    vertexBuffer = NULL;
    indexBuffer  = NULL;
  }
  
  void draw(OpenGLGC* gc) 
  {
    vertexBuffer -> bind();
    indexBuffer  -> bind();
   
    vertices      -> draw(gc);
   
    vertexBuffer -> unbind();
    indexBuffer  -> unbind();
  }
};
  
}

