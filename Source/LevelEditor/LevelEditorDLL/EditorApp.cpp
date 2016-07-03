/*!
 * \file EditorApp.cpp
 * \date 2016/07/03 10:06
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

#include "EditorStd.h"
#include "EditorApp.h"
#include "EditorLogic.h"

EditorApp editorApp;

BaseEngineLogic* EditorApp::VCreateLogic()
   {
   m_pEngineLogic = ENG_NEW EditorLogic( g_pApp->m_pRenderer );
   m_pEngineLogic->Init();

   return m_pEngineLogic;
   }