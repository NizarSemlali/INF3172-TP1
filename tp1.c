nclude "tp1.h"
#define MKDIR "creation_repertoire"
#define RMDIR "suppression_repertoire"
#define MKFILE "creation_fichier"
#define RMFILE "suppression_fichier"
#define READFILE "lire_fichier"


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
            
            char* commande = (char *)malloc(22) ; 
            char* chemin = (char *)malloc(40); 
            int compteur = 1 ;
            char *token; 
            char *newLine = strchr(line ,'\n'); // Suppression du retour chariot 
            if (newLine) *newLine = 0;
            
            token = strtok(line," ");
            strcpy(commande,token);


            token = strtok(NULL, " ");
            strcpy(chemin,token); 





            if (strcmp(commande,MKDIR) == 0 ) {

                creation_repertoire(chemin);

            } else if (strcmp(commande,RMDIR) == 0) {

                suppression_repertoire(chemin);


            } else if (strcmp(commande,MKFILE) == 0) {

                char* contenu = (char *)malloc(256);

                while( token != NULL ) {
         
                    token = strtok(NULL, " ");
                    printf("Token : %s \n", token);
                    strcat(contenu,token);
            
                }

                //printf("Contenu : %s \n", contenu);

                creation_fichier(chemin,contenu);

                free(contenu);


            } else if (strcmp(commande,RMFILE) == 0) {

                suppression_fichier(chemin);

            } else if (strcmp(commande,READFILE) == 0) {

                lire_fichier(chemin);

            } 
            
            free(commande);
            free(chemin);

        }

      
      fclose(disque);
      fclose(instructions);



    }


    return 0 ; 


}


void creation_repertoire(char* chemin){

printf("Créer répertoire \n") ; 

}

void suppression_repertoire(char* chemin){

    printf("Supprimer répertoire \n") ; 
}

void creation_fichier(char* chemin, char* contenu){

    printf("Créer fichier \n") ; 
}

void suppression_fichier(char* chemin) {

    printf("Supprimer fichier \n") ; 
}

void lire_fichier(char* chemin){

    printf("Lire fichier \n") ; 
}
