/*!
 * \file RigidBodySolver.cpp
 * \date 2017/11/11 10:14
 *
 * \author scw00
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#include "EngineStd.h"
#include "RigidBodySolver.h"
#include "RigidBody.h"

#define SEQUENTIAL_IMPULSE_ITERATION_NUM 4
#define WARM_STARGING_RATIO 0.8f

void RigidBodySolver::SolveConstraint( std::vector<Manifold>& manifolds, float deltaSeconds )
   {
   // Do sequential impulses
   for( int i = 0; i < SEQUENTIAL_IMPULSE_ITERATION_NUM; ++i )
      {
      for( auto& manifold : manifolds )
         {
         for( int j = 0; j < manifold.m_ContactPointCount; ++j )
            {
            if(i == 0)
               {
               manifold.m_ContactPoints[ j ].m_AccumulatedImpulseN *= WARM_STARGING_RATIO;
               manifold.m_ContactPoints[ j ].m_AccumulatedImpulseT *= WARM_STARGING_RATIO;
               manifold.m_ContactPoints[ j ].m_AccumulatedImpulseBT *= WARM_STARGING_RATIO;
               }
            // Compute the corrective impulse, but don¡¦t apply it.
            float lambda = CalculateLambda(manifold, j, deltaSeconds, 0);
            ApplyImpulse(manifold, j, lambda, 0);
            float lambdaT = CalculateLambda( manifold, j, deltaSeconds, 1 );
            ApplyImpulse( manifold, j, lambdaT, 1 );
            float lambdaBT = CalculateLambda( manifold, j, deltaSeconds, 2 );
            ApplyImpulse( manifold, j, lambdaBT, 2 );
            }
         }
      }
   }

float RigidBodySolver::CalculateLambda( Manifold& manifold, int contactPtIdx, float deltaSeconds, int axisIdx )
   {
   auto& contact = manifold.m_ContactPoints[ contactPtIdx ];
   auto& pRBA = manifold.m_pRigidBodyA;
   auto& pRBB = manifold.m_pRigidBodyB;
   const Vec3& axis = ( axisIdx == 0 ? contact.m_Normal : ( axisIdx == 1 ? contact.m_Tangent : contact.m_Bitangent ) );
   const Vec3& nRACrossAxis = ( axisIdx == 0 ? contact.m_NRACrossN : ( axisIdx == 1 ? contact.m_NRACrossT : contact.m_NRACrossBT ) );
   const Vec3& rBCrossAxis = ( axisIdx == 0 ? contact.m_RBCrossN : ( axisIdx == 1 ? contact.m_RBCrossT : contact.m_RBCrossBT ) );
   // numerator = -( JV + b )
   // ENG_LOG( "Test", ToStr( contact.m_Normal) );
   /* float Cr = manifold.m_CombinedRestitution * contact.m_Normal.Dot( -pRBA->m_LinearVelocity - pRBA->m_AngularVelocity.Cross( contact.m_SupportPoint.m_PointA - pRBA->m_GlobalCentroid )
                                                                      + pRBB->m_LinearVelocity + pRBB->m_AngularVelocity.Cross( contact.m_SupportPoint.m_PointB - pRBB->m_GlobalCentroid ) );
                                                                      ENG_LOG("Test", ToStr(Cr));*/
   /// ENG_LOG( "Test", ToStr( contact.m_Normal.Dot( contact.m_SupportPoint.m_PointB - contact.m_SupportPoint.m_PointA ) ) );
   // ENG_LOG( "Test", ToStr( 0.2f / deltaSeconds * contact.m_Normal.Dot( contact.m_SupportPoint.m_PointB - contact.m_SupportPoint.m_PointA ) ) );
   float numerator = -( -axis.Dot( pRBA->m_LinearVelocity )
                        + nRACrossAxis.Dot( pRBA->m_AngularVelocity )
                        + axis.Dot( pRBB->m_LinearVelocity )
                        + rBCrossAxis.Dot( pRBB->m_AngularVelocity )
                        );
                       // - 0.1f / deltaSeconds * contact.m_PenetrationDepth );
   if(axisIdx == 0)
      {
      numerator -= ( manifold.m_CombinedRestitution * axis.Dot( -pRBA->m_LinearVelocity - pRBA->m_AngularVelocity.Cross( contact.m_RA )
                     + pRBB->m_LinearVelocity + pRBB->m_AngularVelocity.Cross( contact.m_RB ) )
                     + 0.2f / deltaSeconds * axis.Dot( contact.m_PointBWS - contact.m_PointAWS ) );
      }
   // denominator = J M-1 J^t
   Vec3 term1 = -axis * pRBA->m_InverseMass;
   Vec3 term2 = nRACrossAxis * pRBA->m_GlobalInverseInertia;
   Vec3 term3 = axis * pRBB->m_InverseMass;
   Vec3 term4 = rBCrossAxis * pRBB->m_GlobalInverseInertia;
   float denominator = term1.Dot( -axis )
      + term2.Dot( nRACrossAxis )
      + term3.Dot( axis )
      + term4.Dot( rBCrossAxis );

   return numerator / denominator;
   }

