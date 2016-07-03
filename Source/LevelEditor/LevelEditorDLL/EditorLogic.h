#pragma once
/*!
 * \file EditorLogic.h
 * \date 2016/07/03 9:27
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

class EditorLogic : public BaseEngineLogic
   {
   public:
      EditorLogic( shared_ptr<IRenderer> pRenderer );
      virtual void VOnFileDrop( const char* filePath, const Point& dropLocation ) override;
   
   };