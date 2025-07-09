/**
 * @file produit.h
 * @brief Interface utilisateur pour la gestion interactive des produits.
 * 
 * @author Oussama
 * @date 2024-06-28
 * @version 2.0
 */


#ifndef PRODUIT_H
#define PRODUIT_H

#include "sqlite3.h"
#include "database.h"

/**
 * @brief Demande à l'utilisateur les informations d'un produit et l'ajoute à la base.
 * 
 * @param db Pointeur vers la base de données SQLite.
 */
void ajouter_produit_interactif(sqlite3 *db);

/**
 * @brief Demande à l'utilisateur un ID et les nouvelles infos, puis modifie le produit.
 * 
 * @param db Pointeur vers la base de données SQLite.
 */
void modifier_produit_interactif(sqlite3 *db);

/**
 * @brief Demande à l'utilisateur un ID et supprime le produit correspondant.
 * 
 * @param db Pointeur vers la base de données SQLite.
 */
void supprimer_produit_interactif(sqlite3 *db);

/**
 * @brief Affiche tous les produits de la base de données.
 * 
 * @param db Pointeur vers la base de données SQLite.
 */
void lister_produits_interactif(sqlite3 *db);

#endif

