#include <stdlib.h>
#include <stdio.h>
#include <m_allocator.h>
 
void test_alloc()
{
   // MALLOC
   int* a = m_malloc(13*sizeof(char));

   int* c = m_malloc(16*sizeof(int));

   m_free(c);
   int* b = m_malloc(32*sizeof(int));
}

void test_free_split()
{
   printf("\n\n############ Test free and split : ##############\n");

   printf("\nAllocation de memoire de la taille d'un int");
   int* e = m_malloc(sizeof(int));
   *e = 10;
   printf("e = %d\n",*e);

   printf("\nAllocation de memoire de la taille d'un long int");
   long int* a = m_malloc(10*sizeof(long int));
   *a = 10;
   printf("a = %ld\n",*a);

   printf("\nAllocation de memoire de la taille d'un int");
   int* b = m_malloc(sizeof(int));
   *b = 10;
   printf("b = %d\n",*b);

   printf("\nFree sur le long int\n");
   m_free(a);

   printf("\nAllocation de memoire de la taille d'un int a partir de l'adresse memoire du long int ");
   int* c = m_malloc(sizeof(int));
   *c = 10;
   printf("c = %d\n",*c);

   printf("\nAllocation de memoire de la taille d'un int apres le int precedent (ici un split est effectue)");
   int* d = m_malloc(sizeof(int));
   *d = 10;
   printf("d = %d\n",*d);
}

void test_realloc()
{
   printf("\n\n############ Test free and split : ##############\n");

   printf("\nAllocation de memoire de la taille d'un int");
   int* e = m_malloc(sizeof(int));
   *e = 10;
   printf("e = %d\n",*e);

   printf("\nAllocation de memoire de la taille d'un int");
   int* a = m_malloc(sizeof(int));
   *a = 10;
   printf("a = %d\n",*a);

   printf("\nAllocation de memoire de la taille d'un int");
   int* b = m_malloc(sizeof(int));
   *b = 10;
   printf("b = %d\n",*b);

   m_free(b);
   m_realloc(a, sizeof(long int));
}

void test_realloc1()
{
   // realloc for the last block
   int *a = m_malloc(sizeof(int));
   *a = 10;
   printf("(malloc) a = %d\n", *a);
   
   m_realloc(a,2*sizeof(int));
   printf("(realloc) a = %d\n", *a);
   free(a);
}

void test_realloc2()
{
   // realloc for the next block is free and big enough
   int *a = m_malloc(sizeof(int));
   *a = 10;
   printf("(malloc) a = %d\n", *a);

   int *b = m_malloc(sizeof(int));
   *b = 20;
   printf("(malloc) b = %d\n", *b);

   int *c = m_malloc(sizeof(int));
   *c = 30;
   printf("(malloc) c = %d\n", *c);

   m_free(b);
   m_realloc(a, 2*sizeof(int));
   printf("(realloc) a = %d\n", *a);
}

void test_realloc3()
{
   // realloc for decrease the size
   int *a = m_malloc(34*sizeof(long long));
   *a = 10;
   printf("(malloc) a = %d\n", *a);

   m_realloc(a, 2*sizeof(long long));
   printf("(realloc) a = %d\n", *a);
   free(a);
}

void test_realloc4()
{
      // realloc for the next block is free and big enough
   int *a = m_malloc(sizeof(int));
   *a = 10;
   printf("(malloc) a = %d\n", *a);

   int *b = m_malloc(sizeof(int));
   *b = 20;
   printf("(malloc) b = %d\n", *b);

   int *c = m_malloc(sizeof(int));
   *c = 30;
   printf("(malloc) c = %d\n", *c);

   int *d = m_malloc(sizeof(int));
   *d = 40;
   printf("(malloc) d = %d\n", *d);

   free(c);
   m_realloc(a, 2*sizeof(int));
   printf("(realloc) a = %d\n", *a);
   free(a);
   free(c);
   free(d);
}

int main()
{
   printf("%c", 1);
   m_setup_hooks();
   test_alloc();
   //test_free_split();
   //test_realloc2();
 
   return 0;
}
