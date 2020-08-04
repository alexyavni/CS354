////////////////////////////////////////////////////////////////////////////////
// Main File:        mem.c
// This File:        mem.c
// Other Files:      mem.c, mem.h, mem.o, Makefile
// Semester:         CS 354 Fall 2018
//
// Author:           Alexandra Yavnilovitch
// Email:            yavnilovitch@wisc.edu
// CS Login:         yavnilovitch
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include "mem.h"

/*
 * This structure serves as the header for each allocated and free block
 * It also serves as the footer for each free block
 * The blocks are ordered in the increasing order of addresses 
 */
typedef struct blk_hdr {
  int size_status;

  /*
   * Size of the block is always a multiple of 8
   * => last two bits are always zero - can be used to store other information
   *
   * LSB -> Least Significant Bit (Last Bit)
   * SLB -> Second Last Bit 
   * LSB = 0 => free block
   * LSB = 1 => allocated/busy block
   * SLB = 0 => previous block is free
   * SLB = 1 => previous block is allocated/busy
   * 
   * When used as the footer the last two bits should be zero
   */

  /*
   * Examples:
   * 
   * For a busy block with a payload of 20 bytes (i.e. 20 bytes data + 
   * an additional 4 bytes for header) Header:
   * If the previous block is allocated, size_status should be set to 27
   * If the previous block is free, size_status should be set to 25
   * 
   * For a free block of size 24 bytes (including 4 bytes for header + 4 bytes for footer)
   * Header:
   * If the previous block is allocated, size_status should be set to 26
   * If the previous block is free, size_status should be set to 24
   * Footer:
   * size_status should be 24
   * 
   */
}
blk_hdr;

/* Global variable - This will always point to the first block
 * i.e. the block with the lowest address */
blk_hdr * first_blk = NULL;

/*
 * Note: 
 *  The end of the available memory can be determined using end_mark
 *  The size_status of end_mark has a value of 1
 *
 */

/* 
 * Function for allocating 'size' bytes
 * Returns address of allocated block on success 
 * Returns NULL on failure 
 * Here is what this function should accomplish 
 * - Check for sanity of size - Return NULL when appropriate 
 * - Round up size to a multiple of 8 
 * - Traverse the list of blocks and allocate the best free block which can accommodate 
 *		the requested size 
 * - Also, when allocating a block - split it into two blocks
 * Tips: Be careful with pointer arithmetic 
 */
void * Alloc_Mem(int size) {

  // Find the correct double alighnment block size (mult of 8)
  int size_8 = size + 4;
  if (size_8 % 8 != 0) {
    size_8 += (8 - (size_8 % 8));
  }

  // variable for t_size
  int t_size;

  // block header for "current"
  blk_hdr * current = first_blk;

  // Variable to indicate whether curr block is busy (1) or free (-1)
  int busy = -1;

  // block header to store the best fit during iteration
  blk_hdr * best = NULL;

  // variable to store the block size of the best fit option
  int best_t_size = 0;

  while (current -> size_status != 1) {
    // set the t_size of the block based on the header
    t_size = current -> size_status;
    busy = -1;

    // Adjust the t_size variable according to the busy status
    // Check if current block is busy
    if (t_size & 1) {
      t_size = t_size - 1;
      busy = 1;
    }

    // Check if previous block is busy
    if (t_size & 2) {
      t_size = t_size - 2;
    }

    // If the current block is not busy,
    // Check whether the block size is a good fit
    if (busy == -1) {
      // Check whether memory block is large enough
      if (t_size > size_8) {
        // verify that either the best fit block is uninitialized
        // OR the current memory block is smaller than the best fit block
        if ((best == NULL) || (t_size < best_t_size)) {
          best = current;
          best_t_size = t_size;
        }
        // Exit the while loop if the preceding footer is 1 - END MARK
      } else if (((blk_hdr * )((char * ) current + t_size - 4)) -> size_status == 1) 
      		break;
    }
    // Advance the current block to the next header block in memory
    current = (blk_hdr * )((char * ) current + t_size);
  }

  // If no memory was allocated, return NULL
  if (best == NULL) return NULL;

  // Variable for remaining memory in the divided block (size of 2nd block)
  int rem_size = best_t_size - size_8;

  // Set the allocated blk's new size status
  best -> size_status = size_8 + 3;

  // Set the hdr of the new blk (after split)
  blk_hdr * rem = (blk_hdr * )((char * ) best + size_8);
  rem -> size_status = rem_size + 2;

  // Set the footer of the free blk
  blk_hdr * rem_footer = (blk_hdr * )((char * ) rem - 4);
  rem_footer -> size_status = size_8;

  // Set the return variable to the start of the payload
  blk_hdr * ret = (blk_hdr * )((char * ) best + 4);

  return ret;

}
/* 
  Function for freeing up a previously allocated block 
 * Argument - ptr: Address of the block to be freed up 
 * Returns 0 on success 
 * Returns -1 on failure 
 * Here is what this function should accomplish 
 * - Return -1 if ptr is NULL
 * - Return -1 if ptr is not 8 byte aligned or if the block is already freed
 * - Mark the block as free 
 * - Coalesce if one or both of the immediate neighbours are free 
 */
