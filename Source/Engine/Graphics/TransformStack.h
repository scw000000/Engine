#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: TransformStack.h
////////////////////////////////////////////////////////////////////////////////

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
      Mat4x4 GetTop();

   private:
      void PushCurTransform( void );
      const Mat4x4& GetTop( void );

   private:
      std::stack<Transform> m_Stack;
      Mat4x4 m_TopMatrix;
      bool isMatrixDirty;
   };