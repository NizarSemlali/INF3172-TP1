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

static int compteurInode = 1 ;


int main ( int argc, char *argv[]) {


// Ouverture ou création du disque de 500 ko
    disque = fopen("disque.bin", "rb+"); 
    disk* structDisque = (disk*) malloc(sizeof(disk));
    structDisque->listeRepertoires = (repertoire*) malloc(NOMBRE_BLOCKS * sizeof(repertoire));

    if(disque == NULL) {

        disque = fopen("disque.bin", "ab+");

        structDisque->nbr_blk = NOMBRE_BLOCKS ; 
        structDisque->nbr_blk_libres = NOMBRE_BLOCKS ; 
        structDisque->blocks = (block_t**) malloc(NOMBRE_BLOCKS * sizeof(block_t));
        structDisque->inodes = (inode*) malloc(NOMBRE_BLOCKS * sizeof(inode));
        
        // Tout les blocs sont libres au début:     

        printf("Démarrage du disque ...\n");
        printf("Création des blocs ...\n");

        int i = 0;
        do {           
            structDisque->blocks[i] = (block_t*) malloc(sizeof(block_t)); 
            if (structDisque->blocks[i] == NULL)
            {
                printf("Démarrage echoué !\n");
                exit(EXIT_FAILURE);
            }
            structDisque->blocks[i]->num_block = i;
            structDisque->blocks[i]->estVide = 0 ; 

            fwrite (structDisque->blocks[i]->data,1, sizeof(structDisque->blocks[i]->data), disque);

            i++;
    
        } while(i < NOMBRE_BLOCKS);       
    
        printf("Création de %d blocs terminée avec succès ! \n",structDisque->nbr_blk);

        // Creation du répertoire racine
        printf("Création du répertoire racine ...\n");
        repertoire* root =  (repertoire*) malloc(sizeof(repertoire));
        strcpy(root->nom, "racine");
        strcpy(root->parent, "");
        root->nbr_fichiers = 0;
        printf("Création du répertoire racine terminée avec succès ! \n");
        
        structDisque->listeRepertoires[0] = *root;
        structDisque->nbRepertoires = 1 ; 
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

            char* commande = NULL;
            char* chemin = NULL;
            commande = malloc (22 * sizeof(char));
            chemin = malloc (40 * sizeof(char));           
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

                    creation_repertoire(chemin, structDisque);

                } else if (strcmp(commande,RMDIR) == 0) {

                    suppression_complete_dir(chemin, structDisque);
                    
                } else if (strcmp(commande,MKFILE) == 0) {          

                    int tailleContenu = 0 ;
                    bool flag = false ; 
                    char* contenuTemp = calloc(256, sizeof(char));
                    char* contenu = calloc(256, sizeof(char));
                  
                    token = strtok(NULL, " "); 

                    if ( token == NULL ) 
                    {
                        fprintf(stderr, "Le fichier que vous essayez de créer ne doit pas être vide !\n");
                    } else  {

                        while( token != NULL && !flag){

                            tailleContenu += strlen(token) + 1 ; // +1 pour l'espace
                        
                            if (tailleContenu > 256) {
                                fprintf(stderr, "La taille du contenu du fichier est trop longue !\n");
                                flag = true ; 
                            }

                            strcat(contenuTemp,token);        // Récupération du contenu
                            strcat(contenuTemp," ");
                            token = strtok(NULL, " ");
                        
                        }
                    
                      
                        if(!flag)
                        {
                            strncpy(contenu, contenuTemp, tailleContenu-1); // Suppression de l'espace à la fin  

                            int taille = taille_chemin(chemin);                   
                            creation_fichier(chemin,contenu,taille, structDisque);

                            free(contenu);
                            free(contenuTemp);
                        }
                    }
                    
                    

                } else if (strcmp(commande,RMFILE) == 0) {

                    suppression_fichier(chemin, structDisque);
                    

                } else if (strcmp(commande,READFILE) == 0) {

                    int taille = taille_chemin(chemin);
                    lire_fichier(chemin,taille, structDisque);

                }

            }     
            
        
        free(commande);
        free(chemin);

        }
    }

      free(structDisque);
      free(structDisque->listeRepertoires);
      free(structDisque->blocks);

      fclose(disque);
      fclose(instructions);

      return 0 ; 


}


                      //  ------------------------------------------------------------  //
                      //                                                                //
                      //  Méthodes de validation du chemin et recherche de répertoires  //
                      //                                                                //
                      //  ------------------------------------------------------------- //


