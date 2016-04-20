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
// you should not derive this class, instead, you should 
// derive IResourceExtraData instead
class ResHandle
   {
   friend class ResCache;

   public:
      ResHandle( Resource &resource, char *buffer, unsigned int size, ResCache *pResCache );
      virtual ~ResHandle();

      unsigned int GetSize( void ) const { return m_Size; }
      void SetSize( unsigned size ){ m_Size = size; }
      char *GetBuffer() const { return m_pBuffer; }
     // char *WritableBuffer() { return m_pBuffer; }
      shared_ptr<IResourceExtraData> GetExtraData() const { return m_Extra; }
      void SetExtraData( shared_ptr<IResourceExtraData> extra ) { m_Extra = extra; }

   protected:
      Resource m_Resource;
      char *m_pBuffer;
      unsigned int m_Size;
      shared_ptr<IResourceExtraData> m_Extra;
      ResCache *m_pResCache;
   };

class ResourceLoader : public IResourceLoader
   {
   public:
      ResourceLoader( std::vector< std::string > patterns ) : m_Patterns( patterns ) { };
	   virtual bool VUseRawFile() = 0;
	   virtual bool VDiscardRawBufferAfterLoad() = 0;
	   virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize)  = 0;
	   virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle) = 0;
	   virtual const std::vector< std::string >& VGetPattern( void ) override { return m_Patterns; }
      virtual bool VUsePreAllocate( void ) override = 0;
      virtual bool VIsPatternMatch( const char* str ) override ;

   protected:
      std::vector< std::string > m_Patterns;
   };


// This loader doesn't require further processing after decompressing zip file
// It loads pure raw file into ResHandle
class DefaultResourceLoader : public ResourceLoader
   {
   public:
      DefaultResourceLoader( void );
	   virtual bool VUseRawFile() override { return true; }
	   virtual bool VDiscardRawBufferAfterLoad() { return true; }
	   virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) override { return rawSize; }
	   virtual bool VLoadResource (char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle) override { return true; }
      virtual bool VUsePreAllocate( void ) override { return true; }
      virtual bool VAddNullZero( void ) override { return true; }
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
      bool Allocate( char** pAllocBuffer, unsigned int size, bool useRealAllocation );
      char *PostAllocate( unsigned int size );
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
