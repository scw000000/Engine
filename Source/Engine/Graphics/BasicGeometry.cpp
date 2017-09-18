/*!
 * \file BasicGeometry.cpp
 * \date 2017/09/17 9:45
 *
 * \author scw00
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#include "EngineStd.h"
#include "BasicGeometry.h"
#include ".\Renderer\RendererLoader.h"

const char* const VERTEX_SHADER_FILE_NAME = "Effects\\DebugWireframe.vs.glsl";
const char* const FRAGMENT_SHADER_FILE_NAME = "Effects\\DebugWireframe.fs.glsl";

#define VERTEX_LOCATION 0

bool BasicGeometry::Init()
   {
   m_Shader.push_back( shared_ptr< OpenGLShader >( ENG_NEW VertexShader( shared_ptr< Resource >( ENG_NEW Resource( VERTEX_SHADER_FILE_NAME ) ) ) ) );
   m_Shader.push_back( shared_ptr< OpenGLShader >( ENG_NEW FragmentShader( shared_ptr< Resource >( ENG_NEW Resource( FRAGMENT_SHADER_FILE_NAME ) ) ) ) );
   

   for( auto shader : m_Shader )
      {
      shader->VOnRestore();
      }
   m_Program = OpenGLRendererLoader::GenerateProgram( m_Shader );
   for( auto shader : m_Shader )
      {
      shader->VReleaseShader( m_Program );
      }
   glUseProgram( m_Program );
   m_Uniforms[ Uniforms_MVP ] = glGetUniformLocation( m_Program, "uMVP" );
   m_Uniforms[ Uniforms_Color ] = glGetUniformLocation( m_Program, "uColor" );
   glUniform3f( m_Uniforms[ Uniforms_Color ], 1.f, 0.f, 0.f );

   glUseProgram( 0 );

   // box vertices
   const float boxVerts[] = {
      // Front face
      -0.5f, -0.5f, 0.5f,
      0.5f, -0.5f, 0.5f,
      0.5f, 0.5f, 0.5f,
      -0.5f, 0.5f, 0.5f,

      // Back face
      -0.5f, -0.5f, -0.5f,
      -0.5f, 0.5f, -0.5f,
      0.5f, 0.5f, -0.5f,
      0.5f, -0.5f, -0.5f
   };

   glGenBuffers( 1, &m_VBOs[ GeometryTypes_Box ][ VBOs_Vertex ] );
   glBindBuffer( GL_ARRAY_BUFFER, m_VBOs[ GeometryTypes_Box ][ VBOs_Vertex ] );
   glBufferData( GL_ARRAY_BUFFER, ENG_ARRAY_SIZE( boxVerts ), boxVerts, GL_STATIC_DRAW );

   const unsigned int boxIndices[] = {
      0, 1, 2, 0, 2, 3,    // Front face
      4, 5, 6, 4, 6, 7,    // Back face
      5, 3, 2, 5, 2, 6,  // Top face
      4, 7, 1, 4, 1, 0, // Bottom face
      7, 6, 2, 7, 2, 1, // Right face
      4, 0, 3, 4, 3, 5  // Left face
      };

   glGenBuffers( 1, &m_VBOs[ GeometryTypes_Box ][ VBOs_Index ] );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_VBOs[ GeometryTypes_Box ][ VBOs_Index ] );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, ENG_ARRAY_SIZE( boxIndices ), boxIndices, GL_STATIC_DRAW );

   m_IndexCount[ GeometryTypes_Box ] = ENG_ARRAY_LENGTH( boxIndices );

   // sphere vertices
   const unsigned int sliceNum = 10;
   const unsigned int sectorNum = ( sliceNum - 1 ) * 2;
   std::vector<Vec3> sphereVerts;
   Vec3 sphericalcoord( 0.f, -1.f, 0.f );
   float deltaRad = ENG_PI / ( sliceNum - 1 );
   // X rotation
   for( int polar = 0; polar < sliceNum; ++polar )
      {
      // Y rotation
      for( int azimuthal = 0; azimuthal < sectorNum; ++azimuthal )
         {
         sphereVerts.push_back( sphericalcoord );
         // skip if it's top or bottom point
         if( polar == 0 || polar == sliceNum - 1 )
            {
            break;
            }
         Quaternion rot;
         rot.BuildAxisRad(g_Up, deltaRad);
         sphericalcoord = rot.XForm( sphericalcoord );

         }
      Quaternion rot;
      rot.BuildAxisRad( -g_Right, deltaRad );
      sphericalcoord = rot.XForm( sphericalcoord );
      }

   glGenBuffers( 1, &m_VBOs[ GeometryTypes_Sphere ][ VBOs_Vertex ] );
   glBindBuffer( GL_ARRAY_BUFFER, m_VBOs[ GeometryTypes_Sphere ][ VBOs_Vertex ] );
   glBufferData( GL_ARRAY_BUFFER, sphereVerts.size() * sizeof(sphereVerts[0]), &sphereVerts[0], GL_STATIC_DRAW );
   
   std::vector<int> sphereIndices;

   for( int i = 0; i < sectorNum; ++i )
      {
      sphereIndices.push_back( ( i + sectorNum - 1 ) % sectorNum + 1 );
      sphereIndices.push_back( 0 );
      sphereIndices.push_back( i + 1 );

      sphereIndices.push_back( sphereVerts.size() - 1 );
      sphereIndices.push_back( ( i + sectorNum - 1 ) % sectorNum + sphereVerts.size() - 1 - sectorNum );
      sphereIndices.push_back( i + sphereVerts.size() - 1 - sectorNum );
      }

   for( int polarStride = 1; polarStride + sectorNum < sphereVerts.size() - 1; polarStride += sectorNum )
      {
      for( int i = 0; i < sectorNum; ++i )
         {
         // Left-down
         sphereIndices.push_back( polarStride + i );
         // Right-down
         sphereIndices.push_back( polarStride + ( i + 1 ) % sectorNum );
         //Right-up
         sphereIndices.push_back( polarStride + sectorNum + ( i + 1 ) % sectorNum );

         // Left-down
         sphereIndices.push_back( polarStride + i );
         //Right-up
         sphereIndices.push_back( polarStride + sectorNum + ( i + 1 ) % sectorNum );
         //Left-up
         sphereIndices.push_back( polarStride + sectorNum + i );
         }
      }

   glGenBuffers( 1, &m_VBOs[ GeometryTypes_Sphere ][ VBOs_Index ] );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_VBOs[ GeometryTypes_Sphere ][ VBOs_Index ] );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof( sphereIndices[ 0 ] ), &sphereIndices[ 0 ], GL_STATIC_DRAW );

   m_IndexCount[ GeometryTypes_Sphere ] = sphereIndices.size();

   return true;
   }

void BasicGeometry::RenderGeometry( BasicGeometry::GeometryTypes geometryType, Color color, Mat4x4 mvp )
   {
   // glLineWidth(10.f);
   glDepthFunc( GL_ALWAYS );
   // save previous render mode
   GLint prevPolygonMode[2];
   glGetIntegerv( GL_POLYGON_MODE, prevPolygonMode );
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

   glBindBuffer( GL_ARRAY_BUFFER, m_VBOs[ geometryType ][ VBOs_Vertex ] );
   glEnableVertexAttribArray( VERTEX_LOCATION );
   glVertexAttribPointer(
      VERTEX_LOCATION,                  // attribute
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      ( void* ) 0            // array buffer offset
      );

   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_VBOs[ geometryType ][ VBOs_Index ] );

   glUseProgram( m_Program );
   glUniform3f( m_Uniforms[ Uniforms_Color ], color.m_Component.r, color.m_Component.g, color.m_Component.b );
   glUniformMatrix4fv( m_Uniforms[ Uniforms_MVP ], 1, GL_FALSE, &mvp[ 0 ][ 0 ] );
   glDrawElements(
      GL_TRIANGLES,
      m_IndexCount[ geometryType ],
      GL_UNSIGNED_INT,
      ( void* ) 0
      );

   glBindFramebuffer( GL_FRAMEBUFFER, 0 );

   glUseProgram( 0 );
   glDepthFunc( GL_LEQUAL );
   glPolygonMode( GL_BACK, prevPolygonMode[0] );
   glPolygonMode( GL_FRONT, prevPolygonMode[1] );
   }