int Free_Mem(void * ptr) {
  // Check if the passed-in ptr is NULL
  if (ptr == NULL) {
    return -1;
  }

  // Check 8-byte alignment
  if (((unsigned int) ptr) % 8 != 0) {
    return -1;
  }

  // Create a block header using the payload ptr
  blk_hdr * header = (blk_hdr * )((char * ) ptr - 4);

  // Set the t_size of the block
  int t_size = header -> size_status;

  // Variable to indicate busy status of the neighbor blocks
  int prevBusy = -1;
  int nextBusy = -1;

  // Adjust t_size of the current block
  if (t_size & 1) {
    t_size = t_size - 1;
  }

  if (t_size & 2) {
    // PREV BLK BUSY
    prevBusy = 1;
    t_size = t_size - 2;
  }

  // find the footer of the previous block
  blk_hdr * footPrev = header - 1;

  // find the header of the previous block
  blk_hdr * hdrPrev = (blk_hdr * )((char * ) footPrev - (footPrev -> size_status) + 4);

  // find the header of the next block
  blk_hdr * next = (blk_hdr * )((char * ) header + t_size);

  // Find the busy status of the next block
  if ((next -> size_status) & 1) {
    nextBusy = 1;
  }

  // Check condition where surrounding blocks are busy
  if (prevBusy == 1 && nextBusy == 1) {
    // NO COALESCING REQUIRED

    header -> size_status -= 1;
    blk_hdr * footer = (blk_hdr * )((char * ) header + t_size - 4);
    footer -> size_status = t_size;
    next -> size_status -= 2;

    return 0;
  }

  // Check condition where the prev block is free, next is busy
  else if (prevBusy == -1 && nextBusy == 1) {
    // COALESCE WITH PREV

    int prevBlkSize = footPrev -> size_status;
    int newSize = prevBlkSize + t_size;

    hdrPrev -> size_status = newSize + 2;
    blk_hdr * footer = (blk_hdr * )((char * ) hdrPrev + newSize - 4);
    footer -> size_status = newSize;
    next -> size_status -= 2;
    return 0;
  }

  // Check condition where the next block is free, previous is busy
  else if (prevBusy == 1 && nextBusy == -1) {
    // COALESCE WITH NEXT

    int nextBlkSize = next -> size_status - 2;
    int newSize = t_size + nextBlkSize;

    blk_hdr * nextFoot = (blk_hdr * )((char * ) next + nextBlkSize - 4);
    nextFoot -> size_status = newSize;
    header -> size_status = newSize + 2;

    return 0;
  }

  // Check condition where both neighboring blocks are free
  else if (prevBusy == -1 && nextBusy == -1) {
    // COALESCE WITH PREV AND NEXT

    int prevBlkSize = footPrev -> size_status;
    int nextBlkSize = next -> size_status - 2;
    int newSize = prevBlkSize + nextBlkSize + t_size;

    blk_hdr * footer = (blk_hdr * )((char * ) hdrPrev + newSize - 4);
    footer -> size_status = newSize;
    hdrPrev -> size_status = newSize + 2;

    return 0;
  }

  return -1;
}

