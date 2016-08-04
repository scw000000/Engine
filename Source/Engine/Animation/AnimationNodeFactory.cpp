/*!
 * \file AnimationNodeFactory.cpp
 * \date 2016/08/02 17:56
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
#include "AnimationNodeFactory.h"

//AnimationNodeFactory& AnimationNodeFactory::GetSingleton( void )
//   {
//   static AnimationNodeFactory animNodeFactory;
//   return animNodeFactory;
//   }
//
//shared_ptr< IAnimationNode > AnimationNodeFactory::BuildAnimNode( AnimNodeType animType, LuaPlus::LuaObject& eventData )
//   {
//   auto pAnimNode = CreateAnimNode( animType );
//   if( !pAnimNode )
//      {
//      return pAnimNode;
//      }
//   if( !pAnimNode->VInit( eventData ) )
//      {
//      return shared_ptr < IAnimationNode > ( );
//      }
//   return pAnimNode;
//   }
//
//AnimationNodeFactory::AnimationNodeFactory( void )
//   {
//   }
//
//shared_ptr< IAnimationNode > AnimationNodeFactory::CreateAnimNode( AnimNodeType animType )
//   {
//   return  shared_ptr< IAnimationNode >( m_AnimNodeFactory.Create( animType ) );
//   }
