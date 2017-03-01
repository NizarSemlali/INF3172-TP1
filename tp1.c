#include "tp1.h"
#define MKDIR "creation_repertoire"
#define RMDIR "suppression_repertoire"
#define MKFILE "creation_fichier"
#define RMFILE "suppression_fichier"
#define READFILE "lire_fichier"
#include <sys/types.h>
#include <sys/stat.h>



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

        // Lecture de chaque ligne
        while ( fgets ( line, sizeof line, instructions ) != NULL ) { 
        /*    
            char* commande = (char *)malloc(22) ; 
            char* chemin = (char *)malloc(40); 
            char *token; 
            // Suppression du retour chariot 
            char *newLine = strchr(line ,'\n'); 
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
                
             	token = strtok(NULL, " ");
             	
				while( token != NULL ){
					
                    strcat(contenu,token);
                    strcat(contenu," ");
					token = strtok(NULL, " ");
                    
                }
//il faut gere l'espace a la fin !
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

 	int statue;
 	statue = mkdir(chemin,0777);

 	if (statue == -1)
        printf("repertoire non cree\n");
	else 
		printf("repertoire cree\n");
	
}

void suppression_repertoire(char* chemin){

    printf("Supprimer répertoire \n") ; 
 //   rmdir(chemin);
}

void creation_fichier(char* chemin, char* contenu){

    printf("Créer fichier %s\n",contenu) ; 

        char *nomDuFichier;
    FILE* fichier = NULL ; 

    nomDuFichier = nom_de_fichier(chemin);
    
    printf( "Nom du fichier:  %s\n", nomDuFichier );
	fichier = fopen(nomDuFichier, "r");

	if(fichier != NULL) {

		fprintf(stderr, "%s\n", strerror(errno));

	} else {

		fichier = fopen(nomDuFichier, "w");
		fputs(contenu,fichier);
		
	}

	fclose(fichier)	;
}



void suppression_fichier(char* chemin) {

	int statut ; 
	char *nomDuFichier ; 
    nomDuFichier = nom_de_fichier(chemin);
   
   statut = remove(nomDuFichier);

   if(statut != 0) 
   {
      fprintf(stderr, "%s\n", strerror(errno));
   }
    
}

void lire_fichier(char* chemin){

    FILE* fichier = NULL ; 
    char *nomDuFichier ; 
    nomDuFichier = nom_de_fichier(chemin);
    char chaine[256] = "";

    fichier = fopen(nomDuFichier, "r");

	if(fichier == NULL) {

		fprintf(stderr, "%s\n", strerror(errno));

	} else {

		 while (fgets(chaine, 256, fichier) != NULL) 
        {
            printf("%s", chaine); // On affiche la chaîne qu'on vient de lire
        }
        fclose(fichier);

	}

	

}


char* nom_de_fichier(char*chemin){

	char *token;
	char *nomDuFichier;
	nomDuFichier = token = strtok(chemin, "/");
 
   	while( token != NULL ) 
    {
        nomDuFichier = token;
        token = strtok(NULL, "/");
    }

    return nomDuFichier;

}
*/


     
     




