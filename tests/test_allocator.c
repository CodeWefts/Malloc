#include <stdlib.h>
#include <stdio.h>
 
#include <m_allocator.h>
 
void test_alloc()
{
   // MALLOC
   m_show_info();
   
   int* a = m_malloc(sizeof(int));
   printf("\n(malloc) a = %d\n", *a);
}
 
int main()
{
   m_setup_hooks();
   test_alloc();
 
   return 0;
}
