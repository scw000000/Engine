/*!
 * \file SkyRenderComponent.h
 * \date 2016/05/03 16:54
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

#include "RenderComponent.h"

class SkyRenderComponent : public BaseRenderComponent 
   {
   public:
      static const char *g_Name;
      virtual const char *VGetName( ) const { return g_Name; }
      SkyRenderComponent( void );
      virtual void Destory( void ) override;

   protected:
      virtual bool VDelegateInit( TiXmlElement* pData ) override;
      virtual shared_ptr<SceneNode> VCreateSceneNode( void ) override;  // factory method to create the appropriate scene node

      // editor stuff
      //virtual void VCreateInheritedXmlElements(TiXmlElement* pBaseElement);

   protected:
      shared_ptr<Resource> m_pTextureResource;
      shared_ptr<Resource> m_pMeshResource;
   };