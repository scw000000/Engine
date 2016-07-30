/*!
 * \file AnimationManager.h
 * \date 2016/07/30 21:31
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

class IAnimationManager
   {
   public:
      IAnimationManager( void );
      virtual ~IAnimationManager( void );

      virtual void Update( unsigned long elapsedMs ) = 0;
      private:

   };
