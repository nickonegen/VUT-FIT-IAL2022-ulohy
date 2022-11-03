/**
 * @file iter/btree.c
 * @author Onegenimasu <https://github.com/Onegenimasu>
 * @brief Binárny vyhľadávací strom — iteratívna varianta
 * @date 2022-xx-xx
 *
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
	*tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
	// Strom je prázdny
	if (tree == NULL) {
		return false;
	}

	char currentKey = tree->key;

	// Uzol nájdený
	if (currentKey == key) {
		*value = tree->value;
		return true;
	}

	// TODO
	return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
	// Alokovanie nového uzla
	bst_node_t *newNode = (bst_node_t *)malloc(sizeof(bst_node_t));
	if (newNode == NULL) {
		return; // Chyba alokácie pamäte
	}

	// Nastavenie hodnôt uzla
	newNode->key = key;
	newNode->value = value;
	newNode->left = newNode->right = NULL;

	// Strom je prázdny - vložiť uzol ako koreň stromu
	if (*tree == NULL) {
		*tree = newNode;
		return;
	}

	bst_node_t *currentNode = *tree;
	bool done = false;

	// Iteratívne vloženie do podstromu
	while (!done) {
		// Pomocné premenné
		char currentKey = currentNode->key;
		bool hasLeftSubtree = currentNode->left != NULL;
		bool hasRightSubtree = currentNode->right != NULL;

		// Zhodný kľúč
		if (key == currentKey) {
			// Nahradenie hodnoty
			currentNode->value = value;
			free(newNode); // Nie je potrebný nový uzol
			done = true;
		}

		// Vloženie do ľavého podstromu
		if (key < currentKey) {
			if (hasLeftSubtree) {
				// Pokračovanie v prechádzaní ľavého podstromu
				currentNode = currentNode->left;
			} else {
				// Nájdený najľavejší uzol - vloženie
				currentNode->left = newNode;
				done = true;
			}
		}

		// Vloženie do pravého podstromu
		if (key > currentKey) {
			if (hasRightSubtree) {
				// Pokračovanie v prechádzaní pravého podstromu
				currentNode = currentNode->right;
			} else {
				// Nájdený najpravejší uzol - vloženie
				currentNode->right = newNode;
				done = true;
			}
		}
	}
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
	// Kontrola, či je strom neprázdny (pre istotu)
	if (*tree == NULL) {
		return;
	}

	// Nájdený najpravejší potomok
	if ((*tree)->right == NULL) {
		// Nahradenie kľúča a hodnoty `target` podľa nájdeného uzla
		target->key = (*tree)->key;
		target->value = (*tree)->value;

		// Nastavenie ukazovateľa na podstrom na ľavo od nájdeného uzla
		bst_node_t *temp = *tree;
		*tree = (*tree)->left;

		// Uvoľnenie pamäte najpravejšieho potomka
		free(temp);
		return;
	}

	// TODO
	return;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
	// Koreň stromu je prázdny
	if (*tree == NULL) {
		return;
	}

	char currentKey = (*tree)->key;
	bool hasLeftSubtree = (*tree)->left != NULL;
	bool hasRightSubtree = (*tree)->right != NULL;

	// Zhodný kľúč - odstránenie uzla
	if (currentKey == key) {
		// Uzol nemá potomkov - list
		if (!hasLeftSubtree && !hasRightSubtree) {
			free(*tree);
			*tree = NULL;
			return;
		}

		// Uzol má jeden podstrom
		if (hasLeftSubtree ^ hasRightSubtree) {
			bst_node_t *temp = *tree;
			*tree = hasLeftSubtree ? (*tree)->left : (*tree)->right;
			free(temp);
			return;
		}

		// Uzol má podstromy na oboch stranách
		if (hasLeftSubtree && hasRightSubtree) {
			bst_replace_by_rightmost(*tree, &(*tree)->left);
			return;
		}
	}

	// TODO
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
	// Prázdny strom - nie je čo rušiť
	if (*tree == NULL) {
		return;
	}

	// TODO

	// Uvoľnenie pamäte aktuálneho uzla
	free(*tree);
	*tree = NULL;
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
	// TODO
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
	// Prázdny strom - nie je čo prechádzať
	if (tree == NULL) {
		return;
	}

	// TODO
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
	// TODO
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
	// Prázdny strom - nie je čo prechádzať
	if (tree == NULL) {
		return;
	}

	// TODO
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
	// TODO
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
	// Prázdny strom - nie je čo prechádzať
	if (tree == NULL) {
		return;
	}

	// TODO
}
