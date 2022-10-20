                PROJET MALLOC


CREATION DE LA STRUCTURE Metadata :

Dans le fichier metadata.h on cree une metadata qui possede
- une metadata "next";
- un booleen busy;
- un long unsigned int arraySize;
- un pointeur void addressArray;


CREATION DES FONCTIONS DU PROJET :

Dans le fichier m_allocator.c on cree les fonctions 

- void m_show_info(Metadata* meta) - 
  Affiche les informations du projet

- Metadata* get_free_block(size_t size)
   Fonction get_free_block servant à trouver un block libre dans la liste chainee 
   ENTREE : un entier size correspondant a la taille que l'on veut pouvoir placer 
   dans la structure
   SORTIE : si le block est trouver, on renvoie la data/structure correspondant 
   au pointeur du block 
   sinon, on renvoie NULL

- void* newData(size_t size)
   Fonction newData servant a la creation d'un nouveau block rattache a la structure 
   ENTREE : Un entier size correspondant a la taille du block que l'on veut ajouter 
   a notre liste chainee
   SORTIE : Renvoie la data/structure correspondant au pointeur du block 

- void* m_malloc(size_t size)
   Fonction m_malloc servant a "gerer/controler" le comportement de la memoire 
   ENTREE : Un entier size correspondant a la taille de la memoire que 
   l'on veut allouee
   SORTIE : Renvoie la data/structure correspondant au pointeur du block

- Metadata* split(Metadata* meta, size_t size)
   Fonction split servant à diviser l'espace d'un block libre et assez grand 
   afin d'optimiser l'espace d'allocation de memoire
   ENTREE : Une metadata libre dont la taille est assez grande et un entier 
   size servant à savoir l'espace pris 
   SORTIE : Renvoie la data/structure correspondant au pointeur du block

- void fusion(void)
   Fonction fusion servant à fusionner plusieurs structure consecutives dont 
   l'espace n'est pas occupe 
   ENTREE : NONE
   SORTIE : NONE

- void m_free(void* ptr)
   Fonction m_free servant a definir une structure en tant que non-occuper
   ENTREE : Un pointeur donnant/amenant au block que l'on veut liberer
   SORTIE : NONE

- void* m_calloc(size_t nb, size_t size)
   Fonction m_calloc servant a initialiser la memoire avec des zero
   ENTREE : Un entier nb correspondant au nombre de composant se trouvant dans 
   le tableau, et un entier size correspondant a la taille du tableau
   SORTIE : Renvoie le tableau composer de zero

- void* m_realloc(void* ptr, size_t size)
   Fonction m_realloc servant a "reorganiser la memoire" 
   ENTREE : Un pointeur que l'on voudra liberer et un entier size correspondant a 
   la taille redefinissant celle du bloc pointer 
   SORTIE : Renvoie un tableau/data/bloc si on trouve le pointeur, sinon renvoie NULL
