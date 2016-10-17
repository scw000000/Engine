#pragma once
/*!
 * \file ScriptResource.h
 * \date 2016/08/04 15:21
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

class ScriptResourceLoader : public ResourceLoader
   {
   public:
      ScriptResourceLoader( void );
      virtual bool VUseRawFile() override { return false; }
      virtual bool VDiscardRawBufferAfterLoad() override { return true; }
      virtual bool VAddNullZero() override { return true; }
      virtual unsigned int VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize ) override { return 0; }
      virtual bool VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle ) override;
      virtual bool VUsePreAllocate( void ) override { return true; }
   };