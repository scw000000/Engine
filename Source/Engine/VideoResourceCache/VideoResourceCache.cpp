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

VideoTextureResource::VideoTextureResource( const std::string &name, bool caseSensitive, bool bIsSRGB ) :
   Resource( name, caseSensitive )
   , m_bIsSRGB( bIsSRGB )
   {
   
   }

bool VideoTextureResource::VInit( TiXmlElement* pData, bool caseSensitive )
   {
   return Resource::VInit( pData, caseSensitive );
   }

TiXmlElement* VideoTextureResource::VGenerateXML( void )
   {
   // TODO: add options for it
   return Resource::VGenerateXML();
   }

TiXmlElement* VideoTextureResource::VGenerateOverridesXML( TiXmlElement* pResource )
   {
   // TODO: add options for it
   return Resource::VGenerateOverridesXML( pResource );
   }

VideoResourceHandle::VideoResourceHandle( shared_ptr< Resource > pResource, unsigned int size, VideoResourceCache *pResCache ) :
   m_pResource( pResource ),
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

shared_ptr< VideoResourceHandle > VideoResourceCache::GetHandle( shared_ptr< Resource > pResource )
   {
   shared_ptr< VideoResourceHandle > handle( Find( pResource ) );
   // if the handle is not loaded yet ( cannot find the resource handle map ), then load it from CPU, then load to GPU
   if( !handle )
      {
      handle = Load( pResource );
      }
   else // It exist, update its order in the least recently used list
      {
      Update( handle );
      }
   ENG_ASSERT( handle );
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

shared_ptr< VideoResourceHandle > VideoResourceCache::Find( shared_ptr< Resource > pResource )
   {
   auto i = m_Resources.find( pResource->m_Name );
   if( i == m_Resources.end() )
      return shared_ptr< VideoResourceHandle >();

   return i->second;
   }

void VideoResourceCache::Update( shared_ptr< VideoResourceHandle > handle )
   {
   m_lruResHandleList.remove( handle );
   m_lruResHandleList.push_front( handle );
   }

shared_ptr< VideoResourceHandle > VideoResourceCache::Load( shared_ptr< Resource > pResource )
   {
   auto fileHandle = m_pResourceCache->GetHandle( pResource );
   auto handle = shared_ptr< VideoResourceHandle >();
   if( !fileHandle ) // cant load from CPU, return
      {
      return handle;
      }

   shared_ptr< IVideoResourceLoader > loader;
   for( auto it = m_ResourceLoaders.begin(); it != m_ResourceLoaders.end(); ++it )
      {
      // find correspond matching loader based on resource name
      if( ( *it )->VIsPatternMatch( pResource->m_Name.c_str() ) )
         {
         loader = ( *it );
         break;
         }
      }

   if( !loader )
      {
      ENG_ASSERT( loader && _T( "Resource loader not found!" ) );
      return handle;
      }
   
   handle = shared_ptr< VideoResourceHandle >( ENG_NEW VideoResourceHandle( pResource, 0u, this ) );
   int success = loader->VLoadResource( fileHandle, handle );
   
   if( success != S_OK )
      {
      ENG_ASSERT( success && "Loader VloadResource Failed" );
      return shared_ptr< VideoResourceHandle >();
      }

   if( handle )
      {
      m_lruResHandleList.push_front( handle ); // this resource is newly loaded, put it in the start of list
      m_Resources[ pResource->m_Name ] = handle; // add resource name & handle mapping
      }

   return handle;;
   }

void VideoResourceCache::Free( shared_ptr< VideoResourceHandle > gonner )
   {
   m_lruResHandleList.remove( gonner ); 
   m_Resources.erase( gonner->m_pResource->m_Name );
   }

void VideoResourceCache::FreeOneResource()
   {
   auto gonner = m_lruResHandleList.end();
   --gonner;
   shared_ptr< VideoResourceHandle > handle = *gonner;
   Free( handle );
   }



