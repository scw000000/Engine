////////////////////////////////////////////////////////////////////////////////
// Filename: Material.cpp  
////////////////////////////////////////////////////////////////////////////////

#include "EngineStd.h"
#include "Material.h"

Material::Material( void ) : m_TextureResource( "art\\default.jpg" )
   {
   m_Diffuse = g_White;        
   m_Ambient = Color( 0.10f, 0.10f, 0.10f, 1.0f );       
   m_Specular = g_White;       
   m_Emissive = g_Black;      
   m_Shininess = 0.f;          
   }

void Material::SetTextureResource( const Resource& newTexture )
   {
   m_TextureResource = newTexture;
   }

void Material::SetAmbient( const Color &color )
   {
	m_Ambient = color;
   }

void Material::SetDiffuse( const Color &color )
   {
	m_Diffuse = color;
   }  

void Material::SetSpecular( const Color &color, const float shininess )
   {
	m_Specular = color;
	m_Shininess = shininess;
   }

void Material::SetEmissive( const Color &color )
   {
	m_Emissive = color;
   }

void Material::SetAlpha( float alpha )
   {
	m_Diffuse.SetAlpha( alpha );
   }

void Material::ApplyMaterial( void )
   {
	glMaterialfv( GL_FRONT, GL_AMBIENT, m_Ambient );
   glMaterialfv( GL_FRONT, GL_DIFFUSE, m_Diffuse );
   glMaterialfv( GL_FRONT, GL_SPECULAR, m_Specular );
   glMaterialfv( GL_FRONT, GL_EMISSION, m_Emissive );

   glMaterialfv( GL_FRONT, GL_SHININESS, &m_Shininess ); 
   }

