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

class MeshResourceExtraData : public IResourceExtraData
   {
	friend class SdkMeshResourceLoader;

   public:
      MeshResourceExtraData() : m_pScene( NULL ) { m_Radius = 0.f; };
	   virtual ~MeshResourceExtraData() { aiReleaseImport( m_pScene ); }
	   virtual std::string VToString() { return "MeshResourceExtraData"; }

   public:
      const struct aiScene* m_pScene;
      float m_Radius;
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
