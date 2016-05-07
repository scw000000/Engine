#pragma once

/*!
 * \file PhysicsEvents.h
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

#include "Events.h"
#include "ScriptEvent.h"

class EvtData_PhysTrigger_Enter : public BaseEventData<EvtData_PhysTrigger_Enter>
   {
   public:
      EvtData_PhysTrigger_Enter( void )
         {
         m_TriggerID = -1;
         m_Other = INVALID_ACTOR_ID;
         }

      explicit EvtData_PhysTrigger_Enter( int triggerID, ActorId other )
         : m_TriggerID( triggerID ),
         m_Other( other )
         {
         }

      IEventDataPtr VCopy( void ) const { return IEventDataPtr( ENG_NEW EvtData_PhysTrigger_Enter( m_TriggerID, m_Other ) ); }

      int GetTriggerId( void ) const { return m_TriggerID; }

      ActorId GetOtherActor( void ) const { return m_Other; }

   private:
      int m_TriggerID;
      ActorId m_Other;
   };

class EvtData_PhysTrigger_Leave : public BaseEventData<EvtData_PhysTrigger_Leave>
   {
   public:
      EvtData_PhysTrigger_Leave( void )
         {
         m_TriggerID = -1;
         m_Other = INVALID_ACTOR_ID;
         }

      explicit EvtData_PhysTrigger_Leave( int triggerID, ActorId other )
         : m_TriggerID( triggerID ),
         m_Other( other )
         {
         }

      virtual IEventDataPtr VCopy( ) const { return IEventDataPtr( ENG_NEW EvtData_PhysTrigger_Leave( m_TriggerID, m_Other ) ); }

      int GetTriggerId( void ) const { return m_TriggerID; }

      ActorId GetOtherActor( void ) const { return m_Other; }

   private:
      int m_TriggerID;
      ActorId m_Other;
   };

class EvtData_PhysCollision : public ScriptEvent
   {
   public:
      EvtData_PhysCollision( )
         {
         m_ActorA = INVALID_ACTOR_ID;
         m_ActorB = INVALID_ACTOR_ID;
         m_SumNormalForce = Vec3( 0.0f, 0.0f, 0.0f );
         m_SumFrictionForce = Vec3( 0.0f, 0.0f, 0.0f );
         }

      explicit EvtData_PhysCollision( ActorId actorA,
                                   ActorId actorB,
                                   Vec3 sumNormalForce,
                                   Vec3 sumFrictionForce,
                                   Vec3List collisionPoints )
                                   : m_ActorA( actorA ),
                                   m_ActorB( actorB ),
                                   m_SumNormalForce( sumNormalForce ),
                                   m_SumFrictionForce( sumFrictionForce ),
                                   m_CollisionPoints( collisionPoints )
         {
         }

      virtual IEventDataPtr VCopy( ) const { return IEventDataPtr( ENG_NEW EvtData_PhysCollision( m_ActorA, m_ActorB, m_SumNormalForce, m_SumFrictionForce, m_CollisionPoints ) ); }

      ActorId GetActorA( void ) const { return m_ActorA; }

      ActorId GetActorB( void ) const { return m_ActorB; }

      const Vec3& GetSumNormalForce( void ) const { return m_SumNormalForce; }

      const Vec3& GetSumFrictionForce( void ) const { return m_SumFrictionForce; }

      const Vec3List& GetCollisionPoints( void ) const { return m_CollisionPoints; }

      virtual void VBuildEventData( void ); 

      EXPORT_FOR_SCRIPT_EVENT( EvtData_PhysCollision );
   
   private:
      ActorId m_ActorA;
      ActorId m_ActorB;
      Vec3 m_SumNormalForce;
      Vec3 m_SumFrictionForce;
      Vec3List m_CollisionPoints;
   };


class EvtData_PhysSeparation : public BaseEventData<EvtData_PhysSeparation> 
   {
   public:
      EvtData_PhysSeparation( )
      {
      m_ActorA = INVALID_ACTOR_ID;
      m_ActorB = INVALID_ACTOR_ID;
      }

      explicit EvtData_PhysSeparation( ActorId actorA, ActorId actorB )
         : m_ActorA( actorA )
         , m_ActorB( actorB )
         {
         }

      virtual IEventDataPtr VCopy( ) const { return IEventDataPtr( ENG_NEW EvtData_PhysSeparation( m_ActorA, m_ActorB ) ); }

      ActorId GetActorA( void ) const { return m_ActorA; }

      ActorId GetActorB( void ) const { return m_ActorB; }
   
   private:
      ActorId m_ActorA;
      ActorId m_ActorB;
   };
