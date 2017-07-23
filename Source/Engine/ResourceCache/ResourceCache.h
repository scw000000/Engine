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
      Resource( const std::string &name = "", bool caseSensitive = false );
      std::string GetExtension( void ) const;
      std::string GetFileName( void ) const;
      std::string GetPath( void ) const;
      virtual bool VInit( TiXmlElement* pData, bool caseSensitive = false );
      virtual TiXmlElement* VGenerateXML( void );
      virtual TiXmlElement* VGenerateOverridesXML( TiXmlElement* pResource );
      ~Resource( void ) {};
   public:
      std::string m_Name;
   };

// an expansion class of resource
// you should not derive this class, instead, you should 
// derive IResourceExtraData instead
class ResHandle
   {
   friend class ResourceCache;

   public:
      ResHandle( shared_ptr< Resource > pResource, char *buffer, unsigned int size, ResourceCache *pResCache );
      virtual ~ResHandle();

      unsigned int GetSize( void ) const { return m_Size; }
      void SetSize( unsigned size ){ m_Size = size; }
      char *GetBuffer() const { return m_pBuffer; }
     // char *WritableBuffer() { return m_pBuffer; }
      shared_ptr<IResourceExtraData> GetExtraData() const { return m_pExtraData; }
      void SetExtraData( shared_ptr<IResourceExtraData> extra ) { m_pExtraData = extra; }
      shared_ptr< Resource > GetResource( void ) const { return m_pResource; }

   protected:
      shared_ptr< Resource > m_pResource;
      char *m_pBuffer;
      unsigned int m_Size;
      shared_ptr<IResourceExtraData> m_pExtraData;
      ResourceCache *m_pResCache;
   };

class ResourceLoader : public IResourceLoader
   {
   public:
      ResourceLoader( std::vector< std::string > patterns ) : m_Patterns( patterns ) {};
      virtual unsigned int VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize ) { return 0; };
      virtual const std::vector< std::string >& VGetPattern( void ) const override { return m_Patterns; }
      virtual bool VIsPatternMatch( const char* str ) const override;

   protected:
      std::vector< std::string > m_Patterns;
   };


// This loader doesn't require further processing after decompressing zip file
// It loads pure raw file into ResHandle
class DefaultResourceLoader : public ResourceLoader
   {
   public:
      DefaultResourceLoader( void );
      virtual bool VUseRawFile() const override { return true; }
	   virtual bool VDiscardRawBufferAfterLoad() const override { return true; }
	   virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) override { return rawSize; }
      virtual int VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle ) override { return S_OK; }
      virtual bool VUsePreAllocate( void ) const override { return true; }
      virtual bool VAddNullZero( void ) const override { return true; }
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

      int Init();
      // Note that the DefaultResourceLoader is last one in the list, so any loader will match before it if the file format is supported
      template< typename T > void RegisterLoader( void );

      shared_ptr< ResHandle > GetHandle( shared_ptr< Resource > pResource );
      int Preload( const std::string pattern, void (*progressCallback)( int, bool & ) );
      void Flush( void );
      bool IsUsingDevelopmentDirectories( void ) const { ENG_ASSERT( m_pResourceFile ); return m_pResourceFile->VIsUsingDevelopmentDirectories(); }
      std::vector<std::string> Match( const std::string pattern );
      bool IsFileExist( shared_ptr< Resource > pResource );

   protected:
      shared_ptr< ResHandle > Find( shared_ptr< Resource > pResource );
      void Update( shared_ptr< ResHandle > handle  );
      shared_ptr< ResHandle > Load( shared_ptr<Resource > pResource );
      void Free( shared_ptr< ResHandle > gonner );

      bool MakeRoom( unsigned int size );
      bool Allocate( char** pAllocBuffer, unsigned int size, bool useRealAllocation );
      char *PostAllocate( unsigned int size );
      void FreeOneResource();
      void MemoryHasBeenFreed( unsigned int size );

   protected:
      ResHandleList m_lruResHandleList; // least recently used list
      ResHandleMap m_Resources; // map file name to resource handle to speed up searching, instead of finding in handle list
      ResourceLoaders m_ResourceLoaders;
      // this menber stores zip file 
      IResourceFile *m_pResourceFile;

      unsigned int m_CacheSize; // size in bytes
      unsigned int m_AllocatedSize;
   };

template< typename T > void ResourceCache::RegisterLoader( void )
   {
   if( !std::is_base_of< IResourceLoader, T >::value )
      {
      ENG_ERROR( "Invalid implementation register" );
      return;
      }

   m_ResourceLoaders.push_front( shared_ptr< IResourceLoader >( ENG_NEW T() )  );
   }

class ResourceZipFile : public IResourceFile
   {
   public:
	   ResourceZipFile( const std::wstring resFileName ) { m_pZipFile = NULL; m_ResFileName = resFileName; }
	   ~ResourceZipFile();

	   virtual bool VOpen() override;
      virtual int VGetRawResourceSize( shared_ptr< Resource > pResource ) override;
      virtual int VGetRawResource( shared_ptr< Resource > pResource, char *buffer ) override;
      virtual int VGetNumResources() const override;
      virtual std::string VGetResourceName( int num ) const override;
      virtual bool VIsUsingDevelopmentDirectories( void ) const { return false; }
   
   private:
      ZipFile *m_pZipFile;
	   std::wstring m_ResFileName;
   };
