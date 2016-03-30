////////////////////////////////////////////////////////////////////////////////
// Filename: ResourceCache.cpp
////////////////////////////////////////////////////////////////////////////////

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "EngineStd.h"
#include "ResourceCache.h"

#include <list>
#include <map>
#include <cctype>			// for std::tolower


// before restore the file name, trans from the alphabets to lowercase
Resource::Resource( const std::string &name )
   {
   m_name = name;
   std::transform( m_name.begin(), m_name.end(), m_name.begin(), ( int(*)(int) )std::tolower );
   }


ResHandle::ResHandle( Resource &resource, char *buffer, unsigned int size, ResCache *pResCache ) : m_resource( resource )
   {
   const int i = 0;
   int j = i;
   j++;
   }

// The destructor truely free the allocated memory to resource cache
ResHandle::~ResHandle()
   {
   SAFE_DELETE_ARRAY( m_buffer );
   m_pResCache->MemoryHasBeenFreed( m_size );
   }

ResCache::ResCache( const unsigned int sizeInMb, IResourceFile *resFile )
   {
   m_cacheSize = sizeInMb * 1024 * 1024;
   m_allocated = 0;
   m_file = resFile;
   }

// delete all of the resources handle in the list
ResCache::~ResCache()
   {
   while( !m_lruResHandleList.empty() )
      {
      FreeOneResource();
      }
      SAFE_DELETE( m_file );
   }

bool ResCache::Init()
   {
   bool ret = false;
   if( m_file->VOpen() )
      {
      RegisterLoader( shared_ptr< IResourceLoader > ( ENG_NEW DefaultResourceLoader() ) );
      ret = true;
      }
   return ret;
   }

void ResCache::RegisterLoader( shared_ptr< IResourceLoader > loader )
   {
   m_resourceLoaders.push_front( loader );
   }

