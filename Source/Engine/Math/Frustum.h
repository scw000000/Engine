#pragma once
/*!
 * \file Frustum.h
 * \date 2016/10/29 11:53
 *
 * \author scw00
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

class IFrustum
   {
   public:
      virtual ~IFrustum( void ) { }
      /**
      * @brief Return if a single point is inside the frustum, point should be in Frustum's local space
      *
      * @param  point const Vec3 & point
      * @return bool
      */
      virtual bool VInside( const Vec3 &point ) const = 0;

      /**
      * @brief return if a shpere is inside the frustum, point should be in Frustum's local space
      *
      * @param  point const Vec3 & point
      * @param  radius float radius
      * @return bool
      */
      virtual bool VInside( const Vec3 &point, float radius ) const = 0;
   };

class BaseFrustum : public IFrustum
   {
   public:
      virtual bool VInside( const Vec3 &point ) const override;
      virtual bool VInside( const Vec3 &point, float radius ) const override;

   protected:
      enum Side { Near, Far, Top, Right, Bottom, Left, NumPlanes };
      
      Plane m_Planes[ NumPlanes ];
      // represent four vertices for near plane rectangle
      Vec3 m_NearPlaneVerts[ 4 ];
      // represent four vertices for far plane rectangle 
      Vec3 m_FarPlaneVerts[ 4 ];
   };

class PerspectiveFrustum : public BaseFrustum
   {
   public:
      PerspectiveFrustum( void );
      void Init( const float fov, const float aspect, const float nearClipDis, const float farClipDis );

      void SetFOV( float fov ) { m_Fov = fov; Init( m_Fov, m_Aspect, m_NearDis, m_FarDis ); }
      void SetAspect( float aspect ) { m_Aspect = aspect; Init( m_Fov, m_Aspect, m_NearDis, m_FarDis ); }
      void SetNear( float nearClip ) { m_NearDis = nearClip; Init( m_Fov, m_Aspect, m_NearDis, m_FarDis ); }
      void SetFar( float farClip ) { m_FarDis = farClip; Init( m_Fov, m_Aspect, m_NearDis, m_FarDis ); }

   public:
      // field of view in radians
      float m_Fov;
      // width divided by height
      float m_Aspect;
      // near plane distance
      float m_NearDis;
      // far plane distance
      float m_FarDis;

   };