/*!
 * \file ResourceCache.cpp
 * \date 2016/05/19 19:01
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

#include "EngineStd.h"
#include "ResourceCache.h"

#include <list>
#include <map>
#include <cctype>			// for std::tolower


// before restore the file name, trans from the alphabets to lowercase to prevent case-sensitive situation
Resource::Resource( const std::string &name, bool caseSensitive )
   {
   m_Name = name;
   if( !caseSensitive )
      {
      std::transform( m_Name.begin(), m_Name.end(), m_Name.begin(), ( int(*)(int) )std::tolower );
      }
   }

std::string Resource::GetExtension( void )
   {
   size_t extensionStart = m_Name.find_last_of( '.' );
   ENG_ASSERT( extensionStart != std::string::npos );
   return m_Name.substr( extensionStart + 1 );
   }

bool Resource::Init( TiXmlElement* pData, bool caseSensitive )
   {
   if( !pData || !pData->Attribute( "path" ) )
      {
      return false;
      }
   m_Name = pData->Attribute( "path" );
   if( !caseSensitive )
      {
      std::transform( m_Name.begin(), m_Name.end(), m_Name.begin(), ( int( *)( int ) )std::tolower );
      }
   return true;
   }

TiXmlElement* Resource::GenerateXML( void )
   {
   TiXmlElement* pRetNode = ENG_NEW TiXmlElement( "Resource" );
   pRetNode->SetAttribute( "path", m_Name.c_str() );
   return pRetNode;
   }

TiXmlElement* Resource::GenerateOverridesXML( TiXmlElement* pResource )
   {
   TiXmlElement* pRetNode = GenerateXML();
   if( !strcmp( pRetNode->Attribute( "path" ), pResource->Attribute( "path" ) ) )
      {
      pRetNode->RemoveAttribute( "path" );
      }
   return pRetNode;
   }

ResHandle::ResHandle( Resource &resource, char *buffer, unsigned int size, ResourceCache *pResCache ) : m_Resource( resource )
   {
   m_pBuffer = buffer;
	m_Size = size;
	m_Extra = NULL;
	m_pResCache = pResCache;
   }

// The destructor truely free the allocated memory to resource cache
ResHandle::~ResHandle()
   {
   SAFE_DELETE_ARRAY( m_pBuffer );
   m_pResCache->MemoryHasBeenFreed( m_Size );
   }

bool ResourceLoader::VIsPatternMatch( const char* str )  
   {
   for( auto pattern : m_Patterns )
      {
      if( WildcardMatch( pattern.c_str(), str ) )
         {
         return true;
         }
      }
   return false;
   }

DefaultResourceLoader::DefaultResourceLoader( void ) : ResourceLoader( std::vector< std::string >( 1, "*" ) )
   {

   }

ResourceCache::ResourceCache( const unsigned int sizeInMb, IResourceFile *resFile )
   {
   m_CacheSize = sizeInMb * 1024 * 1024;
   m_AllocatedSize = 0;
   m_pResourceFile = resFile;
   }

// delete all of the resources handle in the list
ResourceCache::~ResourceCache()
   {
   while( !m_lruResHandleList.empty() )
      {
      FreeOneResource();
      }
      SAFE_DELETE( m_pResourceFile );
   }

bool ResourceCache::Init()
   {
   bool ret = false;
   if( m_pResourceFile->VOpen() )
      {
      RegisterLoader( shared_ptr< IResourceLoader > ( ENG_NEW DefaultResourceLoader() ) );
      ret = true;
      }
   return ret;
   }

// The DefaultResourceLoader is last one in the list, so any loader will match it before it
void ResourceCache::RegisterLoader( shared_ptr< IResourceLoader > loader )
   {
   m_ResourceLoaders.push_front( loader );
   }

// This function is callled by ResCache::preload to 
shared_ptr< ResHandle > ResourceCache::GetHandle( Resource *resource )
   {
   shared_ptr< ResHandle > handle( Find( resource ) );
   // if the handle is not loaded yet ( cannot find the resource handle map ), then load it
   if( !handle )
      {
      handle = Load( resource );
      }
   else // It exist, update its order in the least recently used list
      {
      Update( handle );
      }
   return handle;
   }

// preload specific type of resource
int ResourceCache::Preload( const std::string pattern, void (*progressCallback)( int, bool & ) )
   {
   if( !m_pResourceFile )
      {
      return 0;
      }
   int numFiles = m_pResourceFile->VGetNumResources();
   int loaded = 0;
   bool cancel = false;
   // for all of files in the zip file
   for( int i = 0; i < numFiles; ++i )
      {
      // try to get every file name in the zip file
      Resource resource( m_pResourceFile->VGetResourceName( i ) );
      // load the file if pattern is matched
      if( WildcardMatch( pattern.c_str(), resource.m_Name.c_str() ) )
         {
         shared_ptr< ResHandle > handle = g_pApp->m_pResCache->GetHandle( &resource ); // use resource cache to load resource
         ++loaded;
         }
      if( progressCallback )
         {
         progressCallback( i * 100 / numFiles, cancel );
         }
      }
   return loaded;
   }

//
// ResCache::Flush									- not described in the book
//
//    Frees every handle in the cache - this would be good to call if you are loading a new
//    level, or if you wanted to force a refresh of all the data in the cache - which might be 
//    good in a development environment.
//
void ResourceCache::Flush( void )
   {
   while ( !m_lruResHandleList.empty())
	   {
		shared_ptr<ResHandle> handle = *( m_lruResHandleList.begin() );
		Free( handle );
		// m_lruResHandleList.pop_front(); // should this be called? remove is alreadt been called in Free()
	   }
   }

shared_ptr< ResHandle > ResourceCache::Find( Resource *resource )
   {
   ResHandleMap::iterator i = m_Resources.find( resource->m_Name );
	if ( i == m_Resources.end() )
		return shared_ptr<ResHandle>();

	return i->second;
   }

// move the handle to front of list is this resource is accessed by engine
void ResourceCache::Update( shared_ptr< ResHandle > handle  )
   {
   m_lruResHandleList.remove(handle);
	m_lruResHandleList.push_front(handle);

   }

shared_ptr< ResHandle > ResourceCache::Load( Resource *resource )
   {
   shared_ptr< IResourceLoader > loader;
   shared_ptr< ResHandle > handle;

   for( auto it = m_ResourceLoaders.begin(); it != m_ResourceLoaders.end(); ++it )
      {
      // find correspond matching loader based on resource name
      if( (*it)->VIsPatternMatch( resource->m_Name.c_str() ) )
         {
         loader = (*it);
         break;
         }
      }

   if( !loader )
      {
      ENG_ASSERT( loader && _T( "Default resource loader not found!" ) );
      return handle;
      }
   // find correspond file in zip file and return its size
   int rawSize = m_pResourceFile->VGetRawResourceSize( *resource );
	if ( rawSize < 0 )
	   { 
		ENG_ASSERT( rawSize > 0 && "Resource size returned -1 - Resource not found" );
		return shared_ptr<ResHandle>();
	   }
   // add addition one byte if needed, lua resource loader will use this feature
   int allocSize = rawSize + ( (loader->VAddNullZero() ) ? (1) : (0));
   // if the resource do not need additional processing, then just call Allocate, 
   // because we dont need additional buffer ( rawBuffer )
   char *pRawBuffer = NULL;
   if( loader->VUseRawFile() )
      {
      Allocate( &pRawBuffer, allocSize, true );
      }
   else
      {
      pRawBuffer = ENG_NEW char[ allocSize ];
      }
   memset( pRawBuffer, 0, allocSize );
   // allocate rawBuffer fail or cannot load raw file from zip fIle
   if( !pRawBuffer || !m_pResourceFile->VGetRawResource( *resource, pRawBuffer ) )
      {
      return shared_ptr< ResHandle >();
      }
   char *pBuffer = NULL;
   unsigned int size = 0;

   if( loader->VUseRawFile() ) // use raw pBuffer directly
      {
      pBuffer = pRawBuffer;
      handle = shared_ptr< ResHandle >( ENG_NEW ResHandle( *resource, pBuffer, rawSize, this ) );
      }
   else // allocate another buffer for processing
      {
      if( loader->VUsePreAllocate() )
         {
         size = loader->VGetLoadedResourceSize( pRawBuffer, rawSize ); // chances are that loader will not use this buffer, so it may return size 0
         if( !pRawBuffer || !Allocate( &pBuffer, size, true ) ) // allocation failed, return null pointer directly
            {
            return shared_ptr<ResHandle>();
            }
         }
      // set buffer for handle
      handle = shared_ptr< ResHandle >( ENG_NEW ResHandle( *resource, pBuffer, size, this ) );
      // loader store processed data into handle from pRawBuffer
      bool success = loader->VLoadResource( pRawBuffer, rawSize, handle ); 

      if( !loader->VUsePreAllocate() )
         {
         // the resource has been allocated in 3rd party lib
         // we only have to test if we can make room for the resource we are allocating
         if( !Allocate( &pBuffer, handle->GetSize(), false ) )
            {
            ENG_ASSERT( false && "Post Allocation Failed" );
            return shared_ptr<ResHandle>();
            } 
         }

      if( loader->VDiscardRawBufferAfterLoad() )
         {
         // after processing resource, delete raw data
         SAFE_DELETE_ARRAY( pRawBuffer ); 
         }
      
      if( !success ) // process raw file failed 
         {
         ENG_ASSERT( success && "Loader VloadResource Failed" );
         return shared_ptr<ResHandle>();
         }
      }
   if( handle )
      {
      m_lruResHandleList.push_front( handle ); // this resource is newly loaded, put it in the start of list
      m_Resources[ resource->m_Name ] = handle; // add resource name & handle mapping
      }

   return handle;
   }

// remove the resource from list and map
void ResourceCache::Free( shared_ptr< ResHandle > gonner )
   {
   m_lruResHandleList.remove( gonner ); // This calls the destructor of shared_ptr
	m_Resources.erase( gonner->m_Resource.m_Name );
   // Note - the resource might still be in use by something,
	// so the cache can't actually count the memory freed until the
	// ResHandle pointing to it is destroyed. ( destructor of ResHandle has been called ) 
   }

bool ResourceCache::MakeRoom( unsigned int size )
   {
   if( size > m_CacheSize ) // impossible, this resource is lager than the whole cache
      {
      return false;
      }

   while( size > ( m_CacheSize - m_AllocatedSize ) ) // while the size is larger than free cache size, keep remove resource out of cache 
      {
      if( m_lruResHandleList.empty() ) // the cache is already empty, but still the free space is smaller than resource size
         {
         return false;
         }
      FreeOneResource();
      }
   return true;
   }

bool ResourceCache::Allocate( char** pAllocBuffer, unsigned int size, bool useRealAllocation )
   {
   if( !MakeRoom( size ) )
      {
      *pAllocBuffer = NULL;
      return false;
      }

   if( useRealAllocation )
      {
      *pAllocBuffer = ENG_NEW char[size];
      if( *pAllocBuffer )
         {
         m_AllocatedSize += size; // update allocated size
         }
      return true;
      }
   else
      {
      *pAllocBuffer = NULL;
      return true;
      }
   }

// remove last resource from the list and map
void ResourceCache::FreeOneResource()
   {
   auto gonner = m_lruResHandleList.end();
   --gonner;
   shared_ptr< ResHandle > handle = *gonner;
   Free( handle );
   // m_lruResHandleList.pop_back();
  // m_Resources.erase( handle->m_Resource.m_name );
   }

// This function is called by destructor of ResHandle
void ResourceCache::MemoryHasBeenFreed( unsigned int size )
   {
   m_AllocatedSize -= size;
   }

//
// ResCache::Match									- not described in the book
//
//   Searches the resource cache assets for files matching the pattern. Useful for providing a 
//   a list of levels for a main menu screen, for example.
//
std::vector<std::string> ResourceCache::Match( const std::string pattern )
   {
	std::vector<std::string> matchingNames;
	if ( m_pResourceFile == NULL )
		return matchingNames;
   // get total number of files in zip file
	int numFiles = m_pResourceFile->VGetNumResources();
	for ( int i = 0; i < numFiles; ++i )
	   {
		std::string name = m_pResourceFile->VGetResourceName( i );
		std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);
		if ( WildcardMatch( pattern.c_str(), name.c_str() ) )
		   {
			matchingNames.push_back(name);
		   }
	   }
	return matchingNames;
   }

//========================================================================
// class ResourceZipFile					- not discussed in the book
//
//    This class implements the IResourceFile interface with a ZipFile.
//
ResourceZipFile::~ResourceZipFile() 
   { 
	SAFE_DELETE( m_pZipFile ); 
   }

bool ResourceZipFile::VOpen()
   {
	m_pZipFile = ENG_NEW ZipFile;
      if (m_pZipFile)
      {
		return m_pZipFile->Init( m_ResFileName.c_str() );
	   }
	return false;	
   }

int ResourceZipFile::VGetRawResourceSize(const Resource &r)
   {
	int resourceNum = m_pZipFile->Find( r.m_Name.c_str() );
	if (resourceNum == -1)
		return -1;

	return m_pZipFile->GetFileLen(resourceNum);
   }

int ResourceZipFile::VGetRawResource( const Resource &r, char *buffer )
   {
	int size = 0;
	optional<int> resourceNum = m_pZipFile->Find( r.m_Name.c_str() );
	if (resourceNum.valid())
	   {
		size = m_pZipFile->GetFileLen( *resourceNum );
		m_pZipFile->ReadFile( *resourceNum, buffer );
	   }
	return size;	
   }

int ResourceZipFile::VGetNumResources() const 
   { 
	return ( m_pZipFile == NULL ) ? 0 : m_pZipFile->GetNumFiles(); 
   }

// return file name based on file index
std::string ResourceZipFile::VGetResourceName( int num ) const 
   { 
	std::string resName = "";
	if ( m_pZipFile != NULL && num >= 0 && num<m_pZipFile->GetNumFiles() )
	   {
		resName = m_pZipFile->GetFileName( num ); 
	   }
	return resName;
   }
