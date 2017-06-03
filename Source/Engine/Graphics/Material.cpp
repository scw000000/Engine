/*!
 * \file Material.cpp
 * \date 2016/05/19 19:49
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
#include "Material.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"

Material::Material( void ) : m_pDiffuseTextureRes( ENG_NEW VideoTextureResource( "art\\default.jpg", false, true ) )
   {
   m_Diffuse = g_White;        
   m_Ambient = Color( 0.20f, 0.20f, 0.20f, 1.0f );       
   m_Specular = g_White;       
   m_Emissive = g_Black;      
   m_Shininess = 0.f;          
   m_AlphaType = AlphaOpaque;
   m_MeshIndex = 0;
   }

Material::Material( const aiScene* pAiScene, unsigned int meshIndex, const std::string& filePath ) :
   m_MeshIndex( meshIndex )
   {
   ENG_ASSERT( meshIndex < pAiScene->mNumMeshes );
   auto pAiMesh = pAiScene->mMeshes[ meshIndex ];
   auto pAiMateral = pAiScene->mMaterials[ pAiMesh->mMaterialIndex ];

   unsigned int typeCount = pAiMateral->GetTextureCount( aiTextureType_DIFFUSE );
   ENG_ASSERT( typeCount <= 1 );
   if( typeCount > 0 )
      {
      aiString relFilePath;
      pAiMateral->GetTexture( aiTextureType_DIFFUSE, 0, &relFilePath );
      std::string fullPath( filePath );
      fullPath.append( relFilePath.C_Str() );
      m_pDiffuseTextureRes = shared_ptr< VideoTextureResource >( ENG_NEW VideoTextureResource( fullPath, false, true ) );
      }

   typeCount = pAiMateral->GetTextureCount( aiTextureType_NORMALS );
   ENG_ASSERT( typeCount <= 1 );
   if( typeCount > 0 )
      {
      aiString relFilePath;
      pAiMateral->GetTexture( aiTextureType_NORMALS, 0, &relFilePath );
      std::string fullPath( filePath );
      fullPath.append( relFilePath.C_Str() );
      m_pNormalTextureRes = shared_ptr< VideoTextureResource >( ENG_NEW VideoTextureResource( fullPath, false, false ) );
      }
   }

bool Material::Init( TiXmlElement* pData )
   {
   if( !pData )
      {
      return false;
      }
   m_pDiffuseTextureRes->VInit( pData->FirstChildElement( "Texture" ) );

   m_Diffuse.Init( pData->FirstChildElement( "Diffuse" ) );

   m_Ambient.Init( pData->FirstChildElement( "Ambient" ) );

   return true;
   }

void Material::SetTextureResource( shared_ptr< VideoTextureResource > pResource )
   {
   m_pDiffuseTextureRes = pResource;
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
   m_AlphaType = AlphaType::AlphaMaterial;
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

TiXmlElement* Material::GenerateXML( void )
   {
   TiXmlElement* pRetNode = ENG_NEW TiXmlElement( "Material" );

   TiXmlElement* pTexture = m_pDiffuseTextureRes->VGenerateXML();
   pTexture->SetValue( "Texture" );
   pRetNode->LinkEndChild( pTexture );

   TiXmlElement* pDiffuse = m_Diffuse.GenerateXML();
   pDiffuse->SetValue( "Diffuse" );
   pRetNode->LinkEndChild( pDiffuse );

   TiXmlElement* pAmbient = m_Ambient.GenerateXML();
   pAmbient->SetValue( "Ambient" );
   pRetNode->LinkEndChild( pAmbient );

   return pRetNode;
   }

TiXmlElement* Material::GenerateOverridesXML( TiXmlElement* pResource )
   {
   TiXmlElement* pRetNode = ENG_NEW TiXmlElement( "Material" );
   
   TiXmlElement* pTexture = m_pDiffuseTextureRes->VGenerateOverridesXML( pResource->FirstChildElement( "Texture" ) );
   pTexture->SetValue( "Texture" );
   pRetNode->LinkEndChild( pTexture );

   TiXmlElement* pDiffuse = m_Diffuse.GenerateOverridesXML( pResource->FirstChildElement( "Diffuse" ) );
   pDiffuse->SetValue( "Diffuse" );
   pRetNode->LinkEndChild( pDiffuse );

   TiXmlElement* pAmbient = m_Ambient.GenerateOverridesXML( pResource->FirstChildElement( "Ambient" ) );
   pAmbient->SetValue( "Ambient" );
   pRetNode->LinkEndChild( pAmbient );

   return pRetNode;
   
   }

