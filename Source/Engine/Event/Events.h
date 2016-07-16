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
      virtual EventType VGetEventType( void ) const = 0;
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

      virtual EventType VGetEventType( void ) const override { return s_EventType; };

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

class Event_Destroy_Actor : public BaseEvent<Event_Destroy_Actor>
   {
   public:
      explicit Event_Destroy_Actor( ActorId id ) : m_Id( id ) {  }
      virtual IEventPtr VCopy( void ) const { return IEventPtr( ENG_NEW Event_Destroy_Actor( m_Id ) ); }
      //virtual void VSerialize( std::ostre )
     // virtual const char* GetName( void ) const { return "EvtDat_Destyot_Actor"; } 
      ActorId GetId( void ) const { return m_Id; }

   private:
      ActorId m_Id;
   };

class Event_Move_Actor : public BaseEvent<Event_Move_Actor>
   {


public:

   Event_Move_Actor( void ) { m_Id = INVALID_ACTOR_ID; }

   Event_Move_Actor( ActorId id, const Mat4x4& matrix ) : m_Id( id ), m_Matrix( matrix ){ }

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

   virtual IEventPtr VCopy( ) const override{ return IEventPtr( ENG_NEW Event_Move_Actor( m_Id, m_Matrix ) ); }

   ActorId GetId( void ) const { m_Id; }

   const Mat4x4& GetMatrix( void ) const { return m_Matrix; }

   private:
      ActorId m_Id;
      Mat4x4 m_Matrix;
   };

class Event_New_Render_Component_Root : public BaseEvent<Event_New_Render_Component_Root>
   {

   public:
      Event_New_Render_Component_Root( void ) { m_ActorId = INVALID_ACTOR_ID; }

      explicit Event_New_Render_Component_Root( ActorId actorId, weak_ptr<SceneNode> pSceneNode ) : m_ActorId( actorId ), m_pSceneNode( pSceneNode ) {}

      virtual IEventPtr VCopy(void) const override { return IEventPtr( ENG_NEW Event_New_Render_Component_Root( m_ActorId, m_pSceneNode ) ); }

    //  virtual const char* GetName(void) const override { return "EvtData_New_Render_Component"; }

      const ActorId GetActorId(void) const { return m_ActorId; }

      weak_ptr<SceneNode> GetSceneNode( void ) const { return m_pSceneNode; }

   private:
      ActorId m_ActorId;
      
      weak_ptr<SceneNode> m_pSceneNode;
   
   };

class Event_New_Scene_Node : public BaseEvent < Event_New_Scene_Node >
   {

   public:
      Event_New_Scene_Node( void ) {  }

      explicit Event_New_Scene_Node( weak_ptr<SceneNode> pSceneNode ) : m_pSceneNode( pSceneNode ) {}

      virtual IEventPtr VCopy( void ) const override { return IEventPtr( ENG_NEW Event_New_Scene_Node( m_pSceneNode ) ); }

      weak_ptr<SceneNode> GetSceneNode( void ) const { return m_pSceneNode; }

   private:
      weak_ptr<SceneNode> m_pSceneNode;

   };

class Event_Destory_Render_Component : public BaseEvent < Event_Destory_Render_Component >
   {

   public:
      Event_Destory_Render_Component( void ) {}

      explicit Event_Destory_Render_Component( StrongRenderComponentPtr pRenderComp ) : m_pRenderComp( pRenderComp ) {}

      virtual IEventPtr VCopy( void ) const override { return IEventPtr( ENG_NEW Event_Destory_Render_Component( m_pRenderComp ) ); }

      StrongRenderComponentPtr GetStrongRenderComp( void ) const { return m_pRenderComp; }

   private:
      StrongRenderComponentPtr m_pRenderComp;

   };

class Event_Destroy_Scene_Node : public BaseEvent < Event_Destroy_Scene_Node >
   {

   public:
      Event_Destroy_Scene_Node( void ) {}

      explicit Event_Destroy_Scene_Node( weak_ptr< ISceneNode > pSceneNode ) : m_pSceneNode( pSceneNode ) {}

      virtual IEventPtr VCopy( void ) const override { return IEventPtr( ENG_NEW Event_Destroy_Scene_Node( m_pSceneNode ) ); }

      weak_ptr< ISceneNode > GetSceneNodePtr( void ) const { return m_pSceneNode; }

   private:
      weak_ptr< ISceneNode > m_pSceneNode;

   };