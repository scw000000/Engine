/*!
 * \file Math.cpp
 * \date 2016/06/11 18:33
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
#include "Math.h"


//
// void Interpolate									- Chapter 9, page 258
//
float Interpolate( float normalizedValue, float begin, float end )
   {
	// first check input values
	ENG_ASSERT( normalizedValue >= 0.0f );
	ENG_ASSERT( normalizedValue <= 1.0f );

	return ( normalizedValue * (end - begin) ) + begin;
   }

Vec3 Interpolate( float normalizedValue, Vec3 begin, Vec3 end )
   {
   return Vec3( Interpolate( normalizedValue, begin.x, end.x ), 
                Interpolate( normalizedValue, begin.y, end.y ),
                Interpolate( normalizedValue, begin.z, end.z ) );
   }


//
// void MapYDeadZone								- Chapter 9, page 258
//
void MapYDeadZone( Vec3 &input, float deadZone )
   {
	if (deadZone>=1.0f)
		return;

	// The dead zone is assumed to be zero close to the origin
	// so we have to interpolate to find the right dead zone for
	// our current value of X. ( larger x, larger deadzone )
	float actualDeadZone = Interpolate( fabs( input.x ), 0.0f, deadZone );

	if (fabs( input.y ) < actualDeadZone )
	   {	
		input.y = 0.0f;
		return;
	   }

	// Y is outside of the dead zone, but we still need to 
	// interpolate it so we don't see any popping.

	// Map Y values [actualDeadZone, 1.0f] to [0.0f, 1.0f]
	float normalizedY = ( input.y - actualDeadZone ) / (1.0f - actualDeadZone);
	input.y = normalizedY;
   }

