#pragma once
/*!
 * \file Geometry.h
 * \date 2016/05/07 14:29
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
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_access.hpp>

#define GLM_FORCE_RADIANS 

const float  ENG_PI = glm::pi<float>();
const float  ENG_2PI = glm::two_pi<float>();
const float  ENG_HALFPI = glm::half_pi<float>();
const float  ENG_QUARTERPI = glm::quarter_pi<float>();

const float  ENG_DEGREES_OVER_RADIANS = 180.0f / ENG_PI;
const float  ENG_RADIANS_OVER_DEGREES = ENG_PI / 180.0f;

class Transform;

typedef shared_ptr<Transform> TransformPtr;

#define RADIANS_TO_DEGREES(x) ((x) * ENG_DEGREES_OVER_RADIANS )
#define DEGREES_TO_RADIANS(x) ((x) * ENG_RADIANS_OVER_DEGREES )

class Vec3;
class Vec4;

typedef std::list<Vec3> Vec3List;
typedef std::list<Vec4> Vec4List;

class Vec3 : public glm::vec3
   {
   public:
      Vec3() : glm::vec3() { }
      Vec3( const glm::vec3 &v3 ) : glm::vec3( v3 ) { /*x = v3.x; y = v3.y; z = v3.z;*/ }
	   Vec3( float _x, float _y, float _z ) : glm::vec3( _x, _y, _z ) { }
      Vec3( double _x, double _y, double _z ) : glm::vec3( _x, _y, _z ) { }
      Vec3( int _x, int _y, int _z ) : glm::vec3( _x, _y, _z ) {}
     // Vec3( const Vec4 &v4 ) { x = v4.x; y = v4.y; z = v4.z; }

      Vec3 operator+( const Vec3& other ) const { return Vec3( this->x + other.x, this->y + other.y, this->z + other.z ); }
      Vec3 operator-( const Vec3& other ) const { return Vec3( this->x - other.x, this->y - other.y, this->z - other.z ); }
      Vec3& operator+=( const Vec3& other ) { return ( *this = *this + other ); }
      Vec3& operator-=( const Vec3& other ) { return *this += (-other); }

      Vec3 operator*( float other ) const { return Vec3( this->x * other, this->y * other, this->z * other ); }
      Vec3 operator/( float other ) const { return Vec3( this->x / other, this->y / other, this->z / other ); }

      Vec3 operator*( const Vec3& other ) const { return Vec3( this->x * other.x, this->y * other.y, this->z * other.z ); }
      Vec3 operator/( const Vec3& other ) const { return Vec3( this->x / other.x, this->y / other.y, this->z / other.z ); }

      Vec3& operator*=( float other ) { return ( *this = *this * other ); }
      Vec3& operator/=( float other ) { return ( *this = *this / other ); }

      inline float Length(){ return glm::length<float, glm::highp, glm::tvec3>( (*this) ); }
	   inline Vec3( const class Vec4 &v4 );

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
      bool Init( TiXmlElement* pData );
      TiXmlElement* GernerateXML( void );
      TiXmlElement* GenerateOverridesXML( TiXmlElement* pResource );

   public:
      static const Vec3 g_Zero;
      static const Vec3 g_Identity;

   private:
   };

inline Vec3 operator-( const Vec3& left ){ return Vec3( -( left.x ), -( left.y ), -( left.z ) ); }

inline Vec3 operator*( float scalar, const Vec3& right ) { return Vec3( right.x * scalar, right.y * scalar, right.z * scalar ); }

inline Vec3 operator/( const float scalar, const Vec3& right ) { return Vec3( scalar / right.x, scalar / right.y, scalar / right.z ); }


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

class Mat4x4;

