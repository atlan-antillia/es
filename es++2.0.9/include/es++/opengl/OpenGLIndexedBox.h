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
 *  OpenGLIndexedBox.h
 *
 *****************************************************************************/

//2017/02/25 Updated for VS2015
#pragma once

#include <es++/Vertex.h>
#include <es++/opengl/Normal3.h>

#include <es++/opengl/FaceIndices.h>

#include <es++/opengl/OpenGLIndexedVertices.h>
#include <es++/opengl/OpenGLQuadSurfaces.h>

namespace Es {

class OpenGLIndexedBox :public OpenGLIndexedVertices {
private:
  static const int VERTEX_COUNT = 8;
  static const int FACE_COUNT   = 6;
  Vertex<3>                    vertices[VERTEX_COUNT];
  FaceIndices<4>               indices[FACE_COUNT];
  
  SmartPtr<OpenGLQuadSurfaces> surfaces;
  Normal3*                     normals; //shallow copy 
  size_t                       numNormals;
  
  void set(Vertex<3>& v, float x, float y, float z)
  {
  	v.value[0] = x;
  	v.value[1] = y;
  	v.value[2] = z;
  }
	
  void set(FaceIndices<4>& f, int a, int b, int c, int d)
  {
  	f.index[0] = a;
  	f.index[1] = b;
  	f.index[2] = c;
  	f.index[3] = d;
  }
	
public:
  OpenGLIndexedBox(GLfloat w=1.0f, GLfloat h=1.0f, GLfloat d=1.0f, GLfloat x=0.0f, GLfloat y=0.0f, GLfloat z=0.0f)
  {
    int i = 0;
    set(vertices[i++],  float(0.0*w + x), float(0.0*h + y), float(0.0*d + z) ); 
    set(vertices[i++],  float(1.0*w + x), float(0.0*h + y), float(0.0*d + z) );
    set(vertices[i++],  float(1.0*w + x), float(1.0*h + y), float(0.0*d + z) );
    set(vertices[i++],  float(0.0*w + x), float(1.0*h + y), float(0.0*d + z) ); 
    set(vertices[i++],  float(0.0*w + x), float(0.0*h + y), float(-1.0*d + z)); 
    set(vertices[i++],  float(1.0*w + x), float(0.0*h + y), float(-1.0*d + z));
    set(vertices[i++],  float(1.0*w + x), float(1.0*h + y), float(-1.0*d + z));
    set(vertices[i  ],  float(0.0*w + x), float(1.0*h + y), float(-1.0*d + z));
    
              
    i = 0;
    set(indices[i++], 0, 1, 2, 3);
    set(indices[i++], 1, 5, 6, 2);
    set(indices[i++], 5, 4, 7, 6);
    set(indices[i++], 4, 0, 3, 7);
    set(indices[i++], 4, 5, 1, 0);
    set(indices[i++], 3, 2, 6, 7);
       
    surfaces = new OpenGLQuadSurfaces(vertices, CountOf(vertices), indices, CountOf(indices));
    normals = surfaces->calculateSurfaceNormals(numNormals);
    //  surfaces->displayNormals();
   }
   

  virtual GLenum getInterleavedArraysFormat()
  {
    return (GLenum)0; //unused
  }
  
  virtual GLenum getPrimitiveType()
  {
    return GL_QUADS;
  }
  
  virtual GLfloat* getVertices()
  {
    return NULL; // unused
  }
  
  virtual int getVerticesDataSize()
  {
    return sizeof(vertices);
  }

  virtual int getNumberOfVertices()
  {
    return CountOf(vertices);
  }
  
  virtual GLuint* getIndices()
  {
    return NULL; //unused
  }
  
  virtual int getIndicesDataSize()
  {
    return sizeof(indices);
  }
  
  virtual int getNumberOfIndices()
  {
    return CountOf(indices);
  }
  
  virtual void draw(OpenGLGC* gc)
  {  
    gc->frontFace(GL_CCW);

    gc->enable(GL_CULL_FACE);
    gc->cullFace(GL_BACK);
    glEnable(GL_NORMALIZE);
        
    for (int i = 0; i< CountOf(indices); i++) {
      gc->begin(GL_QUADS);
      FaceIndices<4> quad = indices[i];
      gc->draw( normals[i], 
                    vertices[ quad.index[0] ], 
                    vertices[ quad.index[1] ], 
                    vertices[ quad.index[2] ],
                    vertices[ quad.index[3] ]
      );
      //gc->flush();
      gc ->end();
    }
  }
       
};

}
 
    
