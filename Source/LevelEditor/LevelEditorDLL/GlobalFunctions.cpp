/*!
* \file GlobalFunctions.cpp
* \date 2016/05/26 8:49
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
#include "GlobalFunctions.h"
#include "..\Main\Engine.h"

EngineApp engineApp;

int EditorMain( int *pWindow, int screenWidth, int screenHeight )
   {
   // Set up checks for memory leaks.
   // Game Coding Complete reference - Chapter 21, page 834
   //
   int tmpDbgFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );

   // set this flag to keep memory blocks around
   // tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;	       // this flag will cause intermittent pauses in your game and potientially cause it to run out of memory!

   // perform memory check for each alloc/dealloc
   //tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;		   // remember this is VERY VERY SLOW!

   /*
   _CRTDBG_LEAK_CHECK_DF is used at program initialization to force a
   //   leak check just before program exit. This is important because
   //   some classes may dynamically allocate memory in globally constructed
   //   objects.
   */
   tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;

   _CrtSetDbgFlag( tmpDbgFlag );

   Logger::Init( "logging.xml" );

   g_pApp->m_EngineOptions.Init( "EditorOptions.xml" );
   g_pApp->m_EngineOptions.m_ScreenSize.x = screenWidth;
   g_pApp->m_EngineOptions.m_ScreenSize.y = screenHeight;

   g_pApp->InitInstance( ( SDL_Window* )pWindow, g_pApp->m_EngineOptions.m_ScreenSize.x, g_pApp->m_EngineOptions.m_ScreenSize.y );

  // g_pApp->MainLoop();

  // Logger::Destroy();

   return true;
   }

void SingleLoop( void )
   {
   g_pApp->SingleLoop();
   std::cout << "test" << std::endl;
  // ENG_LOG( "Test", "SZZZZZ" );
  // std::string s( ( "string" ) ); 
   }

void Shutdown( void )
   {
   g_pApp->OnClose();
   Logger::Destroy();
   }


//   FUTURE WORK: This should return a bool specifying if the level was successfully opened.
//
//void OpenLevel( BSTR fullPathLevelFile )
//   {
//   // We need to strip off the project directory from the filename first.
//   std::string levelFile = ws2s( std::wstring( fullPathLevelFile, SysStringLen( fullPathLevelFile ) ) );
//   EditorLogic* pEditorLogic = ( EditorLogic* ) g_pApp->m_pGame;
//   if( pEditorLogic )
//      {
//      std::string assetsDir = "\\Assets\\";
//      int projDirLength = pEditorLogic->GetProjectDirectory().length() + assetsDir.length();
//      g_pApp->m_Options.m_Level = levelFile.substr( projDirLength, levelFile.length() - projDirLength );
//      pEditorLogic->VChangeState( BGS_LoadingGameEnvironment );
//      }
//   }


//int GetNumActors()
//   {
//   EditorLogic* pGame = ( EditorLogic* ) g_pApp->m_pGame;
//   return ( pGame ) ? pGame->GetNumActors() : 0;
//   }


//void GetActorList( int *ptr, int numActors )
//   {
//   // To keep things simple, we pass the actor ids to the C# app
//   // the C# app iterates through the actor ids, and calls back into
//   // the unmanaged dll to get the appropriate information about each
//   // actor
//   EditorLogic* pGame = ( EditorLogic* ) g_pApp->m_pGame;
//   if( pGame )
//      {
//      ActorMap::const_iterator itr;
//      int actorArrayIndex;
//      for( itr = pGame->GetActorMap().begin(), actorArrayIndex = 0;
//           itr != pGame->GetActorMap().end() && actorArrayIndex < numActors; itr++, actorArrayIndex++ )
//         {
//         ActorId actorId = itr->first;
//         ptr[ actorArrayIndex ] = actorId;
//         }
//      }
//   }


//int GetActorXmlSize( ActorId actorId )
//   {
//   StrongActorPtr pActor = MakeStrongPtr( g_pApp->m_pGame->VGetActor( actorId ) );
//   if( !pActor )
//      {
//      return 0;
//      }
//   std::string xml = pActor->ToXML();
//   return xml.length();
//   }


