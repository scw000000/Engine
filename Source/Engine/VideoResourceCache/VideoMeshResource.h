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
      VideoMeshResourceExtraData( void ) { ENG_ZERO_MEM( m_MeshBuffers ); }
      ~VideoMeshResourceExtraData( void );

   public:
      enum MeshBufferData
         {
         MeshBufferData_Vretex,
         MeshBufferData_Normal,
         MeshBufferData_UV,
         MeshBufferData_Index,
         MeshBufferData_Num
         };
      GLuint m_MeshBuffers[ MeshBufferData_Num ];
   };

class VideoMeshResourceLoader : public VideoResourceLoader
   {
      public:
      VideoMeshResourceLoader( void );
      virtual int VLoadResource( shared_ptr<ResHandle> handle, shared_ptr< VideoResourceHandle > videoHandle ) override;
    //  static shared_ptr< VideoMeshResourceExtraData > LoadAndReturnTextureObject( const Resource& resource );
   };