int valider_chemin(char* chemin, disk* structDisque) 
{
    int statut = 1 ; 
    repertoire* parent = trouver_repertoire(obtenir_parent(chemin, structDisque),structDisque);

    if (parent == NULL) 
    {    
        if (valider_racine(chemin, structDisque) != 1) 
        {
            fprintf(stderr, "Le répertoire parent n'existe pas ! \n");
            statut = 0;
        }
    }

    free(parent);
    return statut ; 

}



repertoire* trouver_repertoire(char* nom, disk* structDisque)
{
    repertoire* cible = NULL;
    int taille = structDisque->nbRepertoires ;

    int i;
    for (i = 0; i < taille; i++) 
    {
        if (strcmp(structDisque->listeRepertoires[i].nom, nom) == 0) 
        {
            cible = malloc(sizeof(repertoire));
            *cible = structDisque->listeRepertoires[i];
            break;
        }
        
    }
    
    return cible;
}



char* obtenir_parent(char *chemin, disk* structDisque) 
{
    char str[100];
    char *token;
    char *enfant;
    char *parent = calloc(256, sizeof(char));

    strcpy(str, chemin);
    token = strtok(str, "/");

    while (token != NULL)
    {
        enfant = token;
        token = strtok(NULL, "/");
        if (token != NULL)
        {
            strcat(parent, "/");
            strcat(parent, enfant);
        }
    }

    return parent;
}



int valider_racine(char* chemin, disk* structDisque)
{
    return strlen(obtenir_parent(chemin,structDisque)) == 0;
}




int repertoire_existe(char *chemin, disk* structDisque) 
{     
    repertoire* dir = trouver_repertoire(chemin, structDisque);
    int statut = 0;

    if (dir != NULL) 
    {
        statut = 1;
    }

    free(dir);
    return statut;
}      



