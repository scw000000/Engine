#pragma once
/*!
 * \file RigidBodySolver.h
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
#include "CollisionDetector.h"

class RigidBodySolver
   {
   public:
      RigidBodySolver() {};

      void SolveConstraint(std::vector<Manifold>& manifolds, float deltaSeconds);

      private:
      void CalculateJacobian(Manifold& manifold);
      float CalculateLambda( Manifold& manifold, int contactPtIdx, float deltaSeconds );
      void ApplyImpulse( Manifold& manifold, int contactPtIdx, float lambda );
      
   };