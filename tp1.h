#include "link.h"


/*
 * Structure représentant un bloc sur le disque.
*/ 

typedef struct {

    char chaine[16];

} bloc ;


/*
 * Crée le répertoire s'il n'existe pas.
 * 
 * Gère les erreurs suivantes :
 * - Répertoire existe déjà.
 * - Un des répertoires parents n'existe pas.
 * - Le chemin absolu du répertoire est trop long.
 *
 * @param *chemin : Chemin du répertoire à créer.
*/ 
void creation_repertoire(char* chemin) ;

/*
 * Supprime le répertoire et tout son contenu.
 * 
 * Gère les erreurs suivantes :
 * - Répertoire n'existe pas.
 *
 * @param *chemin : Chemin du répertoire à supprimer.
*/ 
void suppression_repertoire(char* chemin) ;

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

/*
 * Récupère le nom du fichier à créer. à lire ou à supprimer.
 *
 * @param *chemin : Chemin du fichier.
*/ 
char* nom_de_fichier(char*chemin);
