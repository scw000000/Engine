/*!
 * \file Dialog.cpp
 * \date 2016/05/15 19:15
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
#include "PromptBox.h"
#include "GUIManager.h"
#include <iostream> 
#include <fstream> 

PromptBox::~PromptBox( void )
   {
   }

PromptBox::PromptBox( CEGUI::Window* pRoot, Uint32 eventType, const std::wstring& msg, const std::wstring& title, int buttonFlags )
   {
   ENG_ASSERT( pRoot );
   // Be aware! creating CEGUI window must specify unique name, or it will throw exception
   CEGUI::Window* p_Window = GUIManager::CreateCEGUIWindow( "GlossySerpentFHD/FrameWindow", ws2s( msg ) );
   m_pWindow = static_cast<CEGUI::FrameWindow*>( p_Window );  
   pRoot->addChild( m_pWindow );
   m_pWindow->setAlwaysOnTop( true );

   ENG_ASSERT( eventType != ((Uint32)-1) );
   m_EventType = eventType;

   buttonFlags &= 0xF;
   int numButtons = 2;
	if ( (buttonFlags == MB_ABORTRETRYIGNORE) || (buttonFlags == MB_CANCELTRYCONTINUE) || (buttonFlags == MB_CANCELTRYCONTINUE) )
	   {
		numButtons = 3;
	   }
	else if (buttonFlags == MB_OK)
	   {
		numButtons = 1;
	   }
   float btnWidth = ( m_pWindow->getRootContainerSize().d_width * 0.075f);
   float btnHeight = (m_pWindow->getRootContainerSize().d_height * 0.037f);
   float borderWidth = ( m_pWindow->getRootContainerSize().d_width * 0.02f);
   float borderHeight =  ( m_pWindow->getRootContainerSize().d_height * 0.008f);
   m_pWindow->setWidth( CEGUI::UDim( 0.0f, (float)numButtons * ( borderWidth + btnWidth ) + borderWidth ) );

   m_pWindow->setText(  ws2UTF8s( g_pApp->GetString( title ) ) );
   CEGUI::MultiLineEditbox* textBox = static_cast<CEGUI::MultiLineEditbox*> ( GUIManager::CreateCEGUIWindow( "GlossySerpentFHD/MultiLineEditbox", "textbox", Vec4( 0.0f, 0.0f, 0.0f, 0.0f ), Vec4( 1.0f, 0.0f, 1.0f, 0.f ) ) );
   // We need to attach it before get corret line number
   CEGUI::NamedArea namearea( textBox->getName() );
   namearea.setArea( CEGUI::ComponentArea() );
   m_pWindow->addChild( textBox ); 
   textBox->setText(  ws2UTF8s( g_pApp->GetString( msg ) ) );
   textBox->setReadOnly( true ); 
   float textBoxHeight = ( textBox->getFont()->getLineSpacing() + 5.0f ) * static_cast<float> ( textBox->getFormattedLines(  ).size() );
   
   textBox->setHeight(  CEGUI::UDim( 0.0f, textBoxHeight ) );
   float btnBaseHeight = textBox->getPixelSize().d_height;
   m_pWindow->setHeight( CEGUI::UDim( 0.0f, m_pWindow->getTitlebar()->getPixelSize().d_height + btnBaseHeight + btnHeight + 2 * borderHeight ) );
   
   m_pWindow->setVerticalAlignment( CEGUI::VA_CENTRE );
   m_pWindow->setHorizontalAlignment( CEGUI::HA_CENTRE );
   m_pWindow->setSizingEnabled( false );

   m_pWindow->setCloseButtonEnabled( false );
   //m_pWindow->getCloseButton()->setID( 0 );
   //m_pWindow->getCloseButton()->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &Dialog::OnButtonClicked, this ) );
      

	if ( (buttonFlags == MB_ABORTRETRYIGNORE) || (buttonFlags == MB_CANCELTRYCONTINUE) )
	   {
		// The message box contains three push buttons: Cancel, Try Again, Continue. 
		// This is the new standard over Abort,Retry,Ignore
      std::string windowName = m_pWindow->getName().c_str();

      CEGUI::PushButton* button0 = static_cast<CEGUI::PushButton*> ( GUIManager::CreateCEGUIWindow( "GlossySerpentFHD/Button", windowName + " IDS_CONTINUE", Vec4( 0.0f, borderWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button0->setText( ws2UTF8s( g_pApp->GetString( _T( "IDS_CONTINUE" ) ) ) );
      button0->setID( IDCONTINUE );
      button0->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &PromptBox::OnButtonClicked, this ) );
      m_pWindow->addChild( button0 );

      CEGUI::PushButton* button1 = static_cast<CEGUI::PushButton*> ( GUIManager::CreateCEGUIWindow( "GlossySerpentFHD/Button", windowName + " IDS_TRYAGAIN", Vec4( 0.0f, borderWidth * 2.0f + btnWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button1->setText( ws2UTF8s( g_pApp->GetString( _T( "IDS_TRYAGAIN" ) ) ) );
      button1->setID( IDTRYAGAIN );
      button1->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &PromptBox::OnButtonClicked, this ) );
      m_pWindow->addChild( button1 );

      CEGUI::PushButton* button2 = static_cast<CEGUI::PushButton*> ( GUIManager::CreateCEGUIWindow( "GlossySerpentFHD/Button", windowName + " IDS_CANCEL", Vec4( 0.0f, borderWidth * 3.0f + btnWidth * 2.0f, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button2->setText( ws2UTF8s( g_pApp->GetString( _T( "IDS_CANCEL" ) ) )  );
      button2->setID( IDCANCEL );
      button2->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &PromptBox::OnButtonClicked, this ) );
      m_pWindow->addChild( button2 );
	   }
	else if (buttonFlags == MB_OKCANCEL)
	   {
		//The message box contains two push buttons: OK and Cancel.
      std::string windowName = m_pWindow->getName().c_str();

      CEGUI::PushButton* button0 = static_cast<CEGUI::PushButton*> ( GUIManager::CreateCEGUIWindow( "GlossySerpentFHD/Button", windowName + " IDS_OK", Vec4( 0.0f, borderWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button0->setText( ws2UTF8s( g_pApp->GetString( _T( "IDS_OK" ) ) ) );
      button0->setID( IDOK );
      button0->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &PromptBox::OnButtonClicked, this ) );
      m_pWindow->addChild( button0 );

      CEGUI::PushButton* button1 = static_cast<CEGUI::PushButton*> ( GUIManager::CreateCEGUIWindow( "GlossySerpentFHD/Button", windowName + " IDS_CANCEL", Vec4( 0.0f, borderWidth * 2.0f + btnWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button1->setText( ws2UTF8s( g_pApp->GetString( _T( "IDS_CANCEL" ) ) )  );
      button1->setID( IDCANCEL );
      button1->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &PromptBox::OnButtonClicked, this ) );
      m_pWindow->addChild( button1 );
	   }
	else if (buttonFlags == MB_RETRYCANCEL)
	   {
		//The message box contains two push buttons: Retry and Cancel.
      std::string windowName = m_pWindow->getName().c_str();

      CEGUI::PushButton* button0 = static_cast<CEGUI::PushButton*> ( GUIManager::CreateCEGUIWindow( "GlossySerpentFHD/Button", windowName + " IDS_RETRY", Vec4( 0.0f, borderWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button0->setText( ws2UTF8s( g_pApp->GetString( _T( "IDS_RETRY" ) ) )  );
      button0->setID( IDRETRY );
      button0->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &PromptBox::OnButtonClicked, this ) );
      m_pWindow->addChild( button0 );

      CEGUI::PushButton* button1 = static_cast<CEGUI::PushButton*> ( GUIManager::CreateCEGUIWindow( "GlossySerpentFHD/Button", windowName + " IDS_CANCEL", Vec4( 0.0f, borderWidth * 2.0f + btnWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button1->setText( ws2UTF8s( g_pApp->GetString( _T( "IDS_CANCEL" ) ) )  );
      button1->setID( IDCANCEL );
      button1->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &PromptBox::OnButtonClicked, this ) );
      m_pWindow->addChild( button1 );
	   }
	else if (buttonFlags == MB_YESNO)
	   {
		//The message box contains two push buttons: Yes and No.
      std::string windowName = m_pWindow->getName().c_str();

      CEGUI::PushButton* button0 = static_cast<CEGUI::PushButton*> ( GUIManager::CreateCEGUIWindow( "GlossySerpentFHD/Button", windowName + " IDS_YES", Vec4( 0.0f, borderWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button0->setText( ws2UTF8s( g_pApp->GetString( _T( "IDS_YES" ) ) )  );
      button0->setID( IDYES );
      button0->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &PromptBox::OnButtonClicked, this ) );
      m_pWindow->addChild( button0 );

      CEGUI::PushButton* button1 = static_cast<CEGUI::PushButton*> ( GUIManager::CreateCEGUIWindow( "GlossySerpentFHD/Button", windowName + " IDS_NO", Vec4( 0.0f, borderWidth * 2.0f + btnWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button1->setText( ws2UTF8s( g_pApp->GetString( _T( "IDS_NO" ) ) )  );
      button1->setID( IDNO );
      button1->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &PromptBox::OnButtonClicked, this ) );
      m_pWindow->addChild( button1 );
	   }
	else if (buttonFlags == MB_YESNOCANCEL)
	   {
		//The message box contains three push buttons: Yes, No, and Cancel.
      std::string windowName = m_pWindow->getName().c_str();

      CEGUI::PushButton* button0 = static_cast<CEGUI::PushButton*> ( GUIManager::CreateCEGUIWindow( "GlossySerpentFHD/Button", windowName + " IDS_YES", Vec4( 0.0f, borderWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button0->setText( ws2UTF8s( g_pApp->GetString( _T( "IDS_YES" ) ) )  );
      button0->setID( IDYES );
      button0->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &PromptBox::OnButtonClicked, this ) );
      m_pWindow->addChild( button0 );

      CEGUI::PushButton* button1 = static_cast<CEGUI::PushButton*> ( GUIManager::CreateCEGUIWindow( "GlossySerpentFHD/Button", windowName + " IDS_NO", Vec4( 0.0f, borderWidth * 2.0f + btnWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button1->setText( ws2UTF8s( g_pApp->GetString( _T( "IDS_NO" ) ) )  );
      button1->setID( IDNO );
      button1->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &PromptBox::OnButtonClicked, this ) );
      m_pWindow->addChild( button1 );

      CEGUI::PushButton* button2 = static_cast<CEGUI::PushButton*> ( GUIManager::CreateCEGUIWindow( "GlossySerpentFHD/Button", windowName + " IDS_CANCEL", Vec4( 0.0f, borderWidth * 3.0f + btnWidth * 2.0f, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button2->setText( ws2UTF8s( g_pApp->GetString( _T( "IDS_CANCEL" ) ) )  );
      button2->setID( IDCANCEL );
      button2->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &PromptBox::OnButtonClicked, this ) );
      m_pWindow->addChild( button2 );
	   }
	else //if (buttonFlags & MB_OK)
	   {
        // The message box contains one push button: OK. This is the default.
		std::string windowName = m_pWindow->getName().c_str();

      CEGUI::PushButton* button0 = static_cast<CEGUI::PushButton*> ( GUIManager::CreateCEGUIWindow( "GlossySerpentFHD/Button", windowName + " IDS_OK", Vec4( 0.0f, borderWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button0->setText( ws2UTF8s( g_pApp->GetString( _T( "IDS_OK" ) ) )  );
      button0->setID( IDOK );
      button0->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &PromptBox::OnButtonClicked, this ) );
      m_pWindow->addChild( button0 );
	   }
   std::ofstream fout("temp.xml");

      
   CEGUI::XMLSerializer xml( fout );
   m_pWindow->writeXMLToStream( xml );

      fout.close( );
   }

bool PromptBox::OnButtonClicked( const CEGUI::EventArgs& arg )
   {
   const CEGUI::MouseEventArgs& mouseArgs = static_cast<const CEGUI::MouseEventArgs&>( arg );
   int id = mouseArgs.window->getID();
   SDL_Event event;
   SDL_zero(event); /* or SDL_zero(event) */
   event.type = m_EventType;
   event.user.code = id;
   ENG_ASSERT( SDL_PushEvent( &event ) >= 0 );
   return true;
   }