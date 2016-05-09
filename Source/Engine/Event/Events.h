#pragma once

/*!
 * \file Events.h
 * \date 2016/05/07 14:02
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
#include <strstream>
#include "FastDelegate.h"

class IEvent;
class SceneNode;

typedef unsigned long EventType;
typedef shared_ptr<IEvent> IEventPtr;
typedef fastdelegate::FastDelegate1<IEventPtr> EventListenerDelegate;

class IEvent
   {
   public:
      virtual ~IEvent(void) {}
      // This function returns eventType when you don't know the event instance ID
      // IF you want to know the instance ID of a specific event type, access the const static
      // menber variable in BaseEvent instead
      virtual const EventType& VGetEventType( void ) const = 0;
      virtual float VGetTimeStamp( void ) const = 0;
      // for Networking
      // virtual void VSerialize( std::ostrstream& out ) const  = 0;
      virtual IEventPtr VCopy( void ) const = 0;
      virtual const char* GetName( void ) const = 0;
   };

// Using Curiously recurring template pattern (CRTP) to prevent declaring GUID mulit times
template <typename T>class BaseEvent : virtual public IEvent
   {
   public:
      explicit BaseEvent( const float timeStamp = 0.0f ) : m_TimeStamp( timeStamp ) {  };
      
      virtual ~BaseEvent( void ) { }

      virtual const EventType& VGetEventType( void ) const override { return s_EventType; };

      virtual float VGetTimeStamp( void ) const override { return m_TimeStamp; }
   
      virtual const char* GetName( void ) const override { return s_pName; }

   public:
       // GUID of this event
      const static EventType  s_EventType;
      const static char*      s_pName;

   protected:
      

   private:
      
      const float m_TimeStamp;
   };

class EvtData_Destroy_Actor : public BaseEvent<EvtData_Destroy_Actor>
   {
   public:
      explicit EvtData_Destroy_Actor( ActorId id ) : m_Id( id ) {  }
      virtual IEventPtr VCopy( void ) const { return IEventPtr( ENG_NEW EvtData_Destroy_Actor( m_Id ) ); }
      //virtual void VSerialize( std::ostre )
     // virtual const char* GetName( void ) const { return "EvtDat_Destyot_Actor"; } 

   private:
      ActorId m_Id;
   };

class EvtData_Move_Actor : public BaseEvent<EvtData_Move_Actor>
   {


public:

   EvtData_Move_Actor( void ) { m_Id = INVALID_ACTOR_ID; }

   EvtData_Move_Actor( ActorId id, const Mat4x4& matrix ) : m_Id( id ), m_Matrix( matrix ){ }

   //virtual void VSerialize( std::ostrstream &out ) const
   //   {
   //   out << m_Id << " ";
   //   for( int i = 0; i < 4; ++i )
   //      {
   //      for( int j = 0; j < 4; ++j )
   //         {
   //         out << m_Matrix.m[i][j] << " ";
   //         }
   //      }
   //   }

   /*virtual void VDeserialize( std::istrstream& in )
      {
      in >> m_Id;
      for( int i = 0; i < 4; ++i )
         {
         for( int j = 0; j < 4; ++j )
            {
            in >> m_Matrix.m[i][j];
            }
         }
      }*/

   virtual IEventPtr VCopy( ) const override{ return IEventPtr( ENG_NEW EvtData_Move_Actor( m_Id, m_Matrix ) ); }

   ActorId GetId( void ) const { m_Id; }

   const Mat4x4& GetMatrix( void ) const { return m_Matrix; }

   private:
      ActorId m_Id;
      Mat4x4 m_Matrix;
   };

class EvtData_New_Render_Component : public BaseEvent<EvtData_New_Render_Component>
   {

   public:
      EvtData_New_Render_Component( void ) { m_ActorId = INVALID_ACTOR_ID; }

      explicit EvtData_New_Render_Component( ActorId actorId, shared_ptr<SceneNode> pSceneNode ) : m_ActorId(actorId), m_pSceneNode(pSceneNode) { }

      virtual IEventPtr VCopy(void) const override { return IEventPtr( ENG_NEW EvtData_New_Render_Component( m_ActorId, m_pSceneNode ) ); }

    //  virtual const char* GetName(void) const override { return "EvtData_New_Render_Component"; }

      const ActorId GetActorId(void) const { return m_ActorId; }

      shared_ptr<SceneNode> GetSceneNode(void) const { return m_pSceneNode; }

   private:
      ActorId m_ActorId;
      
      shared_ptr<SceneNode> m_pSceneNode;
   
   };