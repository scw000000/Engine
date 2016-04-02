////////////////////////////////////////////////////////////////////////////////
// Filename: UserInterface.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "UserInterface.h"
#include "GUIManager.h"

Dialog::Dialog( CEGUI::Window* pWindow, const std::wstring& msg, const std::wstring& title, int buttonFlags )
   {
   m_pWindow = pWindow;
   //std::cout << ws2s( std::wstring( _T("幹你娘") ) ) << std::endl;
   std::wstring aa = L"我管你123还是abc就算あいうえお也\n可以！阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿阿";

   m_pWindow->setText( (CEGUI::utf8*)( ws2UTF8s( title ).c_str() ) );
   CEGUI::FrameWindow* pFrameWindow = static_cast<CEGUI::FrameWindow*> ( m_pWindow );
   pFrameWindow->setTitleBarEnabled( true );

   CEGUI::MultiLineEditbox* textBox = static_cast<CEGUI::MultiLineEditbox*> ( GUIManager::CreateCEGUIWindow( "WindowsLook/MultiLineEditbox", "textbox", Vec4( 0.0f, 0.0f, 0.0f, 0.0f ), Vec4( 1.0f, 0.0f, 0.7f, 0.f ) ) );
   pFrameWindow->addChild( textBox );
   textBox->setText(  (CEGUI::utf8*)( ws2UTF8s( aa ).c_str() ) );
   textBox->setReadOnly( true );
   // To disable scrol bar
   textBox->setWordWrapping( true );

   CEGUI::PushButton* button = static_cast<CEGUI::PushButton*> ( GUIManager::CreateCEGUIWindow( "WindowsLook/Button", "button", Vec4( 0.1f, 0.0f, 0.8f, 0.0f ), Vec4( 0.25f, 0.0f, 0.15f, 0.f ) ) );
   pFrameWindow->addChild( button );

   CEGUI::PushButton* button2 = static_cast<CEGUI::PushButton*> ( GUIManager::CreateCEGUIWindow( "WindowsLook/Button", "button2", Vec4( 0.65f, 0.0f, 0.8f, 0.0f ), Vec4( 0.25f, 0.0f, 0.15f, 0.f ) ) );
   pFrameWindow->addChild( button2 );
   // textBox->set
  
   //DefaultWindow 
	/*
	// Find the dimensions of the message
 	RECT rc;
	SetRect( &rc, 0,0,0,0);
	m_UI.CalcTextRect( msg.c_str(), m_UI.GetDefaultElement(DXUT_CONTROL_STATIC,0), &rc );
    int msgWidth = rc.right - rc.left;
    int msgHeight = rc.bottom - rc.top;

	int numButtons = 2;
	if ( (buttonFlags == MB_ABORTRETRYIGNORE) ||
		(buttonFlags == MB_CANCELTRYCONTINUE) ||
		(buttonFlags == MB_CANCELTRYCONTINUE) )
	{
		numButtons = 3;
	}
	else if (buttonFlags == MB_OK)
	{
		numButtons = 1;
	}

	int btnWidth = (int)((float) g_pApp->GetScreenSize().x * 0.15f);
	int btnHeight = (int)((float) g_pApp->GetScreenSize().y * 0.037f);
	int border = (int)((float) g_pApp->GetScreenSize().x * 0.043f);

	m_Width = std::max(msgWidth + 2 * border, btnWidth + 2 * border);
	m_Height = msgHeight + (numButtons * (btnHeight+border) ) + (2 * border);

	m_PosX = (g_pApp->GetScreenSize().x - m_Width)/2;
	m_PosY = (g_pApp->GetScreenSize().y - m_Height)/2;
	m_UI.SetLocation( m_PosX, m_PosY );

	m_UI.SetSize( m_Width, m_Height );
	//m_UI.SetBackgroundColors(g_Gray40);

	D3DCOLOR red = D3DCOLOR_ARGB(0xc0,0xff,0x00,0x00);
	m_UI.SetBackgroundColors(red);

	int iY = border; 
	int iX = (m_Width - msgWidth) / 2; 

	m_UI.AddStatic( 0, msg.c_str(), iX, iY, msgWidth, msgHeight);

	iX = (m_Width - btnWidth) / 2;
	iY = m_Height - btnHeight - border;

	buttonFlags &= 0xF;
	if ( (buttonFlags == MB_ABORTRETRYIGNORE) ||
		 (buttonFlags == MB_CANCELTRYCONTINUE) )

	{
		// The message box contains three push buttons: Cancel, Try Again, Continue. 
		// This is the new standard over Abort,Retry,Ignore
		m_UI.AddButton( IDCONTINUE, g_pApp->GetString(_T("IDS_CONTINUE")).c_str(), iX, iY - (2*border), btnWidth, btnHeight );
		m_UI.AddButton( IDTRYAGAIN, g_pApp->GetString(_T("IDS_TRYAGAIN")).c_str(), iX, iY - border, btnWidth, btnHeight );
		m_UI.AddButton( IDCANCEL, g_pApp->GetString(_T("IDS_CANCEL")).c_str(), iX, iY, btnWidth, btnHeight );
	}
	else if (buttonFlags == MB_OKCANCEL)
	{
		//The message box contains two push buttons: OK and Cancel.
		m_UI.AddButton( IDOK, g_pApp->GetString(_T("IDS_OK")).c_str(), iX, iY - border, btnWidth, btnHeight );
		m_UI.AddButton( IDCANCEL, g_pApp->GetString(_T("IDS_CANCEL")).c_str(), iX, iY, btnWidth, btnHeight );
	}
	else if (buttonFlags == MB_RETRYCANCEL)
	{
		//The message box contains two push buttons: Retry and Cancel.
		m_UI.AddButton( IDRETRY, g_pApp->GetString(_T("IDS_RETRY")).c_str(), iX, iY - border, btnWidth, btnHeight );
		m_UI.AddButton( IDCANCEL, g_pApp->GetString(_T("IDS_CANCEL")).c_str(), iX, iY, btnWidth, btnHeight );
	}
	else if (buttonFlags == MB_YESNO)
	{
		//The message box contains two push buttons: Yes and No.
		m_UI.AddButton( IDYES, g_pApp->GetString(_T("IDS_YES")).c_str(), iX, iY - border, btnWidth, btnHeight, 0x59 );
		m_UI.AddButton( IDNO, g_pApp->GetString(_T("IDS_NO")).c_str(), iX, iY, btnWidth, btnHeight, 0x4E );
	}
	else if (buttonFlags == MB_YESNOCANCEL)
	{
		//The message box contains three push buttons: Yes, No, and Cancel.
		m_UI.AddButton( IDYES, g_pApp->GetString(_T("IDS_YES")).c_str(), iX, iY - (2*border), btnWidth, btnHeight );
		m_UI.AddButton( IDNO, g_pApp->GetString(_T("IDS_NO")).c_str(), iX, iY - border, btnWidth, btnHeight );
		m_UI.AddButton( IDCANCEL, g_pApp->GetString(_T("IDS_CANCEL")).c_str(), iX, iY, btnWidth, btnHeight );
	}
	else //if (buttonFlags & MB_OK)
	{
        // The message box contains one push button: OK. This is the default.
		m_UI.AddButton( IDOK, g_pApp->GetString(_T("IDS_OK")).c_str(), iX, iY, btnWidth, btnHeight );
	}*/
   }