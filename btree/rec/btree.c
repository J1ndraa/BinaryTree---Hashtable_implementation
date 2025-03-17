/* Ukol vypracoval: Halva Jindrich, xhalva05
 *
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
    *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    if (tree == NULL){
        return false;
    }
    //shodny klic nalezen
    if (tree->key == key){
        *value = tree->value;
        return true;
    }
    //vstoupime do leve vetve
    if (key < tree->key){
        return bst_search(tree->left, key, value);
    }
    //vstoupim do prave vetve, hledam vyssi klic
    if (key > tree->key){
        return bst_search(tree->right, key, value);
    }
    //klic nenalezen
    return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if(*tree == NULL) {
        //alokujeme novy uzel
        *tree = malloc(sizeof(bst_node_t));
        //overeni alokace
        if (*tree == NULL) {
            return;
        }
        //uzlu priradime dane hodnoty
        (*tree)->value = value;
        (*tree)->key = key;
        (*tree)->right = NULL;
        (*tree)->left = NULL;
    }
    else{
        //narazili jsme na shodny klic, prepiseme hodnotu
        if((*tree)->key == key){
            (*tree)->value = value;
        }
        //pokud je zadany klic mensi, nez klic aktualniho uzlu, vstoupime do leveho podstromu
        if(key < (*tree)->key){
            bst_insert(&(*tree)->left, key, value);
        }
        //pokud je zadany klic vetsi, nez klic aktualniho uzlu, vstoupime do praveho podstromu
        if(key > (*tree)->key) {
            bst_insert(&(*tree)->right, key, value);
        }
    }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if((*tree)->right == NULL){
        target->key = (*tree)->key;
        target->value = (*tree)->value;
        bst_node_t *mostright_ptr = *tree;
        *tree = (*tree)->left;
        free(mostright_ptr);
        return;
    }
    bst_replace_by_rightmost(target,/*ukazatel na pravy podstrom*/&((*tree)->right));
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    if (*tree == NULL){
        return;
    }
    //hledany klic se nachazi v levem podstromu
    if (key < (*tree)->key) {
        bst_delete(&(*tree)->left, key);
    }
    //hledany klic se nachazi v pravem podstromu
    else if (key > (*tree)->key) {
        bst_delete(&(*tree)->right, key);
    }
    else {
        //         nalezen uzel s klicem         //

        //nema zadneho syna
        if (((*tree)->right == NULL) && ((*tree)->left) == NULL) {
            free(*tree);
            (*tree) = NULL;
        }
        else {
            //ma dva syny
            if (((*tree)->right != NULL) && ((*tree)->left) != NULL) {
                bst_replace_by_rightmost(*tree, &((*tree)->left));
            }
            else {
                //ma praveho syna
                if ((*tree)->right != NULL) {
                    bst_node_t *Righttmp = *tree;
                    (*tree) = (*tree)->right;
                    free(Righttmp);
                }
                //ma leveho syna
                else if ((*tree)->left != NULL) {
                    bst_node_t *Lefttmp = *tree;
                    (*tree) = (*tree)->left;
                    free(Lefttmp);
                }
            }
        }
    }
}
/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    //uz prazdny strom
    if (tree == NULL) {
        return;
    }
    //prazdnu uzel
    if (*tree == NULL) {
        return;
    }
    //smazani podstromu
    if((*tree)->left != NULL){
        bst_dispose(&((*tree)->left));
    }
    if((*tree)->right != NULL){
        bst_dispose(&((*tree)->right));
    }
    // uvolneni uzlu
    free(*tree);
    *tree = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    if (tree != NULL) {
        //zpracovani uzlu
        bst_add_node_to_items(tree, items);

        if(tree->left != NULL) {
            bst_preorder(tree->left, items);
        }
        if(tree->right != NULL) {
            bst_preorder(tree->right, items);
        }
    }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    if (tree != NULL) {
        if(tree->left != NULL) {
            bst_inorder(tree->left, items);
        }
        //zpracovani uzlu
        bst_add_node_to_items(tree, items);

        if(tree->right != NULL) {
            bst_inorder(tree->right, items);
        }
    }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    if (tree != NULL) {
        if(tree->left != NULL) {
            bst_postorder(tree->left, items);
        }
        if(tree->right != NULL) {
            bst_postorder(tree->right, items);
        }
        //zpracovani uzlu
        bst_add_node_to_items(tree, items);
    }
}