shared_ptr< ResHandle > ResCache::GetHandle( Resource *resource )
   {
   shared_ptr< ResHandle > handle( Find( resource ) );
   // if the handle is not loaded yet ( cannot find the resource handle ), then load it
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
int ResCache::Preload( const std::string pattern, void (*progressCallback)( int, bool & ) )
   {
   if( !m_file )
      {
      return 0;
      }
   int numFiles = m_file->VGetNumResources();
   int loaded = 0;
   bool cancel = false;
   for( int i = 0; i < numFiles; ++i )
      {
      Resource resource( m_file->VGetResourceName( i ) );
      if( WildcardMatch( pattern.c_str(), resource.m_name.c_str() ) )
         {
         shared_ptr< ResHandle > handle = g_pApp->m_ResCache->GetHandle( &resource ); // use resource cache to load resource
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
void ResCache::Flush( void )
   {
   while ( !m_lruResHandleList.empty())
	   {
		shared_ptr<ResHandle> handle = *( m_lruResHandleList.begin() );
		Free( handle );
		m_lruResHandleList.pop_front(); // should this be called? remove is alreadt been called in Free()
	   }
   }

shared_ptr< ResHandle > ResCache::Find( Resource *resource )
   {
   ResHandleMap::iterator i = m_resources.find( resource->m_name );
	if (i == m_resources.end())
		return shared_ptr<ResHandle>();

	return i->second;
   }

// move the handle to front of list is this resource is accessed by engine
void ResCache::Update( shared_ptr< ResHandle > handle  )
   {
   m_lruResHandleList.remove(handle);
	m_lruResHandleList.push_front(handle);

   }

shared_ptr< ResHandle > ResCache::Load( Resource *resource )
   {
   shared_ptr< IResourceLoader > loader;
   shared_ptr< ResHandle > handle;

   for( auto it = m_resourceLoaders.begin(); it != m_resourceLoaders.end(); ++it )
      {
      // find correspond matching loader based on resource name
      if( WildcardMatch( (*it)->VGetPattern().c_str(), resource->m_name.c_str() ) )
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

   unsigned int rawSize = m_file->VGetRawResourceSize( *resource );
   // if the resource do not need additional processing, then just allocate rawSize
   char *rawBuffer = loader->VUseRawFile()? Allocate( rawSize ): ENG_NEW char[rawSize];
   // allocate fail because of out of memory
   if( !rawBuffer )
      {
      return shared_ptr< ResHandle >();
      }
   m_file->VGetRawResource( *resource, rawBuffer );
   char *buffer = NULL;
   unsigned int size = 0;

   if( loader->VUseRawFile() ) // use raw buffer directly
      {
      buffer = rawBuffer;
      handle = shared_ptr< ResHandle >( ENG_NEW ResHandle( *resource, buffer, rawSize, this ) );
      }
   else // allocate another buffer for processing
      {
      size = loader->VGetLoadedResourceSize( rawBuffer, rawSize );
      buffer = Allocate( size );
      if( !rawBuffer || !buffer ) // allocation failed, return null pointer directly
         {
         return shared_ptr<ResHandle>();
         }
      handle = shared_ptr< ResHandle >( ENG_NEW ResHandle( *resource, buffer, size, this ) );
      bool success = loader->VLoadResource( rawBuffer, rawSize, handle ); // for getting processed resource
      SAFE_DELETE_ARRAY( rawBuffer ); // after processing resource, delete raw data
      if( !success ) // process raw file failed 
         {
         return shared_ptr<ResHandle>();
         }
      }
   if( handle )
      {
      m_lruResHandleList.push_front( handle ); // this resource is newly loaded, put it in the start of list
      m_resources[ resource->m_name ] = handle; // add resource name & handle mapping
      }
   //ENG_ASSERT( loader && _T( "Default resource loader not found!" ) );
   return handle;
   }

// remove the resource from list and map
void ResCache::Free( shared_ptr< ResHandle > gonner )
   {
   m_lruResHandleList.remove(gonner); // This calls the destructor of shared_ptr
	m_resources.erase(gonner->m_resource.m_name);
   // Note - the resource might still be in use by something,
	// so the cache can't actually count the memory freed until the
	// ResHandle pointing to it is destroyed. ( destructor of ResHandle has been called ) 
   }

bool ResCache::MakeRoom( unsigned int size )
   {
   if( size > m_cacheSize ) // impossible, this resource is lager than the whole cache
      {
      return false;
      }

   while( size > ( m_cacheSize - m_allocated ) ) // while the size is larger than free cache size, keep remove resource out of cache 
      {
      if( m_lruResHandleList.empty() ) // the cache is already empty, but still the free space is smaller than resource size
         {
         return false;
         }
      FreeOneResource();
      }
   return true;
   }

char* ResCache::Allocate( unsigned int size )
   {
   if( !MakeRoom( size ) )
      {
      return NULL;
      }
   char *mem = ENG_NEW char[size];
   if( mem )
      {
      m_allocated += size; // update allocated size
      }
   return mem;
   }

// remove last resource from the list and map
void ResCache::FreeOneResource()
   {
   auto gonner = m_lruResHandleList.end();
   --gonner;
   shared_ptr< ResHandle > handle = *gonner;
   m_lruResHandleList.pop_back();
   m_resources.erase( handle->m_resource.m_name );
   }

// This function is called by destructor of ResHandle
void ResCache::MemoryHasBeenFreed( unsigned int size )
   {
   m_allocated -= size;
   }

//
// ResCache::Match									- not described in the book
//
//   Searches the resource cache assets for files matching the pattern. Useful for providing a 
//   a list of levels for a main menu screen, for example.
//
std::vector<std::string> ResCache::Match(const std::string pattern)
   {
	std::vector<std::string> matchingNames;
	if ( m_file == NULL )
		return matchingNames;

	int numFiles = m_file->VGetNumResources();
	for (int i=0; i<numFiles; ++i)
	   {
		std::string name = m_file->VGetResourceName(i);
		std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);
		if ( WildcardMatch( pattern.c_str(), name.c_str() ) )
		   {
			matchingNames.push_back(name);
		   }
	   }
	return matchingNames;
   }

