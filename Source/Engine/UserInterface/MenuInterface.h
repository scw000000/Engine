/*!
 * \file MenuInterface.h
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
#include "UserInterface.h"

class MenuInterface : public UserInterface
   {
   public:
      MenuInterface( const Resource& layoutRes );
      virtual void VInit( void ) override;

   private:
      bool StartLevel( const CEGUI::EventArgs& arg );

   private:
      CEGUI::Listbox* m_pLevelList;
      CEGUI::PushButton* m_pStartButton;
   };