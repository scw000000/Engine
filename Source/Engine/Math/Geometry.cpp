/*!
 * \file Geometry.cpp
 * \date 2016/06/12 14:45
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
#include "Geometry.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
const Vec2 Vec2::g_Zero( 0.0f, 0.0f );
const Vec2 Vec2::g_Identity( 1.0f, 1.0f );

const Vec3 Vec3::g_Zero( 0.0f, 0.0f, 0.0f );
const Vec3 Vec3::g_Identity( 1.0f, 1.0f, 1.0f );

const Mat3x3 Mat3x3::g_Identity( 1.f );
const Mat3x3 Mat3x3::g_Zero( 0.f );

const Vec4 Vec4::g_ZeroVector( 0.0f, 0.0f, 0.0f, 0.0f );
const Vec4 Vec4::g_ZeroPoint( 0.0f, 0.0f, 0.0f, 1.0f );
const Vec4 Vec4::g_Identity( 1.0f, 1.0f, 1.0f, 1.0f );

const Mat4x4 Mat4x4::g_Identity( glm::mat4( 1.0f ) );

const Quaternion Quaternion::g_Identity( 1.0f, 0.0f, 0.0f, 0.0f );

const Transform Transform::g_Identity( Vec3::g_Zero, Vec3::g_Identity, Quaternion::g_Identity );


bool Vec2::IsIntersect( const Vec2& p0, const Vec2& p1, const Vec2& p2, const Vec2& p3 )
   {
   Vec2 v0 = p1 - p0;
   Vec2 v1 = p3 - p2;

   float s, t;
   s = ( -v0.y * ( p0.x - p2.x ) + v0.x * ( p0.y - p2.y ) ) / ( -v1.x * v0.y + v0.x * v1.y );
   t = ( v1.x * ( p0.y - p2.y ) - v1.y * ( p0.x - p2.x ) ) / ( -v1.x * v0.y + v0.x * v1.y );

   if( s >= 0 && s <= 1 && t >= 0 && t <= 1 )
      {
      //// Collision detected
      //if( i_x != NULL )
      //   *i_x = p0.x + ( t * v0.x );
      //if( i_y != NULL )
      //   *i_y = p0.y + ( t * v0.y );
      return true;
      }

   return false; // No collision
   }

bool Vec2::IsInClockwiseDirection( const Vec2& p0, const Vec2& p1, const Vec2& p2 )
   {
   Vec3 v02( p2 - p0 );
   Vec3 v01( p1 - p0 );
   return Vec3( v02 ).Cross( Vec3( v01 ) ).z >= 0.f;
   }

bool Vec2::Init( TiXmlElement* pData )
   {
   if( !pData )
      {
      return false;
      }
   bool success = ( TIXML_SUCCESS == pData->QueryFloatAttribute( "x", &x ) )
      | ( TIXML_SUCCESS == pData->QueryFloatAttribute( "y", &y ) );

   return success;
   }

TiXmlElement* Vec2::GernerateXML( void ) const
   {
   TiXmlElement* pRetNode = ENG_NEW TiXmlElement( "Vector2" );
   pRetNode->SetAttribute( "x", ToStr( x ).c_str() );
   pRetNode->SetAttribute( "y", ToStr( y ).c_str() );

   return pRetNode;
   }


bool Vec3::Init( TiXmlElement* pData )
   {
   if( !pData )
      {
      return false;
      }
   bool success = ( TIXML_SUCCESS == pData->QueryFloatAttribute( "x", &x ) )
      | ( TIXML_SUCCESS == pData->QueryFloatAttribute( "y", &y ) )
      | ( TIXML_SUCCESS == pData->QueryFloatAttribute( "z", &z ) );

   return success;
   }

TiXmlElement* Vec3::GenerateXML( void ) const
   {
   TiXmlElement* pRetNode = ENG_NEW TiXmlElement( "Vector3" );
   pRetNode->SetAttribute( "x", ToStr( x ).c_str() );
   pRetNode->SetAttribute( "y", ToStr( y ).c_str() );
   pRetNode->SetAttribute( "z", ToStr( z ).c_str() );
   
   return pRetNode;
   }

TiXmlElement* Vec3::GenerateOverridesXML( TiXmlElement* pResource ) const
   {
   TiXmlElement* pRetNode = GenerateXML();
   if( !std::strcmp( pRetNode->Attribute( "x" ), pResource->Attribute( "x" ) ) &&
       !std::strcmp( pRetNode->Attribute( "y" ), pResource->Attribute( "y" ) ) &&
       !std::strcmp( pRetNode->Attribute( "z" ), pResource->Attribute( "z" ) ) )
      {
      pRetNode->RemoveAttribute( "x" );
      pRetNode->RemoveAttribute( "y" );
      pRetNode->RemoveAttribute( "z" );
      }
   return pRetNode;
   }

Mat3x3 Vec3::OuterProduct( const Vec3& other ) const
   {
   return Mat3x3( other.x * ( *this ), other.y * ( *this ), other.z * ( *this ) );
   }

// aiMat is row major order matrix
Mat4x4::Mat4x4( const aiMatrix4x4t<float>& aiMat ) : glm::mat4( aiMat.a1, aiMat.a2, aiMat.a3, aiMat.a4, 
                                                                aiMat.b1, aiMat.b2, aiMat.b3, aiMat.b4,
                                                                aiMat.c1, aiMat.c2, aiMat.c3, aiMat.c4, 
                                                                aiMat.d1, aiMat.d2, aiMat.d3, aiMat.d4 )
   {}

bool Vec4::Init( TiXmlElement* pData )
   {
   if( !pData )
      {
      return false;
      }
   bool success = ( TIXML_SUCCESS == pData->QueryFloatAttribute( "x", &x ) )
      | ( TIXML_SUCCESS == pData->QueryFloatAttribute( "y", &y ) )
      | ( TIXML_SUCCESS == pData->QueryFloatAttribute( "z", &z ) )
      | ( TIXML_SUCCESS == pData->QueryFloatAttribute( "w", &w ) );
      
   return success;
   }

TiXmlElement* Vec4::GernerateXML( void ) const
   {
   TiXmlElement* pRetNode = ENG_NEW TiXmlElement( "Vector4" );
   pRetNode->SetAttribute( "x", ToStr( x ).c_str() );
   pRetNode->SetAttribute( "y", ToStr( y ).c_str() );
   pRetNode->SetAttribute( "z", ToStr( z ).c_str() );
   pRetNode->SetAttribute( "w", ToStr( w ).c_str() );

   return pRetNode;
   }

bool Plane::IsAbove( const Vec3& p ) const
   {
   return ( p.Dot( n ) + d > 0.f );
   }

bool Plane::IsAbove( const Vec3& p, const float radius ) const
   {
   float distance = SignedDistance( p );
   // we need to consider if the point is outside the plane, but its radius may compensate for it
   // -> we should test distance + radius >= 0
   return ( distance >= -radius );
   }


Vec3 Plane::GetProjectPoint( const Vec3& p ) const
   {
   // project point r = p + tn
   // r dot n + d = 0
   // ( p + tn ) dot n + d = 0
   // n.p + t * (n dot n) + d = 0
   // t = -( np + d ) / (n dot n)
   return p -( SignedDistance( p ) ) * n;
   }

float Plane::SignedDistance( const Vec3& p ) const
   {
   return p.Dot( n ) + d;
   }

Color::Color() 
   {
   memset( &m_Array[0], 0, sizeof( ColorComponents ) );
   //r = 0.f;
   //g = 0.f;
   //b = 0.f;
   //a = 0.f;
   }


Color::Color( const Color& color )
   {
   memcpy( this->m_Array, &color, sizeof( ColorComponents ) );
//   r = color.r;
//   g = color.g;
//   b = color.b;
//   a = color.a;
   Satuate();
   }
    
Color::Color( const float red, const float green, const float blue, const float alpha ) // : r( red ), g( green ), b( blue ), a( alpha )
   {
   m_Component.r = red;
   m_Component.g = green;
   m_Component.b = blue;
   m_Component.a = alpha;
  // m_Color[0] = red;
   //m_Color[1] = green;
   //m_Color[2] = blue;
   //m_Color[3] = alpha;
   Satuate();
   }

// assignment operators
Color& Color::operator += ( const Color& color )
   {

   m_Array[0] += color.m_Array[0];
   m_Array[1] += color.m_Array[1];
   m_Array[2] += color.m_Array[2];
   m_Array[3] += color.m_Array[3];
   Satuate();
   return *this;
   }

Color& Color::operator -= ( const Color& color )
   {
   m_Array[0] -= color.m_Array[0];
   m_Array[1] -= color.m_Array[1];
   m_Array[2] -= color.m_Array[2];
   m_Array[3] -= color.m_Array[3];
  // r -= color.r;
   //g -= color.g;
   //b -= color.b;
   //a -= color.a;
   Satuate();
   return *this;
   }

Color& Color::operator *= ( float scale )
   {
   m_Array[0] *= scale;
   m_Array[1] *= scale;
   m_Array[2] *= scale;
   m_Array[3] *= scale;
   //r *= scale;
   //g *= scale;
   //b *= scale;
   //a *= scale;
   Satuate();
   return *this;
   }

Color& Color::operator /= ( float scale )
   {
   m_Array[0] /= scale;
   m_Array[1] /= scale;
   m_Array[2] /= scale;
   m_Array[3] /= scale;
   //r /= scale;
   //g /= scale;
   //b /= scale;
   //a /= scale;
   Satuate();
   return *this;
   }

// unary operators
Color Color::operator + () const
   {
   return *this;
   }

Color Color::operator - () const
   {
   return Color( -m_Array[0], -m_Array[1], -m_Array[2], -m_Array[3] );
   }

// binary operators
Color Color::operator + ( const Color& color ) const
   {
   Color ret( *this );
   return ( ret += color );
   }

Color Color::operator - ( const Color& color) const
   {
   Color ret( *this );
   return ( ret -= color );
   }

Color Color::operator * ( float scale ) const
   {
   return ( Color( *this ) *= scale );
   }

Color Color::operator / ( float scale ) const
   {
   Color ret( *this );
   return ( ret /= scale );
   }

bool Color::operator == ( const Color& color ) const
   {
   return memcmp ( &m_Array[0], &color.m_Array[0], sizeof( ColorComponents ) ) == 0;
  // return ( r == color.r && g == color.g && b ==  color.b && a == color.a );
   }

bool Color::operator != ( const Color& color ) const
   {
   return !( ( *this ) == color );
   }

 void Color::Satuate()
    {
    m_Array[ 0 ] = std::max( 0.f, std::min( m_Array[ 0 ], 1.0f ) );
    m_Array[ 1 ] = std::max( 0.f, std::min( m_Array[ 1 ], 1.0f ) );
    m_Array[ 2 ] = std::max( 0.f, std::min( m_Array[ 2 ], 1.0f ) );
    m_Array[ 3 ] = std::max( 0.f, std::min( m_Array[ 3 ], 1.0f ) );
   // r = min( r, 1.0f );
//    g = glm::min( g, 1.0f );
  //  b = glm::min( b, 1.0f );
    //a = glm::min( a, 1.0f );

    }

 bool Color::Init( TiXmlElement* pData )
    {
    if( !pData )
       {
       return false;
       }
    bool success = ( TIXML_SUCCESS == pData->QueryFloatAttribute( "r", &m_Component.r ) )
       | ( TIXML_SUCCESS == pData->QueryFloatAttribute( "g", &m_Component.g ) )
       | ( TIXML_SUCCESS == pData->QueryFloatAttribute( "b", &m_Component.b ) )
       | ( TIXML_SUCCESS == pData->QueryFloatAttribute( "a", &m_Component.a ) );
    return success;
    }

 TiXmlElement* Color::GenerateXML( void )
    {
    TiXmlElement* pRetNode = ENG_NEW TiXmlElement( "Color" );
    pRetNode->SetAttribute( "r", ToStr( m_Component.r ).c_str() );
    pRetNode->SetAttribute( "g", ToStr( m_Component.g ).c_str() );
    pRetNode->SetAttribute( "b", ToStr( m_Component.b ).c_str() );
    pRetNode->SetAttribute( "a", ToStr( m_Component.a ).c_str() );
    
    return pRetNode;
    }

TiXmlElement* Color::GenerateOverridesXML( TiXmlElement* pResource )
    {
    TiXmlElement* pRetNode = GenerateXML();
    if( !std::strcmp( pRetNode->Attribute( "r" ), pResource->Attribute( "r" ) ) &&
        !std::strcmp( pRetNode->Attribute( "g" ), pResource->Attribute( "g" ) ) &&
        !std::strcmp( pRetNode->Attribute( "b" ), pResource->Attribute( "b" ) ) &&
        !std::strcmp( pRetNode->Attribute( "a" ), pResource->Attribute( "a" ) ) )
       {
       pRetNode->RemoveAttribute( "r" );
       pRetNode->RemoveAttribute( "g" );
       pRetNode->RemoveAttribute( "b" );
       pRetNode->RemoveAttribute( "a" );
       }
    return pRetNode;
    }

Transform::Transform( const Mat4x4& toWorld )
   {
   m_ToWorld = toWorld;
   m_FromWorld = toWorld.Inverse();
   m_IsFromWorldDirty = false;
   } 

Transform::Transform( const Vec3& position, const Vec3& scale,const Quaternion& rotation )
   {
   m_ToWorld = rotation.GetRotationMatrix44( );// m = R
   m_ToWorld.MultScale( scale );// m = R S
   m_ToWorld.SetToWorldPosition( position );// m = T R S
   m_FromWorld = m_ToWorld.Inverse( );
   m_IsFromWorldDirty = false;
   }

bool Transform::Init( TiXmlElement* pData )
   {
   Vec3 pitchYawRoll;
   Vec3 position;
   Vec3 scale;
   if( !pData )
      {
      return false;
      }
   if( pitchYawRoll.Init( pData->FirstChildElement( "PitchYawRoll" ) ) )
      {
      SetPitchYawRollDeg( pitchYawRoll );
      }

   if( position.Init( pData->FirstChildElement( "Position" ) ) )
      {
      SetPosition( position );
      }

   if( scale.Init( pData->FirstChildElement( "Scale" ) ) )
      {
      SetScale( scale );
      }

   return true;
   }

TiXmlElement* Transform::GenerateXML( void ) const
   {
   TiXmlElement* pRetXMLNode = ENG_NEW TiXmlElement( "Transform" );

   TiXmlElement* pPosition = GetToWorldPosition().GenerateXML();
   pPosition->SetValue( "Position" );
   pRetXMLNode->LinkEndChild( pPosition );

   TiXmlElement* pRotation = GetPitchYawRollDeg().GenerateXML();
   pRotation->SetValue( "PitchYawRoll" );
   pRetXMLNode->LinkEndChild( pRotation );

   TiXmlElement* pScale = GetScale().GenerateXML();
   pScale->SetValue( "Scale" );
   pRetXMLNode->LinkEndChild( pScale );

   return pRetXMLNode;
   }

TiXmlElement* Transform::GenerateOverridesXML( TiXmlElement* pResourceNode ) const
   {
   TiXmlElement* pRetXMLNode = ENG_NEW TiXmlElement( "Transform" );

   TiXmlElement* pPosition = GetToWorldPosition().GenerateOverridesXML( pResourceNode->FirstChildElement( "Position" ) );
   pPosition->SetValue( "Position" );
   pRetXMLNode->LinkEndChild( pPosition );

   TiXmlElement* pRotation = GetPitchYawRollDeg().GenerateOverridesXML( pResourceNode->FirstChildElement( "PitchYawRoll" ) );
   pRotation->SetValue( "PitchYawRoll" );
   pRetXMLNode->LinkEndChild( pRotation );

   TiXmlElement* pScale = GetScale().GenerateOverridesXML( pResourceNode->FirstChildElement( "Scale" ) );
   pScale->SetValue( "Scale" );
   pRetXMLNode->LinkEndChild( pScale );

   return pRetXMLNode;
   }

AABB::AABB( void ) : m_Min( Vec3::g_Zero ), m_Max( Vec3::g_Zero )
   {}

AABB::AABB( const Vec3& min, const Vec3& max ) : m_Min( min ), m_Max( max )
   {}

bool AABB::IsIntersect( const AABB& other, float margin ) const
   {
   if( m_Min.x - margin > other.m_Max.x || other.m_Min.x - margin > m_Max.x )
      {
      return false;
      }
   if( m_Min.y - margin > other.m_Max.y || other.m_Min.y - margin > m_Max.y )
      {
      return false;
      }
   if( m_Min.z - margin > other.m_Max.z || other.m_Min.z - margin > m_Max.z )
      {
      return false;
      }

   return true;
   }

bool AABB::IsContain( const AABB& other ) const
   {
   if( m_Max.x < other.m_Max.x
       || m_Max.y < other.m_Max.y
       || m_Max.z < other.m_Max.z )
      {
      return false;
      }

   if( m_Min.x > other.m_Min.x
       || m_Min.y > other.m_Min.y
       || m_Min.z > other.m_Min.z )
      {
      return false;
      }
   return true;
   }

AABB AABB::Union( const AABB& other )
   {
   Vec3 unionMax( std::max( m_Max.x, other.m_Max.x )
                  , std::max( m_Max.y, other.m_Max.y )
                  , std::max( m_Max.z, other.m_Max.z ) );

   Vec3 unionMin( std::min( m_Min.x, other.m_Min.x )
                  , std::min( m_Min.y, other.m_Min.y )
                  , std::min( m_Min.z, other.m_Min.z ) );

   return AABB( unionMin, unionMax );
   }

float AABB::GetVolume() const
   {
   return ( m_Max.x - m_Min.x ) * ( m_Max.y - m_Min.y ) * ( m_Max.z - m_Min.z );
   }


//Transform::Transform( const Mat4x4& toWorld )
//   {
//   Mat4x4 temp( toWorld );
//   m_Scale = toWorld.GetScale();
//   temp.MultScale( 1.0f / m_Scale );
//   m_Quat = temp.GetQuaternion();
//   m_Quat.Normalize();
//
//   m_Pos = toWorld.GetToWorldPosition();
//   } 
//
//Transform::Transform( const Vec3& position, const Vec3& scale,const Quaternion& rotation ) : m_Pos( position ), m_Scale( scale ), m_Quat( rotation )
//   {
//   
//   }