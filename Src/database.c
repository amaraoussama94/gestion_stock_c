/**
 * @file database.c
 * @brief Implémentation des fonctions de gestion de la base de données SQLite.
 * @author Oussama Amara
 * @date 2024-06-28
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

/**
 * @brief Initialise la base de données SQLite.
 * 
 * @param db Pointeur vers le pointeur de base de données SQLite.
 * @param filename Nom du fichier de base de données.
 * @return 0 en cas de succès, code d'erreur SQLite sinon.
 */

int db_init(sqlite3 **db, const char *filename) {
    int rc = sqlite3_open(filename, db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur ouverture DB: %s\n", sqlite3_errmsg(*db));
        return rc;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS produits ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "nom TEXT NOT NULL,"
                      "quantite INTEGER,"
                      "prix REAL);";

    char *err_msg = NULL;
    rc = sqlite3_exec(*db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur création table: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(*db);  // Close DB on failure
        *db = NULL;
    }

    return rc;
}


/**
 * @brief Ferme la connexion à la base de données.
 * 
 * @param db Pointeur vers la base de données SQLite.
 */
void db_close(sqlite3 *db) {
    sqlite3_close(db);
}

/**
 * @brief Ajoute un produit à la base de données.
 * 
 * @param db Pointeur vers la base de données SQLite.
 * @param p Pointeur vers la structure Produit à ajouter.
 * @return 0 en cas de succès, -1 sinon.
 */
int db_ajouter_produit(sqlite3 *db, const Produit *p) {
    const char *sql = "INSERT INTO produits (nom, quantite, prix) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return -1;
    sqlite3_bind_text(stmt, 1, p->nom, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, p->quantite);
    sqlite3_bind_double(stmt, 3, p->prix);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE ? 0 : -1;
}

/**
 * @brief Affiche tous les produits de la base de données.
 * 
 * @param db Pointeur vers la base de données SQLite.
 * @return 0 en cas de succès, -1 sinon.
 */
int db_lister_produits(sqlite3 *db) {
    const char *sql = "SELECT id, nom, quantite, prix FROM produits;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return -1;

    printf("Liste des produits :\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ID: %d | Nom: %s | Quantité: %d | Prix: %.2f\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_text(stmt, 1),
               sqlite3_column_int(stmt, 2),
               sqlite3_column_double(stmt, 3));
    }

    sqlite3_finalize(stmt);
    return 0;
}

/**
 * @brief Supprime un produit de la base de données.
 * 
 * @param db Pointeur vers la base de données SQLite.
 * @param id Identifiant du produit à supprimer.
 * @return 0 en cas de succès, -1 sinon.
 */
int db_supprimer_produit(sqlite3 *db, int id) {
    const char *sql = "DELETE FROM produits WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return -1;
    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE ? 0 : -1;
}

/**
 * @brief Modifie un produit existant dans la base de données.
 * 
 * @param db Pointeur vers la base de données SQLite.
 * @param p Pointeur vers la structure Produit contenant les nouvelles données.
 * @return 0 en cas de succès, -1 sinon.
 */
int db_modifier_produit(sqlite3 *db, const Produit *p) {
    const char *sql = "UPDATE produits SET nom = ?, quantite = ?, prix = ? WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return -1;
    sqlite3_bind_text(stmt, 1, p->nom, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, p->quantite);
    sqlite3_bind_double(stmt, 3, p->prix);
    sqlite3_bind_int(stmt, 4, p->id);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE ? 0 : -1;
}

/**
 * @brief Vérifie si un produit avec l'ID donné existe dans la base de données.
 *
 * @param db Pointeur vers la base de données SQLite.
 * @param id Identifiant du produit à vérifier.
 * @return 1 si le produit existe, 0 sinon.
 */
int db_produit_existe_par_id(sqlite3 *db, int id) {
    const char *sql = "SELECT COUNT(*) FROM produits WHERE id = ?;";
    sqlite3_stmt *stmt;
    int existe = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return 0;

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        existe = sqlite3_column_int(stmt, 0) > 0;
    }

    sqlite3_finalize(stmt);
    return existe;
}

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
int db_produit_existe(sqlite3 *db, const char *nom) {
    const char *sql = "SELECT COUNT(*) FROM produits WHERE nom = ?";
    sqlite3_stmt *stmt;
    int existe = 0;
// Préparation de la requête SQL
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return 0;
// Liaison du paramètre
/*SQLITE_TRANSIENT not  SQLITE_STATIC :This ensures SQLite makes its own copy of the string, 
avoiding undefined behavior if the original buffer is modified or freed.*/
    sqlite3_bind_text(stmt, 1, nom, -1, SQLITE_TRANSIENT);
// Exécution de la requête
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        existe = sqlite3_column_int(stmt, 0) > 0;
    }
// Libération des ressources
    sqlite3_finalize(stmt);
    return existe;
}

