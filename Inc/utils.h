/**
 * @file utils.h
 * @brief Fonctions utilitaires pour la gestion des entrées utilisateur.    
 * @author Oussama Amara
 * @date 2024-06-28
 * @version 1.0
 */
#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Obtient le chemin complet du fichier de base de données.
 * Cette fonction construit le chemin complet du fichier de base de données
 *  en utilisant le répertoire de l'exécutable courant et le nom du fichier de base
 * de données fourni.   
 * @param db_name Le nom du fichier de base de données.
 * @return Le chemin complet du fichier de base de données.
 */
char* get_db_path(const char* db_name) ;

/**
 * @brief Lit une chaîne de caractères depuis l'entrée standard.
 *
 * Cette fonction lit une ligne de texte, supprime le caractère '\n' s'il est présent,
 * et vide le tampon si l'entrée dépasse la taille du buffer.
 *
 * @param buffer Le tableau où stocker la chaîne lue.
 * @param taille La taille maximale du buffer (incluant le '\0').
 * @return 0 si la lecture est réussie, -1 en cas d'erreur.
 */
int lire_chaine(char *buffer, size_t taille) ;
/**
 * @brief Lit un entier positif depuis l'entrée standard avec validation.
 * @return L'entier lu, ou -1 si l'entrée est invalide.
 */
int lire_entier();

/**
 * @brief Lit un flottant positif depuis l'entrée standard avec validation.
 * @return Le flottant lu, ou -1.0f si l'entrée est invalide.
 */
float lire_flottant();

#endif // UTILS_H
