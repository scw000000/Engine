#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: MatrixStack.h
////////////////////////////////////////////////////////////////////////////////

#include <stack>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class MatrixStack
   {
   public:
      void Push( void );
      void Pop( void );
      Mat4x4 GetTop() const { return m_Stack.top(); }
      void MultMatrixLocal( const Mat4x4& matrix );
      void PushRelMatrix( const Mat4x4& matrix );
   private:
      std::stack<Mat4x4> m_Stack;
   };