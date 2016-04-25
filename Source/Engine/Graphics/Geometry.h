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
const float  ENG_QUARTERPI = glm::quarter_pi<float>();

const float  ENG_DEGREES_OVER_RADIANS = 180.0f / ENG_PI;
const float  ENG_RADIANS_OVER_DEGREES = ENG_PI / 180.0f;

#define RADIANS_TO_DEGREES(x) ((x) * ENG_DEGREES_OVER_RADIANS )
#define DEGREES_TO_RADIANS(x) ((x) * ENG_RADIANS_OVER_DEGREES )

class Vec3 : public glm::vec3
   {
   public:
      Vec3() : glm::vec3() { }
      Vec3( const glm::vec3 &v3 ) : glm::vec3( v3 ) { /*x = v3.x; y = v3.y; z = v3.z;*/ }
	   Vec3( const float _x, const float _y, const float _z ) : glm::vec3( _x, _y, _z ) { }
      Vec3( const double _x, const double _y, const double _z ) : glm::vec3( _x, _y, _z ) { }
     // Vec3( const Vec4 &v4 ) { x = v4.x; y = v4.y; z = v4.z; }

      Vec3 operator-( void ){ return Vec3( -( this->x ), -( this->y ), -( this->z ) ); }
      Vec3 operator+( const Vec3& other ) const { return Vec3( this->x + other.x, this->y + other.y, this->z + other.z ); }
      Vec3 operator-( const Vec3& other ) const { return Vec3( this->x - other.x, this->y - other.y, this->z - other.z ); }
      Vec3& operator+=( const Vec3& other ) { return ( *this = *this + other ); }
      Vec3& operator-=( const Vec3& other ) { return *this += (-other); }

      Vec3 operator*( const float& other ) const { return Vec3( this->x * other, this->y * other, this->z * other ); }
      Vec3 operator/( const float& other ) const { return Vec3( this->x / other, this->y / other, this->z / other ); }
      Vec3& operator*=( const float& other ) { return ( *this = *this * other ); }
      Vec3& operator/=( const float& other ) { return ( *this = *this / other ); }

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

extern Vec3 g_Up;
extern Vec3 g_Right;
extern Vec3 g_Forward;

class Vec4 : public glm::vec4
   {
   public:
      Vec4() : glm::vec4() { x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f; }
      Vec4( const glm::vec4 &v4 ) : glm::vec4( v4 ) { /*x = v4.x; y = v4.y; z = v4.z; w = v4.w;*/ }
      Vec4( const glm::vec3 &v3 ) { x = v3.x; y = v3.y; z = v3.z; w = 1.0f; }
      Vec4( const float _x, const float _y, const float _z, const float _w ) { x = _x; y = _y; z = _z; w = _w; }
      Vec4( const double _x, const double _y, const double _z, const double _w ) { x = (float)_x; y = (float)_y; z = (float)_z; w = (float)_w; }

      inline float Length() { return glm::length<float, glm::highp, glm::tvec4>( *this ); }
      inline Vec4* Normalize() { *this = glm::normalize<float, glm::highp, glm::tvec4>( *this ); return this;  }
      inline float Dot( const Vec4 &b ) { return glm::dot<float, glm::highp, glm::tvec4>( *this, b ); }
   };

inline Vec3::Vec3( const class Vec4 &v4 ) { x = v4.x; y = v4.y; z = v4.z; }

extern Vec4 g_Up4;
extern Vec4 g_Right4;
extern Vec4 g_Forward4;

class Quaternion : public glm::fquat
   {
   public:
      Quaternion( void ) : glm::fquat() { }
      Quaternion( glm::fquat &q) : glm::fquat(q) { }

      Vec3 GetPitchYawRoll( void ) { return glm::eulerAngles( *this ); }

      void Normalize() { *this = glm::normalize<float, glm::highp>( *this ); }
      void Slerp( const Quaternion &start, const Quaternion &end, float cooef ) { *this = glm::slerp( start, end, cooef ); }
      void GetAxisAngle( Vec3& axis, float &angle ) const { axis = glm::axis( *this ); angle = glm::angle( *this ); }
      
      void BuildAxisRad( const Vec3& axis, const float& radian ) { *this = glm::angleAxis( radian, axis ); }
      void BuildYawPitchRollRad( const float yawRadian, const float pitchRadian, const float rollRadian ) { *this = glm::fquat( Vec3( yawRadian, pitchRadian, rollRadian ) ); }
      void BuildYawPitchRollReg( const float yawDegree, const float pitchDegree, const float rollDegree ) 
         { *this = glm::fquat( Vec3( DEGREES_TO_RADIANS( yawDegree ), DEGREES_TO_RADIANS( pitchDegree ), DEGREES_TO_RADIANS( rollDegree ) ) ); }
      void Build44Matrix( const class Mat4x4& mat );
      
   public:
	   static const Quaternion g_Identity;
   }; 

class Mat4x4 : public glm::mat4
   {
   public:
      Mat4x4() : glm::mat4() { }
      Mat4x4( glm::mat4 &mat) : glm::mat4( mat ) { }

      inline void SetPosition( Vec3 const &pos )
         {
         (*this)[3] = Vec4( pos );
         }
      inline void SetPosition( Vec4 const &pos )
         {
         (*this)[3] = pos;
         }
      inline Vec3 GetPosition( void ) const { return Vec3( (*this)[3] ); }
      inline Vec3 GetForward( void ) const;
      inline Vec3 GetForWard( void ) const;
      inline Vec3 GetRight( void ) const;
      inline Vec3 GetUp( void ) const;
      inline Vec3 GetPitchYawRoll() const;
      Quaternion GetQuaternion( void ) const { return glm::quat_cast( *this ); }
      inline Vec4 Xform( const Vec4 &v ) const { return (*this) * v; }
      inline Vec3 Xform( const Vec3 &v ) const { return Vec3( Xform( Vec4( v ) ) ); }
      inline Mat4x4 Inverse() const { return glm::inverse( *this ); }
      // glm matrix is column major order
      inline void BuildTranslation( const Vec3 &pos ) {
         *this = glm::translate( *this, pos );
         }
      inline void BuildTranslation( const float x, const float y, const float z ) 
         { 
         *this = glm::translate( *this, Vec3( x, y, z ) );
         }
      inline void BuildRotationXRad( const float radians ) { *this = glm::rotate( radians , glm::vec3( 1.0f, 0.0f, 0.0f ) ); }
      inline void BuildRotationYRad( const float radians ) { *this = glm::rotate( radians , glm::vec3( 0.0f, 1.0f, 0.0f ) ); }
      inline void BuildRotationZRad( const float radians ) { *this = glm::rotate( radians , glm::vec3( 0.0f, 0.0f, 1.0f ) ); }
      // Rotate around counterclockwise direction, 
      // Yaw->Yaxis, Pitch->Xaxis, Roll->Zaxis
      // Caution: This will wipe out your position infomation
      inline void BuildPitchYawRollRad( const float pitchRadian, const float yawRadian, const float rollRadian )
         { *this = glm::eulerAngleYXZ( yawRadian, pitchRadian, rollRadian ); }
      // Caution: This will wipe out your position infomation
      inline void BuildPitchYawRollDeg( const float pitchDegree, const float yawDegree, const float rollDegree )
         { *this = glm::eulerAngleYXZ( DEGREES_TO_RADIANS( yawDegree ), DEGREES_TO_RADIANS( pitchDegree ), DEGREES_TO_RADIANS( rollDegree ) ); }
      // Caution: This will wipe out your position infomation
      inline void BuildRotationQuat( const Quaternion &q ) { *this = mat4_cast( q ); }
      inline void BuildProjection( float fovy, float aspect, float zNear, float zFar );
      static Mat4x4 ViewMatrix( const Vec3& eye, const Vec3& center, const Vec3& up ) { return Mat4x4( glm::lookAt( eye, center, up ) ); }
      // Caution! this matrix is not the same as glm::lookat, the whole direction
      // is reversed
      static Mat4x4 LookAt( const Vec3& eye, const Vec3& center, const Vec3& up ){ return ViewMatrix( eye, 2.0f * eye - center, up ) ; }
   public: 
      static const Mat4x4 g_Identity;

   private:
      inline Vec3 GetXFormDirection( Vec3 vec ) const;
   };

inline Vec3 Mat4x4::GetForward( void ) const
   {
   return this->GetXFormDirection( g_Forward );
   }

inline Vec3 Mat4x4::GetRight( void ) const
   {
   return this->GetXFormDirection( g_Right );
   }

inline Vec3 Mat4x4::GetUp( void ) const
   {
   return this->GetXFormDirection( g_Up );
   }

inline Vec3 Mat4x4::GetXFormDirection( Vec3 vec ) const
   {
   Mat4x4 justRot = *this;
	justRot.SetPosition( Vec3( 0.f,0.f,0.f ) );
	Vec3 forward = justRot.Xform( vec );
	return forward;
   }

inline void Quaternion::Build44Matrix( const Mat4x4& mat ) { *this = glm::quat_cast( mat ); }

inline Vec3 Mat4x4::GetPitchYawRoll() const
{
   float yaw, pitch, roll;
	
   pitch = asin( -(*this)[2][1] );

   double threshold = 0.001; // Hardcoded constant - burn him, he's a witch
   double test = cos(pitch);

   if(test > threshold) 
   {
      roll = atan2( (*this)[0][1] , (*this)[1][1] );
      yaw = atan2(  (*this)[2][0] , (*this)[2][2] );
   }
   else 
   {
      roll = atan2( -(*this)[1][0] , (*this)[0][0]  );
      yaw = 0.0;
   }

	return ( Vec3(pitch, yaw, roll) );
}

inline void Mat4x4::BuildProjection( float fovy, float aspect, float zNear, float zFar )
   {
   *this = glm::perspective( fovy, aspect, zNear, zFar);
   }

typedef struct ColorComponents
   {
   float r;
   float g;
   float b;
   float a;
   }ColorComponents;

typedef struct Color
   {
   public:
    Color();
    Color( const Color& color );
    Color( const float * );
    Color( float red, float green, float blue, float alpha );

    float GetAlpha( void ) const { return m_Component.a; }
    void SetAlpha( float alpha ) { m_Component.a = alpha; }
    // casting
    operator DWORD () const;

    // assignment operators
    Color& operator += ( const Color& );
    Color& operator -= ( const Color& );
    Color& operator *= ( float );
    Color& operator /= ( float );

    // unary operators
    Color operator + () const;
    Color operator - () const;

    // binary operators
    Color operator + ( const Color& ) const;
    Color operator - ( const Color& ) const;
    Color operator * ( float ) const;
    Color operator / ( float ) const;

    friend Color operator * ( float, const Color& );

    bool operator == ( const Color& ) const;
    bool operator != ( const Color& ) const;

    operator const GLfloat*( void ){ return &m_Array[0]; }

   public:
      union
         {
         float m_Array[4];
         ColorComponents m_Component;
         };   
   private:
      void Satuate();
   }Color;

/*
typedef struct Color
   {
   public:
    Color();
    Color( const Color& color );
    Color( const float * );
    Color( float red, float green, float blue, float alpha );

    float GetAlpha( void ) const { return m_Color[3]; }
    void SetAlpha( float alpha ) { m_Color[3] = alpha; }
    // casting
    operator DWORD () const;

    // assignment operators
    Color& operator += ( const Color& );
    Color& operator -= ( const Color& );
    Color& operator *= ( float );
    Color& operator /= ( float );

    // unary operators
    Color operator + () const;
    Color operator - () const;

    // binary operators
    Color operator + ( const Color& ) const;
    Color operator - ( const Color& ) const;
    Color operator * ( float ) const;
    Color operator / ( float ) const;

    friend Color operator * ( float, const Color& );

    bool operator == ( const Color& ) const;
    bool operator != ( const Color& ) const;

    operator const GLfloat*( void ){ return m_Color; }
   public:
      float m_Color[4];
    //  float r;
    //  float g;
    //  float b;
    //  float a;
   
   private:
      void Satuate();
   }Color;
*/

class Plane
   {
   public:
      Plane( void ) { n = g_Up; d = 0.f; };
      // To make a plane with normal vector points upward, The order of the points 
      //should be counter clockwise for right handed system 
      void Init( const Vec3& p0, const Vec3& p1, const Vec3& p2 )
         {
         n = glm::cross( p1 - p0, p2 - p0 );
         // for plane ax + by + cz + d = 0; d = -( ax + by + cz ) = - dot( n, a point on plane )
         d = -1.0f * n.Dot( p0 );
         Normalize();
         }
      // constructor based on coefficient
      Plane( const float a, const float b, const float c, const float w ) : n( a, b, c ), d( w )  { }
      void Normalize() { float lengthInv = 1.0f / n.Length(); n *= lengthInv; d *= lengthInv; }
      // Inside is defined as same side of normal
      bool Inside( Vec3 p ) const ;
      // Inside is defined as same side of normal, radius means it is a sphere
      bool Inside( Vec3 p, const float radius ) const ;

   public:
      // normatl vector
      Vec3 n;
      // distance
      float d;
   };

class Frustum
   {
   public:
      Frustum( void );
      void Init( const float fov, const float aspect, const float nearClipDis, const float farClipDis );
      // return if a single point is inside the frustum, point should be in Frustum's local space
      bool Inside( const Vec3 &point ) const ;
      // return if a shpere is inside the frustum, point should be in Frustum's local space
      bool Inside( const Vec3 &point, float radius ) const ;
      
      void SetFOV(float fov) { m_Fov=fov; Init(m_Fov, m_Aspect, m_NearDis, m_FarDis); }
	   void SetAspect(float aspect) { m_Aspect=aspect; Init(m_Fov, m_Aspect, m_NearDis, m_FarDis); }
	   void SetNear(float nearClip) { m_NearDis=nearClip; Init(m_Fov, m_Aspect, m_NearDis, m_FarDis); }
	   void SetFar(float farClip) { m_FarDis=farClip; Init(m_Fov, m_Aspect, m_NearDis, m_FarDis); }

   public:
      enum Side { Near, Far, Top, Right, Bottom, Left, NumPlanes };

      Plane m_Planes[ NumPlanes ];
      // represent four vertices for near plane rectangle
      Vec3 m_NearPlaneVerts[4];
      // represenr four vertices for far plane rectangle 
      Vec3 m_FarPlaneVerts[4];

      // field of view in radians
      float m_Fov;
      // width divided by height
      float m_Aspect;
      // near plane distance
      float m_NearDis;
      // far plane distance
      float m_FarDis;

   };

class Transform
   {
   public:
      Transform( void );
      Transform( const Mat4x4* pToWorld, const Mat4x4* pFromWorld = NULL );
      Transform( const Vec3* position, const Quaternion* rotation = NULL );

      void SetPosition( Vec3 position ){ m_ToWorld.SetPosition( position ); UpDateFromWorld(); }
      void SetPosition( float x, float y, float z ) { SetPosition( Vec3( x, y, z ) ); }

      inline void SetRotation( const Quaternion& quat );

      inline void SetPitchYawRollRad( float pitch, float yaw, float roll );     
      void SetPitchYawRollRad( const Vec3& vec ){ SetPitchYawRollRad( vec.x, vec.y, vec.z ); }

      inline void SetPitchYawRollDeg( float pitch, float yaw, float roll );
      void SetPitchYawRollDeg( const Vec3& vec ){ SetPitchYawRollDeg( vec.x, vec.y, vec.z ); }

      Mat4x4 GetToWorld( void ) const { return m_ToWorld; }
      Mat4x4 GetFromWorld( void ) const { return m_FromWorld; }

      Quaternion GetQuaternion( void ) const { return m_ToWorld.GetQuaternion(); };
      Vec3       GetPitchYawRoll( void ) const { return m_ToWorld.GetPitchYawRoll(); };
      Vec3       GetPosition( void ) const { return m_ToWorld.GetPosition(); }

   private:
      void UpDateFromWorld( void ){ m_FromWorld = m_ToWorld.Inverse(); }
      void UpDateToWorld( void ){ m_ToWorld = m_FromWorld.Inverse(); }

   private:
      Mat4x4 m_ToWorld;
      Mat4x4 m_FromWorld;
   };

inline void Transform::SetRotation( const Quaternion& quat )
   {
   Vec3 position = this->GetPosition(); 
   m_ToWorld.BuildRotationQuat( quat );
   this->SetPosition( position );
   UpDateFromWorld(); 
   }

inline void Transform::SetPitchYawRollRad( float pitch, float yaw, float roll )
   { 
   Vec3 position = this->GetPosition(); 
   m_ToWorld.BuildPitchYawRollRad( pitch, yaw, roll ); 
   this->SetPosition( position );
   UpDateFromWorld(); 
   };

inline void Transform::SetPitchYawRollDeg( float pitch, float yaw, float roll ) 
   { 
   Vec3 position = this->GetPosition(); 
   m_ToWorld.BuildPitchYawRollDeg( pitch, yaw, roll ); 
   this->SetPosition( position );
   UpDateFromWorld(); 
   }