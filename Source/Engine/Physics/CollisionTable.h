/*!
 * \file CollisionTable.h
 * \date 2016/06/26 16:53
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

typedef unsigned int CollisionId;
typedef unsigned int CollisionMask;

class CollisionTable : ENG_Noncopyable
   {
   public:
      static CollisionTable& GetSingleton( void );
      bool Init( TiXmlElement* pData );
      CollisionId GetIdFromName( const std::string& typeName );
      bool isCollidable( CollisionId id1, CollisionId id2 );

   private:
      CollisionTable( void ) { m_CurrMaxBit = -1; };
      bool AddCollisionType( const std::string& typeName );

      typedef std::map< CollisionId, CollisionMask > CollisionTableMap;
      typedef std::map< std::string, CollisionId > CollisionIdMap;
      
      CollisionTableMap m_CollisionTableMap;
      CollisionIdMap m_CollisionIdMap;

      int m_CurrMaxBit;
   };