void RigidBodySolver::ApplyImpulse( Manifold& manifold, int contactPtIdx, float lambda, int axisIdx )
   {
   auto& contact = manifold.m_ContactPoints[ contactPtIdx ];
   const Vec3& axis = ( axisIdx == 0 ? contact.m_Normal : ( axisIdx == 1 ? contact.m_Tangent : contact.m_Bitangent ) );
   const Vec3& nRACrossAxis = ( axisIdx == 0 ? contact.m_NRACrossN : ( axisIdx == 1 ? contact.m_NRACrossT : contact.m_NRACrossBT ) );
   const Vec3& rBCrossAxis = ( axisIdx == 0 ? contact.m_RBCrossN : ( axisIdx == 1 ? contact.m_RBCrossT : contact.m_RBCrossBT ) );
   float& accumulatedImpulse = ( axisIdx == 0 ? contact.m_AccumulatedImpulseN : ( axisIdx == 1 ? contact.m_AccumulatedImpulseT : contact.m_AccumulatedImpulseBT ) );

   // Make a copy of the old accumulated impulse.
   float prevImpulse = accumulatedImpulse;
   // Add the corrective impulse to the accumulated impulse.
   accumulatedImpulse += lambda;
   // Clamp the accumulated impulse.
   if(axisIdx == 0)
      {
      accumulatedImpulse = std::max( 0.f, accumulatedImpulse );
      }
   else
      {
     // accumulatedImpulse += lambda * 3.f;
      float bound = manifold.m_CombinedFriction * contact.m_AccumulatedImpulseN;
      accumulatedImpulse = std::max( -bound, std::min( bound, accumulatedImpulse ) );
      }
   
   // Compute the change in the accumulated impulse using the copy from step 2.
   float deltaImpulse = accumulatedImpulse - prevImpulse;

   auto& pRBA = manifold.m_pRigidBodyA;
   // Apply the impulse delta 
   pRBA->m_LinearVelocity += pRBA->m_InverseMass * -axis * deltaImpulse;
   pRBA->m_AngularVelocity += ( pRBA->m_GlobalInverseInertia * nRACrossAxis ) * deltaImpulse;

   auto& pRBB = manifold.m_pRigidBodyB;
   // Apply the impulse delta 
   pRBB->m_LinearVelocity += pRBB->m_InverseMass * axis * deltaImpulse;
   pRBB->m_AngularVelocity += pRBB->m_GlobalInverseInertia * rBCrossAxis * deltaImpulse;

   //float numerator = ( -contact.m_Normal.Dot( pRBA->m_LinearVelocity )
   //                     + contact.m_Jacobian.m_NRACrossN.Dot( pRBA->m_AngularVelocity )
   //                     + contact.m_Normal.Dot( pRBB->m_LinearVelocity )
   //                     + contact.m_Jacobian.m_RBCrossN.Dot( pRBB->m_AngularVelocity )
   //                     // + 0.8f / deltaSeconds * contact.m_Normal.Dot( contact.m_SupportPoint.m_PointB - contact.m_SupportPoint.m_PointA ) );
   //                     );
   //float n1 = -contact.m_Normal.Dot( pRBA->m_LinearVelocity );
   //float n2 = contact.m_Jacobian.m_NRACrossN.Dot( pRBA->m_AngularVelocity );
   //float n3 = contact.m_Normal.Dot( pRBB->m_LinearVelocity );
   //float n4 = contact.m_Jacobian.m_RBCrossN.Dot( pRBB->m_AngularVelocity );
   }