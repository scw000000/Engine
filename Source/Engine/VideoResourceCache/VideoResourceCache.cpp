/*!
 * \file VideoResourceCache.cpp
 * \date 2017/01/02 8:20
 *
 * \author scw00
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#include "EngineStd.h"
#include "VideoResourceCache.h"
#include "VideoTextureResource.h"
#include "VideoMeshResource.h"

VideoResourceHandle::VideoResourceHandle( const Resource &resource, unsigned int size, VideoResourceCache *pResCache ) :
   m_Resource( resource ),
   m_pResCache( pResCache )
   {
   // size is not supported yet
   }

bool VideoResourceLoader::VIsPatternMatch( const char* str ) const
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

VideoResourceCache::VideoResourceCache( const unsigned int sizeInMb, ResourceCache* pResCache ) :
   m_pResourceCache( pResCache )
   {
   m_CacheSizeInByte = sizeInMb;
   ENG_ASSERT( pResCache );
   }

VideoResourceCache::~VideoResourceCache()
   {
   while( !m_lruResHandleList.empty() )
      {
      FreeOneResource();
      }
   }

int VideoResourceCache::Init()
   {
   RegisterLoader< VideoTextureResourceLoader >();
   RegisterLoader< VideoMeshResourceLoader >();
   return S_OK;
   }

shared_ptr< VideoResourceHandle > VideoResourceCache::GetHandle( const Resource& resource )
   {
   shared_ptr< VideoResourceHandle > handle( Find( resource ) );
   // if the handle is not loaded yet ( cannot find the resource handle map ), then load it from CPU, then load to GPU
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

void VideoResourceCache::Flush( void )
   {
   while( !m_lruResHandleList.empty() )
      {
      auto handle = *( m_lruResHandleList.begin() );
      Free( handle );
      }
   }

shared_ptr< VideoResourceHandle > VideoResourceCache::Find( const Resource& resource )
   {
   auto i = m_Resources.find( resource.m_Name );
   if( i == m_Resources.end() )
      return shared_ptr< VideoResourceHandle >();

   return i->second;
   }

void VideoResourceCache::Update( shared_ptr< VideoResourceHandle > handle )
   {
   m_lruResHandleList.remove( handle );
   m_lruResHandleList.push_front( handle );
   }

shared_ptr< VideoResourceHandle > VideoResourceCache::Load( const Resource& resource )
   {
   auto fileHandle = m_pResourceCache->GetHandle( resource );
   auto handle = shared_ptr< VideoResourceHandle >();
   if( !fileHandle ) // cant load from CPU, return
      {
      return handle;
      }

   shared_ptr< IVideoResourceLoader > loader;
   for( auto it = m_ResourceLoaders.begin(); it != m_ResourceLoaders.end(); ++it )
      {
      // find correspond matching loader based on resource name
      if( ( *it )->VIsPatternMatch( resource.m_Name.c_str() ) )
         {
         loader = ( *it );
         break;
         }
      }

   if( !loader )
      {
      ENG_ASSERT( loader && _T( "Rresource loader not found!" ) );
      return handle;
      }
   
   handle = shared_ptr< VideoResourceHandle >( ENG_NEW VideoResourceHandle( resource, 0u, this ) );
   int success = loader->VLoadResource( fileHandle, handle );
   
   if( success != S_OK )
      {
      ENG_ASSERT( success && "Loader VloadResource Failed" );
      return shared_ptr< VideoResourceHandle >();
      }

   if( handle )
      {
      m_lruResHandleList.push_front( handle ); // this resource is newly loaded, put it in the start of list
      m_Resources[ resource.m_Name ] = handle; // add resource name & handle mapping
      }

   return handle;;
   }

void VideoResourceCache::Free( shared_ptr< VideoResourceHandle > gonner )
   {
   m_lruResHandleList.remove( gonner ); 
   m_Resources.erase( gonner->m_Resource.m_Name );
   }

void VideoResourceCache::FreeOneResource()
   {
   auto gonner = m_lruResHandleList.end();
   --gonner;
   shared_ptr< VideoResourceHandle > handle = *gonner;
   Free( handle );
   }



