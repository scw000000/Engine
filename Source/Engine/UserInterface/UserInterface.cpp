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
#include "HumanView.h"
#include "..\Controller\Controller.h"

UserInterface::UserInterface( const Resource& layoutRes ) : m_LayoutResource( layoutRes ), m_pGUIManager( g_pApp->m_pEngineLogic->m_pGUIManager )
   {
   m_IsMouseCursorEnable = false;
   m_pRoot = CEGUI::WindowManager::getSingleton( ).createWindow( "DefaultWindow", "human_root" );
   m_pRoot->setMousePassThroughEnabled( true );

   m_pUIRoot = CEGUI::WindowManager::getSingleton( ).loadLayoutFromFile( m_LayoutResource.m_Name );
   m_pRoot->addChild( m_pUIRoot );

   m_pPromptRoot = CEGUI::WindowManager::getSingleton( ).createWindow( "DefaultWindow", "prompt_root" );
   m_pPromptRoot->setMousePassThroughEnabled( true );
   m_pPromptRoot->setAlwaysOnTop( true );
   m_pRoot->addChild( m_pPromptRoot );

   m_ModalEventType = g_pApp->RegisterEvent( 1 );
   }

UserInterface::~UserInterface( void )
   {
   }

void UserInterface::VInit( void )
   {
   }

void UserInterface::VSetIsMouseCursorEnable( bool isDisplay )
   {
   m_IsMouseCursorEnable = isDisplay;
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
      bool prevMouseDisplay = m_IsMouseCursorEnable;
      bool prevMouseLock = g_pApp->GetHumanView()->m_pController->VIsPointerLocked();
      m_pGUIManager->SetDisplayMouseCursor( true );
      VSetIsMouseCursorEnable( true );
      g_pApp->GetHumanView()->m_pController->VSetPointerLocked( false );

      shared_ptr<PromptBox> pPromptBox( ENG_NEW PromptBox( m_pPromptRoot, m_ModalEventType, msg, title, buttonFlags ) );
      pPromptBox->GetWindow()->setModalState( true );
      int result = g_pApp->Modal( pPromptBox, defaultAnswer );
      m_pPromptRoot->destroyChild( pPromptBox->GetWindow() );
      if( !m_pPromptRoot->getChildCount() )
         {
         VSetIsMouseCursorEnable( prevMouseDisplay );
         g_pApp->GetHumanView()->m_pController->VSetPointerLocked( prevMouseLock );
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

bool UserInterface::VIsMouseCursorEnable( void )
   {
   return m_IsMouseCursorEnable;
   }

Uint32 UserInterface::GetModalEventType( void )
   {
   return m_ModalEventType;
   }