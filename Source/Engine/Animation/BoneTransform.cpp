/*!
 * \file BoneTransform.cpp
 * \date 2016/07/30 22:17
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
#include "BoneTransform.h"


BoneTransform BoneTransform::Interpolate( const BoneTransform& other, float factor )
   {
   float invFactor = 1 - factor;
   aiQuaternion ret;
   aiQuaternion::Interpolate( ret, m_Rotation, other.m_Rotation, factor );
   ret.Normalize();
   return BoneTransform( invFactor * m_Scale + factor * other.m_Scale, ret, invFactor * m_Translation + factor * other.m_Translation );
   }