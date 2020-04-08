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
 *  OpenGLTriangleSurfaces.h
 *
 *****************************************************************************/

#pragma once

#include <es++/opengl/Normal3.h>
#include <es++/Vertex.h>
#include <es++/opengl/TriangleIndices.h>
#include <es++/Vector3f.h>
#include <es++/opengl/FaceIndices.h>

namespace Es {
  
class OpenGLTriangleSurfaces {
private:
  const Vertex<3>* vertexArray;
  size_t         numberOfVertices;
  const FaceIndices<3>* faceArray;
  size_t         numberOfFaces;
  
  Normal3*       normals;
  size_t         numberOfNormals;
  
public:
  OpenGLTriangleSurfaces(const Vertex<3>* vertices, size_t numVertices, 
              const FaceIndices<3>* faces, size_t numFaces)
  :vertexArray(NULL),
  numberOfVertices(numVertices),
  faceArray(NULL),
  numberOfFaces(numFaces),
  normals(NULL),
  numberOfNormals(0)
  {
    //Deep copy
    if (vertices && numVertices >0) {
      vertexArray = new Vertex<3>[numVertices];
      memcpy((void*)vertexArray, (const void*)vertices, sizeof(Vertex3)* numVertices);
    } else {
      throw IException("Invalid vertices parameters.");
    }
    if (faces && numFaces >0) {
      faceArray = new FaceIndices<3>[numFaces];
      memcpy((void*)faceArray, (const void*)faces, sizeof(FaceIndices<3>)* numFaces);
    } else {
      throw IException("Invalid faces parameters.");
    }
    //calculateNormals();    
  }
  
  
  ~OpenGLTriangleSurfaces()
  {
    delete [] vertexArray;
    vertexArray = NULL;
    delete [] faceArray;
    faceArray   = NULL;
    delete [] normals;
    normals = NULL;
  }
  
  Normal3* calculateSurfaceNormals(size_t& numNormals) //,
  {
    numberOfNormals = numberOfFaces;
    if (normals == NULL) {
      normals = new Normal3[numberOfFaces];
    }
      
    for (int i = 0; i<numberOfFaces; i++) {
        FaceIndices<3> tri = faceArray[i];
        Vertex<3> ve1 = vertexArray[ tri.index[0] ];
        Vertex<3> ve2 = vertexArray[ tri.index[1] ];
        Vertex<3> ve3 = vertexArray[ tri.index[2] ];

        Vector3f v1(ve1);
        Vector3f v2(ve2);
        Vector3f v3(ve3);

        Vector3f normal = Vector3f::crossProduct(v2 - v1, v3 - v1);
        normal.normalize();
        
        normals[i].x = normal[0];
        normals[i].y = normal[1];
        normals[i].z = normal[2];
    } 
    numNormals = numberOfNormals;
    return normals;
    
  }
};

}

    
