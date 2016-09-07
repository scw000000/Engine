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
         m_PoiintArrayIdx = arrayIdx;
         m_SortedArrayIdx = 0;
         m_CWHullPoint = NULL;
         m_CCWHullPoint = NULL;
         m_PairHullPoint = NULL;
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
         if( isForward && m_CWHullPoint )
            {
            m_CWHullPoint->m_CCWHullPoint = NULL;
            m_CWHullPoint = NULL;
            }
         else if( !isForward && m_CCWHullPoint )
            {
            m_CCWHullPoint->m_CWHullPoint = NULL;
            m_CCWHullPoint = NULL;
            }
         }

      void LinkHull( DelaunayPoint& other, bool isCWDir = true )
         {
         UnlinkHull( isCWDir );
         other.UnlinkHull( !isCWDir );
         if( isCWDir )
            {
            m_CWHullPoint = &other;
            other.m_CCWHullPoint = this;
            }
         else
            {
            m_CCWHullPoint = &other;
            other.m_CWHullPoint = this;
            }
         }

      void LinkPairHull( DelaunayPoint& other )
         {
         UnlinkPairHull();
         m_PairHullPoint = &other;
         other.m_PairHullPoint = this;
         }

      void UnlinkPairHull( void )
         {
         if( m_PairHullPoint )
            {
            m_PairHullPoint->m_PairHullPoint = NULL;
            }
         m_PairHullPoint = NULL;
         }

      DelaunayPoint* GetAdjacentHullPoint( bool isCWDir )
         {
         if( isCWDir )
            {
            return m_CWHullPoint;
            }
         return m_CCWHullPoint;
         }

   public:
      const Vec2* m_Point;
      unsigned int m_PoiintArrayIdx;
      unsigned int m_SortedArrayIdx;
      DelaunayPoint* m_CWHullPoint; // clock-wise convex hull edge point of this point
      DelaunayPoint* m_CCWHullPoint; // counter clock-wise convex hull edge point of this point
      DelaunayPoint* m_PairHullPoint; // for hull that has only two points

      std::map< DelaunayPoint*, DelaunayPoint* > m_LinkedEdges;
   };

bool IsPointInsideCircle( const DelaunayPoint& circlePA, 
                              const DelaunayPoint& circlePB,
                              const DelaunayPoint& circlePC, 
                              const DelaunayPoint& testPoint )
   {
   return Mat4x4( circlePA.m_Point->x, circlePA.m_Point->y, circlePA.m_Point->x * circlePA.m_Point->x + circlePA.m_Point->y * circlePA.m_Point->y, 1,
                  circlePB.m_Point->x, circlePB.m_Point->y, circlePB.m_Point->x * circlePB.m_Point->x + circlePB.m_Point->y * circlePB.m_Point->y, 1,
                  circlePC.m_Point->x, circlePC.m_Point->y, circlePC.m_Point->x * circlePC.m_Point->x + circlePC.m_Point->y * circlePC.m_Point->y, 1,
                  testPoint.m_Point->x, testPoint.m_Point->y, testPoint.m_Point->x * testPoint.m_Point->x + testPoint.m_Point->y * testPoint.m_Point->y, 1 ).Determinant() <= 0.f;
   }

