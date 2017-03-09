# Travail pratique 1

## Description

Ce programme est une simulation d'un système de fichiers d'un système d'exploitation . 
  
Ce travail pratique est réalisé dans le cadre du cours Principes des systèmes d'exploitation ( INF3172 ), Hiver 2017 À L'université du Québec À Montréal .  

## Auteurs

Nizar Semlali 
  
Othmane Bouaddi

## Fonctionnement

### Fonctionnement général 

* Le logiciel se crée un fichier de 500 Ko qu'il utilisera pour simuler le disque de l'ordinateur.
  
* Il y stocke des chaînes de caractères qui joueront le rôle des fichiers gérés par le système.
  
* Le système nomme les chaînes de caractères (l'équivalent d'un nom de fichier), et le disque a des blocs de 16 octets (donc 32 000 blocs). 
  
* Les blocs libres et la fragmentation sont gérées sur le disque. 
  
* La structure de données mise en place dans le système de fichiers est l'i-node. 
  
* Chaque i-node peut contenir le numéro de 8 blocs directement, et il permet également l'indirection simple . 
  
  

### Fonctionnement détaillé 
  
Au démarrage du programme, si les fichiers nécessaires à l'exécution du programme ne sont pas
présents, il devra les créer. Le programme sera lancé à la console avec un fichier en paramètre.
Ce fichier contiendra les opérations que le système de fichiers devra effectuer. 
La fin du fichier d'entrée signifiera la fin de l'exécution du programme .
  
Les commandes supportées sont :  
  
  * creation_repertoire <nom du répertoire>
  * suppression_repertoire <nom du répertoire>
  * creation_fichier <nom du fichier> <contenu du fichier>
  * suppression_fichier <nom du fichier>
  * lire_fichier <nom du fichier>
  

  
## Contenu du projet

* Fichier tp1.c contenant le code source du projet, avec la fonction main
* Fichier tp1.h contenant l'interface des méthodes
* Fichier link.h contenant les constantes et imports de bibliothèques
* Fichier README.md qui décrit le contenu du projet
* Fichier Makefile supportant les appels make et make clean 


## Références

Pas de références.

## Statut

Méthodes creation_repertoire et suppression_repertoire implémentées .

WIP pour le reste . 
