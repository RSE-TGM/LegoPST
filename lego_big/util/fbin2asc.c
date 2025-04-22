/*
   modulo fbin2asc.c
   tipo 
   release 1.3
   data 7/21/94
   reserved @(#)fbin2asc.c	1.3
*/
#include <stdio.h>

void leggi_testa_bin(); 
void scrivi_testa_asc();
void scrivi_corpo_asc();
int leggi_corpo_bin();

int main(int argc, char *argv[])
{
   leggi_testa_bin();
   scrivi_testa_asc();
   while ( leggi_corpo_bin() )
      scrivi_corpo_asc();
}
