#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: ResourceCache.h
////////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include <zlib.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "ZipFile.h"			// needed for ZipContentsMap

// Basic definition of a single resource description, ex: a textrue, a music file, etc.
// It only contains a name data for now
class Resource
   {
   public:
      std::string m_name;
      Resource( const std::string &name );
   };

class IResourceExtraData
   {
   public:
	   virtual std::string VToString()=0;
   };

// an expansion class of resrouce
class ResHandle
   {
   friend class ResCache;

   public:
      ResHandle( Resource &resource, char *buffer, unsigned int size, ResCache *pResCache );
      virtual ~ResHandle();

      unsigned int Size() const { return m_Size; }
      char *GetBuffer() const { return m_pBuffer; }
     // char *WritableBuffer() { return m_pBuffer; }
      shared_ptr<IResourceExtraData> GetExtraData() const { return m_Extra; }
      void SetExtra( shared_ptr<IResourceExtraData> extra ) { m_Extra = extra; }

   protected:
      Resource m_Resource;
      char *m_pBuffer;
      unsigned int m_Size;
      shared_ptr<IResourceExtraData> m_Extra;
      ResCache *m_pResCache;
   };

// This loader doesn't require further processing after decompressing zip file
class DefaultResourceLoader : public IResourceLoader
   {
   public:
	   virtual bool VUseRawFile() { return true; }
	   virtual bool VDiscardRawBufferAfterLoad() { return true; }
	   virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) { return rawSize; }
	   virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle) { return true; }
	   virtual std::string VGetPattern() { return "*"; }

   };

typedef std::list< shared_ptr< ResHandle > > ResHandleList;
typedef std::map < std::string, shared_ptr < ResHandle > > ResHandleMap;
typedef std::list< shared_ptr< IResourceLoader > > ResourceLoaders;

// This class holds all of resource loader and ptr to resource handle
// Also, this class holds resource file - a zip file with all assets
class ResCache
   {
   friend class ResHandle;

   public:
      ResCache( const unsigned int sizeInMb, IResourceFile *resFile );
      ~ResCache();

      bool Init();
      void RegisterLoader( shared_ptr< IResourceLoader > loader );

      shared_ptr< ResHandle > GetHandle( Resource *r );
      int Preload( const std::string pattern, void (*progressCallback)( int, bool & ) );
      void Flush( void );
      std::vector<std::string> Match( const std::string pattern );

   protected:
      shared_ptr< ResHandle > Find( Resource *resource );
      void Update( shared_ptr< ResHandle > handle  );
      shared_ptr< ResHandle > Load( Resource *resource );
      void Free( shared_ptr< ResHandle > gonner );

      bool MakeRoom( unsigned int size );
      char *Allocate( unsigned int size );
      void FreeOneResource();
      void MemoryHasBeenFreed( unsigned int size );

   protected:
      ResHandleList m_lruResHandleList; // least recently used list
      ResHandleMap m_Resources;
      ResourceLoaders m_ResourceLoaders;
      // this menber stores zip file 
      IResourceFile *m_file;

      unsigned int m_cacheSize; // size in bytes
      unsigned int m_allocated;
   };

class ResourceZipFile : public IResourceFile
   {
   public:
	   ResourceZipFile( const std::wstring resFileName ) { m_pZipFile = NULL; m_resFileName = resFileName; }
	   virtual ~ResourceZipFile();

	   virtual bool VOpen();
	   virtual int VGetRawResourceSize( const Resource &r );
	   virtual int VGetRawResource( const Resource &r, char *buffer );
	   virtual int VGetNumResources() const;
	   virtual std::string VGetResourceName( int num ) const;
      virtual bool VIsUsingDevelopmentDirectories( void ) const { return false; }
   
   private:
      ZipFile *m_pZipFile;
	   std::wstring m_resFileName;
   };
