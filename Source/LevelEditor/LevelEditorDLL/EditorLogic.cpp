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
#include <sstream>  
#include "..\Actors\Actor.h"

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
         VCreateActor( fileRes, pTransform );
         }
      else if( !std::strcmp( rootName.c_str(), "World" ) )
         {
         VClearWorld();
         std::string newLevelInstanceDir = fileRes.GetFileName();
         VLoadLevel();
         }
      }
   }

void EditorLogic::VSaveAllActors() 
   {
   for( auto it = m_Actors.begin(); it != m_Actors.end(); ++it )
      {
      VSaveActor( it->first );
      }
   }

void EditorLogic::VSaveActor( ActorId id )
   {
   auto actorIt = m_Actors.find( id );
   if( actorIt == m_Actors.end() )
      {
      return;
      }
   TiXmlElement* pResource = XmlResourceLoader::LoadAndReturnRootXmlElement( *actorIt->second->m_pActorClassResource );
   TiXmlElement* pOverrides = actorIt->second->GenerateOverridesXML( pResource );

   if( actorIt->second->m_pActorInstanceResource ) // this actor is not drag-drop generated
      {
      XMLHelper::WriteXMLToFile( ( std::string( "Assets\\" ) + actorIt->second->m_pActorInstanceResource->m_Name ).c_str(), pOverrides );
      }
   else
      {
      std::string fileName = actorIt->second->m_pActorClassResource->GetFileName();
      std::stringstream ss;
      ss << "Assets\\" << g_pApp->m_EngineOptions.GetFullActorInstanceDirectory() << fileName.substr( 0, fileName.find_last_of( "." ) ) << actorIt->second->GetId() << ".xml";
      XMLHelper::WriteXMLToFile( ss.str().c_str(), pOverrides );
      }
   std::stringstream ss;
   ss << "Saved Actor " << actorIt->first << " to file";
   ENG_LOG( "System", ss.str().c_str() );
   }

TiXmlElement* EditorLogic::VGenerateWorldXML(void)
   {
   TiXmlElement* pRet = ENG_NEW TiXmlElement( "World" );
  
   TiXmlElement* pStaicActors = ENG_NEW TiXmlElement( "StaticActors" );
   pRet->LinkEndChild( pStaicActors );

   for( auto actorIt : m_Actors )
      {
      if( actorIt.second->m_pActorInstanceResource )
         {
         TiXmlElement* pActor = ENG_NEW TiXmlElement( "Actor" );
         pStaicActors->LinkEndChild( pActor );
         pActor->SetAttribute( "actorinstance", actorIt.second->m_pActorInstanceResource->GetFileName().c_str() );
         }
      }
   return pRet;
   }

void EditorLogic::VSaveWorld( void )
   {
   TiXmlElement* pWorld = VGenerateWorldXML();
   std::stringstream ss;
   ss << "Assets\\" << m_pLevelManager->GetCurrentLevel();
   XMLHelper::WriteXMLToFile( ss.str().c_str(), pWorld );
   ENG_LOG( "System", "Saved world to file" );
   }