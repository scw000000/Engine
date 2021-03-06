#pragma once
/*!
 * \file MeshResource.h
 * \date 2016/06/25 16:41
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

#include "assimp/cimport.h"
#include "assimp/scene.h"

#define MAXIMUM_BONES_PER_ACTOR 200
#define MAXIMUM_BONES_PER_VEREX 4

struct aiScene;

extern Mat4x4 aiMat4x4ToMat4( aiMatrix4x4 aiMat44 );

class BoneData
   {
   public:
      BoneData( void ) {}
      BoneData( const aiMatrix4x4& offset, BoneId id );

   public:
      // transform vertices from model space to bone space
      aiMatrix4x4 m_BoneOffset;
      BoneId m_BoneId;
      private:
   };

typedef std::unordered_map< std::string, BoneData > BoneMappingData;

class MeshResourceExtraData : public IResourceExtraData
   {
   friend class MeshResourceLoader;

   public:
      MeshResourceExtraData( unsigned int meshNum );
      virtual ~MeshResourceExtraData() { aiReleaseImport( m_pScene ); }
      //virtual std::string VToString() { return "MeshResourceExtraData"; }
      aiAnimation* FindAnimation( const std::string& animationName ) const;
      aiAnimation* FindAnimation( unsigned int animIdx ) const;

   public:
      const struct aiScene* m_pScene;
      unsigned int m_NumVertexIndex;
      std::vector<BoneMappingData> m_BoneMappingData;
      aiMatrix4x4  m_GlobalInverseTransform;

   protected:
      void LoadBones( void );
   };


class MeshResourceLoader : public ResourceLoader
   {
   public:
      MeshResourceLoader( void );
      virtual bool VUseRawFile() const override { return false; }
      virtual bool VDiscardRawBufferAfterLoad() const override { return true; }
	   virtual unsigned int VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize ) override ;
	   virtual int VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle ) override ;
      virtual bool VUsePreAllocate( void ) const override { return false; }
      static const aiScene* LoadAndReturnScene( shared_ptr< Resource > pResource );
   };
