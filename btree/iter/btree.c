/* Ukol vypracoval: Halva Jindrich, xhalva05
 *
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    bool found = false;
    bst_node_t *node = tree;
    //prochazeni stromem
    while(node != NULL){
        //nalezen stejny klic
        if(node->key == key){
            found = true;
            *value = node->value;
            return found;
        }
        //klic nenalezen (zatim?)
        else {
            if (key < node->key) {
                node = node->left;
            }
            else {
                node = node->right;
            }
            //konec stromu
            if (node == NULL) {
                break;
            }
        }
    }
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if(tree == NULL){
        return;
    }
    bst_node_t *node = (*tree);
    bst_node_t *where = NULL;
    //prochazeni stromem
    while(node != NULL) {
        where = node;
        //nalezen stejny klic, akorat prepiseme value
        if (node->key == key) {
            node->value = value;
            return;
        }
            //klic nenalezen (zatim?)
        else {
            if (key < node->key) {
                node = node->left;
            } else {
                node = node->right;
            }
            //konec stromu
            if (node == NULL) {
                break;
            }
        }
    }//konec cyklu
    //alokace pro novy prvek
    node = (bst_node_t *) malloc(sizeof(bst_node_t));
    if(node == NULL){
        return;
    }
    node->key = key;
    node->value = value;
    node->right = NULL;
    node->left = NULL;
    //otcovsky uzel neexistuje
    if(where == NULL){
        (*tree) = node;
    }
    //pripojeni k otcovskemu uzlu

    else if(key < where->key){
        where->left = node;
    }
    else if(key > where->key){
        where->right = node;
    }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if(target == NULL){
        return;
    }
    bst_node_t *actNode = (*tree);
    //prochazim strom tak dlouho, dokud nenarazim na nejpravejsi uzel
    while(actNode->right != NULL){
        actNode = actNode->right;
    }
    //nahrazeni hodnot
    target->key = actNode->key;
    target->value = actNode->value;
    //vymazani prvku pomoci funkce jez je implementovana nize
    bst_delete(tree, actNode->key);
    return;
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    //jestli je co odstranovat
    if((*tree) == NULL){
        return;
    }
    bst_node_t *prevNode = NULL;
    bst_node_t *actNode = (*tree);
    //hledani stejneho klice
    while(actNode != NULL){
        if(key < actNode->key){
            prevNode = actNode;
            actNode = actNode->left;
            continue;
        }
        if(key > actNode->key){
            prevNode = actNode;
            actNode = actNode->right;
            continue;
        }
        break;
    }
    //uzel se stejnym klicem nebyl nalezen
    if(actNode == NULL){
        return;
    }
    //uzel nema potomky
    if((actNode->right == NULL) && (actNode->left == NULL)) {
        //jediny uzel -> cely strom je zrusen
        if (prevNode == NULL) {
            free(actNode);
        } else {
            if (prevNode->right == actNode) {
                prevNode->right = NULL;
                free(actNode);
            } else if (prevNode->left == actNode) {
                prevNode->left = NULL;
                free(actNode);
            }
        }
    }
    //uzel ma pouze praveho potomka
    else if((actNode->right != NULL) && (actNode->left == NULL)){
        //nema otce
        if(prevNode == NULL){
            (*tree) = actNode->right;
            free(actNode);
        }
        else{
            if(prevNode->right == actNode){
                prevNode->right = actNode->right;
                free(actNode);
            }
            else if(prevNode->left == actNode){
                prevNode->left = actNode->right;
                free(actNode);
            }
        }
    }
    //uzel ma pouze leveho potomka
    else if((actNode->right == NULL) && (actNode->left != NULL)) {
        //nema otce
        if(prevNode == NULL){
            (*tree) = actNode->left;
            free(actNode);
        }
        else{
            if(prevNode->right == actNode){
                prevNode->right = actNode->left;
                free(actNode);
            }
            else if(prevNode->left == actNode){
                prevNode->left = actNode->left;
                free(actNode);
            }
        }
    }
    //ma oba potomky
    else if((actNode->right != NULL) && (actNode->left != NULL)) {
        bst_replace_by_rightmost(actNode,&(actNode->left));
    }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    //neni co odstranovat
    if(*tree == NULL){
        return;
    }

    bst_node_t *actNode = (*tree);
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    if(stack == NULL){
        return;
    }
    stack_bst_init(stack);

    //na zacatek do zasobniku umistime koren
    stack_bst_push(stack, actNode);

    //iteruju dokud v zasobniku neco je
    //kdyz narazim na uzel ktery ma potomky, automaticky je pridam na vrchol zasobniku
    while (stack_bst_empty(stack) != true) {
        actNode = stack_bst_pop(stack);
        //jsem na konci vetve
        if (actNode == NULL) {
            continue;
        }
        //levy potomek
        if (actNode->left != NULL) {
            stack_bst_push(stack, actNode->left);
        }
        //pravy potomek
        if (actNode->right != NULL) {
            stack_bst_push(stack, actNode->right);
        }
        //odstraneni aktualniho uzlu
        //pokracuju nize do stromu
        free(actNode);
    }
    *tree = NULL;
    //uvolneni alokovaneho zasobniku
    free(stack);
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
    bst_node_t *actNode = tree;

    while(actNode != NULL){
        //koren podstromu poslu do items
        bst_add_node_to_items(actNode,items);
        stack_bst_push(to_visit,actNode);
        actNode = actNode->left;
    }

}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    if(tree == NULL){
        return;
    }
    //chceme prazdne pole na zacatku
    if(items->size != 0){
        return;
    }
    //alokace zasobniku
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    if(stack == NULL){
        return;
    }
    stack_bst_init(stack);
    bst_node_t *actNode = tree;
    //prvni spusteni funkce pro koren stromu
    bst_leftmost_preorder(actNode,stack,items);

    //dokud neni zasobnik prazdny iteruju
    while(stack_bst_empty(stack) != true){
        actNode = stack_bst_pop(stack);
        //pokud existuje pravy podstrom necham ho take projit funkci leftmost...
        if(actNode->right != NULL){
            bst_leftmost_preorder(actNode->right, stack, items);
        }
    }

    //uvolneni alokovaneho zasobniku
    free(stack);
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    bst_node_t *actNode = tree;

    while(actNode != NULL){
        stack_bst_push(to_visit,actNode);
        actNode = actNode->left;
    }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    if(tree == NULL){
        return;
    }
    //chceme prazdne pole na zacatku
    if(items->size != 0){
        return;
    }
    //alokace zasobniku
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    if(stack == NULL){
        return;
    }
    stack_bst_init(stack);
    bst_node_t *actNode = tree;
    //prvni spusteni funkce pro koren stromu
    bst_leftmost_inorder(actNode,stack);

    //dokud neni zasobnik prazdny...
    while(stack_bst_empty(stack) != true){
        actNode = stack_bst_pop(stack);
        bst_add_node_to_items(actNode,items);

        if(actNode->right != NULL){
            bst_leftmost_inorder(actNode->right,stack);
        }

    }
    //uvolneni alokovaneho zasobniku
    free(stack);
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    bst_node_t *actNode = tree;

    while (actNode != NULL){
        stack_bst_push(to_visit,actNode);
        stack_bool_push(first_visit,1);
        actNode = actNode->left;
    }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    if(tree == NULL){
        return;
    }
    //chceme prazdne pole na zacatku
    if(items->size != 0){
        return;
    }
    //alokace zasobniku
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    if(stack == NULL){
        return;
    }
    //alokace zasobniku ktery uchovava informaci o tom, zda byl uzel navstiven poprve
    stack_bool_t *stack_bool = malloc(sizeof(stack_bool_t));
    if(stack_bool == NULL){
        return;
    }
    stack_bst_init(stack);
    stack_bool_init(stack_bool);
    bst_node_t *actNode = tree;
    bool first_visit;
    //prvni spusteni funkce pro koren stromu
    bst_leftmost_postorder(actNode,stack,stack_bool);

    while(stack_bst_empty(stack) != true){
        actNode = stack_bst_pop(stack);
        first_visit = stack_bool_pop(stack_bool);

        //pokud aktualni uzel byl navstiven pouze jednou, ulozim ho do zasobniku
        //pokud ma pravy podstrom, vejdu do nej
        if(first_visit){
            stack_bst_push(stack,actNode);
            stack_bool_push(stack_bool, 0);

            if(actNode->right != NULL){
                bst_leftmost_postorder(actNode->right,stack,stack_bool);
            }
        }
        //uzel uz byl navstiven, muzu ho zaznamenat do pole items
        else{
            bst_add_node_to_items(actNode,items);
        }
    }
    //uvolneni zasobniku
    free(stack);
    free(stack_bool);
}
