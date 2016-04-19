#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: MeshResouce.h
////////////////////////////////////////////////////////////////////////////////

#include <assimp/cimport.h>
#include <assimp/scene.h>

struct aiScene;

class MeshResourceExtraData : public IResourceExtraData
   {
	friend class SdkMeshResourceLoader;

   public:
	   MeshResourceExtraData() : m_pScene( NULL ) { };
	   virtual ~MeshResourceExtraData() { aiReleaseImport( m_pScene ); }
	   virtual std::string VToString() { return "MeshResourceExtraData"; }

   public:
      const struct aiScene* m_pScene;
   };

// TODO: expand this loader to obkj loader
class MeshResourceLoader : public IResourceLoader
   {
   public:
	   virtual bool VUseRawFile() override { return false; }
	   virtual bool VDiscardRawBufferAfterLoad() override { return false; } // TODO: check if its right
	   virtual unsigned int VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize ) override ;
	   virtual bool VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle ) override ;
      virtual bool VUsePreAllocate( void ) override { return false; }
	   virtual std::string VGetPattern() = 0;
   };

class ObjMeshResourceLoader : public MeshResourceLoader
   {
   public:
	   virtual std::string VGetPattern( void ) override { return "*.obj"; }
   };
