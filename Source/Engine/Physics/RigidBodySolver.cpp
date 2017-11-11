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

constexpr int SEQUENTIAL_IMPULSE_ITERATION_NUM = 4;

void RigidBodySolver::SolveConstraint( std::vector<Manifold>& manifolds )
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
            float lambda = CalculateLambda(manifold, j);
            ApplyImpulse(manifold, j, lambda);
            }
         }
      }
   }

void RigidBodySolver::CalculateJacobian( Manifold& manifold )
   {
   for( int i = 0; i < manifold.m_ContactPointCount; ++i )
      {
      auto& jacobian = manifold.m_ContactPoints[ i ].m_Jacobian;

      }
   }

float RigidBodySolver::CalculateLambda( Manifold& manifold, int contactPtIdx )
   {

   return 0.f;
   }

void RigidBodySolver::ApplyImpulse( Manifold& manifold, int contactPtIdx, float lambda )
   {

   }