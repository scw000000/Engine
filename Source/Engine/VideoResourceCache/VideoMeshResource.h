#pragma once
/*!
 * \file VideoMeshResource.h
 * \date 2017/01/02 14:31
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
#include "VideoResourceCache.h"
#include "assimp/material.h"
#include "../../ResourceCache/MeshResource.h"

struct BoneDataOfVertex
   {
      public:
      BoneDataOfVertex( void )
         {
         ENG_ZERO_MEM( m_BoneIDs );
         ENG_ZERO_MEM( m_BoneWeights );
         }
      void SetBoneData( BoneId boneID, float boneWeight, int idx );

      public:
      BoneId m_BoneIDs[ MAXIMUM_BONES_PER_VEREX ];
      float m_BoneWeights[ MAXIMUM_BONES_PER_VEREX ];
   };

class VideoMeshResourceExtraData : public IVideoResourceExtraData
   {
   public:
   VideoMeshResourceExtraData( unsigned int meshCount );
      ~VideoMeshResourceExtraData( void );

   public:
      enum MeshBufferData
         {
         MeshBufferData_Vertex,
         MeshBufferData_Normal,
         MeshBufferData_UV,
         MeshBufferData_Tangent,
         MeshBufferData_Bitangent,
         MeshBufferData_Index,
         MeshBufferData_BoneData,
         MeshBufferData_Num
         };
      std::vector< GLuint[ MeshBufferData_Num ] > m_BufferObjects;
      enum MeshCount
         {
         MeshCount_Vertex,
         MeshCount_Index,
         MeshCount_Bone,
         MeshCount_Num
         };
      std::vector< unsigned int[ MeshCount_Num ] > m_MeshCount; // Stores # of it's data per mesh
      std::vector< float > m_Radius;
   };

class VideoMeshResourceLoader : public VideoResourceLoader
   {
   public:
      VideoMeshResourceLoader( void );
      virtual int VLoadResource( shared_ptr<ResHandle> handle, shared_ptr< VideoResourceHandle > videoHandle ) override;
      static VideoMeshResourceExtraData* LoadAndReturnMeshResourceExtraData( shared_ptr< Resource > pResource );
   private:
      shared_ptr< VideoResourceHandle > LoadTexture( aiMaterial* pMaterial, aiTextureType type, const std::string& filePath = "" ) const;
   };