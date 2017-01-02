/*!
 * \file VIdeoTextureResource.h
 * \date 2017/01/02 10:42
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
#include "VideoResourceCache.h"



class VideoTextureResourceExtraData : public IVideoResourceExtraData
   {
   public:
      VideoTextureResourceExtraData( void ) { m_TexObj = 0; }
      ~VideoTextureResourceExtraData( void );
   public:
      GLuint m_TexObj;
   };

class VideoTextureResourceLoader : public VideoResourceLoader 
   {
   public:
      VideoTextureResourceLoader( void );
      virtual int VLoadResource( shared_ptr<ResHandle> handle, shared_ptr< VideoResourceHandle > videoHandle ) override;
      static GLuint LoadAndReturnTextureObject( const Resource& resource );
   };

