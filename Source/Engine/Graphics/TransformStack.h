#pragma once
/*!
 * \file TransformStack.h
 * \date 2016/06/08 15:10
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

#include <stack>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class TransformStack
   {
   public:
      TransformStack( void );
      void Push( const Transform& transform );
      void Pop( void );
      Mat4x4 GetTopMatrix( void );
      Transform GetTopTransForm( void ) const;

   private:
      void PushCurTransform( void );
      void UpdateTopMatrix( void );

   private:
      std::stack<Transform> m_Stack;
      Mat4x4 m_TopMatrix;
      bool isMatrixDirty;
   };