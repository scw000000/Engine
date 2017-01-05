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
#include "SceneNodes.h"


enum AlphaType;

class Material
   {
   public:
      Material( void );
      bool Init( TiXmlElement* pData );
      void SetTextureResource( const Resource& newTextureRes );
      Resource GetTextureResource( void ) const { return m_TextureResource; };

      void SetAmbient( const Color& color );
	   Color GetAmbient( void ) const { return m_Ambient; }

	   void SetDiffuse( const Color &color );
	   Color GetDiffuse( void ) { return m_Diffuse; }

	   void SetSpecular( const Color &color, const float shininess );
      Color GetSpecular( void ) const { return m_Specular; }

	   void SetEmissive(const Color &color);
	   Color GetEmissive( void ) const { return m_Emissive; }

      void SetAlpha( const float alpha );
	   bool HasAlpha( void ) const { return GetAlpha() != fOPAQUE; }
	   float GetAlpha( void ) const { return m_Diffuse.GetAlpha(); }

	   void ApplyMaterial( void );

      TiXmlElement* GenerateXML( void );
      TiXmlElement* GenerateOverridesXML( TiXmlElement* pResource );

      void SetMeshIndex( unsigned int idx ) { m_MeshIndex = idx; }
      unsigned int GetMeshIndex( void ) const { return m_MeshIndex; }
   private:
      Color    m_Diffuse;       
      Color    m_Ambient;      
      Color    m_Specular;      
      Color    m_Emissive;      
      float    m_Shininess;     
      AlphaType m_AlphaType;
      unsigned int m_MeshIndex;
      Resource m_TextureResource;
   };