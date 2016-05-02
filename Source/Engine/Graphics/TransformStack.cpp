////////////////////////////////////////////////////////////////////////////////
// Filename: TransformStack.cpp
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "TransformStack.h"

TransformStack::TransformStack( void )
   {
   isMatrixDirty = true;
   }

void TransformStack::Push( const Transform& transform )
   {
   PushCurTransform();
   m_Stack.top() = m_Stack.top() * transform;
   isMatrixDirty = true;
   }

void TransformStack::Pop( void )
   {
   ENG_ASSERT( m_Stack.size() );
   m_Stack.pop();
   isMatrixDirty = true;
   }

const Mat4x4& TransformStack::GetTop( void )
   {
   if( isMatrixDirty )
      {
      this->UpdateTopMatrix();
      }
   return m_TopMatrix; 
   }

void TransformStack::PushCurTransform( void )
   {
   if( !m_Stack.size() )
      {
      m_Stack.push( Transform::g_Identity );
      }
   else
      {
      m_Stack.push( m_Stack.top() );
      }
   }

void TransformStack::UpdateTopMatrix( void )
   {
   m_TopMatrix = m_Stack.top().GetToWorld();
   isMatrixDirty = false;
   }






