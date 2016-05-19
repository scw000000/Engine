/*!
 * \file UserInterface.cpp
 * \date 2016/05/19 16:23
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

#include "EngineStd.h"
#include "UserInterface.h"

UserInterface::UserInterface( const Resource& layoutRes ) : m_LayoutResource( layoutRes ), m_pGUIManager( g_pApp->m_pEngineLogic->m_pGUIManager )
   {
   m_IsMouseCursorEnable = false;
   m_ModalEventType = 0;
   m_pRoot = 0;
   m_pUIRoot = 0;
   m_pPromptRoot = 0;
   m_pMouseArrowImg = 0;
   }

UserInterface::~UserInterface( void )
   {
   if( m_pRoot )
      {
      CEGUI::WindowManager::getSingleton( ).destroyWindow( m_pRoot );
      }
   if( m_pUIRoot )
      {
      CEGUI::WindowManager::getSingleton( ).destroyWindow( m_pUIRoot );
      }
   if( m_pPromptRoot )
      {
      CEGUI::WindowManager::getSingleton( ).destroyWindow( m_pPromptRoot );
      }
   }

void UserInterface::VInit( void )
   {
   m_pRoot = CEGUI::WindowManager::getSingleton( ).createWindow( "DefaultWindow", "human_root" );
   m_pRoot->setMousePassThroughEnabled( true );

   m_pUIRoot = CEGUI::WindowManager::getSingleton( ).loadLayoutFromFile( m_LayoutResource.m_Name );
   m_pRoot->addChild( m_pUIRoot );

   m_pPromptRoot = CEGUI::WindowManager::getSingleton( ).createWindow( "DefaultWindow", "prompt_root" );
   m_pPromptRoot->setMousePassThroughEnabled( true );
   m_pPromptRoot->setAlwaysOnTop( true );
   m_pRoot->addChild( m_pPromptRoot );

   m_pGUIManager->AttachLayout( m_pRoot );

   m_ModalEventType = g_pApp->RegisterEvent( 1 );
   }

void UserInterface::VSetDisplayMouseCursor( bool isDisplay )
   {
   SetMouseCursor( m_pRoot, isDisplay? m_pMouseArrowImg: NULL );
   }

void UserInterface::SetMouseCursor( CEGUI::Window* pWindow, const CEGUI::Image* pImage )
   {
   pWindow->setMouseCursor( pImage );
   size_t childNum = pWindow->getChildCount( );
   for( size_t i = 0; i < childNum; ++i )
      {
      SetMouseCursor( pWindow->getChildAtIdx( i ), pImage );
      }
   }

int UserInterface::VAsk( MessageBox_Questions question )
   {
   std::wstring msg;
   std::wstring title;
   int buttonFlags;
   int defaultAnswer = IDOK;

   switch( question )
      {
         case QUESTION_WHERES_THE_CD:
            {
            msg = ( _T( "IDS_QUESTION_WHERES_THE_CD" ) );
            title = ( _T( "IDS_ALERT" ) );
            buttonFlags = MB_RETRYCANCEL;
            defaultAnswer = IDCANCEL;
            break;
            }
         case QUESTION_QUIT_GAME:
            {
            msg = ( _T( "IDS_QUESTION_QUIT_GAME" ) );
            title = ( _T( "IDS_QUESTION" ) );
            buttonFlags = MB_YESNO;
            defaultAnswer = IDNO;
            break;
            }
         default:
         ENG_ASSERT( 0 && _T( "Undefined question in GUIManager::Ask" ) );
         return IDCANCEL;
      }

   if( m_pGUIManager )
      {
      if( m_pPromptRoot->getChildCount() > 10 )
         {
         ENG_ASSERT( 0 && "Too Many nested dialogs!" );
         return defaultAnswer;
         }
      VSetDisplayMouseCursor( true );
      shared_ptr<PromptBox> pPromptBox( ENG_NEW PromptBox( m_pPromptRoot, m_ModalEventType, msg, title, buttonFlags ) );
      pPromptBox->m_pWindow->setModalState( true );
      int result = g_pApp->Modal( pPromptBox, defaultAnswer );

      if( !m_pPromptRoot->getChildCount() )
         {
         VSetDisplayMouseCursor( m_IsMouseCursorEnable );
         }
      return result;
      }
   // If the engine is not exist, still pop a message box
   return 0;
   //return ::MessageBox(g_pApp ? g_pApp->GetHwnd() : NULL, msg.c_str(), title.c_str(), buttonFlags);
   }

bool UserInterface::VHasModalWindow( void ) 
   {
   return m_pPromptRoot->getChildCount() > 0;
   }

Uint32 UserInterface::GetModalEventType( void )
   {
   return m_ModalEventType;
   }