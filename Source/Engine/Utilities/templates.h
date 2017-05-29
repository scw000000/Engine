#pragma once
/*!
 * \file templates.h
 * \date 2017/03/20 9:04
 *
 * \author scw00
 * Contact: scw000000@gmail.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

/*!
* \class ENG_Noncopyable
*
* \brief Modified version of non copyable class
*  reference https://msdn.microsoft.com/zh-tw/library/dn457344.aspx
* \author SCW
* \date 05 2016
*/
class ENG_Noncopyable
   {
      public:
      ENG_Noncopyable( void ) {}; // Default constructor 

      ENG_Noncopyable( const ENG_Noncopyable& x ) = delete;
      ENG_Noncopyable& operator=( const ENG_Noncopyable& x ) = delete;
   };

//template <class T>
//class singleton
//{
//	T m_OldValue;
//	T* m_pGlobalValue;
//
//public:
//   // class menber pointer point to new location
//   // copy pointer data to oldValue 
//   // copy newValue data to globalValue
//   // -> move m_pGlobalValue to new location, and assign its value to new value
//   // -> before that, copy its original value to m_OldValue
//	singleton(T newValue, T* globalValue)
//	{ 
//		m_pGlobalValue = globalValue;
//		m_OldValue = *globalValue; 
//		*m_pGlobalValue = newValue;
//	}
//
//	virtual ~singleton() { *m_pGlobalValue = m_OldValue; }
//};

template < typename T > class Singleton : public ENG_Noncopyable
   {
   public:
      static T& GetSingleton( void )
         {
         if( !m_pInstance )
            {
            ENG_ASSERT( m_bHasBeenCreated == false && "The singleton class cannot be create-release more then once" );
            m_pInstance = ENG_NEW T;
            ENG_ASSERT( m_pInstance );
            m_bHasBeenCreated = true;
            }
         return *m_pInstance;
         }

      static void DeleteSingleton( void )
         {
         if( m_pInstance )
            {
            SAFE_DELETE( m_pInstance );
            }
         }
   
   private:
      Singleton( void ) { m_bHasBeenCreated = true }
      virtual ~Singleton( void ) { }

   private:
      static T* m_pInstance;
      static bool m_bHasBeenCreated;
   };

#define REGISTER_SINGLETON( x ) class x; typedef Singleton< x > S##x; bool Singleton< x >::m_bHasBeenCreated = false; x* Singleton< x >::m_pInstance = nullptr

#define EXPOSE_CONSTRUCTOR( x ) friend class Singleton< x >

template <class Type>
shared_ptr<Type> MakeStrongPtr(weak_ptr<Type> pWeakPtr)
{
   // expired -> use_count() == 0.
   if (!pWeakPtr.expired())
       return shared_ptr<Type>(pWeakPtr);
   else
       return shared_ptr<Type>();
}


//////////////////////////////////////////////////////////////////////////////
// optional.h
//
// An isolation point for optionality, provides a way to define
// objects having to provide a special "null" state.
//
// In short:
//
// struct optional<T>
// {
//     bool m_bValid;
//
//	   T	m_data;
// };
//
//

#include <new>

class optional_empty { };

template <unsigned long size>
class optional_base
{
public:
    // Default - invalid.

    optional_base() : m_bValid(false) { }

    optional_base & operator = (optional_base const & t)
    {
		m_bValid = t.m_bValid;
		return * this;
    }

	//Copy constructor
    optional_base(optional_base const & other)
		: m_bValid(other.m_bValid)  { }

	//utility functions
	bool const valid() const		{ return m_bValid; }
	bool const invalid() const		{ return !m_bValid; }

protected:
    bool m_bValid;
    char m_data[size];  // storage space for T
};

