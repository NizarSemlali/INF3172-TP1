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
 * Structure représentant le fichier 
*/ 
 typedef struct {
 	char nom[40];
 	char contenu[256];
 } fichier;


/*
 * Structure représentant un répertoire
*/ 
 typedef struct {
 	fichier listeFichiers[256];
 	char nom[40];
 	char parent[40];
 	int nbr_fichiers;
 } repertoire;


/*
 * Fichier servant de disque pour l'écriture et la lecture des données
*/ 
 FILE * disque;


/*
 * Vérifie si le chemin est valide ( Si le/les répertoire(s) parent(s) existe(nt)).
 *
 * @param chemin : Chemin à vérifier.
*/ 
 int valider_chemin(char* chemin);


 /*
 * Valide si le répertoire cible est le répertoire à la racine. 
 *
 * @param chemin : Chemin à vérifier.
*/ 
 int valider_racine(char* chemin);


 /*
 * Retourne le parent du répertoire cible . 
 *
 * @param chemin : Chemin dont on veut le parent.
*/ 
 char* obtenir_parent(char* chemin);


/*
 * Cherche et retourne un repertoire cible. 
 *
 * @param nom : Le répertoire voulu.
*/ 
 repertoire* trouver_repertoire(char* nom) ;


/*
 * Retourne la taille du disque.
 */ 
 int taille_disque();

/*
 * Vérifie si le répertoire existe.
 * 
 * @param chemin : Chemin du répertoire recherché.
 */ 
 int repertoire_existe(char* chemin);


/*
 * Remplit la liste de répertoires à travers le disque.
 * 
 * @param liste : Liste complète de répertoires.
 */ 
void remplir_liste_repertoires(repertoire* liste);


/*
 * Crée le répertoire s'il n'existe pas.
 * 
 * Gère les erreurs suivantes :
 * - Répertoire existe déjà.
 * - Un des répertoires parents n'existe pas.
 * - Le chemin absolu du répertoire est trop long.
 *
 * @param chemin : Chemin du répertoire à créer.
*/ 
 void creation_repertoire(char* chemin) ;


/*
 * Supprime le répertoire et tout son contenu ( RÉCURSIVEMENT ) .
 * 
 * Gère les erreurs suivantes :
 * - Répertoire n'existe pas.
 *
 * @param chemin : Chemin du répertoire à supprimer.
*/ 
void suppression_complete_dir(char* chemin);

/*
 * Supprime le répertoire cible . 
 *
 * @param cible : Cible à supprimer.
*/ 
void supprimer_repertoire_cible(char* cible);


/*
 * Vérifie si le chemin et le répertoire ont la même racine.
 *
 * @param chemin1 : Chemin cible.
 * @param chemin2 : Chemin du répertoire .
*/ 
 int meme_racine(const char* chemin1, const char* chemin2);







										// WIP //




/*
 * Crée un fichier avec un contenu.
 *
 * Gère les erreurs suivantes :
 * - Un fichier ne peut pas être vide;
 * - Le fichier existe déjà;
 * - Le répertoire n'existe pas;
 * - Le fichier est trop volumineux;
 * - Le chemin absolu du fichier est trop long;
 * - Le disque est plein.
 *
 * @param *chemin : Chemin du fichier à créer.
 * @param *contenu : Contenu du fichier.
*/ 
 void creation_fichier(char* chemin, char* contenu) ;

/*
 * Supprime le fichier.
 * Gère les erreurs suivantes :
 * - Le fichier n'existe pas.
 *
 * @param *chemin : Chemin du fichier à supprimer.
*/ 
 void suppression_fichier(char* chemin) ;


/*
 * Affiche le contenu du fichier à l'écran.
 * Gère les erreurs suivantes :
 * - Le fichier n'existe pas.
 *
 * @param *chemin : Chemin du fichier à afficher.
*/ 
 void lire_fichier(char* chemin) ;

