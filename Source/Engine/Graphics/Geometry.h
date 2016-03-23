#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: Geometry.h
////////////////////////////////////////////////////////////////////////////////

///////////////////////
//3rd party INCLUDES //
///////////////////////
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_FORCE_RADIANS 

const float  ENG_PI = glm::pi<float>();
const float  ENG_2PI = glm::two_pi<float>();
const float  ENG_HALFPI = glm::half_pi<float>();
const float  ENG_DEGREE2RADIAN = ENG_PI / 180.f;
const float  ENG_RADIAN2DEGREE = 180.f / ENG_PI;

class Vec3 : public glm::vec3
   {
   public:
      Vec3() : glm::vec3() { x = 0; y = 0; z = 0; }
      Vec3( const glm::vec3 &v3 ) { x = v3.x; y = v3.y; z = v3.z; }
	   Vec3( const float _x, const float _y, const float _z ) { x=_x; y=_y; z=_z; }
      Vec3( const double _x, const double _y, const double _z ) { x = (float)_x; y = (float)_y; z = (float)_z; }
     // Vec3( const Vec4 &v4 ) { x = v4.x; y = v4.y; z = v4.z; }

      inline float Length(){ return glm::length<float, glm::highp, glm::tvec3>( (*this) ); }
	   inline Vec3(const class Vec4 &v4);

      inline Vec3* Normalize()
         {
         (*this) = glm::normalize<float, glm::highp, glm::tvec3>( (*this) );
         return this; 
         }
      inline float Dot( const Vec3 &b ) const
         {
         return glm::dot<float, glm::highp, glm::tvec3>( *this, b );
         }
      inline Vec3 Cross( const Vec3 &b ) const
         {
         glm::cross( glm::vec3(), glm::vec3() );
         return ( glm::cross<float, glm::highp>( (*this), b ) );
         }

   private:
   };



class Vec4 : public glm::vec4
   {
   public:
      Vec4() : glm::vec4() { x = 0; y = 0; z = 0; w = 0; }
      Vec4( const glm::vec4 &v4 ){ x = v4.x; y = v4.y; z = v4.z; w = v4.w; }
      Vec4( const glm::vec3 &v3 ){ x = v3.x; y = v3.y; z = v3.z; w = 1.0f; }
      Vec4( const float _x, const float _y, const float _z, const float _w ) { x = _x; y = _y; z = _z; w = _w; }
      Vec4( const double _x, const double _y, const double _z, const double _w ) { x = (float)_x; y = (float)_y; z = (float)_z; w = (float)_w; }

      inline float Length() { return glm::length<float, glm::highp, glm::tvec4>( *this ); }
      inline Vec4* Normalize() { *this = glm::normalize<float, glm::highp, glm::tvec4>( *this ); return this;  }
      inline float Dot( const Vec4 &b ) { return glm::dot<float, glm::highp, glm::tvec4>( *this, b ); }
   };

inline Vec3::Vec3( const class Vec4 &v4 ) { x = v4.x; y = v4.y; z = v4.z; }

class Quaternion : public glm::fquat
   {
   public:
      Quaternion() : glm::fquat() { }
      Quaternion( glm::fquat &q) : glm::fquat(q) { }

      void Normalize() { *this = glm::normalize<float, glm::highp>( *this ); }
      void Slerp( const Quaternion &start, const Quaternion &end, float cooef ) { *this = glm::slerp( start, end, cooef ); }
      void GetAxisAngle( Vec3& axis, float &angle ) const { axis = glm::axis( *this ); angle = glm::angle( *this ); }
      
      void BuildAxisRadian( const Vec3& axis, const float& radian ) { *this = glm::angleAxis( radian, axis ); }
      void BuildYawPitchRoll( const float yawRadians, const float pitchRadians, const float rollRadians ) { *this = glm::fquat( Vec3( yawRadians, pitchRadians, rollRadians ) ); }
      void Build44Matrix( const class Mat4x4& mat );

   public:
	   static const Quaternion g_Identity;
   }; 

class Mat4x4 : public glm::mat4
   {
   public:
      Mat4x4() : glm::mat4() { }
      Mat4x4( glm::mat4 &mat) {   memcpy( this, &mat[0][0], sizeof(mat)); }

      inline void SetPosition( Vec3 const &pos )
         {
         (*this)[3] = Vec4( pos );
         }
      inline void SetPosition( Vec4 const &pos )
         {
         (*this)[3] = pos;
         }
      inline Vec3 GetPosition() const { return Vec3( (*this)[3] ); }
      inline Vec4 Xfrom( const Vec4 &v ) const { return (*this) * v; }
      inline Vec3 Xfrom( const Vec3 &v ) const { return Vec3( Xfrom( Vec4( v ) ) ); }
      inline Mat4x4 Inverse() const { return glm::inverse( *this ); }
      // glm matrix is column major order
      inline void BuildTranslation( const Vec3 &pos ) {
         *this = glm::translate( *this, pos );
         //*this = Mat4x4::g_Identity; (*this)[3] = Vec4( pos ); 
         }
      inline void BuildTranslation( const float x, const float y, const float z ) 
         { 
         *this = glm::translate( *this, Vec3( x, y, z ) );
       //  *this = Mat4x4::g_Identity; 
       //  (*this)[3][0] = x;
       //  (*this)[3][1] = y;
       //  (*this)[3][2] = z;
         }
      inline void BuildRotationX( const float radians ) { *this = glm::rotate( radians , glm::vec3( 1.0f, 0.0f, 0.0f ) ); }
      inline void BuildRotationY( const float radians ) { *this = glm::rotate( radians , glm::vec3( 0.0f, 1.0f, 0.0f ) ); }
      inline void BuildRotationZ( const float radians ) { *this = glm::rotate( radians , glm::vec3( 0.0f, 0.0f, 1.0f ) ); }
      inline void BuildYawPitchRoll( const float yawRadians, const float pitchRadians, const float rollRadians )
         { *this = glm::eulerAngleYXZ( yawRadians, pitchRadians, rollRadians ); }
      // Todo: implement quat class first
      inline void BuildRotationQuat() { }
      
   public: 
      static const Mat4x4 g_Identity;
   };

inline void Quaternion::Build44Matrix( const Mat4x4& mat ) { *this = glm::quat_cast( mat ); }