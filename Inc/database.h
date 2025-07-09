/**
 * @file database.h
 * @brief Interface pour la gestion de la base de données SQLite des produits.
 * @author Oussama Amara
 * @date 2024-06-28
 * @version 1.0
 */

#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

/**
 * @struct Produit
 * @brief Structure représentant un produit en stock.
 */
typedef struct {
    int id;             /**< Identifiant unique du produit */
    char nom[100];      /**< Nom du produit */
    int quantite;       /**< Quantité en stock */
    float prix;         /**< Prix unitaire */
} Produit;

/**
 * @brief Initialise la base de données SQLite.
 * 
 * @param db Pointeur vers le pointeur de base de données SQLite.
 * @param filename Nom du fichier de base de données.
 * @return 0 en cas de succès, code d'erreur SQLite sinon.
 */
int db_init(sqlite3 **db, const char *filename);

/**
 * @brief Ferme la connexion à la base de données.
 * 
 * @param db Pointeur vers la base de données SQLite.
 */
void db_close(sqlite3 *db);

/**
 * @brief Ajoute un produit à la base de données.
 * 
 * @param db Pointeur vers la base de données SQLite.
 * @param p Pointeur vers la structure Produit à ajouter.
 * @return 0 en cas de succès, -1 sinon.
 */
int db_ajouter_produit(sqlite3 *db, const Produit *p);

/**
 * @brief Affiche tous les produits de la base de données.
 * 
 * @param db Pointeur vers la base de données SQLite.
 * @return 0 en cas de succès, -1 sinon.
 */
int db_lister_produits(sqlite3 *db);

/**
 * @brief Supprime un produit de la base de données.
 * 
 * @param db Pointeur vers la base de données SQLite.
 * @param id Identifiant du produit à supprimer.
 * @return 0 en cas de succès, -1 sinon.
 */
int db_supprimer_produit(sqlite3 *db, int id);

/**
 * @brief Modifie un produit existant dans la base de données.
 * 
 * @param db Pointeur vers la base de données SQLite.
 * @param p Pointeur vers la structure Produit contenant les nouvelles données.
 * @return 0 en cas de succès, -1 sinon.
 */
int db_modifier_produit(sqlite3 *db, const Produit *p);

/**
 * @brief Vérifie si un produit avec l'ID donné existe dans la base de données.
 *
 * @param db Pointeur vers la base de données SQLite.
 * @param id Identifiant du produit à vérifier.
 * @return 1 si le produit existe, 0 sinon.
 */
int db_produit_existe_par_id(sqlite3 *db, int id) ;

/**
 * @brief Vérifie si un produit avec le nom donné existe dans la base de données.
 *
 * Cette fonction interroge la base de données pour vérifier si un produit
 * avec le nom spécifié existe déjà. Elle retourne 1 si le produit existe,
 * 0 sinon.
 *
 * @param db Pointeur vers la base de données SQLite.
 * @param nom Nom du produit à vérifier.
 * @return 1 si le produit existe, 0 sinon.
 */
int db_produit_existe(sqlite3 *db, const char *nom) ;

#endif
