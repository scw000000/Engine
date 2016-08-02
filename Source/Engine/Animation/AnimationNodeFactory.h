/*!
 * \file AnimationNodeFactory.h
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
#include "AnimationNode.h"

class AnimationNodeFactory : public ENG_Noncopyable
   {
   public:
      static AnimationNodeFactory& GetSingleton( void );
      shared_ptr< IAnimationNode > BuildAnimNode( AnimNodeType animType, LuaPlus::LuaObject& eventData );
      template <typename T> void RegisterAnimNodeCreation( AnimNodeType type );

   private:
      AnimationNodeFactory( void );
      shared_ptr< IAnimationNode > CreateAnimNode( AnimNodeType animType );

   private:
      GenericObjectFactory< IAnimationNode, AnimNodeType > m_AnimNodeFactory;
      
   };

template <typename T> void AnimationNodeFactory::RegisterAnimNodeCreation( AnimNodeType type )
   {
   ENG_ASSERT( GetSingleton().m_AnimNodeFactory.Register<T>( type ) );
   }

#define REGISTER_ANIM_NODE( nodeClass ) AnimationNodeFactory::GetSingleton().RegisterAnimNodeCreation< nodeClass >( nodeClass::s_AnimNodeType )