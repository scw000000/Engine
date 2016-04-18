#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: MeshResouce.h
////////////////////////////////////////////////////////////////////////////////

class ObjMeshResourceExtraData : public IResourceExtraData
   {
	friend class SdkMeshResourceLoader;

   public:
	   ObjMeshResourceExtraData() { };
	   virtual ~ObjMeshResourceExtraData() { }
	   virtual std::string VToString() { return "ObjMeshResourceExtraData"; }
   };

class ObjMeshResourceLoader : public IResourceLoader
   {
   public:
	   virtual bool VUseRawFile() { return false; }
	   virtual bool VDiscardRawBufferAfterLoad() { return false; } // TODO: check if its right
	   virtual unsigned int VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize );
	   virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle);
	   virtual std::string VGetPattern() { return "*.obj"; }
   };
