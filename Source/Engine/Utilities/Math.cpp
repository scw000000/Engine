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

//bool myfunction( Vec2& i, V j ) { return ( i<j ); }

class DelaunayPoint
   {
   public:
   DelaunayPoint( const Vec2* point, unsigned int arrayIdx )
         {
         m_Point = point;
         m_ArrayIdx = arrayIdx;
         m_NextHullPoint = NULL;
         m_PrevHullPoint = NULL;
         }

  /* DELAUNAYPOINT( const DELAUNAYPOINT& target )
      {

      }*/
         
      void Link( DelaunayPoint& other )
         {
         if( other.m_LinkedEdges.find( this ) != other.m_LinkedEdges.end() )
            {
            return;
            }
         other.m_LinkedEdges[ this ] = this;
         m_LinkedEdges[ &other ] = &other;
         }

      void UnlinkHull( bool isForward )
         {
         if( isForward && m_NextHullPoint )
            {
            m_NextHullPoint->m_PrevHullPoint = NULL;
            m_NextHullPoint = NULL;
            }
         else if( !isForward && m_PrevHullPoint )
            {
            m_PrevHullPoint->m_NextHullPoint = NULL;
            m_PrevHullPoint = NULL;
            }
         }

      void LinkHull( DelaunayPoint& other, bool isForward = true )
         {
         UnlinkHull( isForward );
         other.UnlinkHull( !isForward );
         if( isForward )
            {
            m_NextHullPoint = &other;
            other.m_PrevHullPoint = this;
            }
         else
            {
            m_PrevHullPoint = &other;
            other.m_NextHullPoint = this;
            }
         }

      DelaunayPoint* GetAdjacentHullPoint( bool isNextDir )
         {
         if( isNextDir )
            {
            return m_NextHullPoint;
            }
         return m_PrevHullPoint;
         }

   public:
      const Vec2* m_Point;
      unsigned int m_ArrayIdx;
      DelaunayPoint* m_NextHullPoint; // clock-wise convex hull edge point of this point
      DelaunayPoint* m_PrevHullPoint; // counter clock-wise convex hull edge point of this point
      std::map< DelaunayPoint*, DelaunayPoint* > m_LinkedEdges;
   };

void Triangulation_Merge( std::vector< DelaunayPoint >& points, int start, int leftBound, int rightBound )
   {
   ENG_ASSERT( points[ leftBound - 1 ].m_NextHullPoint || points[ leftBound - 1 ].m_NextHullPoint );
   ENG_ASSERT( points[ leftBound ].m_NextHullPoint || points[ leftBound ].m_NextHullPoint );

   //find lower common tangent first
   int leftPrevIdx = -1;
   int rightPrevIdx = -1;
   bool isLeftIdxOptimal = false;
   bool isrightIdxOptimal = false;
   int leftCurIdx = leftBound - 1;
   int rightCurIdx = leftBound;
   bool firstRound = true;
   bool isRightHullCCWNext = true;
   bool leftHullCWDir = true;
   int rightHullPointCount = rightBound - leftBound;

   if( rightHullPointCount > 2 )
      {
      Vec3 crossProduct = Vec3( *points[ leftBound ].m_Point - *points[ leftBound ].m_Point ).Cross(
         *points[ leftBound ].m_NextHullPoint->m_Point - *points[ leftBound ].m_Point
         );
      isRightHullCCWNext = crossProduct.z > 0.f ? true: false;
      crossProduct = Vec3( *points[ leftBound + 1 ].m_Point - *points[ leftBound ].m_Point ).Cross(
         *points[ leftBound ].m_NextHullPoint->m_Point - *points[ leftBound ].m_Point
         );
      }

   while( !isLeftIdxOptimal || !isrightIdxOptimal )
      {
      if( isrightIdxOptimal )
         {
         firstRound = false;
         
         if( rightHullPointCount > 2 ) // nontrivial case, its at least a triangle
            {
            
            while( true )
               {
               
               }
            }
         else // trivial case, its a point or a line segment
            {
            if( rightHullPointCount == 1 )
               {
               rightCurIdx = leftBound;
               }
            else
               {
               rightCurIdx = points[ leftBound ].m_Point->y <= points[ leftBound + 1 ].m_Point->y ? leftBound : leftBound + 1;
               }
            }
         isrightIdxOptimal = true;
         }
      else
         {
         //auto currLowIdx = findNextRightLowPoint( curRIdx, rBound );
         }
      }

   //auto findNextLeftLowPoint = [ &] ( int rStart, int lEndBound ) -> int
   //   {
   //   int validIdx = rStart;
   //   for( int i = rStart; i > lEndBound; --i )
   //      {
   //      if( points[ index[ i ] ].x == points[ index[ validIdx ] ].x )
   //         {
   //         validIdx = i;
   //         }
   //      else
   //         {
   //         return validIdx;
   //         }
   //      }
   //   return validIdx;
   //   };

   //auto findNextRightLowPoint = [ &] ( int lStart, int rEndBound ) -> int
   //   {
   //   int validIdx = lStart;
   //   for( int i = lStart; i < rEndBound; ++i )
   //      {
   //      if( points[ index[ i ] ].x == points[ index[ validIdx ] ].x )
   //         {
   //         validIdx = i;
   //         }
   //      else
   //         {
   //         return validIdx;
   //         }
   //      }
   //   return validIdx;
   //   };

   //auto isIntersect = [ & ] ( Vec2 lineIdx1, Vec2 lineIdx2 ) -> bool 
   //   {

   //   return true;
   //   };
   

   //for( int l = lBound - 1; l >= start; --l )
   //   {
   //   }
   //find first pair of common tangent
   ///*float leftMaxXValue = points[ index[ lBound - 1 ] ].x;
   //auto findLeftRightMost = [ & ] ( int x )
   //{
   //if(  )
   //{

   //}
   //};
   //std::find_e*/
   }

void Triangulation_Recursive( std::vector< DelaunayPoint >& points, int start, int rightBound )
   {
   if( rightBound - start <= 3 )
      {
      for( int i = start; i < rightBound - 1; ++i )
         {
         points[ i ].LinkHull( points[ i + 1 ] );
         }
      if( rightBound - start == 3 )
         {
         points[ rightBound - 1 ].LinkHull( points[ start ] );
         }
      return;
      }
   int lBound = ( rightBound - start ) / 2 + 1;
   Triangulation_Recursive( points, start, lBound );
   Triangulation_Recursive( points, lBound, rightBound );
   Triangulation_Merge( points, start, lBound, rightBound );
   }

std::vector< std::vector< bool > > Triangulation( const std::vector< Vec2 >& points )
   {
   std::vector< std::vector< bool > > ret( points.size(), std::vector< bool >( points.size(), false ) );
   std::vector< DelaunayPoint > delaunayPoints;
   delaunayPoints.reserve( points.size() );
   for( unsigned int i = 0; i < points.size(); ++i )
      {
      delaunayPoints.push_back( DelaunayPoint( &points[ i ], i ) );
      }
   //sort index by x value first
   std::sort( delaunayPoints.begin(), delaunayPoints.end(),
              [ &] ( DelaunayPoint& a, DelaunayPoint& b )
               {
               if( a.m_Point->x < b.m_Point->x )
                  {
                  return true;
                  }
               else if( a.m_Point->x == b.m_Point->x )
                  {
                  return a.m_Point->y < b.m_Point->y;
                  }
               return false;
      } );

   Triangulation_Recursive( delaunayPoints, 0, points.size() );
   return ret;
   }

