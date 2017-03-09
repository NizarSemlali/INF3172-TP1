/****************************************************************************
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*     http://www.apache.org/licenses/LICENSE-2.0
* 
*   Copyright [2016] [Nizar Semlali]
*   Copyright [2016] [Othmane Bouaddi]
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
****************************************************************************/

#include "tp1.h"

                        //  --------------------------------  //
                        //                                    //
                        //           Fonction main            //
                        //                                    //
                        //  --------------------------------  //

int main ( int argc, char *argv[]) {

// Ouverture ou création du disque de 500 ko
    disque = fopen("disque.bin", "rb+"); 

    if (disque == NULL) {

      disque = fopen("disque.bin", "ab+");

    }

// Ouverture du fichier d'instructions passé en paramètre
    FILE* instructions = NULL;
    instructions = fopen(argv[1], "r");

    if (instructions == NULL) {

        fprintf(stderr, "Erreur ! Le fichier d'instructions n'existe pas ! \n");
        exit(0);

    } else {

        char line [ 400 ]; // Taille maximale qu'une ligne pourrait prendre 

        // Lecture de chaque ligne
        while ( fgets ( line, sizeof line, instructions ) != NULL ) { 

            char* commande = calloc(22, sizeof(char)); 
            char* chemin = calloc(40, sizeof(char)); 
            char *token; 
            
            char *newLine = strchr(line ,'\n'); // Suppression du retour chariot 
            if (newLine) *newLine = 0;

            
            token = strtok(line," ");
            strcpy(commande,token);     // Récupération de la commande

            token = strtok(NULL, " ");
            if ( strlen(token) > 40 ) {
                
                fprintf(stderr, "Le chemin absolu est trop long !\n");

            } else {
                
                strcpy(chemin,token); // Récupération du chemin

              // Exécution de l'instruction en question : 
                if (strcmp(commande,MKDIR) == 0 ) {

                    creation_repertoire(chemin);

                } else if (strcmp(commande,RMDIR) == 0) {

                    suppression_complete_dir(chemin);

                } else if (strcmp(commande,MKFILE) == 0) {			

                    int tailleContenu = 0 ;
                    char* contenu = calloc(256, sizeof(char));

                    token = strtok(NULL, " ");

                    while( token != NULL ){

                        tailleContenu += strlen(token) + 1 ; // +1 pour l'espace
                        
                        if (tailleContenu > 256) {
                            fprintf(stderr, "Taille du fichier trop longue !\n");
                            exit(1);
                        }

                        strcat(contenu,token);        // Récupération du contenu
                        strcat(contenu," ");
                        token = strtok(NULL, " ");
                        
                    }
                    
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

}


                      //  ------------------------------------------------------------  //
                      //                                                                //
                      //  Méthodes de validation du chemin et recherche de répertoires  //
                      //                                                                //
                      //  ------------------------------------------------------------- //


int valider_chemin(char* chemin) {

    int statut = 1 ; 
    repertoire* parent = trouver_repertoire(obtenir_parent(chemin));
    if (parent == NULL) {
        if (valider_racine(chemin) != 1) {
            fprintf(stderr, "Le répertoire parent n'existe pas ! \n");
            statut = 0;
        }
    }

    free(parent);
    return statut ; 

}


repertoire* trouver_repertoire(char* nom) {

    int tailleDisque = taille_disque(); 
    repertoire* liste = calloc(tailleDisque, sizeof(repertoire));
    repertoire* cible = NULL;    

    remplir_liste_repertoires(liste);

    int i;

    for (i = 0; i < tailleDisque; ++i) {

        if (strcmp(liste[i].nom, nom) == 0) {
            cible = malloc(sizeof(repertoire));
            *cible = liste[i];
            break;
        }
    }

    if (tailleDisque > 0) free(liste);

    return cible;
}


char* obtenir_parent(char *chemin) {
        
    char str[100];
    char *token;
    char *enfant;
    char *parent = calloc(256, sizeof(char));

    strcpy(str, chemin);
    token = strtok(str, "/");

    while (token != NULL) {
        enfant = token;
        token = strtok(NULL, "/");
        if (token != NULL) {
            strcat(parent, "/");
            strcat(parent, enfant);
        }
    }
    
    return parent;

}


int valider_racine(char* chemin) {

    return strlen(obtenir_parent(chemin)) == 0;

}


int taille_disque() {

    int tailleDisque = 0;
    repertoire dir;
    fseek(disque, SEEK_SET, 0);
    
    while (fread(&dir, sizeof (dir), 1, disque) == 1) {

        tailleDisque++;

    }

    fseek(disque, SEEK_SET, 0);
    return tailleDisque;
}


void remplir_liste_repertoires(repertoire* liste) {

    int index = 0;
    repertoire dir;
    fseek(disque, SEEK_SET, 0);
    

    while (fread(&dir, sizeof (dir), 1, disque) == 1) {
    
        liste[index++] = dir;

    }
    
    fseek(disque, SEEK_SET, 0);
      
}

int repertoire_existe(char *chemin) {
        
    repertoire* dir = trouver_repertoire(chemin);
    int existe = 1;

    if (dir == NULL) {

        existe = 0;

    }

    free(dir);
    return existe;

}      


int meme_racine(const char* chemin1, const char* chemin2) {

    int i = 0;

    if (strlen(chemin1) > strlen(chemin2)) return 0;

    while (i < strlen(chemin1)) {

        if (chemin1[i] != chemin2[i]) return 0;
        i++;

    }

    return 1;

}


                            //  --------------------------------  //
                            //                                    //
                            //       Création de répertoire       //
                            //                                    //
                            //  --------------------------------  //


void creation_repertoire(char* chemin){

    if (repertoire_existe(chemin) == 1 ) {

        fprintf(stderr, "Le répertoire existe déjà ! \n");

    } else {

        if ( valider_chemin(chemin) != 0 ) {

            repertoire dir;
            strcpy(dir.nom, chemin);
            strcpy(dir.parent, obtenir_parent(chemin));
            dir.nbr_fichiers = 0;

            if (fwrite(&dir, sizeof (dir), 1, disque) != 1) {
    
                fprintf(stderr, "La création du répertoire a échoué ! \n ");
    
            }

        }

    }


}


                            //  --------------------------------  //
                            //                                    //
                            //      Suppression de répertoire     //
                            //                                    //
                            //  --------------------------------  //


void suppression_complete_dir(char* chemin){

    if (repertoire_existe(chemin) != 1 ) {

        fprintf(stderr, "Le répertoire n'existe pas ! \n");

    } else {
        
        if ( valider_chemin(chemin) != 0 ) {
          
            fseek(disque, SEEK_SET, 0);

            int tailleListe = 0;

            repertoire *liste = calloc(taille_disque(), sizeof(repertoire));

            repertoire dir;

            while (fread(&dir, sizeof (dir), 1, disque) == 1) {

                if (meme_racine(chemin, dir.nom)) {

                    liste[tailleListe++] = dir;

                }
            }

            while (tailleListe > 0) {

                supprimer_repertoire_cible(liste[--tailleListe].nom);

            }

            free(liste);
            fseek(disque, SEEK_SET, 0);

        } 
    
    }


}


void supprimer_repertoire_cible(char* cible) {

    FILE *temp = fopen("temporary.bin", "wb");
    fseek(disque, SEEK_SET, 0);
    repertoire dir;
    fread(&dir, sizeof(repertoire), 1, disque);

    while (!feof(disque)) {

        if (strcmp(dir.nom, cible) != 0)
            fwrite(&dir, sizeof (repertoire), 1, temp);
            fread(&dir, sizeof (repertoire), 1, disque);

    }

    fclose(disque);
    fclose(temp);

    // Suppression de la copie temporaire du disque et conservation de la nouvelle
    system("mv disque.bin toDelete.bin");
    system("mv temporary.bin disque.bin");
    system("rm toDelete.bin");

    disque = fopen("disque.bin", "ab+");
    fseek(disque, SEEK_SET, 0);
}



                            //  --------------------------------  //
                            //                                    //
                            //         Création de fichier        //
                            //                                    //
                            //  --------------------------------  //



void creation_fichier(char* chemin, char* contenu) {


    printf("WIP ! \n"); 


}



                            //  --------------------------------  //
                            //                                    //
                            //       Suppression de fichier       //
                            //                                    //
                            //  --------------------------------  //



 void suppression_fichier(char* chemin) {


      printf("WIP ! \n"); 

 }


                            //  --------------------------------  //
                            //                                    //
                            //         Lecture de fichier         //
                            //                                    //
                            //  --------------------------------  //



 void lire_fichier(char* chemin){


    printf("WIP ! \n"); 

 }
