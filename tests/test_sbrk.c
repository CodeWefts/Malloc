#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
 
int main()
{
    char texte[] = "hey";
    sbrk(sizeof(texte));
    int* a = sbrk(sizeof(texte));
    printf("%p\n",a);

    char b = brk(a);
    printf("%s\n",b);


}
