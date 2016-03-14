#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: MemoryPool.h
////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------
// This class represents a single memory pool.  A memory pool is pool of memory that's split into 
// chunks of equal size, each with a 4-byte header.  The header is treated as a pointer that points
// to the next chunk, making the pool a singly-linked list of memory chunks.
// 
// When the pool is first initialized (via the Init() function), you must pass in a chunk size and
// the number of chunks you want created.  These two values are immutable unless you destroy and
// reinitialize the entire pool.  The chunk size is the size of each chunk, minus the header, in 
// bytes.  The memory pool will allocate the appropriate amount of memory and set up the data
// structure in the Init() call.  Thus, total memory usage will be N * (S + 4) + O, where N is the
// number of chunks, S is the size of each chunk, and O is the overhead for the class (currently 
// 18 + (number of reallocations * 4).
// 
// Call the Alloc() function to retrieve a chunk from the memory pool.  The Alloc() function removes
// the head of the linked list, sets the new head to the next chunk, and returns a pointer to the 
// data section of the old head.  If there aren't anymore chunks left when Alloc() is called, it 
// will allocate another block of N chunks, where N is the number of chunks you passed into Init().
// While Alloc() is typically a very fast function, this reallocation will certainly cost you so 
// choose your initial sizes carefully.
// 
// Call the Free() function to release a chunk of memory back into the memory pool for reuse.  This
// will cause the chunk to the inserted to the front of the list, ready for the next bit.
//--------------------------------------------------------------------------------------------------


/*
this class is currently unused, only related to CHP18, pathing system
so I just leave it here for future 
*/
class MemoryPool
   {
   public:
      MemoryPool( void );
      ~MemoryPool( void );


   private:
      unsigned char** m_ppRawMemoryArray;/* arrat of memory block, each block split up into chunks */

      unsigned char *m_pHead; /* front of the memory chunk linked list */
      unsigned int m_chunkSize; /* size of each chunk */
      unsigned int m_numChunks; /* number of chunks per array */
      unsigned int m_memArraySize; /* number of elements in the memory array */
      bool m_toAllowResize;/* true if the array is allowed to be resized if the array is filled */

   };