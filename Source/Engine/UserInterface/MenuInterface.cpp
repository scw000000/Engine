/*!
 * \file MenuInterface.cpp
 * \date 2016/05/21 13:45
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
#include "MenuInterface.h"
#include "HumanView.h"

MenuInterface::MenuInterface( const Resource& layoutRes ) : UserInterface( layoutRes )
   {
   m_pLevelList = NULL;
   m_pStartButton = NULL;
   m_IsMouseCursorEnable = true;
   }

void MenuInterface::VInit( void )
   {
   auto childNum = m_pUIRoot->getChildCount();
   for( unsigned int i = 0; i < childNum; ++i )
      {
      auto pWindow = m_pUIRoot->getChildAtIdx( i );
      if( pWindow->getName() == std::string( "LevelList" ) )
         {
         m_pLevelList = static_cast<CEGUI::Listbox*>( pWindow );
         }
      else if( pWindow->getName() == std::string( "StartButton" ) )
         {
         m_pStartButton = static_cast<CEGUI::PushButton*>( pWindow );
         }
      }
   ENG_ASSERT( m_pLevelList && m_pStartButton );

   auto levels = g_pApp->m_pEngineLogic->m_pLevelManager->GetLevels();
   for( auto lv : levels )
      {
      CEGUI::ListboxTextItem* it = ENG_NEW CEGUI::ListboxTextItem( lv.c_str() );
      it->setSelectionColours( CEGUI::Colour( 1.0f, 0.0f, 0.0f ) );
      it->setSelectionBrushImage( "GlossySerpentFHD/ListboxSelectionBrush" );
      m_pLevelList->addItem( it );
      }

   m_pStartButton->subscribeEvent( CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber( &MenuInterface::StartLevel, this ) );
 //  CEGUI::ListboxTextItem* it = ENG_NEW CEGUI::ListboxTextItem( "ddddd" );
 //  it->setSelectionColours( CEGUI::Colour( 1.0f, 0.0f, 0.0f ) );
 //  it->setSelectionBrushImage( "GlossySerpentFHD/ListboxSelectionBrush" );
 // // it->setSelected(true);
 ////  it->setTextColours( CEGUI::Colour( 1.0f, 0.0f, 0.0f ) );
 //  m_pLevelList->addItem( it );
 //  m_pLevelList->setItemSelectState( it, true );
 //  std::cout << m_pLevelList->getSelectedCount() << std::endl;
 //  /*CEGUI::ListboxTextItem* it = ENG_NEW CEGUI::ListboxTextItem( "fff" );
 //  m_pLevelList->addItem( it );*/
 //  CEGUI::ListboxTextItem* it2 = ENG_NEW CEGUI::ListboxTextItem( "fsdafdfff" );
 //  m_pLevelList->addItem( it2 );
 //  m_pLevelList->setItemSelectState( it2, true );
 //  std::cout << m_pLevelList->getSelectedCount() << std::endl;

 //  CEGUI::ListboxTextItem* it3 = ENG_NEW CEGUI::ListboxTextItem( "fffaaaaaaaaa" );
 //  it3->setSelected( true );
 //  m_pLevelList->addItem( it3 );
   //m_pLevelList->setItemSelectState()
   //CEGUI::ItemEntry* it = ENG_NEW CEGUI::ItemEntry( "dd","fickj" );
   //it->setText( "mother" );
   //m_pLevelList->addItem()
   
   }

bool MenuInterface::StartLevel( const CEGUI::EventArgs& arg )
   {
   auto selectedLevel = m_pLevelList->getFirstSelectedItem();
   g_pApp->m_pEngineLogic->VLoadLevel();
   g_pApp->GetHumanView()->AttachLayout( shared_ptr<UserInterface> ( ENG_NEW UserInterface( Resource( "Empty.layout", true ) ) ) );
   g_pApp->GetHumanView()->m_pController->VSetPointerLocked( true );
   return true;
   }