template <class T>
class optional : public optional_base<sizeof(T)>
   {
   public:
      // Default - invalid.

      optional()	 {    }
      optional(T const & t)  { construct(t); m_bValid = (true);  }
	   optional(optional_empty const &) {	}

      optional & operator= ( T const & t )
         {
         if (m_bValid)
            {
            * GetT() = t;
            }
         else
            {
            construct( t );
			   m_bValid = true;	// order important for exception safety.
            }
        return * this;
         }

	   //Copy constructor
      optional(optional const & other)
         {
		   if (other.m_bValid)
		      {
			   construct(* other);
            m_bValid = true;	// order important for exception safety.
		      }
         }

      optional & operator = (optional const & other)
         {
		   ENG_ASSERT( ! ( this == & other ) );	// don't copy over self!
		   if ( m_bValid )
		      {						// first, have to destroy our original.
			   m_bValid = false;	// for exception safety if destroy() throws.
								// (big trouble if destroy() throws, though)
			   destroy();
		      }

		   if( other.m_bValid )
		      {
			   construct(* other);
			   m_bValid = true;	// order vital.
      		}
		   return * this;
         }
	   bool const operator == (optional const & other) const
	      {
		   if ( ( ! valid() ) && ( ! other.valid() ) ) { return true; }
		   if ( valid() ^ other.valid() ) { return false; }
		   return ( (* * this) == (* other) );
	      }
      // compare data if both variable are valid
      // else the NULL one is smaller
	   bool const operator < (optional const & other) const
	      {
		   // equally invalid - not smaller.
		   if ( ( ! valid() ) && ( ! other.valid() ) )   { return false; }

		   // I'm not valid, other must be, smaller.
		   if ( ! valid() )	{ return true; }

		   // I'm valid, other is not valid, I'm larger
		   if ( ! other.valid() ) { return false; }

		   return ( ( * * this ) < ( * other ) );
	      }

      ~optional() { if (m_bValid) destroy(); }

	   // Accessors.
	   const T& operator * () const			{ ENG_ASSERT( m_bValid ); return * GetT(); }
	   T& operator * ()						   { ENG_ASSERT( m_bValid ); return * GetT(); }
	   const T* const operator -> () const	{ ENG_ASSERT( m_bValid ); return GetT(); }
	   T		* const operator -> ()			{ ENG_ASSERT( m_bValid ); return GetT(); }

	   //This clears the value of this optional variable and makes it invalid once again.
	   void clear()
	      {
		   if (m_bValid)
		      {
			   m_bValid = false;
			   destroy();
		      }
	      }

	   //utility functions
	   bool const valid() const		{ return m_bValid; }
	   bool const invalid() const		{ return !m_bValid; }

   private:

      // return pointer to m_data ( const version )
      T const * const GetT() const { return reinterpret_cast<T const * const>(m_data); }
      // return pointer to m_data
      T * const GetT()			 { return reinterpret_cast<T * const>(m_data);}
      // new existing memory space with t as initial value
	   void construct(T const & t)  { new ( GetT() ) T(t); }
      void destroy() { GetT()->~T(); }
   };


template <class BaseType, class SubType>
BaseType* GenericObjectCreationFunction(void) { return ENG_NEW SubType; }

template <class BaseClass, class IdType>
class GenericObjectFactory
   {
   typedef BaseClass* (*ObjectCreationFunction)(void);
   std::map<IdType, ObjectCreationFunction> m_creationFunctions;

   public:
      // relate subClass creation with id
      template <class SubClass>
      bool Register(IdType id)
         {
         if ( !IsRegistered( id ) )
            {
            m_creationFunctions[id] = &GenericObjectCreationFunction<BaseClass, SubClass>;  // insert() is giving me compiler errors
            return true;
            }
         return false;
         }
    // Find the relation mapping and create the subClass based on given id
   BaseClass* Create(IdType id)
      {
      auto findIt = m_creationFunctions.find( id );
      if ( findIt != m_creationFunctions.end() )
         {
         ObjectCreationFunction pFunc = findIt->second;
         return pFunc();
         }
      return NULL;
      }

   bool IsRegistered( IdType id )
      {
      return ( m_creationFunctions.find(id) != m_creationFunctions.end() );
      }
   };
