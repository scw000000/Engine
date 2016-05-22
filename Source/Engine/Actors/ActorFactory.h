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
      StrongActorPtr CreateActor( const char *actorResource );
      void ModifyActor( StrongActorPtr pActor, TiXmlElement* overrides );

   protected:
      virtual StrongActorComponentPtr CreateComponent( TiXmlElement* pData );

   protected:
      // This factory is defined in Templates.h
      GenericObjectFactory< IActorComponent, ComponentId > m_ComponentFactory;

   private:
      ActorId GetNextActorId( void ){ ++m_lastActorId; return m_lastActorId; }

   private:
      ActorId m_lastActorId;
   };