int meme_racine(const char* chemin1, const char* chemin2) 
{
    int i = 0;

    if (strlen(chemin1) > strlen(chemin2)) return 0;

    while (i < strlen(chemin1)) 
    {
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



void creation_repertoire(char* chemin, disk* structDisque)
{
    if (repertoire_existe(chemin, structDisque) == 1 ) 
    {
        fprintf(stderr, "Le répertoire que vous essayez de créer existe déjà ! \n");

    } else {

        if ( valider_chemin(chemin, structDisque) != 0 ) 
        {    
            repertoire* dir =  (repertoire*) malloc(sizeof(repertoire));
            strcpy(dir->nom, chemin);
            strcpy(dir->parent, obtenir_parent(chemin, structDisque));
            dir->nbr_fichiers = 0;
            
            structDisque->listeRepertoires[structDisque->nbRepertoires] = *dir;
            structDisque->nbRepertoires++ ;

            free(dir);
        }

    }

}



                            //  --------------------------------  //
                            //                                    //
                            //      Suppression de répertoire     //
                            //                                    //
                            //  --------------------------------  //



void suppression_complete_dir(char* chemin, disk* structDisque)
{

    if (repertoire_existe(chemin, structDisque) != 1 ) 
    {
        fprintf(stderr, "Le répertoire que vous essayez de supprimer n'existe pas !\n");

    } else {

        if ( valider_chemin(chemin, structDisque) != 0 ) 
        {
            int tailleListe = structDisque->nbRepertoires ;
            int totalRepertoires = 0 ;

            repertoire *liste = malloc(tailleListe * sizeof(repertoire));

            int i ; 
            for (i = 0 ; i < tailleListe ; i++ ) 
            {
                if (meme_racine(chemin, structDisque->listeRepertoires[i].nom)) 
                {
                    repertoire* cible = (repertoire*) malloc(sizeof(repertoire));
                    *cible = structDisque->listeRepertoires[i];
                    liste[totalRepertoires] = *cible ; 

                    totalRepertoires++ ; 
                    free(cible);

                }
            }

            while (totalRepertoires >= 0) 
            {
                    supprimer_repertoire_cible(liste[totalRepertoires - 1 ].nom,structDisque);
                    totalRepertoires-- ; 
            }

        free(liste);

        }

    }
    
}



void supprimer_repertoire_cible(char* cible, disk* structDisque) 
{
    int taille = structDisque->nbRepertoires; 

    int i ; 
    for (i = 0 ; i < taille ; i ++) 
    {
        if (strcmp(cible,structDisque->listeRepertoires[i].nom) == 0 )
        {
            strcpy(structDisque->listeRepertoires[i].nom,"");
            structDisque->listeRepertoires[i].nbr_fichiers = 0 ; 
        }
    }

    structDisque->nbRepertoires-- ; 

}



                            //  --------------------------------  //
                            //                                    //
                            //         Création de fichier        //
                            //                                    //
                            //  --------------------------------  //



void creation_fichier(char* chemin, char* contenu, int taille, disk* structDisque)
{
    // Calcul des blocks nécessaires : 
    int nb_blocks = blocks_necessaires(contenu) ;
    int blocks_remplis = 0 ; 

    // Gestion des erreurs de fichier :    
    if (trouver_repertoire(obtenir_parent(chemin, structDisque), structDisque) == NULL
               && taille != 1)
    {
        fprintf(stderr, "Erreur ! Le répertoire où vous essayez de créer le fichier n'existe pas !\n");

    } else if (fichier_existe(chemin,taille,structDisque) == 1 ) 
    {
        fprintf(stderr, "Le fichier que vous essayez de créer existe déjà !\n") ; 

    } else if ( nb_blocks > structDisque->nbr_blk_libres)
    {
        fprintf(stderr, "Le disque est plein !\n") ;

    } else {
        // Fichier créé dans le répertoire racine
        if ( taille == 1 ) 
        {
            int i ; 
            for (i = 0 ; i < structDisque->nbRepertoires ; i++ ) 
            {
                if(strcmp(structDisque->listeRepertoires[i].nom, "racine") == 0 )
                {
                    fichier file ;
                    strcpy(file.nom, chemin);
                    strcpy(file.contenu, contenu);
                    file.numero_inode = compteurInode ;
                    structDisque->listeRepertoires[i].listeFichiers[structDisque->listeRepertoires[i].nbr_fichiers] = file;
                    structDisque->listeRepertoires[i].nbr_fichiers++;
                }
                break; 
            }

            // CREATION DE L'INODE ET ECRITURE SUR LE DISQUE : 

            inode* inodeDuFichier = (inode*)malloc(sizeof(inode)) ; 
            inodeDuFichier->numero_inode = compteurInode ; 
            compteurInode++ ; 


            bool flag = false ; 
            int l = 0;
            while ( l < strlen(contenu) ) {        
                int k ;
                for (k = 0 ; k < 32000 && !flag ; k++)
                {
                    if ( structDisque->blocks[k]->estVide == 0 )
                    {
                        if( blocks_remplis <= 8 ) 
                        {
                            inodeDuFichier->blocksDirects[blocks_remplis] = k ;

                        } else {

                            inodeDuFichier->indirectionSimple[blocks_remplis - 9] = k ;

                        }


                        blocks_remplis++ ; 
                        structDisque->blocks[k]->estVide = 1 ; 

                        int i ; 
                        for(i = 0 ; i<16 ; i++)
                        {
                            structDisque->blocks[k]->data[i] = contenu[l];  
                            l++;

                            if( l > strlen(contenu) ) flag = true ;   
                        }
                       
                    }

                }

            }

            // Sauvegarde de l'Inode sur le disque  
            structDisque->nbr_blk_libres = structDisque->nbr_blk_libres - nb_blocks ;
            structDisque->inodes[compteurInode] = *inodeDuFichier;           
            free(inodeDuFichier);



        // Fichier créé dans un autre répertoire
        } else {

            bool existe = false ; 
            repertoire* repertoire_parent = trouver_repertoire(obtenir_parent(chemin, structDisque),structDisque);
            

            int i ;
            for (i = 0 ; i < repertoire_parent->nbr_fichiers; i++) 
            {
                if (strcmp(repertoire_parent->listeFichiers[i].nom, chemin) == 0) 
                {
                    existe = true ; 
                    break;
                }
            }

            if (existe)fprintf(stderr, "Le fichier que vous essayez de créer existe déjà !\n ");
            else if (valider_chemin(chemin, structDisque) != 1 )fprintf(stderr, "Le répertoire où vous essayez de créer le fichier n'existe pas !\n");
            else {

                if ( valider_chemin(chemin, structDisque) != 0 )
                {
                    int i ; 
                    for (i = 0 ; i < structDisque->nbRepertoires ; i++ ) 
                    {
                        if(strcmp(structDisque->listeRepertoires[i].nom, repertoire_parent->nom) == 0 )
                        {
                            fichier file ;
                            strcpy(file.nom, chemin);
                            strcpy(file.contenu, contenu);
                            file.numero_inode = compteurInode ;
                            structDisque->listeRepertoires[i].listeFichiers[structDisque->listeRepertoires[i].nbr_fichiers] = file;
                            structDisque->listeRepertoires[i].nbr_fichiers++;
                            break; 
                        }
                    }

                    // CREATION DE L'INODE ET ECRITURE SUR LE DISQUE : 

                    inode *inodeDuFichier = (inode*) malloc(sizeof(inode)) ; 
                    inodeDuFichier->numero_inode = compteurInode ; 
                    compteurInode++ ; 
                    

                    bool flag = false ; 
                    int l = 0;
                    while ( l < strlen(contenu) ) 
                    {        
                        int k ;
                        for (k = 0 ; k < 32000 && !flag ; k++)
                        {
                            if ( structDisque->blocks[k]->estVide == 0 )
                            {
                                if( blocks_remplis <= 8 )
                                {
                                    inodeDuFichier->blocksDirects[blocks_remplis] = k ; 
                                } else {
                                    inodeDuFichier->indirectionSimple[blocks_remplis - 9] = k ;
                                }

                                blocks_remplis++ ; 
                                structDisque->blocks[k]->estVide = 1 ;

                                int i ; 
                                for(i = 0 ; i<16 ; i++)
                                {
                                    structDisque->blocks[k]->data[i] = contenu[l];  
                                    l++;
                                
                                    if( l > strlen(contenu) ) flag = true ;   
                                     
                                }
                       
                            }

                        }

                    }
                    
                    // Sauvegarde de l'Inode sur le disque
                    structDisque->nbr_blk_libres = structDisque->nbr_blk_libres - nb_blocks ;
                    structDisque->inodes[compteurInode] = *inodeDuFichier; 
                    free(inodeDuFichier);    

                }

            }


        free(repertoire_parent) ;

        }

    }

}



void afficherBlocksNonLibres( disk* structDisque ) 
{
    int q ;
    for ( q = 0 ; q < 32000 ; q++)
    {  
        if(structDisque->blocks[q]->estVide == 1) 
        {
            printf("Bloc : %d, Data : %s\n",q, structDisque->blocks[q]->data ); 

        }

    }

}



                            //  --------------------------------  //
                            //                                    //
                            //       Suppression de fichier       //
                            //                                    //
                            //  --------------------------------  //



void suppression_fichier(char* chemin, disk* structDisque)
{
    if (valider_chemin(chemin, structDisque) != 1 )fprintf(stderr, "Le répertoire ou vous essayez de supprimer le fichier n'existe pas ! \n");
 
    else 
    {
        repertoire* repertoire_parent;
        int taille = taille_chemin(chemin);

        if (taille == 1)  
        {
            repertoire_parent = trouver_repertoire("racine", structDisque);
        } else {
            repertoire_parent = trouver_repertoire(obtenir_parent(chemin, structDisque), structDisque);
        }


        // LIBÉRATION DE L'INODE ET DES BLOCKS

        if (fichier_existe(chemin,taille, structDisque) != 1)
        {
            fprintf(stderr, "Le fichier que vous essayez de supprimer n'existe pas ! \n");
        } else {

            fichier fichier_a_supprimer = trouver_fichier(repertoire_parent, chemin);

            int inode_du_fichier = fichier_a_supprimer.numero_inode ; 
            int nombreTotalBlocks = blocks_necessaires(fichier_a_supprimer.contenu);
            bool setInodeNull = false ; 

            int i ; 
            for ( i = 0 ; i < 32000 && !setInodeNull ; i++) 
            {
                if (structDisque->inodes[i].numero_inode == inode_du_fichier)
                {
                    int j = 0 ;
                    while ( j < nombreTotalBlocks ) 
                    {
                        int cible = 0 ; 
                        if( j <= 8 ) 
                        {

                            cible = structDisque->inodes[i].blocksDirects[j]; 
                            strcpy(structDisque->blocks[cible]->data, ""); 
                            structDisque->inodes[i].blocksDirects[j] = -1 ; 
                            j++ ;
                                    
                        } else {

                            cible = structDisque->inodes[i].indirectionSimple[j - 9]; 
                            strcpy(structDisque->blocks[cible]->data, ""); 
                            cible = structDisque->inodes[i].indirectionSimple[j - 9] = -1 ; 
                            j++ ;        
                        }

                        structDisque->blocks[j]->estVide = 0 ; 
                            
                    }
                        
                    setInodeNull = true ; 
                    break ; 
                        
                }       

            }


            if ( taille == 1 ) 
            {
                int i ; 
                for (i = 0 ; i < structDisque->nbRepertoires ; i++ ) 
                {
                    if(strcmp(structDisque->listeRepertoires[i].nom, "racine") == 0 ) 
                    {
                        structDisque->listeRepertoires[0].nbr_fichiers--;

                        for (i = 0; i < structDisque->listeRepertoires[i].nbr_fichiers - 1; i++) 
                        {
                            structDisque->listeRepertoires[i].listeFichiers[i] = structDisque->listeRepertoires[i].listeFichiers[i + 1];
                        }
                        break ; 
                    }

                }

            } else {

                int i ; 
                for (i = 0 ; i < structDisque->nbRepertoires ; i++ ) 
                {
                    if(strcmp(structDisque->listeRepertoires[i].nom, repertoire_parent->nom) == 0 ) 
                    {
                        structDisque->listeRepertoires[i].nbr_fichiers--;

                        for (i = 0; i < structDisque->listeRepertoires[i].nbr_fichiers - 1; i++) 
                        {    
                            structDisque->listeRepertoires[i].listeFichiers[i] = structDisque->listeRepertoires[i].listeFichiers[i + 1];
                        }
                        break ; 
                    }

                }

            }

            free(repertoire_parent); 
        }

    }

}



                            //  --------------------------------  //
                            //                                    //
                            //         Lecture de fichier         //
                            //                                    //
                            //  --------------------------------  //



void lire_fichier(char* chemin, int taille, disk* structDisque)
{
        if (fichier_existe(chemin,taille, structDisque) != 1)
        {
            fprintf(stderr, "Le fichier que vous essayez de lire n'existe pas ! \n");
        } else {

                repertoire* repertoire_parent;
                if (taille == 1) 
                {
                    repertoire_parent = trouver_repertoire("racine", structDisque);
                } else {

                    repertoire_parent = trouver_repertoire(obtenir_parent(chemin, structDisque), structDisque);
                }

                fichier fichier_lecture = trouver_fichier(repertoire_parent, chemin);

                int inode_du_fichier = fichier_lecture.numero_inode ; 
                int nombreTotalBlocks = blocks_necessaires(fichier_lecture.contenu);
                bool affiche = false ; 

                int i ; 
                for ( i = 0 ; i < 32000 && !affiche ; i++) 
                {
                    if (structDisque->inodes[i].numero_inode == inode_du_fichier)
                    {
                        int j = 0 ;
                        while ( j < nombreTotalBlocks )
                        {
                            int cible = 0 ; 
                            if( j <= 8 )
                            {
                                cible = structDisque->inodes[i].blocksDirects[j]; 
                                printf("%s",structDisque->blocks[cible]->data) ; 
                                    
                            } else {

                                cible = structDisque->inodes[i].indirectionSimple[j - 9] ; 
                                printf("%s",structDisque->blocks[cible]->data) ; 
                                    
                            }
                            j++ ; 
                            
                        }
                        
                        affiche = true ; 
                        break ; 
                        
                    }

                }

                printf("\n");
                free(repertoire_parent);
        }

}



int taille_chemin(char* chemin)
{
    int taille = 0 ; 
    char* token ; 
    char chemin1[40];
    strcpy(chemin1,chemin);
    token = strtok(chemin1,"/");

    while(token != NULL )
    {
        taille++;
        token = strtok(NULL, "/");

    }

    return taille ; 

}



int fichier_existe(char* chemin, int taille, disk* structDisque)
{
    int i ;
    int fichier_absent = 1;
    repertoire* repertoire_parent;

    if (taille == 1 ) 
    {
        repertoire_parent = trouver_repertoire("racine",structDisque);

    } else {

        repertoire_parent = trouver_repertoire(obtenir_parent(chemin, structDisque), structDisque);
    }


    for (i=0; i < repertoire_parent->nbr_fichiers; i++)
    {
        if (strcmp(repertoire_parent->listeFichiers[i].nom, chemin) == 0)
        {            
            fichier_absent = 0;
            break;
        }
    }
    
    free(repertoire_parent);

    if (fichier_absent == 1)
    {        
        return 0;
    }

    return 1;
}





fichier trouver_fichier(repertoire* rep_parent, char* chemin)
{
    fichier fichier_trouve ;
    int i = 0;
    for (; i < rep_parent->nbr_fichiers; ++i) 
    {
        if (strcmp(rep_parent->listeFichiers[i].nom, chemin) == 0)
        {
            fichier_trouve = rep_parent->listeFichiers[i];   
        }

    }

    return fichier_trouve; 
}



int blocks_necessaires(char* contenu)
{
    int blocks_necessaires = 0 ; 
    int taille = strlen(contenu) ; 

    if ( taille <= 16 )
    {
        blocks_necessaires = 1;
    
    } else {

        while(taille > 0 )
        {
            taille=taille-16;
            blocks_necessaires++;
        }

    }

    return blocks_necessaires ; 
}
