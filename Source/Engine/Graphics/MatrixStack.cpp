////////////////////////////////////////////////////////////////////////////////
// Filename: MatrixStack.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "MatrixStack.h"

void MatrixStack::Push( void )
   {
   if( m_Stack.size() )
      {
      m_Stack.push( Mat4x4::g_Identity );
      }
   else
      {
      m_Stack.push( m_Stack.top() );
      }
   }
      
void MatrixStack::Pop( void )
   {
   ENG_ASSERT( m_Stack.size() );
   m_Stack.pop();
   }
      
void MatrixStack::MultMatrixLocal( const Mat4x4& matrix )
   {
   ENG_ASSERT( m_Stack.size() );
   m_Stack.top() = matrix * m_Stack.top();
   }

void MatrixStack::PushRelMatrix( const Mat4x4& matrix )
   {
   Push();
   MultMatrixLocal( matrix );
   }





