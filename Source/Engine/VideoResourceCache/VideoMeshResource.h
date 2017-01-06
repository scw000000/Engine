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
         MeshBufferData_Num
         };
      std::vector< GLuint[ MeshBufferData_Num ] > m_BufferObjects;
      enum MeshCount
         {
         MeshCount_Vertex,
         MeshCount_Index,
         MeshCount_Num
         };
      std::vector< unsigned int[ MeshCount_Num ] > m_MeshCount;
      std::vector< float > m_Radius;
   };

//struct aiMaterial;

class VideoMeshResourceLoader : public VideoResourceLoader
   {
   public:
      VideoMeshResourceLoader( void );
      virtual int VLoadResource( shared_ptr<ResHandle> handle, shared_ptr< VideoResourceHandle > videoHandle ) override;
      static VideoMeshResourceExtraData* LoadAndReturnMeshResourceExtraData( const Resource& resource );
   private:
      shared_ptr< VideoResourceHandle > LoadTexture( aiMaterial* pMaterial, aiTextureType type, const std::string& filePath = "" ) const;
   };