void Triangulation_Merge( std::vector< DelaunayPoint >& points, int start, int leftBound, int rightBound )
   {
   ENG_ASSERT( points[ leftBound - 1 ].m_CWHullPoint || points[ leftBound - 1 ].m_PairHullPoint );
   ENG_ASSERT( points[ leftBound ].m_CWHullPoint || points[ leftBound ].m_PairHullPoint );

   //find lower common tangent first
   bool isLeftIdxOptimal = false;
   bool isrightIdxOptimal = false;
   int leftCurIdx = leftBound - 1;
   int rightCurIdx = leftBound;
   int rightHullPointCount = rightBound - leftBound;
   int leftHullPointCount = leftBound - start;

   if( rightHullPointCount <= 2 )
      {
      if( rightHullPointCount == 1 )
         {
         rightCurIdx = leftBound;
         isrightIdxOptimal = true;
         }
      else // two points
         {
         rightCurIdx = points[ leftBound ].m_Point->y <= points[ leftBound + 1 ].m_Point->y ? leftBound : leftBound + 1;
         isrightIdxOptimal = true;
         }
      }

   if( leftHullPointCount <= 2 )
      {
      if( leftHullPointCount == 1 )
         {
         leftCurIdx = leftBound - 1;
         isLeftIdxOptimal = true;
         }
      else // two points
         {
         leftCurIdx = points[ leftBound - 1 ].m_Point->y <= points[ leftBound - 2 ].m_Point->y ? leftBound - 1: leftBound -2;
         isLeftIdxOptimal = true;
         }
      }

   while( !isLeftIdxOptimal || !isrightIdxOptimal )
      {
      if( !isrightIdxOptimal )
         {
         if( rightHullPointCount >= 3 ) // nontrivial case, its at least a triangle
            {
            while( !Vec2::IsIntersect( *( points[ leftCurIdx ].m_Point ), 
                                      *( points[ rightCurIdx ].GetAdjacentHullPoint( false )->m_Point ), // its CCW point ( test point )
                                      *( points[ rightCurIdx ].m_Point ),
                                      *( points[ rightCurIdx ].GetAdjacentHullPoint( true ) )->m_Point) ) // its CW point
               {
               auto nxtValidRightHullPoint = ( points[ rightCurIdx ].GetAdjacentHullPoint( false ) );
               rightCurIdx = nxtValidRightHullPoint->m_SortedArrayIdx;
               if( leftHullPointCount >= 3 )
                  {
                  isLeftIdxOptimal = false;
                  }
               }
            
            }
         isrightIdxOptimal = true;
         }
      else
         {
         if( leftHullPointCount >= 3 ) // nontrivial case, its at least a triangle
            {
            while( !Vec2::IsIntersect( *( points[ rightCurIdx ].m_Point ),
               *( points[ leftCurIdx ].GetAdjacentHullPoint( true )->m_Point ), // its CW point ( test point )
               *( points[ leftCurIdx ].m_Point ),
               *( points[ leftCurIdx ].GetAdjacentHullPoint( false ) )->m_Point ) ) // its CCW point
               {
               auto nxtValidLeftHullPoint = ( points[ leftCurIdx ].GetAdjacentHullPoint( true ) );
               leftCurIdx = nxtValidLeftHullPoint->m_SortedArrayIdx;
               if( rightHullPointCount >= 3 )
                  {
                  isrightIdxOptimal = false;
                  }
               }

            }
         isLeftIdxOptimal = true;
         }
      }
   if( rightHullPointCount == 2 )
      {
      auto pTheOtherPoint = points[ rightCurIdx ].m_PairHullPoint;
      points[ rightCurIdx ].UnlinkPairHull();
      points[ rightCurIdx ].LinkHull( *pTheOtherPoint, false );
      }
   if( leftHullPointCount == 2 )
      {
      auto pTheOtherPoint = points[ leftCurIdx ].m_PairHullPoint;
      points[ leftCurIdx ].UnlinkPairHull();
      points[ leftCurIdx ].LinkHull( *pTheOtherPoint, true );
      }

   points[ leftCurIdx ].LinkHull( points[ rightCurIdx ], false );

   // first sort the points
   // next
   std::vector< DelaunayPoint* > rightCandicates;
   std::vector< DelaunayPoint* > leftCandicates;
   bool isRightTurn = true;
   int leftBaseEdgeIdx = leftCurIdx;
   int rightBaseEdgeIdx = rightCurIdx;
   while( true )
      {
      if( isRightTurn )
         {
         for( auto pointIt : points[ rightBaseEdgeIdx ].m_LinkedEdges )
            {
            if( pointIt.first->m_SortedArrayIdx != rightBaseEdgeIdx && !Vec2::IsInClockwiseDirection( *points[ leftBaseEdgeIdx ].m_Point,
                                                                       *points[ rightBaseEdgeIdx ].m_Point, 
                                                                       *( pointIt.first->m_Point ) ) )
               {
               rightCandicates.push_back( &points[ pointIt.first->m_SortedArrayIdx ] );
               }
            }
         if( !rightCandicates.empty() )
            {
            Vec2 vLR = *points[ rightBaseEdgeIdx ].m_Point - *points[ leftBaseEdgeIdx ].m_Point;
            std::sort( rightCandicates.begin(), rightCandicates.end(), [ &] ( const DelaunayPoint*& pA, const DelaunayPoint*& pB )
               {
               return vLR.Dot( *pA->m_Point - *points[ leftBaseEdgeIdx ].m_Point ) > vLR.Dot( *pB->m_Point - *points[ leftBaseEdgeIdx ].m_Point );
               } );

            for( int i = 0; i < rightCandicates.size(); ++i )
               {
               
               }
            }
         }
      else
         {
         }
      isRightTurn != isRightTurn;
      }
   }
//
//typedef struct DelaunayTestResult
//   {
//   public:
//      bool m_
//   }DelaunayTestResult;

void Triangulation_Recursive( std::vector< DelaunayPoint >& points, int start, int rightBound )
   {
   int pointCount = rightBound - start;
   if( pointCount == 3 )
      {
      // stat, start + 1, start + 2 are in clockwise direction
      bool is_Point012_Clockwise = Vec2::IsInClockwiseDirection( *points[ start ].m_Point, *points[ start + 1 ].m_Point, *points[ start + 2 ].m_Point );
      points[ start ].LinkHull( points[ start + 1 ], is_Point012_Clockwise );
      points[ start ].Link( points[ start + 1 ] );
      points[ start + 1 ].LinkHull( points[ start + 2 ], is_Point012_Clockwise );
      points[ start + 1 ].Link( points[ start + 2 ] );
      points[ start + 2 ].LinkHull( points[ start ], is_Point012_Clockwise );
      points[ start + 2 ].Link( points[ start ] );
      return;
      }

   if( pointCount <= 2 )
      {
      if( pointCount == 2 )
         {
         points[ rightBound - 1 ].LinkPairHull( points[ rightBound - 2 ] );
         points[ rightBound - 1 ].Link( points[ rightBound - 2 ] );
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
   for( int i = 0; i < delaunayPoints.size(); ++i )
      {
      delaunayPoints[ i ].m_SortedArrayIdx = i;
      }
   Triangulation_Recursive( delaunayPoints, 0, points.size() );
   return ret;
   }

