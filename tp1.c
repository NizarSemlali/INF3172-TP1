#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

typedef struct {

    char chaine[16];

} bloc ;



int main ( int argc, char *argv[]) {

// Ouverture ou création du disque de 500 ko

    FILE* disque = NULL ; 
    disque = fopen("disque.txt", "rb+"); 

    if (disque == NULL) {

        int i ; 
        disque = fopen("disque.txt", "wb+");
        bloc blocLibre ; 
        
        for ( i = 0 ; i < 32000 ; i++) {
            fwrite(&blocLibre,sizeof(blocLibre),1,disque);
        }

    }

// Ouverture du fichier d'instructions 

    FILE* instructions = NULL;

    instructions = fopen("instructions.txt", "r");

    if (instructions == NULL) {

         fprintf(stderr, "%s\n", strerror(errno));

    } else {

        char c ; 
        char instruction[100];
        int i = 0 ; 
        char *token;

        
        do{    
            c = fgetc(instructions);  
            if( feof(instructions) )
            {
                break ;
            }
            instruction[i] = c ; 
            i++;
        } while ( c != '\n');

        token = strtok(instruction," ");



        while (token != NULL)
        {
            printf( "%s \n" , token);
            token = strtok(NULL, " ");
        }


        /* while ( strcmp(fgetc(nom,40,instructions)," ") != 0 ) {
    
            if ( strcmp(nom,"creation_repertoire") == 0 ) {

                printf("creation repertoire \n") ; 

            } else if (strcmp(nom,"suppression_repertoire") == 0) {

                printf("suppression repertoire  \n") ; 

            } else if (strcmp(nom,"creation_fichier") == 0) {

                printf("creation fichier  \n") ; 

            } else if (strcmp(nom,"suppression_fichier") == 0) {

                printf("suppression fichier  \n") ; 

            } else if (strcmp(nom,"lire_fichier") == 0) {

                printf("lire fichier  \n") ; 

            }

        } */

     }

    fclose(disque);
    fclose(instructions);


    return 0 ; 


}