/*
 * Function used to initialize the memory allocator
 * Not intended to be called more than once by a program
 * Argument - sizeOfRegion: Specifies the size of the chunk which needs to be allocated
 * Returns 0 on success and -1 on failure 
 */
int Init_Mem(int sizeOfRegion) {
  int pagesize;
  int padsize;
  int fd;
  int alloc_size;
  void * space_ptr;
  blk_hdr * end_mark;
  static int allocated_once = 0;

  if (0 != allocated_once) {
    fprintf(stderr,
      "Error:mem.c: Init_Mem has allocated space during a previous call\n");
    return -1;
  }
  if (sizeOfRegion <= 0) {
    fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
    return -1;
  }

  // Get the pagesize
  pagesize = getpagesize();

  // Calculate padsize as the padding required to round up sizeOfRegion 
  // to a multiple of pagesize
  padsize = sizeOfRegion % pagesize;
  padsize = (pagesize - padsize) % pagesize;

  alloc_size = sizeOfRegion + padsize;

  // Using mmap to allocate memory
  fd = open("/dev/zero", O_RDWR);
  if (-1 == fd) {
    fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
    return -1;
  }
  space_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE,
    fd, 0);
  if (MAP_FAILED == space_ptr) {
    fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
    allocated_once = 0;
    return -1;
  }

  allocated_once = 1;

  // for double word alignement and end mark
  alloc_size -= 8;

  // To begin with there is only one big free block
  // initialize heap so that first block meets 
  // double word alignement requirement
  first_blk = (blk_hdr * ) space_ptr + 1;
  end_mark = (blk_hdr * )((void * ) first_blk + alloc_size);

  // Setting up the header
  first_blk -> size_status = alloc_size;

  // Marking the previous block as busy
  first_blk -> size_status += 2;

  // Setting up the end mark and marking it as busy
  end_mark -> size_status = 1;

  // Setting up the footer
  blk_hdr * footer = (blk_hdr * )((char * ) first_blk + alloc_size - 4);
  footer -> size_status = alloc_size;

  return 0;
}

/* 
 * Function to be used for debugging 
 * Prints out a list of all the blocks along with the following information i
 * for each block 
 * No.      : serial number of the block 
 * Status   : free/busy 
 * Prev     : status of previous block free/busy
 * t_Begin  : address of the first byte in the block (this is where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block (as stored in the block header) 
 * (including the header/footer)
 */
void Dump_Mem() {
  int counter;
  char status[5];
  char p_status[5];
  char * t_begin = NULL;
  char * t_end = NULL;
  int t_size;

  blk_hdr * current = first_blk;
  counter = 1;

  int busy_size = 0;
  int free_size = 0;
  int is_busy = -1;

  fprintf(stdout, "************************************Block list***\
                    ********************************\n");
  fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
  fprintf(stdout, "-------------------------------------------------\
                    --------------------------------\n");

  while (current -> size_status != 1) {
    t_begin = (char * ) current;
    t_size = current -> size_status;

    if (t_size & 1) {
      // LSB = 1 => busy block
      strcpy(status, "Busy");
      is_busy = 1;
      t_size = t_size - 1;
    } else {
      strcpy(status, "Free");
      is_busy = 0;
    }

    if (t_size & 2) {
      strcpy(p_status, "Busy");
      t_size = t_size - 2;
    } else {
      strcpy(p_status, "Free");
    }

    if (is_busy)
      busy_size += t_size;
    else
      free_size += t_size;

    t_end = t_begin + t_size - 1;

    fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n", counter, status,
      p_status, (unsigned long int) t_begin, (unsigned long int) t_end, t_size);

    current = (blk_hdr * )((char * ) current + t_size);
    counter = counter + 1;
  }

  fprintf(stdout, "---------------------------------------------------\
                    ------------------------------\n");
  fprintf(stdout, "***************************************************\
                    ******************************\n");
  fprintf(stdout, "Total busy size = %d\n", busy_size);
  fprintf(stdout, "Total free size = %d\n", free_size);
  fprintf(stdout, "Total size = %d\n", busy_size + free_size);
  fprintf(stdout, "***************************************************\
                    ******************************\n");
  fflush(stdout);

  return;
}

