/*!
 * \file BasicGeometry.h
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
#include "Shaders.h"

class BasicGeometry
   {
   public:
      enum GeometryTypes
         {
         GeometryTypes_Box,
         GeometryTypes_Sphere,
         GeometryTypes_Num
         };
   public:
      bool Init();
      void RenderGeometry( GeometryTypes geometryType, Color color, Mat4x4 mvp );

      
   private:
      enum VBOs
         {
         VBOs_Vertex,
         VBOs_Index,
        // VBOs_UV,
         VBOs_Num
         };

      enum Uniforms
         {
         Uniforms_MVP,
         Uniforms_Color,
         Uniforms_Num
         };

      unsigned int m_IndexCount[ GeometryTypes_Num ];
      GLuint m_VBOs[ GeometryTypes_Num ][ VBOs_Num ];
      GLuint m_Uniforms[ Uniforms_Num ];
      std::vector< shared_ptr< OpenGLShader > > m_Shader;
      GLuint m_Program;
      GLuint m_VAO;
   };

REGISTER_SINGLETON( BasicGeometry );