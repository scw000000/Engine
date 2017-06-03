#pragma once
/*!
 * \file VideoResourceCache.h
 * \date 2017/01/02 8:19
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
#include "..\ResourceCache\ResourceCache.h"
class VideoResourceCache;

class VideoTextureResource : public Resource
   {
   public:
      VideoTextureResource( const std::string &name = "", bool caseSensitive = false, bool bIsSRGB = false );
      virtual bool VInit( TiXmlElement* pData, bool caseSensitive = false ) override;
      virtual TiXmlElement* VGenerateXML( void ) override;
      virtual TiXmlElement* VGenerateOverridesXML( TiXmlElement* pResource ) override;


   public:
      bool m_bIsSRGB;
   };

   

class VideoResourceHandle
   {
   friend class VideoResourceCache;
   public:
      VideoResourceHandle( shared_ptr< Resource > pResource, unsigned int size, VideoResourceCache *pResCache );
      unsigned int GetSize( void ) const { return m_Size; }

      shared_ptr< IVideoResourceExtraData > GetExtraData() const { return m_Extra; }
      void SetExtraData( shared_ptr< IVideoResourceExtraData > extra ) { m_Extra = extra; }

   protected:
      shared_ptr< Resource > m_pResource;
      unsigned int m_Size;
      shared_ptr< IVideoResourceExtraData > m_Extra;
      VideoResourceCache *m_pResCache;
   };

class VideoResourceLoader : public IVideoResourceLoader
   {
   public:
      VideoResourceLoader( const std::vector< std::string >& patterns ) : m_Patterns( patterns ) {};
      virtual const std::vector< std::string >&  VGetPattern( void ) const override { return m_Patterns; }
      virtual bool VIsPatternMatch( const char* str ) const override;

   protected:
      const std::vector< std::string >& m_Patterns;
   };

typedef std::list< shared_ptr< VideoResourceHandle > > VideoResHandleList;
typedef std::map < std::string, shared_ptr < VideoResourceHandle > > VideoResHandleMap;
typedef std::list< shared_ptr< IVideoResourceLoader > > VideoResourceLoaders;

class VideoResourceCache
   {
   public:
      VideoResourceCache( const unsigned int sizeInMb, ResourceCache* pResCache );
      ~VideoResourceCache();

      int Init();
      // Note that the DefaultResourceLoader is last one in the list, so any loader will match before it if the file format is supported
      template< typename T > void RegisterLoader( void );

      shared_ptr< VideoResourceHandle > GetHandle( shared_ptr< Resource > pResource );
      //int Preload( const std::string pattern, void( *progressCallback )( int, bool & ) );
      void Flush( void );
      // std::vector<std::string> Match( const std::string pattern );
      //bool IsFileExist( const Resource& resource );

   protected:
      shared_ptr< VideoResourceHandle > Find( shared_ptr< Resource > pResource );
      void Update( shared_ptr< VideoResourceHandle > handle );
      shared_ptr< VideoResourceHandle > Load( shared_ptr< Resource > pResource );
      void Free( shared_ptr< VideoResourceHandle > gonner );

     // bool MakeRoom( unsigned int size );
     // bool Allocate( char** pAllocBuffer, unsigned int size, bool useRealAllocation );
     // char *PostAllocate( unsigned int size );
      void FreeOneResource();
    //  void MemoryHasBeenFreed( unsigned int size );

    protected:
      VideoResHandleList m_lruResHandleList; // least recently used list
      VideoResHandleMap m_Resources;
      VideoResourceLoaders m_ResourceLoaders;
      ResourceCache*     m_pResourceCache;

      unsigned int m_CacheSizeInByte; // size in bytes
    //  unsigned int m_AllocatedSize;
   };


template< typename T > void VideoResourceCache::RegisterLoader( void )
   {
   if( !std::is_base_of< IVideoResourceLoader, T >::value )
      {
      ENG_ERROR( "Invalid implementation register" );
      return;
      }

   m_ResourceLoaders.push_front( shared_ptr< IVideoResourceLoader >( ENG_NEW T() ) );
   }