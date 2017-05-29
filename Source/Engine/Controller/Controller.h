#pragma once
/*!
 * \file Controller.h
 * \date 2016/06/05 14:10
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
class InputManager;

class Controller : public IController
   {
   public:
      Controller( InputManager& manager ) : m_InputManager( manager ) {}
      
   protected:
      InputManager& m_InputManager;
   };