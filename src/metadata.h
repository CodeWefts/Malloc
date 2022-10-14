#include <stdbool.h>

typedef struct Metadata
{
   // Add metadata information needed
   // e.g. size of the block, pointer, free/occupied
   struct Metadata* next;
   bool busy; // False , si non-defini 
   long unsigned int arraySize;
   void* adressArray;

} Metadata;