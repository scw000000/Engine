#pragma once
/*!
 * \file SkySceneNode.h
 * \date 2016/05/03 15:07
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

#include "SceneNodes.h"

#include "..\Shaders.h"


class SkySceneNode : public SceneNode
   {
   public:
	   SkySceneNode( const ActorId actorId, 
                    IRenderComponent* pRenderComponent,
                    shared_ptr<Resource> pMeshResource,
                    shared_ptr<Resource> pTextureResource, 
                    RenderPass renderPass,  
                    TransformPtr pTransform );
      ~SkySceneNode( void );
	   virtual int VOnRestore( Scene *pScene ) override; ;
	   virtual int VOnLostDevice( Scene *pScene ) override { return S_OK; }
      // This function is called by Scene
      // bref: load constant buffers for material, light, etc in shaders
      // load vertex buffer & index buffer and make draw call
	   /**
	    * @brief 
	    *
	    * @param  pScene Scene * pScene
	    * @return int
	    */
	    virtual int VRender( Scene *pScene )  override;
	   //virtual int VPick(Scene *pScene, RayCast *pRayCast);
      GLuint GetProgram( void ) { return m_Program; };

      virtual bool VIsVisible( Scene *pScene ) override { return m_IsActive; };

   protected:
      void ReleaseResource( void );

   protected:
      bool					   m_IsActive;
      Mat4x4               m_Projection; // This variable is modified from the projection matrix of current camera node by increasing far distance
	   GLuint               m_Program;
      VertexShader		   m_VertexShader;
      FragmentShader		   m_FragmentShader;

      GLuint               m_VertexArrayObj;
      enum VB_TYPES
         {
         Vertex_Buffer,
         UV_Buffer,
         Index_Buffer,
         Num_Buffers
         };
      GLuint            m_Buffers[ Num_Buffers ];

      GLuint               m_MVPMatrix;
      GLuint               m_Texture;
      GLuint               m_TextureUni;
      
      shared_ptr<Resource> m_pTextureResource;
      shared_ptr<Resource> m_pMeshResource;
	   
      unsigned long     m_VerticesIndexCount;
   };