void GetActorXML( int *actorXMLAddress, ActorId actorId )
   {
   StrongActorPtr pActor = g_pApp->m_pEngineLogic->VGetActor( actorId ).lock();
   if( !pActor )
      {
      return;
      }
   std::string xml = XMLHelper::WriteXMLToString( pActor->GenerateXML() );

   strncpy_s( reinterpret_cast< char * >( actorXMLAddress ), xml.length() + 1, xml.c_str(), xml.length() );
   }


//int PickActor( int *hWndPtrAddress )
//   {
//   HWND hWnd = ( HWND ) hWndPtrAddress;
//
//   POINT ptCursor;
//   GetCursorPos( &ptCursor );
//
//   // Convert the screen coordinates of the mouse cursor into
//   // coordinates relative to the client window
//   ScreenToClient( hWnd, &ptCursor );
//   RayCast rayCast( ptCursor );
//   EditorLogic* pGame = ( EditorLogic* ) g_pApp->m_pGame;
//
//   if( !pGame )
//      {
//      return INVALID_ACTOR_ID;
//      }
//
//   shared_ptr<EditorHumanView> pView = pGame->GetHumanView();
//   if( !pView )
//      {
//      return INVALID_ACTOR_ID;
//      }
//
//
//   // Cast a ray through the scene. The RayCast object contains an array of Intersection
//   // objects.
//   pView->GetScene()->Pick( &rayCast );
//   rayCast.Sort();
//
//   // If there are any intersections, get information from the first intersection.
//   if( !rayCast.m_NumIntersections )
//      {
//      return INVALID_ACTOR_ID;
//      }
//
//   Intersection firstIntersection = rayCast.m_IntersectionArray[ 0 ];
//   return firstIntersection.m_actorId;
//   }

//void ModifyActor( BSTR bstrActorModificationXML )
//   {
//   std::string actorModificationXML = ws2s( std::wstring( bstrActorModificationXML, SysStringLen( bstrActorModificationXML ) ) );
//
//   TiXmlDocument doc;
//   doc.Parse( actorModificationXML.c_str() );
//   TiXmlElement* pRoot = doc.RootElement();
//   if( !pRoot )
//      return;
//
//   g_pApp->m_pGame->VModifyActor( atoi( pRoot->Attribute( "id" ) ), pRoot );
//   }


void DestroyActor( ActorId actorId )
   {
   g_pApp->m_pEngineLogic->VDestroyActor( actorId );
   }

//int GetLevelScriptAdditionsXmlSize()
//   {
//   TiXmlDocument *optionsDoc = g_pApp->m_Options.m_pDoc;
//   TiXmlElement *pRoot = optionsDoc->RootElement();
//   if( !pRoot )
//      return 0;
//
//   TiXmlElement *worldScriptAdditions = pRoot->FirstChildElement( "WorldScriptAdditions" );
//
//   TiXmlDocument outDoc;
//   TiXmlNode *node = worldScriptAdditions->Clone();
//   outDoc.LinkEndChild( node );
//
//   TiXmlPrinter printer;
//   outDoc.Accept( &printer );
//
//   std::string xml = printer.CStr();
//
//   return xml.length();
//   }


//void GetLevelScriptAdditionsXml( int *xmlAddress )
//   {
//   TiXmlDocument *optionsDoc = g_pApp->m_Options.m_pDoc;
//   TiXmlElement *pRoot = optionsDoc->RootElement();
//   if( !pRoot )
//      return;
//
//   TiXmlElement *worldScriptAdditions = pRoot->FirstChildElement( "WorldScriptAdditions" );
//
//   TiXmlDocument outDoc;
//   TiXmlNode *node = worldScriptAdditions->Clone();
//   outDoc.LinkEndChild( node );
//
//
//   TiXmlPrinter printer;
//   outDoc.Accept( &printer );
//
//   std::string xml = printer.CStr();
//
//   strncpy_s( reinterpret_cast< char * >( xmlAddress ), xml.length() + 1, xml.c_str(), xml.length() );
//   }


