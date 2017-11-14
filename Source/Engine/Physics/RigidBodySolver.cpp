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

static int const& SEQUENTIAL_IMPULSE_ITERATION_NUM = 4;

void RigidBodySolver::SolveConstraint( std::vector<Manifold>& manifolds, float deltaSeconds )
   {
   // Calc jacobian first
   for(auto& manifold : manifolds)
      {
      CalculateJacobian( manifold );
      }

   // Do sequential impulses
   for( int i = 0; i < SEQUENTIAL_IMPULSE_ITERATION_NUM; ++i )
      {
      for( auto& manifold : manifolds )
         {
         for( int j = 0; j < manifold.m_ContactPointCount; ++j )
            {
            // Compute the corrective impulse, but don¡¦t apply it.
            float lambda = CalculateLambda(manifold, j, deltaSeconds);
            ApplyImpulse(manifold, j, lambda);
            }
         }
      }
   }

void RigidBodySolver::CalculateJacobian( Manifold& manifold )
   {
   for( int i = 0; i < manifold.m_ContactPointCount; ++i )
      {
      auto& contact = manifold.m_ContactPoints[ i ];
      auto& jacobian = contact.m_Jacobian;
      auto& pRBA = manifold.pRigidBodyA;
      jacobian.m_NRACrossN = ( pRBA->m_GlobalCentroid - contact.m_SupportPoint.m_PointA ).Cross( contact.m_Normal );
      auto& pRBB = manifold.pRigidBodyB;
      jacobian.m_RBCrossN = ( contact.m_SupportPoint.m_PointB - pRBB->m_GlobalCentroid ).Cross( contact.m_Normal );
      }
   }

float RigidBodySolver::CalculateLambda( Manifold& manifold, int contactPtIdx, float deltaSeconds )
   {
   auto& contact = manifold.m_ContactPoints[ contactPtIdx ];
   auto& pRBA = manifold.pRigidBodyA;
   auto& pRBB = manifold.pRigidBodyB;

   // numerator = -( JV + b )
   // ENG_LOG( "Test", ToStr( contact.m_Normal) );

   /// ENG_LOG( "Test", ToStr( contact.m_Normal.Dot( contact.m_SupportPoint.m_PointB - contact.m_SupportPoint.m_PointA ) ) );
   float numerator = -( -contact.m_Normal.Dot( pRBA->m_LinearVelocity )
                        + contact.m_Jacobian.m_NRACrossN.Dot( pRBA->m_AngularVelocity )
                        + contact.m_Normal.Dot( pRBB->m_LinearVelocity )
                        + contact.m_Jacobian.m_RBCrossN.Dot( pRBB->m_AngularVelocity )
                        + 0.8f / deltaSeconds * contact.m_Normal.Dot( contact.m_SupportPoint.m_PointB - contact.m_SupportPoint.m_PointA ) );

   // denominator = J M-1 J^t
   Vec3 term1 = -contact.m_Normal * pRBA->m_InverseMass;
   Vec3 term2 = contact.m_Jacobian.m_NRACrossN * pRBA->m_GlobalInverseInertia;
   Vec3 term3 = contact.m_Normal * pRBB->m_InverseMass;
   Vec3 term4 = contact.m_Jacobian.m_RBCrossN * pRBB->m_GlobalInverseInertia;
   float denominator = term1.Dot( -contact.m_Normal ) 
      + term2.Dot( contact.m_Jacobian.m_NRACrossN )
      + term3.Dot( contact.m_Normal )
      + term4.Dot( contact.m_Jacobian.m_RBCrossN );

   return numerator / denominator;
   }

void RigidBodySolver::ApplyImpulse( Manifold& manifold, int contactPtIdx, float lambda )
   {
   auto& contact = manifold.m_ContactPoints[ contactPtIdx ];

   // Make a copy of the old accumulated impulse.
   float prevImpulse = contact.m_AccumulatedImpulse;
   // Add the corrective impulse to the accumulated impulse.
   contact.m_AccumulatedImpulse += lambda;
   // Clamp the accumulated impulse.
   contact.m_AccumulatedImpulse = std::max( 0.f, contact.m_AccumulatedImpulse );

   // Compute the change in the accumulated impulse using the copy from step 2.
   float deltaImpulse = contact.m_AccumulatedImpulse - prevImpulse;

   auto& pRBA = manifold.pRigidBodyA;
   // Apply the impulse delta 
   pRBA->m_LinearVelocity += pRBA->m_InverseMass * -contact.m_Normal * deltaImpulse;
   pRBA->m_AngularVelocity += pRBA->m_GlobalInverseInertia * contact.m_Jacobian.m_NRACrossN * deltaImpulse;

   auto& pRBB = manifold.pRigidBodyB;
   // Apply the impulse delta 
   pRBB->m_LinearVelocity += pRBB->m_InverseMass * contact.m_Normal * deltaImpulse;
   pRBB->m_AngularVelocity += pRBB->m_GlobalInverseInertia * contact.m_Jacobian.m_RBCrossN * deltaImpulse;
   }