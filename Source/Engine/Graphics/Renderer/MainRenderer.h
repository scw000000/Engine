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


class MainRenderer : public IMainRenderer
   {
  // friend class SkeletalMeshSceneNode;
   public:
      MainRenderer( void );
      ~MainRenderer( void );
      
      /* virtual int VOnRestore( void ) override;
       virtual void VShutdown( void ) override;*/
      // This functino is called at the begin of HumanView::Render before calling Scene to Rendoer
	   virtual int VPreRender( void ) override;
      // This functino is called at the end of HumanView::Render after calling Scene to Render
	   virtual int VPostRender( void ) override;
	   //virtual void VCalcLighting( Lights *lights, int maximumLights ) override;

      virtual void VSetRenderingAlpha( bool isAlpha ) override;
      virtual void VSetBackgroundColor( const Color& color ) override;
      virtual void VDrawLine( const Vec3& fromWS, const Vec3& toWS, const Color& color ) const override;

   public:
   //   static OpenGLTextureDrawer s_TextureDrawer;

   private:
      
   };
