/*!
 * \file EditorLogic.cpp
 * \date 2016/07/03 9:26
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

#include "EditorStd.h"
#include "EditorLogic.h"
#include "..\ResourceCache\XmlResource.h"
#include "..\Utilities\XMLHelper.h"

EditorLogic::EditorLogic( shared_ptr<IRenderer> pRenderer ) : BaseEngineLogic( pRenderer )
   {
   }

void EditorLogic::VOnFileDrop( const char* filePath, const Point& dropLocation )
   {
   Resource fileRes( filePath );
   // std::string extension = fileRes.GetExtension();
   if( WildcardMatch( "*.xml", fileRes.m_Name.c_str() ) )
      {
      TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement( fileRes );
      std::string rootName = pRoot->Value();
      TransformPtr pTransform( m_pWrold->GetCamera()->VGetProperties().GetTransformPtr() );
      Vec3 projStart;
      Vec3 projEnd;
      m_pWrold->GetCamera()->GetScreenProjectPoint( projStart, projEnd, dropLocation, 17.f );
      pTransform->SetPosition( projEnd );
      if( !std::strcmp( rootName.c_str(), "ActorClass" ) )
         {
         VCreateActorFromClass( fileRes.m_Name.c_str(), pTransform );

         }
      else if( !std::strcmp( rootName.c_str(), "ActorOverrides" ) )
         {
         VCreateActorFromOverrides( fileRes.m_Name.c_str(), pTransform );
         }
      else if( !std::strcmp( rootName.c_str(), "World" ) )
         {
         VClearWorld();
         VLoadLevel( fileRes.m_Name.c_str() );
         }
      }
   }

void EditorLogic::VSaveAllActors() 
   {
   for( auto it = m_Actors.begin(); it != m_Actors.end(); ++it )
      {
      TiXmlElement* pResource = XmlResourceLoader::LoadAndReturnRootXmlElement( *it->second->m_pActorClassResource );
      TiXmlElement* pOverrides = it->second->GenerateOverridesXML( pResource );

      if( it->second->m_pOverridesResource ) // this actor is not drag-drop generated
         {
         XMLHelper::WriteXMLToFile( ( std::string( "Assets\\" ) + it->second->m_pOverridesResource->m_Name ).c_str(), pOverrides );
         }

      
      }
   }