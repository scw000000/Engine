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
      void Update( const unsigned long deltaMs );

      ActorId GetId( void ) const { return m_Id; }
      template<class ComponenType> weak_ptr<ComponenType> GetComponent( ComponentId id )
         {
         ActorComponents::iterator findIt = m_Components.find( id );
         if( findIt != m_Components.end() )
            {
            // Get component ptr as class actor component
            // We need to down cast it to specific actor component class
            StrongActorComponentPtr pBase( findIt->second );
            shared_ptr<ComponenType> pSub( std::static_pointer_cast<ComponenType>( pBase ) );
            // transfer it to weak ptr
            weak_ptr<ComponenType> pWeakSub( pSub );
            return pWeakSub;
            }
         else
            {
            return weak_ptr<ComponenType>();
            }
         }
   private:
      void AddComponent( StrongActorComponentPtr pComponent );

   private:
      ActorId m_Id;
      ActorType m_Type;
      ActorComponents m_Components;
      std::string m_Resource; // actor XML file name
   };