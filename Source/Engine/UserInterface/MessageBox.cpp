////////////////////////////////////////////////////////////////////////////////
// Filename: MessageBox.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "MessageBox.h"
#include "HumanView.h"

/*
const SDL_MessageBoxColorScheme DEFAULT_COLOR_SCHEME = 
   {
      { // .colors (.r, .g, .b) 
         // [SDL_MESSAGEBOX_COLOR_BACKGROUND] 
         { 255,   0,   0 },
         // [SDL_MESSAGEBOX_COLOR_TEXT] 
         {   0, 255,   0 },
         // [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] 
         { 255, 255,   0 },
         // [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] 
         {   0,   0, 255 },
         // [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] 
         { 255,   0, 255 }
      }
   };

MessageBox::MessageBox( std::wstring msg, std::wstring title, int buttonFlags )
   {
   m_UIData.colorScheme = &DEFAULT_COLOR_SCHEME;
   m_UIData.flags = SDL_MESSAGEBOX_INFORMATION;
   m_UIData.window = g_pApp->GetWindow();
   m_UIData.title = ToStr( title.c_str() ).c_str();
   m_UIData.message = ToStr( msg.c_str() ).c_str();


   buttonFlags &= 0xF; // only take last byte of buttonflags
	if ( (buttonFlags == MB_ABORTRETRYIGNORE) ||  (buttonFlags == MB_CANCELTRYCONTINUE) )
      {
		// The message box contains three push buttons: Cancel, Try Again, Continue. 
		// This is the new standard over Abort,Retry,Ignore
      m_UIData.buttons = ENG_NEW SDL_MessageBoxButtonData [] 
         {
            {                                       0, 0, "CONTINUE" },
            { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "TRYAGAIN" },
            { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "CANCEL" },
         };
	   }
	else if (buttonFlags == MB_OKCANCEL)
	   {
		//The message box contains two push buttons: OK and Cancel.
      m_UIData.buttons = ENG_NEW SDL_MessageBoxButtonData [] 
         {
            { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK" },
            { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "CANCEL" },
         };
	   }
	else if (buttonFlags == MB_RETRYCANCEL)
	   {
		//The message box contains two push buttons: Retry and Cancel.
		m_UIData.buttons = ENG_NEW SDL_MessageBoxButtonData [] 
         {
            { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "RETRY" },
            { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "CANCEL" },
         };
	   }
	else if (buttonFlags == MB_YESNO)
	   {
		//The message box contains two push buttons: Yes and No.
		m_UIData.buttons = ENG_NEW SDL_MessageBoxButtonData [] 
         {
            { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "YES" },
            { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "NO" },
         };
	   }
	else if (buttonFlags == MB_YESNOCANCEL)
	   {
		//The message box contains three push buttons: Yes, No, and Cancel.
		m_UIData.buttons = ENG_NEW SDL_MessageBoxButtonData [] 
         {
            {                                       0, 0, "YES" },
            { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "NO" },
            { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "CANCEL" },
         };
	   }
	else //if (buttonFlags & MB_OK)
	   {
        // The message box contains one push button: OK. This is the default.
		m_UIData.buttons = ENG_NEW SDL_MessageBoxButtonData [] 
         {
            { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK" },
         };
	   }

   m_UIData.numbuttons = SDL_arraysize( m_UIData.buttons);
   }

MessageBox::~MessageBox()
   {
	SAFE_DELETE_ARRAY ( m_UIData.buttons );
   }

int MessageBox::VOnRestore()
   {
   return 0;
   }

int MessageBox::VOnRender( double fTime, float fElapsedTime )
   {
   
   }
*/