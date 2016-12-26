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
#include "..\ResourceCache\MeshResource.h"
#include "TexureDrawer.h"
struct aiScene;

/*!
 * \class OpenGlRenderer
 *
 * \brief 
 *  LATER: try to implement shader resource manager to prevent unnecessary waste of memory
 * \author SCW
 * \date 05 2016
 */

struct VertexToBoneMapping
   {
      public:
      BoneId m_BoneIDs[ MAXIMUM_BONES_PER_VEREX ];
      float m_BoneWeights[ MAXIMUM_BONES_PER_VEREX ];

      VertexToBoneMapping( void )
         {
         ENG_ZERO_MEM( m_BoneIDs );
         ENG_ZERO_MEM( m_BoneWeights );
         };

      void AddBoneData( BoneId boneID, float boneWeight );
   };

class OpenGLRenderer : public IRenderer
   {
  // friend class SkeletalMeshSceneNode;
   public:
      OpenGLRenderer( void );
      ~OpenGLRenderer( void );
      
      virtual void VSetBackgroundColor( Color color ) override; 
	   virtual GLuint VOnRestore( void ) override;
	   virtual void VShutdown( void ) override;
      // This functino is called at the begin of HumanView::Render before calling Scene to Rendoer
	   virtual int VPreRender( void ) override;
      // This functino is called at the end of HumanView::Render after calling Scene to Render
	   virtual int VPostRender( void ) override;
	   //virtual void VCalcLighting( Lights *lights, int maximumLights ) override;

      static void SetRenderAlpha( bool isAlpha );

      virtual void VDrawLine( const Vec3& fromWS,const Vec3& toWS,const Color& color ) const override;

   public:
      static OpenGLTextureDrawer s_TextureDrawer;

   private:
      
   };
