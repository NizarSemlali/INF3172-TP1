#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

int main ( int argc, char *argv[]) {


    FILE* fichier = NULL;
    FILE* CD = NULL ; 


    fichier = fopen("instructions.txt", "r");

    if (fichier != NULL) {

    	printf("Le fichier existe !\n");
    } else {

        fprintf(stderr, "%s\n", strerror(errno));

    }

    
    CD = fopen("systemeFichier.txt", "w");

    if ( CD != NULL ) {

        printf("Le fichier existe !\n");
        
    } else {

        fprintf(stderr, "%s\n", strerror(errno));

    }
    
    fclose(fichier);
    return 0 ; 


}
