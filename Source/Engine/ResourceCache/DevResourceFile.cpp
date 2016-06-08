/*!
 * \file DevResourceFile.cpp
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

#include "EngineStd.h"
#include "DevResourceFile.h"
#include "ResourceCache.h"

#include <cctype>

DevResourceFile::DevResourceFile( Mode mode )
   {
   m_AccessMode = mode;

   TCHAR dir[ MAX_PATH ];
   GetCurrentDirectory( MAX_PATH, dir );

   m_AssetsDir = dir;
   /*int lastSlash = m_AssetsDir.find_last_of( L"\\" );
   m_AssetsDir = m_AssetsDir.substr( 0, lastSlash );*/
   m_AssetsDir += L"\\Assets\\";
   }

int DevResourceFile::Find( const std::string &name )
   {
   std::string lowerCase = name;
   std::transform( lowerCase.begin(), lowerCase.end(), lowerCase.begin(), ( int( *)( int ) ) std::tolower );
   FileContentsMap::const_iterator i = m_DirectoryContentsMap.find( lowerCase );
   if( i == m_DirectoryContentsMap.end() )
      return -1;

   return i->second;
   }

bool DevResourceFile::VOpen()
   {
   ReadAssetsDirectory( L"*" );

   //if( m_AccessMode != Editor )
   //   {
   //   ResourceZipFile::VOpen();
   //   }

   //// open the asset directory and read in the non-hidden contents
   //if( m_AccessMode == Editor )
   //   {
   //   ReadAssetsDirectory( L"*" );
   //   }
   //else
   //   {
   //   // FUTURE WORK - iterate through the ZipFile contents and go grab WIN32_FIND_DATA 
   //   //   elements for each one. Then it would be possible to compare the dates/times of the
   //   //   asset in the Zip file with the source asset.
   //   ENG_ASSERT( 0 && "Not implemented yet" );
   //   }

   return true;
   }


int DevResourceFile::VGetRawResourceSize( const Resource &r )
   {
   int num = Find( r.m_Name.c_str() );
   if( num == -1 )
      {
      return -1;
      }
   return m_AssetFileInfo[ num ].nFileSizeLow;
   /*if( m_AccessMode == Editor )
      {
      int num = Find( r.m_Name.c_str() );
      if( num == -1 )
      return -1;

      return m_AssetFileInfo[ num ].nFileSizeLow;
      }

      return ResourceZipFile::VGetRawResourceSize( r );*/
   }

int DevResourceFile::VGetRawResource( const Resource &r, char *buffer )
   {
   int num = Find( r.m_Name.c_str() );
   if( num == -1 )
      {
      return -1;
      }
   std::string fullFileSpec = ws2s( m_AssetsDir ) + r.m_Name.c_str();
   FILE *f = fopen( fullFileSpec.c_str(), "rb" );
   size_t bytes = fread( buffer, 1, m_AssetFileInfo[ num ].nFileSizeLow, f );
   fclose( f );
   return bytes;
   /*if( m_AccessMode == Editor )
      {
      int num = Find( r.m_Name.c_str() );
      if( num == -1 )
      return -1;

      std::string fullFileSpec = ws2s( m_AssetsDir ) + r.m_Name.c_str();
      FILE *f = fopen( fullFileSpec.c_str(), "rb" );
      size_t bytes = fread( buffer, 1, m_AssetFileInfo[ num ].nFileSizeLow, f );
      fclose( f );
      return bytes;
      }

      return ResourceZipFile::VGetRawResource( r, buffer );*/
   }

int DevResourceFile::VGetNumResources() const
   {
   return m_AssetFileInfo.size();
   //return ( m_AccessMode == Editor ) ? m_AssetFileInfo.size() : ResourceZipFile::VGetNumResources();
   }

std::string DevResourceFile::VGetResourceName( int num ) const
   {
   std::wstring wideName = m_AssetFileInfo[ num ].cFileName;
   return ws2s( wideName );
   /*if( m_AccessMode == Editor )
      {
      std::wstring wideName = m_AssetFileInfo[ num ].cFileName;
      return ws2s( wideName );
      }

   return ResourceZipFile::VGetResourceName( num );*/
   }


void DevResourceFile::ReadAssetsDirectory( std::wstring fileSpec )
   {
   HANDLE fileHandle;
   WIN32_FIND_DATA findData;

   // get first file
   std::wstring pathSpec = m_AssetsDir + fileSpec;
   fileHandle = FindFirstFile( pathSpec.c_str(), &findData );
   if( fileHandle != INVALID_HANDLE_VALUE )
      {
      // loop on all remeaining entries in dir
      while( FindNextFile( fileHandle, &findData ) )
         {
         if( findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN )
            continue;

         std::wstring fileName = findData.cFileName;
         if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            {
            if( fileName != L".." && fileName != L"." )
               {
               fileName = fileSpec.substr( 0, fileSpec.length() - 1 ) + fileName + L"\\*";
               ReadAssetsDirectory( fileName );
               }
            }
         else
            {
            fileName = fileSpec.substr( 0, fileSpec.length() - 1 ) + fileName;
            std::wstring lower = fileName;
            std::transform( lower.begin(), lower.end(), lower.begin(), ( int( *)( int ) ) std::tolower );
            wcscpy_s( &findData.cFileName[ 0 ], MAX_PATH, lower.c_str() );
            m_DirectoryContentsMap[ ws2s( lower ) ] = m_AssetFileInfo.size();
            m_AssetFileInfo.push_back( findData );
            }
         }
      }

   FindClose( fileHandle );
   }
