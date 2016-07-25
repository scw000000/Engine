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

#include <assimp/cimport.h>
#include <assimp/scene.h>

struct aiScene;

extern Transform aiMat4x4ToTransform( const aiMatrix4x4& aiMat44 );
typedef unsigned int BoneId;

class BoneData
   {
   public:
      BoneData( const Transform& transform, BoneId id ) { m_BoneOffset = transform;  m_BoneId = id; }

   public:
      Transform m_BoneOffset;
      BoneId m_BoneId;
   private:
   };

typedef std::unordered_map< std::string, BoneData > BoneMappingData;

class MeshResourceExtraData : public IResourceExtraData
   {
   friend class MeshResourceLoader;

   public:
      MeshResourceExtraData() : m_pScene( NULL ) { m_Radius = 0.f; m_NumVertices = 0; m_NumVertexIndex = 0; m_NumBones = 0; };
	   virtual ~MeshResourceExtraData() { aiReleaseImport( m_pScene ); }
	   virtual std::string VToString() { return "MeshResourceExtraData"; }

   public:
      const struct aiScene* m_pScene;
      float m_Radius;
      unsigned int m_NumVertices;
      unsigned int m_NumVertexIndex;
      unsigned int m_NumBones;
      BoneMappingData m_BoneMappingData;

   protected:
      void LoadBones( void );
   };


class MeshResourceLoader : public ResourceLoader
   {
   public:
      MeshResourceLoader( void );
	   virtual bool VUseRawFile() override { return false; }
	   virtual bool VDiscardRawBufferAfterLoad() override { return true; }
	   virtual unsigned int VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize ) override ;
	   virtual bool VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle ) override ;
      virtual bool VUsePreAllocate( void ) override { return false; }
      static const aiScene* LoadAndReturnScene( const Resource& resource );
   };
