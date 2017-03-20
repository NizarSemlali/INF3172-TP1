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

#include "link.h"

/*
 * Fichier servant de disque pour l'écriture et la lecture des données
*/ 
  
  FILE * disque ;



/*
 * Structure représentant l'inode
*/ 
typedef struct inode_struct inode;
struct inode_struct
{
  int numero_inode; 
  int blocksDirects[8];
  int indirectionSimple[8] ; 

} inode_struct;


/*
 * Structure représentant le block sur le disque 
*/ 
typedef struct block_struct block_t;
struct block_struct
{
  int num_block;
  char data[16];
  int estVide;
} block_struct;


/*
 * Structure représentant le fichier 
*/ 
 typedef struct {
 	char nom[40];
 	char contenu[256];
 	int numero_inode ; 
 } fichier;


/*
 * Structure représentant le répertoire
*/ 
 typedef struct {
 	fichier listeFichiers[32000];
 	char nom[40];
 	char parent[40];
 	int nbr_fichiers;
 } repertoire;


/*
 * Structure représentant le disque 
*/ 
typedef struct disk_struct disk;
struct disk_struct
{
  int nbr_blk ;
  int nbr_blk_libres;
  block_t** blocks;
  inode* inodes; 
  repertoire* listeRepertoires;
  int nbRepertoires ; 

} disk_struct;


/*
 * Vérifie si le chemin est valide ( Si le/les répertoire(s) parent(s) existe(nt)).
 *
 * @param chemin : Chemin à vérifier.
 * @param structDisque : Structure du disque
*/ 
 int valider_chemin(char* chemin, disk* structDisque);


/*
 * Valide si le répertoire cible est le répertoire à la racine. 
 *
 * @param chemin : Chemin à vérifier.
 * @param structDisque : Structure du disque.
 *
*/ 
 int valider_racine(char* chemin, disk* structDisque);


/*
 * Retourne le parent du répertoire cible . 
 *
 * @param chemin : Chemin dont on veut le parent.
 * @param structDisque : Structure du disque.
 *
*/ 
 char* obtenir_parent(char* chemin, disk* structDisque);


/*
 * Cherche et retourne un repertoire cible. 
 *
 * @param nom : Le répertoire voulu.
 * @param structDisque : Structure du disque.
 *
*/ 
 repertoire* trouver_repertoire(char* nom, disk* structDisque) ;


/*
 * Vérifie si le répertoire existe.
 * 
 * @param chemin : Chemin du répertoire recherché.
 * @param structDisque : Structure du disque.
 *
*/ 
 int repertoire_existe(char* chemin, disk* structDisque);


/*
 * Crée le répertoire s'il n'existe pas.
 * 
 * Gère les erreurs suivantes :
 *
 *    - Répertoire existe déjà.
 *    - Un des répertoires parents n'existe pas.
 *    - Le chemin absolu du répertoire est trop long.
 *
 * @param chemin : Chemin du répertoire à créer.
 * @param structDisque : Structure du disque.
 *
*/ 
 void creation_repertoire(char* chemin, disk* structDisque) ;


/*
 * Supprime le répertoire et tout son contenu ( RÉCURSIVEMENT ) .
 * 
 * Gère les erreurs suivantes :
 * 
 *    - Répertoire n'existe pas.
 *
 * @param chemin : Chemin du répertoire à supprimer.
 * @param structDisque : Structure du disque.
 *
*/ 
void suppression_complete_dir(char* chemin, disk* structDisque);

/*
 * Supprime le répertoire cible . 
 *
 * @param cible : Cible à supprimer.
 * @param structDisque : Structure du disque.
 *
*/ 
void supprimer_repertoire_cible(char* cible, disk* structDisque);


/*
 * Vérifie si le chemin et le répertoire ont la même racine.
 *
 * @param chemin1 : Chemin cible.
 * @param chemin2 : Chemin du répertoire .
 *
*/ 
 int meme_racine(const char* chemin1, const char* chemin2);



/*
 * Crée un fichier avec un contenu.
 *
 * Gère les erreurs suivantes :
 *
 *     - Un fichier ne peut pas être vide.
 *     - Le fichier existe déjà.
 *     - Le répertoire n'existe pas.
 *     - Le fichier est trop volumineux.
 *     - Le chemin absolu du fichier est trop long.
 *     - Le disque est plein.
 *
 * @param chemin : Chemin du fichier à créer.
 * @param contenu : Contenu du fichier.
 * @param taille : Taille du chemin.
 * @param structDisque : Structure du disque.
 *
*/ 
 void creation_fichier(char* chemin, char* contenu, int taille, disk* structDisque) ;


/*
 * Supprime le fichier.
 *
 * Gère les erreurs suivantes :
 * 
 *      - Le fichier n'existe pas.
 *
 * @param chemin : Chemin du fichier à supprimer.
 * @param structDisque : Structure du disque.
 *
*/ 
 void suppression_fichier(char* chemin, disk* structDisque) ;


/*
 * Affiche le contenu du fichier à l'écran.
 *
 * Gère les erreurs suivantes :
 *
 *       - Le fichier n'existe pas.
 *
 * @param chemin : Chemin du fichier à lire.
 * @param taille : Taille du chemin.
 * @param structDisque : Structure du disque.
 *
*/ 
 void lire_fichier(char* chemin, int taille, disk* structDisque) ;


/* 
 * Retourne la taille du chemin . 
 *
 * @param chemin : Chemin pour lequel on veut déterminer la taille.
 *
*/
int taille_chemin(char* chemin);


/*
 * Détermine si un fichier existe .
 *
 * @param chemin : Chemin du fichier recherché.
 * @param taille : Taille du chemin.
 * @param structDisque : Structure du disque.
 *
*/ 
int fichier_existe(char* chemin, int taille, disk* structDisque);


/*
 * Cherche et retourne un fichier.
 *
 * @param rep_parent : Répertoire parent du fichier recherché.
 * @param nom_fichier : Taille du chemin.
 *
*/ 
fichier trouver_fichier(repertoire *rep_parent, char *nom_fichier);


/*
 * Calcule le nombre de blocks nécessaires pour stocker le contenu d'un fichier.
 *
 * @param contenu : Contenu du fichier.
 *
*/ 
int blocks_necessaires(char* contenu) ; 