
#include <malloc.h> // Necessaire pour les hooks
#include "../include/m_allocator.h"
#include "../src/metadata.h"
#include <unistd.h>

static Metadata* gHead = NULL;

void* malloc_hook(size_t size, const void* caller)            { return m_malloc(size); }
void* realloc_hook(void* ptr, size_t size, const void* caller){ return m_realloc(ptr, size); }
void  free_hook(void* ptr, const void* caller)                { return m_free(ptr); }

void m_setup_hooks(void) //fonction qui ne marche pas sous machine virtuelle
{
   //__malloc_hook = malloc_hook;
   //__realloc_hook = realloc_hook;
   //__free_hook = free_hook;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void m_show_info(Metadata* data)
{
   /*
   Fonction m_show_info servant a afficher les details du projet
   ENTREE : Une structure metadata 
   SORTIE : Affichage de la taille , l'adresse et du booleen montrant si la structure est occupé ou non 
   */

   printf("\nTAILLE DE LA DATA : %ld \n", data->arraySize);
   printf("ADRESSE DE LA DATA : %p \n", data->addressArray);
   printf("OCCUPER ? (1 = TRUE , 0 = FALSE) : %d \n\n", data->busy);
}
Metadata* get_free_block(size_t size)
{
   /*
   Fonction get_free_block servant à trouver un block libre dans la liste chainee 
   ENTREE : un entier size correspondant a la taille que l'on veut pouvoir placer dans la structure
   SORTIE : si le block est trouver, on renvoie la data/structure correspondant au pointeur du block 
            sinon, on renvoie NULL
   */

   Metadata* data = gHead;
   //On parcours TOUTE la structure a la recherche d'un block libre
   while(data->next != NULL)
   {
      data = data->next;
      if ((data->arraySize >= size) && (data->busy == false)) 
      {
         return data;
      }
   }
   return NULL;
}
void* newData(size_t size)
{
   /*
   Fonction newData servant a la creation d'un nouveau block rattache a la structure 
   ENTREE : Un entier size correspondant a la taille du block que l'on veut ajouter a notre liste chainee
   SORTIE : Renvoie la data/structure correspondant au pointeur du block 
   */

   Metadata* newData = sbrk(sizeof(Metadata));
   Metadata* last = gHead;

   //creation de la nouvelle structure
   newData->addressArray = sbrk(size);
   newData->arraySize = size;
   newData->busy = true;
   newData->next = NULL;

   //On parcours TOUTE la structure a la recherche du dernier element pour placer notre nouvelle structure
   while(last->next != NULL)
   {
      last = last->next;
   }

   last->next = newData;
   m_show_info(newData);
   return newData;
}
void* m_malloc(size_t size) 
{
   /*
   Fonction m_malloc servant a "gerer/controler" le comportement de la memoire 
   ENTREE : Un entier size correspondant a la taille de la memoire que l'on veut allouee
   SORTIE : Renvoie la data/structure correspondant au pointeur du block
   */

   // On verifie que la taille est toujours un multiple de 8 
   while(size%8 != 0)
   {
      size++;
   }

   // On definit gHead en tant que noeud principale de la structure
   if(gHead == NULL)
   {
      gHead = sbrk(sizeof(Metadata));
      gHead->addressArray = sbrk(size);
      gHead->arraySize = size;
      gHead->busy = true;
      gHead->next = NULL;

      m_show_info(gHead);
      return gHead->addressArray;
   }

   // si gHead est definit on 
   else // On verite le else ici n'est pas obligatoire mais on peut fermer pour que ce soit plus jolie (^~^)
   {
      Metadata* data = get_free_block(size);

      // On verifie que data est NULL pour cree une nouvelle structure 
      if (data == NULL)
      { 
         Metadata* new = newData(size);
         if(new->addressArray == (void *)-1){return NULL;}
         return new->addressArray; 
      }
      // Si data n'est pas NULL
      else
      {
         if(data->arraySize >= size + sizeof(Metadata)){split(data,size);}
         m_show_info(data);
         if(data->addressArray == (void *)-1){return NULL;}
         return data->addressArray;
      }
   }
}
Metadata* split(Metadata* data, size_t size)
{
   /*
   Fonction split servant à diviser l'espace d'un block libre et assez grand afin d'optimiser l'espace d'allocation de memoire
   ENTREE : Une metadata libre dont la taille est assez grande et un entier size servant à savoir l'espace pris 
   SORTIE : Renvoie la data/structure correspondant au pointeur du block
   */

   Metadata* splitData = data->addressArray + size;
   
   // On cree la structure split
   splitData->addressArray = data->addressArray + size + sizeof(Metadata);
   splitData->arraySize = data->arraySize - (size + sizeof(Metadata));
   splitData->next = data->next;
   splitData->busy = false;

   // On modifie la structure que l'on a split
   data->busy = true;
   data->arraySize = size;
   data->next = splitData;

   return splitData;
}
void fusion(void)
{
   /*
   Fonction fusion servant à fusionner plusieurs structure consecutives dont l'espace n'est pas occupe 
   ENTREE : NONE
   SORTIE : NONE
   */

   Metadata* firstData = gHead;
   Metadata* lastData = firstData->next;
   
   // On parcours toute la structure (IL VAUT MIEUX CREE UN SCHEMA AFIN DE COMPRENDRE COMMENT CELA FONCTIONNE)
   while(lastData != NULL)
   {
      //Lorsque les blocks consecutifs sont libres on agrandi la taille du premier block et on change le next au prochaine de lastData (On skip lastData en gros) cela permet de supprimer la structure non utiliser
      if(firstData->busy == false && lastData->busy == false)
      {
         firstData->arraySize += sizeof(Metadata) + lastData->arraySize; // taille
         lastData = lastData->next;
         firstData->next = lastData;
      }

      //Lorsque lastData tombe sur une structure non-libre on change la place de firstData -> lastData
      else
      {
         firstData = lastData;
         lastData = lastData->next;
      }
   }
   
}
void m_free(void* ptr)
{
   /*
   Fonction m_free servant a definir une structure en tant que non-occuper
   ENTREE : Un pointeur donnant/amenant au block que l'on veut liberer
   SORTIE : 
   */

   Metadata* data = gHead;
   
   // On parcours TOUTE la strucuture
   while(data != NULL)
   {
      if(data->addressArray == ptr)
      {
         data->busy = false;
         fusion(); // cette ligne peut etre deplacer au debut de la boucle, mais elle est bien la aussi, rien de change

         //si le prochain de celui que l'on veut liberer est NULL, on peut tout simplement supprimer la structure que l'on veut liberer afin de ne pas perdre de memoire
         if(data->next == NULL)
         {
            sbrk(-(sizeof(Metadata) + data->arraySize));

         }
         break;
         
      }

      

      data = data->next;
   }
}
void* m_calloc(size_t nb, size_t size)
{
   /*
   Fonction m_calloc servant a initialiser la memoire avec des zero
   ENTREE : Un entier nb correspondant au nombre de composant se trouvant dans le tableau, et un entier size correspondant a la taille du tableau
   SORTIE : Renvoie le tableau composer de zero
   */

   int prod = size * nb;
   char* tab = m_malloc(prod); //recup le pointeur que renvoie malloc
   
   if( tab == (void *)-1){return NULL;}
   else
   {
      for(int i = 0; tab[i] < prod; i++) 
      {
         tab[i] = 0;
      }

      return tab;
   }  
}
void* m_realloc(void* ptr, size_t size)
{
   /*
   Fonction m_realloc servant a "reorganiser la memoire" 
   ENTREE : Un pointeur que l'on voudra liberer et un entier size correspondant a la taille redefinissant celle du bloc pointer 
   SORTIE : Renvoie un tableau/data/bloc si on trouve le pointeur, sinon renvoie NULL
   */

   int sizeWanted = size;
   Metadata* data = gHead;

   while(data != NULL)
   {
      if (data->addressArray == ptr)
      {
         m_free(ptr);

         // BLOC QUI SERT A COPIER
         char * tab = data->addressArray;
         char * tableau = m_malloc(sizeWanted);

         for(int i = 0; i < sizeWanted ; i++)
         {
            if(i < (int)data->arraySize)
            {
               tableau[i] = tab[i]; 
            }

            else
            {
               tableau[i] = 0;
            }
         } // FINDU BLOC QUI COPIE

         return tableau;

      }
   data = data->next;
   
   }
   return NULL;
}





