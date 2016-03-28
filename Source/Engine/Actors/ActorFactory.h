#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: ActorFactory.h
////////////////////////////////////////////////////////////////////////////////


typedef unsigned long AcotrId;
typedef shared_ptr<Actor> StrongActorPtr;
typedef shared_ptr<ActorComponent> StrongActorComponentPtr;

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
      GenericObjectFactory< ActorComponent, ComponentId > m_componentFactory;

   private:
      ActorId GetNextActorId( void ){ ++m_lastActorId; return m_lastActorId; }

   private:
      ActorId m_lastActorId;
   };