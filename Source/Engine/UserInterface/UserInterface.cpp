////////////////////////////////////////////////////////////////////////////////
// Filename: UserInterface.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "UserInterface.h"
#include "GUIManager.h"

Dialog::~Dialog( void )
   {
   std::cout << "delete" << std::endl;
   if( m_pWindow )
      {
      std::cout << "delete" << std::endl;
     // CEGUI::Window* pWindow = m_pWindow->getTitlebar();
      m_pWindow->removeAllEvents();
      
      //m_pWindow->getParent()->destroyChild( m_pWindow );
   //   m_pWindow->removeChild( pWindow );
    //  pWindow = NULL;
     //CEGUI::WindowManager::getSingleton().destroyWindow( m_pWindow->getTitlebar() );
      CEGUI::WindowManager::getSingleton().destroyWindow( m_pWindow );
      m_pWindow = NULL;
      }
   }

Dialog::Dialog( CEGUI::Window* pRoot, const std::wstring& msg, const std::wstring& title, int buttonFlags )
   {
   ENG_ASSERT( pRoot );
   CEGUI::Window* p_Window = CreateCEGUIWindow( "WindowsLook/FrameWindow", "Dialog" );
   m_pWindow = static_cast<CEGUI::FrameWindow*>( p_Window );
   pRoot->addChild( m_pWindow );

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

   m_pWindow->setText( (CEGUI::utf8*)( ws2UTF8s( title ).c_str() ) );
   CEGUI::MultiLineEditbox* textBox = static_cast<CEGUI::MultiLineEditbox*> ( CreateCEGUIWindow( "WindowsLook/MultiLineEditbox", "textbox", Vec4( 0.0f, 0.0f, 0.0f, 0.0f ), Vec4( 1.0f, 0.0f, 1.0f, 0.f ) ) );
   // We need to attach it before get corret line number
   m_pWindow->addChild( textBox ); 
   textBox->setText(  (CEGUI::utf8*)( ws2UTF8s( msg ).c_str() ) );
   textBox->setReadOnly( true ); 
   float textBoxHeight = textBox->getFont()->getLineSpacing() * static_cast<float> ( textBox->getFormattedLines(  ).size() ) + 10.0f ;
   textBox->setHeight(  CEGUI::UDim( 0.0f, textBoxHeight ) );
   float btnBaseHeight = textBox->getPixelSize().d_height;
   m_pWindow->setHeight( CEGUI::UDim( 0.0f, m_pWindow->getTitlebar()->getPixelSize().d_height + btnBaseHeight + btnHeight + 2 * borderHeight ) );
   
   m_pWindow->setPosition( CEGUI::UVector2( CEGUI::UDim( 0.f, ( m_pWindow->getRootContainerSize().d_width - m_pWindow->getPixelSize().d_width ) / 2.0f ), 
      CEGUI::UDim( 0.f, ( m_pWindow->getRootContainerSize().d_height - m_pWindow->getPixelSize().d_height  ) / 2.0f ) ) );

   
	if ( (buttonFlags == MB_ABORTRETRYIGNORE) || (buttonFlags == MB_CANCELTRYCONTINUE) )
	   {
		// The message box contains three push buttons: Cancel, Try Again, Continue. 
		// This is the new standard over Abort,Retry,Ignore
      std::string windowName = m_pWindow->getName().c_str();

      CEGUI::PushButton* button0 = static_cast<CEGUI::PushButton*> ( CreateCEGUIWindow( "WindowsLook/Button", windowName + " IDS_CONTINUE", Vec4( 0.0f, borderWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button0->setText( "CONTINUE" );
      button0->setID( 0 );
      button0->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &Dialog::OnButtonClicked, this ) );
      std::cout << "subscribe" << std::endl;
      m_pWindow->addChild( button0 );

      CEGUI::PushButton* button1 = static_cast<CEGUI::PushButton*> ( CreateCEGUIWindow( "WindowsLook/Button", windowName + " IDS_TRYAGAIN", Vec4( 0.0f, borderWidth * 2.0f + btnWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button1->setText( "TRYAGAIN" );
      button1->setID( 0 );
      m_pWindow->addChild( button1 );

      CEGUI::PushButton* button2 = static_cast<CEGUI::PushButton*> ( CreateCEGUIWindow( "WindowsLook/Button", windowName + " IDS_CANCEL", Vec4( 0.0f, borderWidth * 3.0f + btnWidth * 2.0f, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button2->setText( "CANCEL" );
      button2->setID( 2 );
      m_pWindow->addChild( button2 );
	   }
	else if (buttonFlags == MB_OKCANCEL)
	   {
		//The message box contains two push buttons: OK and Cancel.
      std::string windowName = m_pWindow->getName().c_str();

      CEGUI::PushButton* button0 = static_cast<CEGUI::PushButton*> ( CreateCEGUIWindow( "WindowsLook/Button", windowName + " IDS_OK", Vec4( 0.0f, borderWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button0->setText( "OK" );
      button0->setID( 0 );
      m_pWindow->addChild( button0 );

      CEGUI::PushButton* button1 = static_cast<CEGUI::PushButton*> ( CreateCEGUIWindow( "WindowsLook/Button", windowName + " IDS_CANCEL", Vec4( 0.0f, borderWidth * 2.0f + btnWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button1->setText( "CANCEL" );
      button1->setID( 1 );
      m_pWindow->addChild( button1 );
	   }
	else if (buttonFlags == MB_RETRYCANCEL)
	   {
		//The message box contains two push buttons: Retry and Cancel.
      std::string windowName = m_pWindow->getName().c_str();

      CEGUI::PushButton* button0 = static_cast<CEGUI::PushButton*> ( CreateCEGUIWindow( "WindowsLook/Button", windowName + " IDS_RETRY", Vec4( 0.0f, borderWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button0->setText( "RETRY" );
      button0->setID( 0 );
      m_pWindow->addChild( button0 );

      CEGUI::PushButton* button1 = static_cast<CEGUI::PushButton*> ( CreateCEGUIWindow( "WindowsLook/Button", windowName + " IDS_CANCEL", Vec4( 0.0f, borderWidth * 2.0f + btnWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button1->setText( "CANCEL" );
      button1->setID( 1 );
      m_pWindow->addChild( button1 );
	   }
	else if (buttonFlags == MB_YESNO)
	   {
		//The message box contains two push buttons: Yes and No.
      std::string windowName = m_pWindow->getName().c_str();

      CEGUI::PushButton* button0 = static_cast<CEGUI::PushButton*> ( CreateCEGUIWindow( "WindowsLook/Button", windowName + " IDS_YES", Vec4( 0.0f, borderWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button0->setText( "YES" );
      button0->setID( 0 );
      m_pWindow->addChild( button0 );

      CEGUI::PushButton* button1 = static_cast<CEGUI::PushButton*> ( CreateCEGUIWindow( "WindowsLook/Button", windowName + " IDS_NO", Vec4( 0.0f, borderWidth * 2.0f + btnWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button1->setText( "NO" );
      button1->setID( 1 );
      m_pWindow->addChild( button1 );
	   }
	else if (buttonFlags == MB_YESNOCANCEL)
	   {
		//The message box contains three push buttons: Yes, No, and Cancel.
      std::string windowName = m_pWindow->getName().c_str();

      CEGUI::PushButton* button0 = static_cast<CEGUI::PushButton*> ( CreateCEGUIWindow( "WindowsLook/Button", windowName + " IDS_YES", Vec4( 0.0f, borderWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button0->setText( "YES" );
      button0->setID( 0 );
      m_pWindow->addChild( button0 );

      CEGUI::PushButton* button1 = static_cast<CEGUI::PushButton*> ( CreateCEGUIWindow( "WindowsLook/Button", windowName + " IDS_NO", Vec4( 0.0f, borderWidth * 2.0f + btnWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button1->setText( "NO" );
      button1->setID( 1 );
      m_pWindow->addChild( button1 );

      CEGUI::PushButton* button2 = static_cast<CEGUI::PushButton*> ( CreateCEGUIWindow( "WindowsLook/Button", windowName + " IDS_CANCEL", Vec4( 0.0f, borderWidth * 3.0f + btnWidth * 2.0f, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button2->setText( "CANCEL" );
      button2->setID( 2 );
      m_pWindow->addChild( button2 );
	   }
	else //if (buttonFlags & MB_OK)
	   {
        // The message box contains one push button: OK. This is the default.
		std::string windowName = m_pWindow->getName().c_str();

      CEGUI::PushButton* button0 = static_cast<CEGUI::PushButton*> ( CreateCEGUIWindow( "WindowsLook/Button", windowName + " IDS_OK", Vec4( 0.0f, borderWidth, 0.0f, btnBaseHeight + borderHeight ), Vec4( 0.0f, btnWidth, 0.0f, btnHeight ) ) );
      button0->setText( "OK" );
      button0->setID( 0 );
      m_pWindow->addChild( button0 );
	   }
   }

// CEGUI::UDim( scale, pos ) : scale : relative point on the screen between (0,1)
//                             pos   : additional absolute shift in pixels
// UDim( 0.5, 100 ) -> middle point + shift 100
CEGUI::Window* Dialog::CreateCEGUIWindow( const std::string &type, const std::string& name, const Vec4& position, const Vec4& size )
   {
   CEGUI::Window* pWindow =  CEGUI::WindowManager::getSingleton().createWindow( type, name );
   pWindow->setPosition( CEGUI::UVector2( CEGUI::UDim( position.x, position.y ), CEGUI::UDim( position.z, position.w ) ) );
   pWindow->setSize( CEGUI::USize( CEGUI::UDim( size.x, size.y ), CEGUI::UDim( size.z, size.w ) ) );
   return pWindow;
   }

bool Dialog::OnButtonClicked( const CEGUI::EventArgs& arg )
   {
   std::cout << "fuck" << std::endl;
   return true;
   }