class Quaternion : public glm::fquat
   {
   public:
      Quaternion( void ) : glm::fquat( 1.0f, 0.0f, 0.0f, 0.0f ) { }
      Quaternion( glm::fquat &q) : glm::fquat(q) { }
      Quaternion( const float w, const float x, const float y, const float z ) : glm::fquat( w, x, y, z ) { }
      Quaternion( const Vec3& pitchYawRollRad ) : glm::fquat( pitchYawRollRad ){}

      Vec4 operator*( const Vec4& vec ) const { return Vec4( static_cast<glm::fquat>( *this ) * vec ); }
      Vec3 operator*( const Vec3& vec ) const { return Vec3(  *this * Vec4( vec ) ); }

      Vec3 GetPitchYawRollRad( void ) const { return glm::eulerAngles( *this ); }
      Vec3 GetPitchYawRollDeg( void ) const { Vec3 vec = this->GetPitchYawRollRad(); return Vec3( RADIANS_TO_DEGREES( vec.x ), RADIANS_TO_DEGREES( vec.y ), RADIANS_TO_DEGREES( vec.z ) ); };
      
      Vec3 XForm( const Vec3& vec ){ return *this * vec; }
      
      inline Mat4x4 GetRotationMatrix( void ) const;

      Quaternion Inverse( void ) const { return glm::inverse( *this ); }

      Quaternion Rotate( const Quaternion& quat, const float rad, const Vec3& axis ){ return glm::rotate( *this, rad, axis ); }

      void Normalize() { *this = glm::normalize<float, glm::highp>( *this ); }
      void Slerp( const Quaternion &start, const Quaternion &end, float cooef ) { *this = glm::slerp( start, end, cooef ); }
      void GetAxisAngle( Vec3& axis, float &angle ) const { axis = glm::axis( *this ); angle = glm::angle( *this ); }
      
      void BuildAxisRad( const Vec3& axis, const float& radian ) { Vec3 nAxis = axis;
      nAxis.Normalize();
      *this = glm::angleAxis( radian, nAxis ); }
      
      void BuildPitchYawRollRad( const Vec3& pitchYawRoll ) { *this = glm::fquat( pitchYawRoll ); }
      void BuildPitchYawRollRad( const float pitchRad, const float yawRad, const float rollRad ) { this->BuildPitchYawRollRad( Vec3( pitchRad, yawRad, rollRad ) ); }
      void BuildPitchYawRollDeg( const float pitchDeg, const float yawDeg, const float rollDeg ) 
         { this->BuildPitchYawRollRad( Vec3( DEGREES_TO_RADIANS( pitchDeg ), DEGREES_TO_RADIANS( yawDeg ), DEGREES_TO_RADIANS( rollDeg ) ) ); }
      
      void Build44Matrix( const class Mat4x4& mat );
      
      void AddPitchYawRollRad( const Vec3& pitchYawRollRad )
         { 
         Vec3 EulerAngle = this->GetPitchYawRollRad();  
         EulerAngle += pitchYawRollRad;
         
        // this->BuildPitchYawRollRad( Vec3( NormalizeRad( EulerAngle.x ), NormalizeRad( EulerAngle.y ), NormalizeRad( EulerAngle.z ) ) );
         this->BuildPitchYawRollRad( Vec3( EulerAngle.x , EulerAngle.y , EulerAngle.z  ) );
         
         }
      void AddPitchYawRollRad( const float pitchRad, const float yawRad, const float rollRad )
         {         
         this->AddPitchYawRollRad( Vec3( pitchRad, yawRad, rollRad ) );
         }

      void AddPitchYawRollDeg( const float pitchDeg, const float yawDeg, const float rollDeg ){ this->AddPitchYawRollRad( Vec3( DEGREES_TO_RADIANS( pitchDeg ), DEGREES_TO_RADIANS( yawDeg ), DEGREES_TO_RADIANS( rollDeg ) ) ); }
      void AddPitchYawRollDeg( const Vec3& pitchYawRollDeg ){ this->AddPitchYawRollDeg( DEGREES_TO_RADIANS( pitchYawRollDeg.x ), DEGREES_TO_RADIANS( pitchYawRollDeg.y ), DEGREES_TO_RADIANS( pitchYawRollDeg.z ) ); }
      

      void AddPitchDeg( const float pitchDeg ){ this->AddPitchYawRollDeg( pitchDeg, 0.0f, 0.0f ); }
      void AddYawDeg( const float yawDeg ){ this->AddPitchYawRollDeg( 0.0f, yawDeg, 0.0f ); }
      void AddRollDeg( const float rollDeg ){ this->AddPitchYawRollDeg( 0.0f, 0.0f, rollDeg ); }

   public:
	   static const Quaternion g_Identity;
   }; 

