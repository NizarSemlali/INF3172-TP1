#include "tp1.h"


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
                
        char line [ 320 ]; // Taille maximale qu'une ligne pourrait prendre 
 
        while ( fgets ( line, sizeof line, instructions ) != NULL ) { // Lecture de chaque ligne
            
            int compteur = 1 ;
            char *token; 
            char *newLine = strchr(line ,'\n'); // Suppression du retour chariot 
            if (newLine) *newLine = 0;
            token = strtok(line," ");

            if (strcmp(token,"creation_repertoire") == 0 ) {

                printf("Instruction : création répertoire \n") ; 

            } else if (strcmp(token,"suppression_repertoire") == 0) {

                printf("Instruction : Suppression répertoire  \n") ; 

            } else if (strcmp(token,"creation_fichier") == 0) {

                printf("Instruction : Création fichier  \n") ; 

            } else if (strcmp(token,"suppression_fichier") == 0) {

                printf("Instruction : Suppression fichier  \n") ; 

            } else if (strcmp(token,"lire_fichier") == 0) {

                printf("Instruction : Lire fichier \n") ; 

            }

            while( token != NULL ) {
         
                token = strtok(NULL, " ");

                if ( token != NULL ) {

                    printf( "Paramètre %d : %s\n", compteur , token );
                    compteur++ ; 

                }
            
            }

        }

      fclose(disque);
      fclose(instructions);



    }


    return 0 ; 


}


void creation_repertoire(char* chemin){

printf("Créer répertoire") ; 

}

void suppression_repertoire(char* chemin){

    printf("Supprimer répertoire") ; 
}

void creation_fichier(char* chemin, char* contenu){

    printf("Créer fichier") ; 
}

void suppression_fichier(char* chemin) {

    printf("Supprimer fichier") ; 
}

void lire_fichier(char* chemin){

    printf("Lire fichier") ; 
}






