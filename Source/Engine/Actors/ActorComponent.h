#pragma once
/*!
 * \file ActorComponent.h
 * \date 2016/05/01 10:03
 *
 * \author SCW
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 *  TODO: implement a xml file parser for returning necessary xml data
 *  such as color, transform, etc....
 *
 * \note
 */

#include "../Utilities/String.h"

typedef std::list< weak_ptr<IActorComponent> > ChildComponents;

class TiXmlElement;

class IActorComponent
   {
   friend class ActorFactory;
   public:
      virtual ~IActorComponent( void ){}
      virtual ComponentId VGetId( void ) const = 0;
      virtual const std::string& VGetName( ) const = 0;

      virtual bool VInit( TiXmlElement* pData ) = 0;
      virtual void VPostInit( void ) = 0;
      virtual void VUpdate( const unsigned long deltaMs ) = 0;
      virtual void SetOwner( StrongActorPtr pOwner ) = 0;
      virtual void Destory( void ) = 0;
      virtual TiXmlElement* VGenerateXML( void ) = 0;
      virtual const ChildComponents& GetChildComponents( void ) const = 0;
      virtual const weak_ptr<IActorComponent> GetParentComponent( void ) = 0;

   protected:
      virtual void AddChildComponent( weak_ptr<IActorComponent> ) = 0;
      virtual void SetParentComponent( weak_ptr<IActorComponent> ) = 0;
   };

// Using Curiously recurring template pattern (CRTP) to prevent declaring GUID mulit times
template <typename T>class BaseActorComponent : virtual public IActorComponent 
   {
   friend class ActorFactory;
   public:
      virtual ComponentId VGetId( void ) const override { return s_ComponentId; };
      virtual const std::string& VGetName( void ) const override { return s_Name; }
      virtual void VPostInit( void ) override { };
      virtual void VUpdate( const unsigned long deltaMs ) override { };
      virtual const ChildComponents& GetChildComponents( void ) const override { return m_ChildComponents; };
      virtual const weak_ptr<IActorComponent> GetParentComponent( void ) override { return m_pParentComponent; }

   public:
      // GUID of this event
      const static ComponentId  s_ComponentId;
      const static std::string      s_Name;

   protected:
      virtual void SetOwner( StrongActorPtr pOwner ) override { m_pOwner = pOwner; }
      virtual void AddChildComponent( weak_ptr<IActorComponent> pChild ) override { m_ChildComponents.push_back( pChild ); }
      virtual void SetParentComponent( weak_ptr<IActorComponent> pParent ) override { m_pParentComponent = pParent; }

   protected:
      StrongActorPtr m_pOwner;
      weak_ptr<IActorComponent> m_pParentComponent;
      ChildComponents m_ChildComponents;
   };

//class ActorComponent : public IActorComponent 
//   {
//   friend class ActorFactory;
//
//   public:
//      ~ActorComponent( void ){ m_pOwner.reset(); } // make destructor virtual to prevent leak
//      virtual bool VInit( TiXmlElement* pData ) = 0;
//      virtual void VPostInit( void ) override {}
//      virtual void VUpdate( const unsigned long deltaMs ) override {}
//
//      virtual ComponentId VGetId( void ) const override { return GetIdFromName( VGetName() ); }
//      static ComponentId GetIdFromName( const char* componentName )
//         {
//         void* rawId = HashedString::hash_name( componentName );
//         return reinterpret_cast<ComponentId> ( rawId );
//         }
//   protected:
//      StrongActorPtr m_pOwner;
//
//   private:
//	   void SetOwner( StrongActorPtr pOwner) { m_pOwner = pOwner; }
//   };


//class ActorComponent 
//   {
//   friend class ActorFactory;
//
//public:
//   virtual ~ActorComponent( void ){ m_pOwner.reset(); }
//   virtual void Destory( void ) = 0;
//   virtual bool VInit( TiXmlElement* pData ) = 0;
//   virtual const char *VGetName( ) const = 0;
//   virtual void VPostInit( void ) 
//      {
//      }
//   virtual void VUpdate( const unsigned long deltaMs ) 
//      {
//      }
//
//   virtual ComponentId VGetId( void ) const 
//      {
//      return GetIdFromName( VGetName( ) );
//      }
//   static ComponentId GetIdFromName( const char* componentName )
//      {
//      void* rawId = HashedString::hash_name( componentName );
//      return reinterpret_cast< ComponentId > ( rawId );
//      }
//protected:
//   StrongActorPtr m_pOwner;
//
//private:
//   void SetOwner( StrongActorPtr pOwner )
//      {
//      m_pOwner = pOwner;
//      }
//   };