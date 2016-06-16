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


const Vec3 Vec3::g_Zero( 0.0f, 0.0f, 0.0f );
const Vec3 Vec3::g_Identity( 1.0f, 1.0f, 1.0f );
const Mat4x4 Mat4x4::g_Identity( glm::mat4( 1.0f ) );
const Quaternion Quaternion::g_Identity( 1.0f, 0.0f, 0.0f, 0.0f );
const Transform Transform::g_Identity( Vec3::g_Zero, Vec3::g_Identity, Quaternion::g_Identity );

bool Vec3::Init( TiXmlElement* pData )
   {
   float rX = 0;
   float rY = 0;
   float rZ = 0;
   if( !pData )
      {
      return false;
      }
   if( TIXML_SUCCESS != pData->QueryFloatAttribute( "x", &rX ) )
      {
      return false;
      }
   if( TIXML_SUCCESS != pData->QueryFloatAttribute( "y", &rY ) )
      {
      return false;
      }
   if( TIXML_SUCCESS != pData->QueryFloatAttribute( "z", &rZ ) )
      {
      return false;
      }
   
   x = rX;
   y = rY;
   z = rZ;
   return true;
   }

TiXmlElement* Vec3::GernerateXML( void )
   {
   TiXmlElement* pRetNode = ENG_NEW TiXmlElement( "Vector3" );
   pRetNode->SetAttribute( "x", ToStr( x ).c_str() );
   pRetNode->SetAttribute( "y", ToStr( y ).c_str() );
   pRetNode->SetAttribute( "z", ToStr( z ).c_str() );
   
   return pRetNode;
   }

TiXmlElement* Vec3::GenerateOverridesXML( TiXmlElement* pResource )
   {
   TiXmlElement* pRetNode = GernerateXML();
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

bool Plane::Inside( Vec3 p ) const
   {
   return ( p.Dot( n ) + d >= 0.0f );
   }

bool Plane::Inside( Vec3 p, const float radius ) const 
   {
   float distance = p.Dot( n ) + d;
   // we need to consider if the point is outside the plane, but its radius may compensate for it
   // -> we should test distance + radius >= 0
   return ( distance >= -radius );
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
    float r;
    float g;
    float b;
    float a;

    if( !pData )
       {
       return false;
       }
    if( TIXML_SUCCESS != pData->QueryFloatAttribute( "r", &r ) )
       {
       return false;
       }
    if( TIXML_SUCCESS != pData->QueryFloatAttribute( "g", &g ) )
       {
       return false;
       }
    if( TIXML_SUCCESS != pData->QueryFloatAttribute( "b", &b ) )
       {
       return false;
       }
    if( TIXML_SUCCESS != pData->QueryFloatAttribute( "a", &a ) )
       {
       return false;
       }

    m_Component.r = r;
    m_Component.g = g;
    m_Component.b = b;
    m_Component.a = a;
    return true;
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

Frustum::Frustum( void )
   {
   m_Fov = ENG_QUARTERPI;
   m_Aspect = 4.0f / 3.0f;
   m_NearDis = 1.0f;
   m_FarDis = 1000.0f;
   }

bool Frustum::Inside( const Vec3 &point ) const
   {
   for( int i = 0; i < NumPlanes; ++i )
      {
      if( !m_Planes[i].Inside( point ) )
         {
         return false;
         }
      }
   return true;
   }

bool Frustum::Inside( const Vec3 &point, float radius ) const
   {
   for( int i = 0; i < NumPlanes; ++i )
      {
      if( !m_Planes[i].Inside( point, radius ) )
         {
         return false;
         }
      }
   return true;
   }

void Frustum::Init( const float fov, const float aspect, const float nearClipDis, const float farClipDis )
   {
   m_Fov = fov;
   m_Aspect = aspect;
   m_NearDis = nearClipDis;
   m_FarDis = farClipDis;
   // Fov = 90 -> angle +- 45 degree
   double tanFovOver2 = tan( fov / 2.0f );
   Vec3 nearRight = static_cast<float>( m_NearDis * tanFovOver2 * m_Aspect ) * g_Right;
   Vec3 farRight = static_cast<float>( m_FarDis * tanFovOver2 * m_Aspect ) * g_Right;
   Vec3 nearUp = static_cast<float>( m_NearDis * tanFovOver2 ) * g_Up;
   Vec3 farUp = static_cast<float>( m_FarDis * tanFovOver2 ) * g_Up;

   // These vertives start in upper right and go around clockwise
   m_NearPlaneVerts[0] = ( m_NearDis * g_Forward ) - nearRight + nearUp;
   m_NearPlaneVerts[1] = ( m_NearDis * g_Forward ) + nearRight + nearUp;
   m_NearPlaneVerts[2] = ( m_NearDis * g_Forward ) + nearRight - nearUp;
   m_NearPlaneVerts[3] = ( m_NearDis * g_Forward ) - nearRight - nearUp;

   m_FarPlaneVerts[0] = ( m_FarDis * g_Forward ) - farRight + farUp;
   m_FarPlaneVerts[1] = ( m_FarDis * g_Forward ) + farRight + farUp;
   m_FarPlaneVerts[2] = ( m_FarDis * g_Forward ) + farRight - farUp;
   m_FarPlaneVerts[3] = ( m_FarDis * g_Forward ) - farRight - farUp;

   Vec3 origin(0.0f, 0.0f, 0.0f);

	m_Planes[Near].Init( m_NearPlaneVerts[0], m_NearPlaneVerts[1], m_NearPlaneVerts[2] );
	m_Planes[Far].Init( m_FarPlaneVerts[2], m_FarPlaneVerts[1], m_FarPlaneVerts[0] );
	m_Planes[Right].Init ( origin, m_FarPlaneVerts[1], m_FarPlaneVerts[2] );
	m_Planes[Top].Init( origin, m_FarPlaneVerts[0], m_FarPlaneVerts[1] );
	m_Planes[Left].Init( origin, m_FarPlaneVerts[3], m_FarPlaneVerts[0] );
	m_Planes[Bottom].Init( origin, m_FarPlaneVerts[2], m_FarPlaneVerts[3] );
   }

Transform::Transform( const Mat4x4& toWorld )
   {
   m_ToWorld = toWorld;
   m_FromWorld = toWorld.Inverse();
   m_IsFromWorldDirty = false;
   } 

Transform::Transform( const Vec3& position, const Vec3& scale,const Quaternion& rotation )
   {
   m_ToWorld = rotation.GetRotationMatrix( );// m = R
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
      SetRotation( pitchYawRoll );
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

TiXmlElement* Transform::GenerateXML( void )
   {
   TiXmlElement* pRetXMLNode = ENG_NEW TiXmlElement( "Transform" );

   TiXmlElement* pPosition = GetToWorldPosition().GernerateXML();
   pPosition->SetValue( "Position" );
   pRetXMLNode->LinkEndChild( pPosition );

   TiXmlElement* pRotation = GetPitchYawRollDeg().GernerateXML();
   pRotation->SetValue( "PitchYawRoll" );
   pRetXMLNode->LinkEndChild( pRotation );

   TiXmlElement* pScale = GetScale().GernerateXML();
   pScale->SetValue( "Scale" );
   pRetXMLNode->LinkEndChild( pScale );

   return pRetXMLNode;
   }

TiXmlElement* Transform::GenerateOverridesXML( TiXmlElement* pResourceNode )
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