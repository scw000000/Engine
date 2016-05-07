#pragma once

/*!
 * \file PhysicsDebugDrawer.h
 * \date 2016/05/07 11:32
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


#include "btBulletDynamicsCommon.h"

class BulletDebugDrawer : public btIDebugDraw 
   {
   public:
      // btIDebugDraw interface
      virtual void	drawContactPoint( const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color ) override;
      virtual void	reportErrorWarning( const char* warningString ) override;
      virtual void	draw3dText( const btVector3& location, const char* textString ) override;
      virtual void	setDebugMode( int debugMode ) override;
      virtual int		getDebugMode( ) const override;
      virtual void	drawLine( const btVector3& from, const btVector3& to, const btVector3& color ) override;

      
      /**
       * @brief Read and apply physics debugging option in Engine Option xml file
       *
       * @param   void
       * @return void
       */
      void ReadOptions( void );
   
   private:
      DebugDrawModes               m_DebugModes;
   };