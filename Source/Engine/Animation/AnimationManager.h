#pragma once
/*!
 * \file AnimationManager.h
 * \date 2016/07/30 21:31
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

class AnimationState;
#include "..\LuaScripting\ScriptClass.h"

class IAnimationManager : public ENG_Noncopyable
   {
   public:
      virtual ~IAnimationManager( void ) { };
      virtual void VSetIsRunning( bool isRunning ) = 0;
      virtual bool VGetIsRunning( void ) const = 0;
      virtual void VUpdate( float deltaSeconds ) = 0;
      virtual void VAddAnimationState( shared_ptr< AnimationState > pNewState ) = 0;
      virtual void VAddAnimationState( LuaPlus::LuaObject luaObj ) = 0;
      virtual void VRemoveAnimationState( ActorId actorId ) = 0;
      virtual std::vector< shared_ptr< AnimationState > > VGetAnimationStates( ActorId actorId ) const = 0;
           
   private:
      
   };

class AnimationManager : public IAnimationManager, public BaseScriptClass< AnimationManager >
   {
   public:
      AnimationManager( void );
      static AnimationManager& GetSingleton( void );
      virtual void VSetIsRunning( bool isRunning ) override;
      virtual bool VGetIsRunning( void ) const override { return m_IsRunning; };
      virtual void VUpdate( float deltaSeconds ) override;
      virtual void VAddAnimationState( shared_ptr< AnimationState > pNewState ) override;
      virtual void VAddAnimationState( LuaPlus::LuaObject luaObj ) override;
      virtual void VRemoveAnimationState( ActorId actorId ) override;      
      virtual std::vector< shared_ptr< AnimationState > > VGetAnimationStates( ActorId actorId ) const override;
      void NewAnimationRootNodedelegate( IEventPtr pEvent );

      virtual bool VBuildCppDataFromScript( LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject constructionData ) override; 
      virtual void VRegisterExposedMemberFunctions( LuaPlus::LuaObject& metaTableObj ) override;

   private:
      
      bool m_IsRunning;
      std::map< ActorId, std::vector< shared_ptr< AnimationState > > > m_AnimationStates;
      // This lua obj is for when adding animation state in lua, the mesh index and actor id hasn't beeen
      // set, so it must set in here temperately 
      LuaPlus::LuaObject m_TempLuaObj;
   };
