/*!
 * \file RendererLoader.h
 * \date 2016/12/26 13:36
 *
 * \author scw00
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#include "..\Shaders.h"
#include "..\ResourceCache\MeshResource.h"

class OpenGLRendererLoader
   {
   public:
      static void LoadTexture2D( GLuint* textureId, shared_ptr< Resource > pTextureResource );
      static void LoadMesh( GLuint* pVertexBuffer, GLuint* pUvBuffer, GLuint* pIndexBuffer, GLuint* pNormalBuffer, shared_ptr<ResHandle> pMeshResHandle );
      static void LoadBones( GLuint* pBoneBuffer, shared_ptr<ResHandle> pMeshResHandle );
      static void CompileAndLoadShader( GLuint& shaderObj, shared_ptr< Resource > pResource, GLuint shaderType );
      static GLuint GenerateProgram( const std::vector< GLuint >& shderObjs );
      static GLuint GenerateProgram( const std::vector< shared_ptr< OpenGLShader > >& shderObjs );
   };