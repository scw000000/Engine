#pragma once
/*!
 * \file Math.h
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

#include <functional>
#include <vector>

/* Period parameters */  
#define CMATH_N 624
#define CMATH_M 397
#define CMATH_MATRIX_A 0x9908b0df   /* constant vector a */
#define CMATH_UPPER_MASK 0x80000000 /* most significant w-r bits */
#define CMATH_LOWER_MASK 0x7fffffff /* least significant r bits */

/* Tempering parameters */   
#define CMATH_TEMPERING_MASK_B 0x9d2c5680
#define CMATH_TEMPERING_MASK_C 0xefc60000
#define CMATH_TEMPERING_SHIFT_U(y)  (y >> 11)
#define CMATH_TEMPERING_SHIFT_S(y)  (y << 7)
#define CMATH_TEMPERING_SHIFT_T(y)  (y << 15)
#define CMATH_TEMPERING_SHIFT_L(y)  (y >> 18)

class ENGRandom
   {
   public:
      ENGRandom(void);	
	   unsigned int	Random( unsigned int n );
	   float			Random( );
	   void			SetRandomSeed(unsigned int n);
	   unsigned int	GetRandomSeed(void);
	   void			Randomize(void);

   private:
      unsigned int      rseed;
      unsigned int      rseed_sp;
      unsigned long     mt[ CMATH_N ]; /* Array for the state vector */
      int mti; /* mti==N+1 means mt[N] is not initialized */
   
   };

extern float Interpolate( float normalizedValue, float begin, float end );
extern Vec3 Interpolate( float normalizedValue, Vec3 begin, Vec3 end );
extern std::vector< std::vector< bool > > Triangulation( const std::vector< Vec2 >& points );