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

class OpenGLRendererLoader
   {
   public:
      static void LoadTexture2D( GLuint* textureId, const Resource& pTextureResource );
      static void LoadMesh( GLuint* pVertexBuffer, GLuint* pUvBuffer, GLuint* pIndexBuffer, GLuint* pNormalBuffer, shared_ptr<ResHandle> pMeshResHandle );
      static void LoadBones( GLuint* pBoneBuffer, shared_ptr<ResHandle> pMeshResHandle );
      static void CompileAndLoadShader( GLuint& shaderObj, const Resource& shaderRes, GLuint shaderType );
      static GLuint GenerateProgram( const std::vector< GLuint >& shderObjs );
   };