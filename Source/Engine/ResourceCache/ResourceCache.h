#pragma once
/*!
 * \file ResourceCache.h
 * \date 2016/05/19 19:00
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

#include <zlib.h>
#include "ZipFile.h"			// needed for ZipContentsMap

// Basic definition of a single resource description, ex: a textrue, a music file, etc.
// It only contains a name data for now
class Resource
   {
   public:
      Resource( const std::string &name, bool caseSensitive = false );
      std::string GetExtension( void );
      std::string GetFileName( void );
      bool Init( TiXmlElement* pData, bool caseSensitive = false );
      TiXmlElement* GenerateXML( void );
      TiXmlElement* GenerateOverridesXML( TiXmlElement* pResource );

   public:
      std::string m_Name;
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
   friend class ResourceCache;

   public:
      ResHandle( const Resource &resource, char *buffer, unsigned int size, ResourceCache *pResCache );
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
      ResourceCache *m_pResCache;
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
class ResourceCache
   {
   friend class ResHandle;

   public:
      ResourceCache( const unsigned int sizeInMb, IResourceFile *resFile );
      ~ResourceCache();

      bool Init();
      void RegisterLoader( shared_ptr< IResourceLoader > loader );

      shared_ptr< ResHandle > GetHandle( Resource *r );
      int Preload( const std::string pattern, void (*progressCallback)( int, bool & ) );
      void Flush( void );
      bool IsUsingDevelopmentDirectories( void ) const { ENG_ASSERT( m_pResourceFile ); return m_pResourceFile->VIsUsingDevelopmentDirectories(); }
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
      IResourceFile *m_pResourceFile;

      unsigned int m_CacheSize; // size in bytes
      unsigned int m_AllocatedSize;
   };

class ResourceZipFile : public IResourceFile
   {
   public:
	   ResourceZipFile( const std::wstring resFileName ) { m_pZipFile = NULL; m_ResFileName = resFileName; }
	   ~ResourceZipFile();

	   virtual bool VOpen() override;
      virtual int VGetRawResourceSize( const Resource &r ) override;
      virtual int VGetRawResource( const Resource &r, char *buffer ) override;
      virtual int VGetNumResources() const override;
      virtual std::string VGetResourceName( int num ) const override;
      virtual bool VIsUsingDevelopmentDirectories( void ) const { return false; }
   
   private:
      ZipFile *m_pZipFile;
	   std::wstring m_ResFileName;
   };
