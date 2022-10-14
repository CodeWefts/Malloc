
#include <malloc.h> // Nécessaire pour les hooks
#include "../include/m_allocator.h"
#include "../src/metadata.h"
#include <unistd.h>

static Metadata* gHead = NULL;
void* malloc_hook(size_t size, const void* caller)            { return m_malloc(size); }
void* realloc_hook(void* ptr, size_t size, const void* caller){ return m_realloc(ptr, size); }
void  free_hook(void* ptr, const void* caller)                { return m_free(ptr); }


void m_setup_hooks(void)
{
   //__malloc_hook = malloc_hook;
   //__realloc_hook = realloc_hook;
   //__free_hook = free_hook;
}
 

Metadata* get_free_block(size_t size)
{
   /*
   FONCTION qui renvoie une structure/un bloc libre dont la taille est >= size
   ou NULL dans le cas inverse.

   ENTREE : entier size
   SORTIE : structure metadata SI un bloc est libre SINON NULL
   */

   Metadata* data = gHead;
   while(data != NULL)
   {
      if (data->arraySize >= size && data->busy==false) 
      {
         return data;
      }
      data = data->next;
   }
   
   return NULL;

}


void* m_malloc(size_t size) 
{
   /*Fonction qui permet d'obtenir le nombre d'allocation en octets
   Entrée : Un entier size
   Sortie : 
   */ 

   Metadata* data = get_free_block(size);

   if (data != NULL)
   {
      data->busy = true;
      return data->adressArray;
   }
   
   {
      {
      data = sbrk(sizeof(Metadata));
      data->arraySize = size;
      data->adressArray = sbrk(size);
      data->busy = true;
      }


      return data->adressArray;
   }


}



void* m_realloc(void* ptr, size_t size)
{
   return NULL;
}
 
void* m_calloc(size_t nb, size_t size)
{
   return NULL;

}
 
void m_free(void* ptr)
{
   
}
 


void m_show_info(void)
{
   //printf("\n\n INFORMATION :  %ld , %p , %ld,  %p \n\n\n",sizeof(Metadata), sbrk(sizeof(Metadata)), sizeof(int) ,sbrk(sizeof(int)));
}

