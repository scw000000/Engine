#pragma once
/*!
 * \file DevResourceFile.h
 * \date 2016/05/29 18:42
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

class DevResourceFile : public IResourceFile
   {
   public:
      enum Mode
         {
         Development,	// this mode checks the original asset directory for changes - helps during development
         Editor			// this mode only checks the original asset directory - the ZIP file is left unopened.
         };

      DevResourceFile( Mode mode );

      virtual bool VOpen( void ) override;
      virtual int VGetRawResourceSize( const Resource &r ) override;
      virtual int VGetRawResource( const Resource &r, char *buffer ) override;
      virtual int VGetNumResources( void ) const override;
      virtual std::string VGetResourceName( int num ) const override;
      virtual bool VIsUsingDevelopmentDirectories( void ) const override { return true; }

      int Find( const std::string &path );

   public:
      Mode m_AccessMode;
      std::wstring m_AssetsDir;
      std::vector<WIN32_FIND_DATA> m_AssetFileInfo;
      FileContentsMap m_DirectoryContentsMap;

   protected:
      void ReadAssetsDirectory( std::wstring fileSpec );
   };