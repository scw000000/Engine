////////////////////////////////////////////////////////////////////////////////
// Filename: GUIManager.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "GUIManager.h"

CEGUI::OpenGL3Renderer* GUIManager::s_pRenderer = NULL;
CEGUI::DefaultResourceProvider* GUIManager::s_pResProvider = NULL;

GUIManager::GUIManager( void )
   {
   m_pContext = NULL;
   m_pRoot = NULL;
   m_pPromptRoot = NULL;
   m_pUIRoot = NULL;
   }

void GUIManager::Init(  const std::string& resourceDirectory  )
   {
   if( !s_pRenderer || !s_pResProvider )
      {
      s_pRenderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();
      s_pResProvider = static_cast< CEGUI::DefaultResourceProvider* >( CEGUI::System::getSingleton().getResourceProvider() );
      std::string resourceDir( resourceDirectory );
      s_pResProvider->setResourceGroupDirectory( "imagesets", resourceDir + "imagesets/" );
      s_pResProvider->setResourceGroupDirectory( "schemes", resourceDir + "schemes/" );
      s_pResProvider->setResourceGroupDirectory( "fonts", resourceDir + "fonts/" );
      s_pResProvider->setResourceGroupDirectory( "layouts", resourceDir + "layouts/" );
      s_pResProvider->setResourceGroupDirectory( "looknfeel", resourceDir + "looknfeel/" );
      s_pResProvider->setResourceGroupDirectory( "lua_scripts", resourceDir + "lua_scripts/" );

      CEGUI::ImageManager::setImagesetDefaultResourceGroup( "imagesets" );
      CEGUI::Scheme::setDefaultResourceGroup( "schemes" );
      CEGUI::Font::setDefaultResourceGroup( "fonts" );
      CEGUI::WindowManager::setDefaultResourceGroup( "layouts" );
      CEGUI::WidgetLookManager::setDefaultResourceGroup( "looknfeel" );
      CEGUI::ScriptModule::setDefaultResourceGroup( "lua_scripts" );
      }
   

   m_pContext = &CEGUI::System::getSingleton().createGUIContext( s_pRenderer->getDefaultRenderTarget() );
   LoadScheme( "WindowsLook.scheme" );
   SetFont( "UTF8.ttf" );

   m_pRoot = CEGUI::WindowManager::getSingleton().createWindow( "DefaultWindow", "root" );
   m_pContext->setRootWindow( m_pRoot );
   m_pPromptRoot = CEGUI::WindowManager::getSingleton().createWindow( "DefaultWindow", "prompt_root" );
   m_pRoot->addChild( m_pPromptRoot );
   m_pUIRoot = CEGUI::WindowManager::getSingleton().createWindow( "DefaultWindow", "ui_root" );
   m_pRoot->addChild( m_pUIRoot );
   CEGUI::Window* p_Window = CreateCEGUIWindow( "WindowsLook/FrameWindow", "TestButton", Vec4( 0.5f, 0.0f, 0.5f, 0.0f ), Vec4( 0.3f, 0.0f, 0.3f, 0.f ) );
   ENG_NEW Dialog( p_Window, std::wstring( _T( "我操你娘機掰" ) ), std::wstring( _T( "警告" ) ), MB_RETRYCANCEL );
   m_pPromptRoot->addChild( p_Window );
   }

void GUIManager::Destory( void )
   {
   CEGUI::System::getSingleton().destroyGUIContext( *m_pContext );
   }

void GUIManager::LoadScheme( const std::string& schemeFile )
   {
   CEGUI::SchemeManager::getSingleton().createFromFile( schemeFile );
   }

void GUIManager::SetFont( const std::string& fontFile )
   {
   ENG_ASSERT( m_pContext );
   CEGUI::FontManager::getSingleton().createFreeTypeFont( fontFile, 10/*pt*/, true, fontFile );
   m_pContext->setDefaultFont( fontFile );
  // CEGUI::FontManager::getSingleton().createFromFile( fontFile + ".font" );
  // m_pContext->setDefaultFont( fontFile );
   }

void GUIManager::OnRender( double fTime, float fElapsedTime )
   {
   s_pRenderer->beginRendering();
   m_pContext->draw();
   s_pRenderer->endRendering();
   }

/*
// TODO: implement xml resourceloader & localized string conversion
int GUIManager::Ask( MessageBox_Questions question )
   {
   std::wstring msg;
	std::wstring title;
	int buttonFlags;
	int defaultAnswer = IDOK;

	switch(question)
	{
		case QUESTION_WHERES_THE_CD:
		{
			msg = (_T("IDS_QUESTION_WHERES_THE_CD"));
			title = (_T("IDS_ALERT"));
			buttonFlags = MB_RETRYCANCEL | MB_ICONEXCLAMATION;
			defaultAnswer = IDCANCEL;
			break;
		}
		case QUESTION_QUIT_GAME:
		{
			msg = (_T("IDS_QUESTION_QUIT_GAME"));
			title = (_T("IDS_QUESTION"));
			buttonFlags = MB_YESNO|MB_ICONQUESTION;
			defaultAnswer = IDNO;
			break;
		}
		default:
			ENG_ASSERT(0 && _T("Undefined question in GUIManager::Ask"));
			return IDCANCEL;
	}

	if ( g_pApp && g_pApp->IsRunning() )
	{
		ShowCursor(true);
		shared_ptr<MessageBox> pMessageBox( ENG_NEW MessageBox( msg, title, buttonFlags ) );
		int result = g_pApp->Modal(pMessageBox, defaultAnswer);
		ShowCursor(false);
		return result;
	}
	// If the engine is not exist, still pop a message box
	return -1;
   //return ::MessageBox(g_pApp ? g_pApp->GetHwnd() : NULL, msg.c_str(), title.c_str(), buttonFlags);
   }*/

// CEGUI::UDim( scale, pos ) : scale : relative point on the screen between (0,1)
//                             pos   : additional absolute shift in pixels
// UDim( 0.5, 100 ) -> middle point + shift 100
CEGUI::Window* GUIManager::CreateCEGUIWindow( const std::string &type, const std::string& name, const Vec4& position, const Vec4& size )
   {
   CEGUI::Window* pWindow =  CEGUI::WindowManager::getSingleton().createWindow( type, name );
   pWindow->setPosition( CEGUI::UVector2( CEGUI::UDim( position.x, position.y ), CEGUI::UDim( position.z, position.w ) ) );
   pWindow->setSize( CEGUI::USize( CEGUI::UDim( size.x, size.y ), CEGUI::UDim( size.z, size.w ) ) );
   return pWindow;
   }