class Mat4x4 : public glm::mat4
   {
   public:
      Mat4x4( void ) : glm::mat4() { }
      Mat4x4( const glm::mat4& mat ) : glm::mat4( mat ) { }
      Mat4x4( const Mat4x4& mat ) : glm::mat4( mat ) {}

      inline void SetToWorldPosition( Vec4 const &pos )
         {
         ( *this )[3] = pos;
         }

      // equal to M = T * M
      inline void SetToWorldPosition( Vec3 const &pos )
         {
         SetToWorldPosition( Vec4( pos ) );
         }
      
      inline void SetToWorldPosition( const float x, const float y, const float z ) { SetToWorldPosition( Vec3( x, y, z ) ); }

      inline Vec3 GetToWorldPosition( void ) const { return Vec3( (*this)[3] ); }
      
      inline Vec4 GetRow( const int index ) const { return Vec4( (*this)[index] ); }
      inline Vec4 GetCol( const int index ) const { return Vec4( glm::row( *this, index ) ); }

      // Get Scale from each column's length
      inline Vec3 GetScale( void ) const { return Vec3( Vec3( GetRow( 0 ) ).Length() , Vec3( GetRow( 1 ) ).Length() , Vec3( GetRow( 2 ) ).Length() ); }


      inline Vec3 GetForward( void ) const;
      inline Vec3 GetRight( void ) const;
      inline Vec3 GetUp( void ) const;

      inline Vec3 GetPitchYawRollRad( void ) const;
      inline Vec3 GetPitchYawRollDeg( void ) const;

      Mat4x4 GetRotation( void ) const
         {
         Mat4x4 temp( *this );
         temp.MultScale( 1.0f / temp.GetScale( ) );
         temp.SetToWorldPosition( Vec3::g_Zero );
         return temp;
         }

      Quaternion GetQuaternion( void ) const { 
         return glm::quat_cast( GetRotation(  ) ); 
         }

      inline Vec4 Xform( const Vec4 &v ) const { return (*this) * v; }
      inline Vec3 Xform( const Vec3 &v ) const { return Vec3( Xform( Vec4( v ) ) ); }

      // get rotated vector without shift value
      inline Vec3 GetXFormDirection( const Vec3& vec ) const;

      inline Mat4x4 Inverse( void ) const { return glm::inverse( *this ); }
      // equal to M = M * T
      inline void AddTranslation( const Vec3 &pos ) { *this = glm::translate( *this, pos ); }
      inline void AddTranslation( const float x, const float y, const float z ) { AddTranslation( Vec3( x, y, z ) ); }

      inline void BuildRotationXRad( const float radians ) { *this = glm::rotate( radians , glm::vec3( -1.0f, 0.0f, 0.0f ) ); }
      inline void BuildRotationYRad( const float radians ) { *this = glm::rotate( radians , glm::vec3( 0.0f, 1.0f, 0.0f ) ); }
      inline void BuildRotationZRad( const float radians ) { *this = glm::rotate( radians , glm::vec3( 0.0f, 0.0f, 1.0f ) ); }
      // Rotate around counterclockwise direction, 
      // Yaw->Yaxis, Pitch->Xaxis, Roll->Zaxis
      // Caution: This will wipe out your position infomation
      inline void BuildPitchYawRollRad( const float pitchRad, const float yawRad, const float rollRad )
         { 
         *this = glm::eulerAngleXYZ( pitchRad, yawRad, rollRad );
         /*Quaternion quat;
         quat.BuildPitchYawRollRad( pitchRad, yawRad, rollRad );
         *this = quat.GetRotationMatrix();*/
         }
      // Caution: This will wipe out your position infomation
      inline void BuildPitchYawRollDeg( const float pitchDegree, const float yawDegree, const float rollDegree )
         { this->BuildPitchYawRollRad( DEGREES_TO_RADIANS( pitchDegree ), DEGREES_TO_RADIANS( yawDegree ), DEGREES_TO_RADIANS( rollDegree ) ); }
      // Caution: This will wipe out your position infomation
      inline void BuildRotationQuat( const Quaternion &q ) { *this = mat4_cast( q ); }
      // equal to M = M * S
      inline void MultScale( const Vec3& scale ) { *this = glm::scale( *this, scale ); };
      

      inline void BuildProjection( float fovy, float aspect, float zNear, float zFar );

      static Mat4x4 GetTranslateMatrix( const Vec3& shift ) { Mat4x4 mat = g_Identity; mat.AddTranslation( shift ); return mat; }

      static Mat4x4 GetScaleMatrix( const Vec3& scale ) { Mat4x4 mat = g_Identity; mat.MultScale( scale ); return mat; }

      static Mat4x4 GetRotationMatrix( const Quaternion& quat ) { return quat.GetRotationMatrix(); }

      static Mat4x4 GetRotationMatrix( const float pitchRad, const float yawRad, const float rollRad ){  Mat4x4 mat = g_Identity; mat.BuildPitchYawRollRad( pitchRad, yawRad, rollRad ); return mat; }

      static Mat4x4 ViewMatrix( const Vec3& eye, const Vec3& center, const Vec3& up ) { return Mat4x4( glm::lookAt( eye, center, up ) ); }
      // Caution! this matrix is not the same as glm::lookat, the whole direction
      // is reversed
      static Mat4x4 LookAt( const Vec3& eye, const Vec3& center, const Vec3& up ){ return ViewMatrix( eye, 2.0f * eye - center, up ) ; }

   public: 
      static const Mat4x4 g_Identity;

   private:
      
   };

 inline Mat4x4 Quaternion::GetRotationMatrix( void ) const 
    {
    return glm::mat4_cast( *this ); 
    }

