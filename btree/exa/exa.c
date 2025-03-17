/* Ukol vypracoval: Halva Jindrich, xhalva05
 *
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>



/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    bst_init(tree);

    int i = 0;
    while(input[i] != '\0'){
        int value = 1;
        char c = input[i];
        //pokud je to symbol co muze byt samostatne jako klic
        if( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == ' ') ) {
            if((c >= 'A' && c <= 'Z')){
                c = c + 32;
            }
            if(bst_search(*tree,c,&value)) {
                //search vratil hodnotu aktualni value
                //tu zvysime a pridame do stromu znak
                value++;
                //prepise se value
                bst_insert(tree, c, value);
            }
            else{
                //pridani znaku
                bst_insert(tree,c,value);
            }
        }
        //jiny znak
        else{
            c = '_';
            if(bst_search(*tree,c,&value)) {
                //search vratil hodnotu aktualni value
                //tu zvysime a pridame do stromu znak
                value++;
                bst_insert(tree, c, value);
            }
            else{
                bst_insert(tree,c,value);
            }
        }
        i++;
    }
}

/**
 * Naplni strom prvky ze zadaneho pole.
*/
void TreeFromItems(bst_node_t **newtree, int left, int right, bst_items_t *items) {
    if (left <= right) {
        int middle = (left + right) / 2;

        //alokace noveho uzlu v novem stromu
        *newtree = (bst_node_t *) malloc(sizeof(bst_node_t));
        if(newtree == NULL){
            return;
        }

        //pridani uzlu (prvek ze stredu pole)
        (*newtree)->key = items->nodes[middle]->key;
        (*newtree)->value = items->nodes[middle]->value;

        //rekurzivni volani funkce
        TreeFromItems(&(*newtree)->left,left,middle-1,items);
        TreeFromItems(&(*newtree)->right,middle+1,right,items);
    }
    else{
        (*newtree) = NULL;
    }
}

/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
    //pokud strom nema prvky, nebo ma jeden prvek bez potomku, nic nedelam
    if((*tree == NULL) || (((*tree)->right == NULL) && ((*tree)->left == NULL))){
        return;
    }

    bst_items_t items;

    //alokace pro uzly items
    items.nodes = (bst_node_t **)malloc(sizeof(bst_node_t));
    if (items.nodes == NULL) {
        return;
    }
    items.capacity = 0;
    items.size = 0;

    //nasypu do pole uzly stromu a pak puvodni strom promazu
    bst_inorder(*tree, &items);

    //novy strom ktery uz bude vybalancovany
    bst_node_t *newtree;
    bst_init(&newtree);

    int size = items.size;
    int right = size-1;

    TreeFromItems(&newtree, 0, right, &items);

    //stary strom = novy strom
    bst_dispose(tree);
    *tree = newtree;

    //uvolneni items
    free(items.nodes);
}