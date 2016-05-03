#pragma once
/*!
 * \file Material.h
 * \date 2016/05/01 9:09
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


class Material
   {
   public:
      Material( void );

      void SetTextureResource( const Resource& newTextureRes );
      Resource GetTextureResource( void ) const { return m_TextureResource; };

      void SetAmbient( const Color& color );
	   Color GetAmbient() const { return m_Ambient; }

	   void SetDiffuse( const Color &color );
	   Color GetDiffuse() { return m_Diffuse; }

	   void SetSpecular( const Color &color, const float shininess );
	   void GetSpecular( Color &_color, float &_power ) { _color = m_Specular; _power = m_Shininess; }

	   void SetEmissive(const Color &color);
	   Color GetEmissive() const { return m_Emissive; }

	   void SetAlpha(const float alpha);
	   bool HasAlpha( void ) const { return GetAlpha() != fOPAQUE; }
	   float GetAlpha( void ) const { return m_Diffuse.GetAlpha(); }

	   void ApplyMaterial( void );

   private:
      Resource m_TextureResource;
      Color    m_Diffuse;       
      Color    m_Ambient;      
      Color    m_Specular;      
      Color    m_Emissive;      
      float    m_Shininess;         
   };