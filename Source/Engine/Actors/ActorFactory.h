#pragma once
/*!
 * \file ActorFactory.h
 * \date 2016/05/22 10:12
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

typedef unsigned long AcotrId;
typedef shared_ptr<Actor> StrongActorPtr;

class ActorFactory
   {
   public:
      ActorFactory( void );
      StrongActorPtr CreateActor( Resource* pClassRes, 
                                  Resource* pOverridesRes,
                                  TransformPtr pInitialTransform = NULL, 
                                  ActorId serversActorId = INVALID_ACTOR_ID );
      bool ModifyActor( StrongActorPtr pActor, TiXmlElement* overrides );

   protected:
      StrongActorComponentPtr CreateComponent( const std::string& componentName, TiXmlElement* pData );
      StrongActorComponentPtr BuildComponentTree( TiXmlElement* pData, StrongActorPtr pActor, StrongActorComponentPtr pParent );
      bool ModifyActorComponent( StrongActorComponentPtr pActorComponent, TiXmlElement* pOverrides );


   protected:
      // This factory is defined in Templates.h
      GenericObjectFactory< IActorComponent, ComponentId > m_ComponentFactory;

   private:
      ActorId GetNextActorId( void ){ ++m_LastActorId; return m_LastActorId; }

   private:
      ActorId m_LastActorId;
   };