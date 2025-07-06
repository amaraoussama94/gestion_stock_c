/** 
 * @file produit.c
 *
 * @brief fonctions de gestion des produits pour un système de gestion de stock.
 * Les opérations incluent l'ajout, l'affichage, la recherche, la modification et la suppression de produits.
 *
 * @author Oussama
 * @date 2024-06-28
 * @version 2.0
 */
#include <stdio.h>
#include <stdlib.h>
#include "produit.h"
#include "utils.h"


/**
 * @brief Adds a new product to the database interactively.
 *
 * Prompts the user for product details and inserts the new product into the database.
 *
 * @param db Pointer to the SQLite database connection.
 */
void ajouter_produit_interactif(sqlite3 *db) {
    Produit p;
    printf("Nom : ");
    if (lire_chaine(p.nom, sizeof(p.nom)) != 0) {
        printf("Erreur de lecture du nom.\n");
        return;
    }
    // Vérification d'existence
    if (db_produit_existe(db, p.nom)) {
        printf("Le produit  existe déjà. voulez-vous le modifier via menu modifier ");
        return;
    }

    // Si le produit n'existe pas, on continue l'ajout
    printf("Quantité : ");
    p.quantite = lire_entier();
    if (p.quantite < 0) {
        printf("Quantité invalide.\n");
        return;
    }
    printf("Prix : ");
    p.prix = lire_flottant();
    if (p.prix < 0) {
        printf("Prix invalide.\n");
        return;
    }

    if (db_ajouter_produit(db, &p) == 0)
        printf("Produit ajouté avec succès.\n");
    else
        printf("Erreur lors de l'ajout.\n");
}


/**
 * @brief Interactively modifies the details of a product in the database.
 *
 * Prompts the user to enter the ID of the product to modify, then asks for the new
 * name, quantity, and price. Updates the product in the database using the provided
 * information. Displays a success or error message based on the outcome.
 *
 * @param db Pointer to the opened SQLite database connection.
 */
void modifier_produit_interactif(sqlite3 *db) {
    Produit p;
    printf("ID du produit à modifier : ");
    p.id = lire_entier();
    if (p.id < 0 || !db_produit_existe_par_id(db, p.id)) {
        printf("ID invalide ou produit inexistant.\n");
        return;
    }
    printf("Nouveau nom : ");
    if (lire_chaine(p.nom, sizeof(p.nom)) != 0) {
        printf("Erreur de lecture du nom.\n");
        return;
    }

    printf("Nouvelle quantité : ");
    p.quantite = lire_entier();
    if (p.quantite < 0) {
        printf("Quantité invalide.\n");
        return;
    }

    printf("Nouveau prix : ");
    p.prix = lire_flottant();
    if (p.prix < 0) {
        printf("Prix invalide.\n");
        return;
    }

    if (db_modifier_produit(db, &p) == 0)
        printf("Produit modifié avec succès.\n");
    else
        printf("Erreur lors de la modification.\n");
}

/**
 * @brief Supprime un produit de la base de données de manière interactive.
 *
 * Cette fonction demande à l'utilisateur de saisir l'identifiant (ID) du produit à supprimer,
 * puis appelle la fonction db_supprimer_produit pour effectuer la suppression dans la base de données SQLite.
 * Elle affiche un message indiquant si la suppression a réussi ou échoué.
 *
 * @param db Pointeur vers la base de données SQLite ouverte.
 */
void supprimer_produit_interactif(sqlite3 *db) {
    
    Produit p;
    printf("ID du produit à supprimer : ");
    p.id = lire_entier();
    if (p.id < 0 || !db_produit_existe_par_id(db, p.id)) {
        printf("ID invalide ou produit inexistant.\n");
        return;
    }
    if (db_supprimer_produit(db, p.id) == 0)
        printf("Produit supprimé.\n");
    else
        printf("Erreur lors de la suppression.\n");
}

/**
 * @brief Affiche la liste des produits de manière interactive.
 *
 * Cette fonction appelle db_lister_produits pour afficher tous les produits
 * présents dans la base de données SQLite passée en paramètre. Si une erreur
 * survient lors de l'affichage, un message d'erreur est affiché à l'utilisateur.
 *
 * @param db Pointeur vers la base de données SQLite.
 */
void lister_produits_interactif(sqlite3 *db) {
    if (db_lister_produits(db) != 0)
        printf("Erreur lors de l'affichage des produits.\n");
}

