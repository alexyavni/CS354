/*a few allocations in multiples of 4 bytes*/
#include <assert.h>
#include <stdlib.h>
#include "mem.h"

int main() {
   assert(Init_Mem(4096) == 0);
   printf("****** ALLOC 4 ******\n");
   assert(Alloc_Mem(4) != NULL);
   printf("****** ALLOC 8 ******\n");
   assert(Alloc_Mem(8) != NULL);
   printf("****** ALLOC 16 ******\n");
   assert(Alloc_Mem(16) != NULL);
   printf("****** ALLOC 24 ******\n");
   assert(Alloc_Mem(24) != NULL);
   exit(0);
}
