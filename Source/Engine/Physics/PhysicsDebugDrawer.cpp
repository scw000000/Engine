/*!
 * \file PhysicsDebugDrawer.cpp
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

#include "EngineStd.h"
#include "PhysicsDebugDrawer.h"
#include "../UserInterface/HumanView.h"
#include "..\Graphics\Renderer\RenderHelper.h"

BulletDebugDrawer::BulletDebugDrawer( void )
   {
   m_DebugModes = btIDebugDraw::DBG_NoDebug;
   }

void BulletDebugDrawer::drawContactPoint( const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color )
   {
   if( ( m_DebugModes & btIDebugDraw::DBG_DrawContactPoints ) == 0 )
      {
      return;
      }
   // draw a line to represent the normal.  This only lasts one frame, and is hard to see.
   // it might help to linger this drawn object on screen for a while to make it more noticeable

   btVector3 const startPoint = PointOnB;
   btVector3 const endPoint = PointOnB + normalOnB * distance;

   drawLine( startPoint, endPoint, color );
   }

void BulletDebugDrawer::reportErrorWarning( const char* warningString )
   {
   ENG_WARNING( warningString );
   }

void BulletDebugDrawer::draw3dText( const btVector3& location, const char* textString )
   {
   // FUTURE WORK - BulletDebugDrawer::draw3dText needs an implementation
   }

void BulletDebugDrawer::setDebugMode( int debugMode )
   {
   m_DebugModes = ( DebugDrawModes ) debugMode;
   }

int BulletDebugDrawer::getDebugMode( ) const
   {
   return m_DebugModes;
   }

void BulletDebugDrawer::ReadOptions( )
   {
   TiXmlDocument *optionsDoc = g_pApp->m_EngineOptions.GetEngineOptionDoc();
   TiXmlElement *pRoot = optionsDoc->RootElement( );
   if( !pRoot )
      return;

   int debugModes = btIDebugDraw::DBG_NoDebug;
   TiXmlElement *pNode = pRoot->FirstChildElement( "PhysicsDebug" );
   if( pNode )
      {
      if( pNode->Attribute( "DrawWireFrame" ) )
         {
         std::string attribute( pNode->Attribute( "DrawWireFrame" ) );
         if( attribute == "yes" ) debugModes |= btIDebugDraw::DBG_DrawWireframe;
         }

      if( pNode->Attribute( "DrawAabb" ) )
         {
         std::string attribute( pNode->Attribute( "DrawAabb" ) );
         if( attribute == "yes" ) debugModes |= btIDebugDraw::DBG_DrawAabb;
         }

      if( pNode->Attribute( "DrawFeaturesText" ) )
         {
         std::string attribute( pNode->Attribute( "DrawFeaturesText" ) );
         if( attribute == "yes" ) debugModes |= btIDebugDraw::DBG_DrawFeaturesText;
         }

      if( pNode->Attribute( "DrawContactPoints" ) )
         {
         std::string attribute( pNode->Attribute( "DrawContactPoints" ) );
         if( attribute == "yes" ) debugModes |= btIDebugDraw::DBG_DrawContactPoints;
         }

      if( pNode->Attribute( "NoDeactivation" ) )
         {
         std::string attribute( pNode->Attribute( "NoDeactivation" ) );
         if( attribute == "yes" ) debugModes |= btIDebugDraw::DBG_NoDeactivation;
         }

      if( pNode->Attribute( "NoHelpText" ) )
         {
         std::string attribute( pNode->Attribute( "NoHelpText" ) );
         if( attribute == "yes" ) debugModes |= btIDebugDraw::DBG_NoHelpText;
         }

      if( pNode->Attribute( "DrawText" ) )
         {
         std::string attribute( pNode->Attribute( "DrawText" ) );
         if( attribute == "yes" ) debugModes |= btIDebugDraw::DBG_DrawText;
         }

      if( pNode->Attribute( "ProfileTimings" ) )
         {
         std::string attribute( pNode->Attribute( "ProfileTimings" ) );
         if( attribute == "yes" ) debugModes |= btIDebugDraw::DBG_ProfileTimings;
         }

      if( pNode->Attribute( "EnableSatComparison" ) )
         {
         std::string attribute( pNode->Attribute( "EnableSatComparison" ) );
         if( attribute == "yes" ) debugModes |= btIDebugDraw::DBG_EnableSatComparison;
         }

      if( pNode->Attribute( "DisableBulletLCP" ) )
         {
         std::string attribute( pNode->Attribute( "DisableBulletLCP" ) );
         if( attribute == "yes" ) debugModes |= btIDebugDraw::DBG_DisableBulletLCP;
         }

      if( pNode->Attribute( "EnableCCD" ) )
         {
         std::string attribute( pNode->Attribute( "EnableCCD" ) );
         if( attribute == "yes" ) debugModes |= btIDebugDraw::DBG_EnableCCD;
         }

      if( pNode->Attribute( "DrawConstraints" ) )
         {
         std::string attribute( pNode->Attribute( "DrawConstraints" ) );
         if( attribute == "yes" ) debugModes |= btIDebugDraw::DBG_DrawConstraints;
         }

      if( pNode->Attribute( "DrawConstraintLimits" ) )
         {
         std::string attribute( pNode->Attribute( "DrawConstraintLimits" ) );
         if( attribute == "yes" ) debugModes |= btIDebugDraw::DBG_DrawConstraintLimits;
         }

      if( pNode->Attribute( "FastWireframe" ) )
         {
         std::string attribute( pNode->Attribute( "FastWireframe" ) );
         if( attribute == "yes" ) debugModes |= btIDebugDraw::DBG_FastWireframe;
         }

      if( debugModes != btIDebugDraw::DBG_NoDebug )
         {
         setDebugMode( debugModes );
         }
      }

   }

void BulletDebugDrawer::drawLine( const btVector3& from, const btVector3& to, const btVector3& lineColor )
   {
   if( ( m_DebugModes & btIDebugDraw::DBG_DrawWireframe ) == 0 )
      {
      return;
      }

   Vec3 vec3From( from.x(), from.y(), from.z() );
   Vec3 vec3To( to.x(), to.y(), to.z() );
   Color color( lineColor.x( ), lineColor.y( ), lineColor.z( ) );

   shared_ptr<Scene> pScene = g_pApp->m_pEngineLogic->m_pWrold;
   OpenglRenderHelper::DrawLine( vec3From, vec3To, color );
   }



