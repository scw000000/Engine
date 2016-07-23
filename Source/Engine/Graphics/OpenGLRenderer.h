#pragma once
/*!
 * \file OpenGLRenderer.h
 * \date 2016/05/02 20:24
 *
 * \author SCW
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 *  
 *
 * \note
 */

struct aiScene;

/*!
 * \class OpenGlRenderer
 *
 * \brief 
 *  LATER: try to implement shader resource manager to prevent unnecessary waste of memory
 * \author SCW
 * \date 05 2016
 */
class OpenGLRenderer : public IRenderer
   {
   public:
      OpenGLRenderer( void );
      ~OpenGLRenderer( void );
      
      virtual void VSetBackgroundColor( Color color ) override; 
	   virtual GLuint VOnRestore( void ) override;
	   virtual void VShutdown( void ) override;
      // This functino is called at the begin of HumanView::Render before calling Scene to Rendoer
	   virtual bool VPreRender( void ) override;
      // This functino is called at the end of HumanView::Render after calling Scene to Render
	   virtual bool VPostRender( void ) override;
	   //virtual void VCalcLighting( Lights *lights, int maximumLights ) override;

      static void LoadTexture( GLuint* textureId, const Resource& pTextureResource );
      static void LoadMesh( GLuint* pVertexBuffer, GLuint* pUvBuffer, GLuint* pIndexBuffer, GLuint* pNormalBuffer, shared_ptr<ResHandle> pMeshResHandle );
      static void LoadBones( GLuint* pBoneBuffer, shared_ptr<ResHandle> pMeshResHandle );
      //virtual shared_ptr<IRenderState> VPrepareAlphaPass( void )=0;
	   //virtual shared_ptr<IRenderState> VPrepareSkyBoxPass( void )=0;
      
      static GLuint GenerateShader( const Resource& shaderRes, GLuint shaderType );

      static GLuint GenerateProgram( GLuint vertexShader, GLuint fragmentShader );

      static void SetRenderAlpha( bool isAlpha );

      virtual void VDrawLine( const Vec3& from,const Vec3& to,const Color& color ) const override;

   private:
      static GLuint CompileShader( const GLchar* const* pSrcData, const GLuint shaderID );

      struct VertexToBoneMapping
         {
         #define NUM_BONES_PER_VEREX 4
            public:
            unsigned int m_BoneIDs[ NUM_BONES_PER_VEREX ];
            float m_BoneWeights[ NUM_BONES_PER_VEREX ];

            VertexToBoneMapping( void )
               {
               ENG_ZERO_MEM( m_BoneIDs );
               ENG_ZERO_MEM( m_BoneWeights );
               };

            void AddBoneData( unsigned int boneID, float boneWeight );
         };
   };