inline Vec3 Mat4x4::GetForward( void ) const
   {
   return Vec3( GetRow( 2 ) );
   }

inline Vec3 Mat4x4::GetRight( void ) const
   {
   return Vec3( -GetRow( 0 ) );
   }

inline Vec3 Mat4x4::GetUp( void ) const
   {
   return Vec3( GetRow( 1 ) );
   }

inline Vec3 Mat4x4::GetXFormDirection( const Vec3& vec ) const
   {
   Mat4x4 justRot = *this;
	justRot.SetToWorldPosition( Vec3( 0.f,0.f,0.f ) );
	Vec3 forward = justRot.Xform( vec );
	return forward;
   }

inline void Quaternion::Build44Matrix( const Mat4x4& mat ) { *this = glm::quat_cast( mat ); }

inline Vec3 Mat4x4::GetPitchYawRollRad( void ) const
{
   //Quaternion quat = this->GetQuaternion();
   //return quat.GetPitchYawRollRad();
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

inline Vec3 Mat4x4::GetPitchYawRollDeg() const
   {
   Vec3 vec = this->GetPitchYawRollRad();
   return Vec3( RADIANS_TO_DEGREES( vec.x ), RADIANS_TO_DEGREES( vec.y ), RADIANS_TO_DEGREES( vec.z ) );
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
    Color( const float red, const float green, const float blue, const float alpha = 1.0 );

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

    bool Init( TiXmlElement* pData );
    TiXmlElement* GenerateXML( void );
    TiXmlElement* GenerateOverridesXML( TiXmlElement* pResource );

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
      /**
       * @brief Return if a single point is inside the frustum, point should be in Frustum's local space
       *
       * @param  point const Vec3 & point
       * @return bool
       */
      bool Inside( const Vec3 &point ) const ;

      /**
       * @brief return if a shpere is inside the frustum, point should be in Frustum's local space
       *
       * @param  point const Vec3 & point
       * @param  radius float radius
       * @return bool
       */
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
       Transform( void ) : m_ToWorld( Mat4x4::g_Identity ), m_FromWorld( Mat4x4::g_Identity ), m_IsFromWorldDirty( false ) {  };
       Transform( const Mat4x4& toWorld );
       Transform( const Vec3& position, const Vec3& scale = Vec3::g_Identity, const Quaternion& rotation = Quaternion::g_Identity );
 
       Transform& operator *= ( const Transform& transform )
          {
          m_ToWorld = m_ToWorld * transform.GetToWorld();
          m_IsFromWorldDirty = true;
          return *this;
          };
 
       Transform operator * ( const Transform& transform ) const
          {
          return Transform ( *this ) *= transform;
          };
 
       Vec4 operator* ( const Vec4& vec ) const
          {
          return m_ToWorld * vec;
          }
 
       Vec3 operator*( const Vec3& vec ) const{ return Vec3( *this * Vec4( vec ) ); }
 
       inline Vec3 GetForward( void ) const;
       inline Vec3 GetRight( void ) const;
       inline Vec3 GetUp( void ) const;
 
       inline void SetTransform( const Mat4x4& toWorld );
 
       void SetPosition( const Vec3& pos ){ m_ToWorld.SetToWorldPosition( pos );  m_IsFromWorldDirty = true; }
       void SetPosition( float x, float y, float z ) { SetPosition( Vec3( x, y, z ) ); }
 
       inline void SetRotation( const Quaternion& quat );
       inline void SetRotMatrix( const Mat4x4& rotation );
 
       inline void SetScale( const Vec3& scale );
 
       inline void AddToWorldRotation( const Quaternion& quat );
       inline void AddFromWorldRotation( const Quaternion& quat );
 
       inline void SetPitchYawRollRad( const Vec3& pitchYawRollRad );
       inline void SetPitchYawRollRad( const float pitchRad, const float yawRad, const float rollRad );     
 
       inline void SetPitchYawRollDeg( const float pitchDeg, const float yawDeg, const float rollDeg );
       inline void SetPitchYawRollDeg( const Vec3& vec );
 
       inline void AddToWorldPitchYawRollRad( const Vec3& pitchYawRollRad );
       inline void AddToWorldPitchYawRollRad( const float pitchRad, const float yawRad, const float rollRad );
       inline void AddToWorldPitchYawRollDeg( const float pitchDeg, const float yawDeg, const float rollDeg );
       
       inline void AddFromWorldPitchYawRollRad( const Vec3& pitchYawRollRad );
       inline void AddFromWorldPitchYawRollRad( const float pitchRad, const float yawRad, const float rollRad );
       inline void AddFromWorldPitchYawRollDeg( const float pitchDeg, const float yawDeg, const float rollDeg );
 
       void AddToWorldPosition( const Vec3& shiftVec ){ SetPosition( GetToWorldPosition() + shiftVec ); }
       void AddFromWorldPosition( const Vec3& shiftVec ){ m_ToWorld.AddTranslation( shiftVec ); m_IsFromWorldDirty = true; }
 
       Mat4x4 GetToWorld( void ) const;
 
       Mat4x4 GetFromWorld( void );

       inline Mat4x4 GetRotationMatrix( void ) const ;
 
       Transform  Inverse( void ) { return Transform( GetFromWorld() ); }
 
       Quaternion GetQuaternion( void ) const { return m_ToWorld.GetQuaternion(); }
       
       Vec3       GetPitchYawRollRad( void ) const { return m_ToWorld.GetPitchYawRollRad(); }
       Vec3       GetPitchYawRollDeg( void ) const { return m_ToWorld.GetPitchYawRollDeg(); }
 
       Vec3       GetToWorldPosition( void ) const { return m_ToWorld.GetToWorldPosition(); }
 
       Vec3       GetScale( void ) const { return m_ToWorld.GetScale(); }
       
       bool Init( TiXmlElement* pData );
       TiXmlElement* GenerateXML( void );
       TiXmlElement* GenerateOverridesXML( TiXmlElement* pResourceNode );

    public:
       const static Transform g_Identity;
 
    private:
      inline void SetScaleRotTrans( const Vec3& scale, const Mat4x4& rotation, const Vec3& position );
      inline void UpdateFromWorld( void );

    private:
       Mat4x4 m_ToWorld;
       Mat4x4 m_FromWorld;
       bool   m_IsFromWorldDirty;
    };
 
 inline Vec3 Transform::GetForward( void ) const
    {
    return m_ToWorld.GetForward();
    }
 
 inline Vec3 Transform::GetRight( void ) const
    {
    return m_ToWorld.GetRight();
    }
 
 inline Vec3 Transform::GetUp( void ) const
    {
    return m_ToWorld.GetUp();
    }
 
 inline void Transform::SetTransform( const Mat4x4& toWorld )
    {
    m_ToWorld = toWorld;
    m_IsFromWorldDirty = true;
    }
 
 inline void Transform::SetRotation( const Quaternion& quat )
    {
    SetScaleRotTrans( GetScale(), quat.GetRotationMatrix(), GetToWorldPosition() );
    }

 inline void Transform::SetRotMatrix( const Mat4x4& rotation )
    {
    SetScaleRotTrans( GetScale(), rotation, GetToWorldPosition() );
    }
 
inline void Transform::SetScale( const Vec3& scale )
    {
    SetScaleRotTrans( scale, GetRotationMatrix(), GetToWorldPosition() ); 
    }


 //inline void Transform::AddToWorldRotation( const Quaternion& quat )
 //   {
 //   
 //   }
 //
 //inline void Transform::AddFromWorldRotation( const Quaternion& quat )
 //   {
 //  // this->SetRotation( m_Quat * quat );
 //   }
 
 inline void Transform::SetPitchYawRollRad( const float pitchRad, const float yawRad, const float rollRad )
    {
    //SetScaleRotTrans( m_ToWorld.GetScale(), Mat4x4::GetRotationMatrix( pitchRad, yawRad, rollRad ), m_ToWorld.GetToWorldPosition() );
    SetRotMatrix( Mat4x4::GetRotationMatrix( pitchRad, yawRad, rollRad ) );
    }

 inline void Transform::SetPitchYawRollRad( const Vec3& pitchYawRollRad )
    {
    SetPitchYawRollRad( pitchYawRollRad.x, pitchYawRollRad.y, pitchYawRollRad.z );
    } 
       
 inline void Transform::SetPitchYawRollDeg( const float pitchDeg, const float yawDeg, const float rollDeg )
    {
    SetPitchYawRollRad( DEGREES_TO_RADIANS( pitchDeg ), DEGREES_TO_RADIANS( yawDeg ), DEGREES_TO_RADIANS( rollDeg ) );
    }

 inline void Transform::SetPitchYawRollDeg( const Vec3& vec )
    { 
    SetPitchYawRollDeg( vec.x, vec.y, vec.z );
    }
 /*
 m_Quat * quat -> local rotation
 quat * m_Quat->Global rotation
 */
     
 inline void Transform::AddToWorldPitchYawRollRad( const float pitchRad, const float yawRad, const float rollRad )
    {
    SetRotMatrix( Mat4x4::GetRotationMatrix( pitchRad, yawRad, rollRad ) * GetRotationMatrix() );
    }

 inline void Transform::AddToWorldPitchYawRollRad( const Vec3& pitchYawRollRad )
    {
    AddToWorldPitchYawRollRad( pitchYawRollRad.x, pitchYawRollRad.y, pitchYawRollRad.z );
    }
 
 inline void Transform::AddToWorldPitchYawRollDeg( const float pitchDeg, const float yawDeg, const float rollDeg )
    {
    AddToWorldPitchYawRollRad( DEGREES_TO_RADIANS( pitchDeg ), DEGREES_TO_RADIANS( yawDeg ), DEGREES_TO_RADIANS( rollDeg ) );
    }

 inline void Transform::AddFromWorldPitchYawRollRad( const float pitchRad, const float yawRad, const float rollRad )
    {
    SetRotMatrix( GetRotationMatrix() * Mat4x4::GetRotationMatrix( pitchRad, yawRad, rollRad ) );
    }
 
 inline void Transform::AddFromWorldPitchYawRollRad( const Vec3& pitchYawRollRad )
    {
    AddFromWorldPitchYawRollRad( pitchYawRollRad.x, pitchYawRollRad.y, pitchYawRollRad.z );
   // this->SetScaleRotTrans( m_Quat * Quaternion( pitchYawRollRad ) );
    }
 
 inline void Transform::AddFromWorldPitchYawRollDeg( const float pitchDeg, const float yawDeg, const float rollDeg )
    {
    AddFromWorldPitchYawRollRad( DEGREES_TO_RADIANS( pitchDeg ), DEGREES_TO_RADIANS( yawDeg ), DEGREES_TO_RADIANS( rollDeg ) );
    }
       
 inline Mat4x4 Transform::GetToWorld( void ) const 
    {
    return m_ToWorld;
    }
 
////  inverse ( TRS )^-1 = S^-1 * R^-1 * T^-1
 inline Mat4x4 Transform::GetFromWorld( void )
    {
    UpdateFromWorld();
    return m_FromWorld;
    }

inline Mat4x4 Transform::GetRotationMatrix( void ) const
   {
   return m_ToWorld.GetRotation();
   }

inline void Transform::SetScaleRotTrans( const Vec3& scale, const Mat4x4& rotation, const Vec3& position )
   {
   m_ToWorld = rotation;// m = R
   m_ToWorld.MultScale( scale );// m = R S
   m_ToWorld.SetToWorldPosition( position );// m = T R S
   m_IsFromWorldDirty = true;
   }

inline void Transform::UpdateFromWorld( void )
   {
   if( m_IsFromWorldDirty )
      {
      m_FromWorld = m_ToWorld.Inverse();
      }
   m_IsFromWorldDirty = false;
   }

//class Transform
//   {
//   public:
//      Transform( void ) : m_Pos( Vec3::g_Zero ), m_Scale( Vec3::g_Identity ), m_Quat( Quaternion::g_Identity ) {  };
//      Transform( const Mat4x4& toWorld );
//      Transform( const Vec3& position, const Vec3& scale = Vec3::g_Identity, const Quaternion& rotation = Quaternion::g_Identity );
//
//      //	When Q = quaternion, S = single scalar scale, and T = translation
//	   //	QST(A) = Q(A), S(A), T(A), and QST(B) = Q(B), S(B), T(B)
//
//	   //	QST (AxB) 
//
//	   // QST(A)   = Q(A) * S(A) * P * -Q(A) + T(A)
//	   // QST(AxB) = Q(B) * S(B) * QST(A) * -Q(B) + T(B)
//	   // QST(AxB) = Q(B) * S(B) * [ Q(A) * S(A) * P * -Q(A) + T(A) ] * -Q(B) + T(B)
//	   // QST(AxB) = Q(B) * S(B) * Q(A) * S(A) * P *-Q(A) * -Q(B) + Q(B) * S(B) * T(A) * -Q(B) + T(B)
//	   // QST(AxB) = [Q(B)*Q(A)] * [ S(B) * S(A) ] * P * -[ Q(B) * Q(A) ] + Q(B) * S(B) * T(A) * -Q(B) + T(B)
//
//	   //	Q(AxB) = Q(B)*Q(A)
//	   //	S(AxB) = S(A)*S(B)
//	   //	T(AxB) = Q(B)*S(B)*T(A)*-Q(B) + T(B)
//
//      Transform& operator *= ( const Transform& transform )
//         {
//         SetRotation( m_Quat * transform.GetQuaternion() );
//         SetPosition( m_Pos + m_Quat * ( m_Scale * transform.GetPosition() ) );
//         SetScale( m_Scale * ( m_Quat * transform.GetScale() ) ); // not very sure whichi way is right
//         return *this;
//         };
//
//      Transform operator * ( const Transform& transform ) const { return Transform ( *this ) *= transform; };
//
//      Vec4 operator* ( const Vec4& vec ) const
//         {
//         Vec4 ret = vec;
//         ret = m_Scale * Vec3( ret );
//         ret = m_Quat * ret;
//         if( ret.w )
//            {
//            ret.x += m_Pos.x;
//            ret.y += m_Pos.y;
//            ret.z += m_Pos.z;
//            }
//         return ret;
//         }
//
//      Vec3 operator*( const Vec3& vec ) const{ return Vec3( *this * Vec4( vec ) ); }
//
//      inline Vec3 GetForward( void ) const;
//      inline Vec3 GetRight( void ) const;
//      inline Vec3 GetUp( void ) const;
//
//      inline void SetTransform( const Mat4x4& toWorld );
//
//      void SetPosition( const Vec3& position ){ m_Pos = position; }
//      void SetPosition( float x, float y, float z ) { SetPosition( Vec3( x, y, z ) ); }
//
//      inline void SetRotation( const Quaternion& quat );
//
//      void SetScale( const Vec3& scale ){ m_Scale = scale; }
//
//      inline void AddToWorldRotation( const Quaternion& quat );
//      inline void AddFromWorldRotation( const Quaternion& quat );
//
//      inline void SetPitchYawRollRad( const Vec3& pitchYawRollRad );
//      inline void SetPitchYawRollRad( const float pitchRad, const float yawRad, const float rollRad );     
//
//      inline void SetPitchYawRollDeg( const float pitchDeg, const float yawDeg, const float rollDeg );
//      void SetPitchYawRollDeg( const Vec3& vec ){ SetPitchYawRollDeg( vec.x, vec.y, vec.z ); }
//
//      inline void AddToWorldPitchYawRollRad( const Vec3& pitchYawRollRad );
//      inline void AddToWorldPitchYawRollRad( const float pitchRad, const float yawRad, const float rollRad );
//      inline void AddToWorldPitchYawRollDeg( const float pitchDeg, const float yawDeg, const float rollDeg );
//      
//      inline void AddFromWorldPitchYawRollRad( const Vec3& pitchYawRollRad );
//      inline void AddFromWorldPitchYawRollRad( const float pitchRad, const float yawRad, const float rollRad );
//      inline void AddFromWorldPitchYawRollDeg( const float pitchDeg, const float yawDeg, const float rollDeg );
//
//      void AddToWorldPosition( const Vec3& shiftVec ){ this->SetPosition( m_Pos + shiftVec ); }
//      void AddFromWorldPosition( const Vec3& shiftVec ){ this->AddToWorldPosition( m_Quat * shiftVec ); }
//
//      Mat4x4 GetToWorld( void ) const;
//
//      Mat4x4 GetFromWorld( void ) const;
//
//      Transform  Inverse( void ) const { return Transform( -m_Pos,  1.0f / m_Scale, m_Quat.Inverse() ); }
//
//      Quaternion GetQuaternion( void ) const { return m_Quat; }
//      Vec3       GetPitchYawRollRad( void ) const { return m_Quat.GetPitchYawRollRad(); }
//      Vec3       GetPitchYawRollDeg( void ) const { return m_Quat.GetPitchYawRollDeg(); }
//
//      Vec3       GetPosition( void ) const { return m_Pos; }
//
//      Vec3       GetScale( void ) const { return m_Scale; }
//
//   public:
//      const static Transform g_Identity;
//
//   private:
//
//   private:
//      Quaternion  m_Quat;
//      Vec3        m_Scale;
//      Vec3        m_Pos; // it is expressed in ToWorld location
//   };
//
//inline Vec3 Transform::GetForward( void ) const
//   {
//   return m_Quat * g_Forward;
//   }
//
//inline Vec3 Transform::GetRight( void ) const
//   {
//   return m_Quat * g_Right;
//   }
//
//inline Vec3 Transform::GetUp( void ) const
//   {
//   return m_Quat * g_Up;
//   }
//
//// deprecated
////inline void Transform::SetTransform( const Mat4x4& toWorld )
////   {
////   Mat4x4 temp( toWorld );
////   this->SetRotation( temp.GetQuaternion() );
////   this->SetPosition( toWorld.GetToWorldPosition() );
////   this->SetScale( toWorld.GetScale() );
////   }
//
//inline void Transform::SetRotation( const Quaternion& quat )
//   {
//   m_Quat = quat;
//   }
//
//inline void Transform::AddToWorldRotation( const Quaternion& quat )
//   {
//   this->SetRotation( quat * m_Quat );
//   }
//
//inline void Transform::AddFromWorldRotation( const Quaternion& quat )
//   {
//   this->SetRotation( m_Quat * quat );
//   }
//
//inline void Transform::SetPitchYawRollRad( const Vec3& pitchYawRollRad )
//   {
//   this->SetRotation( Quaternion( pitchYawRollRad ) );
//   } 
//
//inline void Transform::SetPitchYawRollRad( const float pitchRad, const float yawRad, const float rollRad )
//   {
//   this->SetPitchYawRollRad( Vec3( pitchRad, yawRad, rollRad ) );
//   } 
//      
//inline void Transform::SetPitchYawRollDeg( const float pitchDeg, const float yawDeg, const float rollDeg )
//   {
//   this->SetPitchYawRollRad( Vec3( DEGREES_TO_RADIANS( pitchDeg ), DEGREES_TO_RADIANS( yawDeg ), DEGREES_TO_RADIANS( rollDeg ) ) );
//   }
//     
///*
//m_Quat * quat -> local rotation
//quat * m_Quat->Global rotation
//*/
//    
//inline void Transform::AddToWorldPitchYawRollRad( const Vec3& pitchYawRollRad )
//   {
//   this->SetRotation( Quaternion( pitchYawRollRad ) * m_Quat );
//   }
//
//inline void Transform::AddToWorldPitchYawRollRad( const float pitchRad, const float yawRad, const float rollRad )
//   {
//   this->AddToWorldPitchYawRollRad( Vec3( pitchRad, yawRad, rollRad ) );
//   }
//
//inline void Transform::AddToWorldPitchYawRollDeg( const float pitchDeg, const float yawDeg, const float rollDeg )
//   {
//   this->AddToWorldPitchYawRollRad( Vec3( DEGREES_TO_RADIANS( pitchDeg ), DEGREES_TO_RADIANS( yawDeg ), DEGREES_TO_RADIANS( rollDeg ) ) );
//   }
//
//inline void Transform::AddFromWorldPitchYawRollRad( const Vec3& pitchYawRollRad )
//   {
//   this->SetRotation( m_Quat * Quaternion( pitchYawRollRad ) );
//   }
//
//inline void Transform::AddFromWorldPitchYawRollRad( const float pitchRad, const float yawRad, const float rollRad )
//   {
//   this->AddFromWorldPitchYawRollRad( Vec3( pitchRad, yawRad, rollRad ) );
//   }
//
//inline void Transform::AddFromWorldPitchYawRollDeg( const float pitchDeg, const float yawDeg, const float rollDeg )
//   {
//   this->AddFromWorldPitchYawRollRad( Vec3( DEGREES_TO_RADIANS( pitchDeg ), DEGREES_TO_RADIANS( yawDeg ), DEGREES_TO_RADIANS( rollDeg ) ) );
//   }
//      
//inline Mat4x4 Transform::GetToWorld( void ) const 
//   {
//   Mat4x4 toWorld( m_Quat.GetRotationMatrix() ); // m = R
//   toWorld.MultScale( m_Scale ); // m = R S
//   toWorld.SetToWorldPosition( m_Pos ); // m = T R S
//   return toWorld;
//   }
//
//// inverse ( TRS )^-1 = S^-1 * R^-1 * T^-1
// inline Mat4x4 Transform::GetFromWorld( void ) const 
//   {
//   Mat4x4 fromWorld( Mat4x4::g_Identity );
//   fromWorld.SetToWorldPosition( -m_Pos );  // m = T-1
//   fromWorld = m_Quat.Inverse().GetRotationMatrix() * fromWorld; // m = R-1 * T-1 
//   Vec4 invScale( 1.0f / m_Scale );
//   fromWorld[0] = fromWorld[0] * invScale;
//   fromWorld[1] = fromWorld[1] * invScale;
//   fromWorld[2] = fromWorld[2] * invScale;
//   fromWorld[3] = fromWorld[3] * invScale;
//   return fromWorld;
//   }

