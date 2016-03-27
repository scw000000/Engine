#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Actor.h
////////////////////////////////////////////////////////////////////////////////


typedef std::string ActorType; 


class Actor
   {
   friend class ActorFactory;
   typedef std::map< ComponentId, StrongActorComponentPtr > ActorComponents;

   public:
      explicit Actor( ActorId id );
      ~Actor( void );

      bool Init( TiXmlElement* pData );
      void PostInit( void );
      // To break out the circular reference strongPtr chain of Actor & actorComponent
      void Destroy( void );
      void Update( int deltaMs );

      ActorId GetId( void ) const { return m_id; }
      template<class ComponenType> weak_ptr<ComponenType> GetComponent( ComponentId id )
         {
         ActorComponents::iterator findIt = m_components.find( id );
         if( findIt != m_components.end() )
            {
            // Get component ptr as class actor component
            // We need to down cast it to specific actor component class
            StrongActorPtrComponentPtr pBase( findIt->second );
            shared_ptr<ComponenType> pSub( std::static_pointer_cast<ComponenType>( pBase ) );
            // transfer it to weak ptr
            weak_ptr<ComponenType> pWeakSub( pSub );
            return pWeakSub;
            }
         
         }
   private:
      void AddComponent( StrongActorComponentPtr pComponent );

   private:
      ActorId m_id;
      ActorType m_type;
      ActorComponents m_components;
      std::string m_resource;
   };