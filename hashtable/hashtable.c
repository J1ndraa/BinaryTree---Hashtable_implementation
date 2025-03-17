/*  Ukol vypracoval: Halva Jindrich, xhalva05
 *
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
    int i = 0;
    //kazdou bunku taulky nastavim na null
    while (i != MAX_HT_SIZE){
        (*table)[i] = NULL;
        i++;
    }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    //table musi byt inicializovany
    if (table == NULL) {
        return NULL;
    }
    //ziskani indexu pres hash
    int index = get_hash(key);
    ht_item_t *item = (*table)[index];

    while(item != NULL){
        if((strcmp(item->key,key)) == 0){
            return item;
        }
        item = item->next;
    }
    return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    //table musi byt inicializovany
    if(table == NULL){
        return;
    }

    ht_item_t *item;
    //pokud je v tabulce uz orvek se stejnym klicem, prepise se jeho hodnota
    if((item = ht_search(table, key)) != NULL){
        item->value = value;
        return;
    }
    //alokace pameti pro novy prvek
    item = malloc(sizeof(ht_item_t));
    if(item == NULL){
        return;
    }
    item->value = value;
    item->key = key;

    int index = get_hash(key);
    //pokud na indexu neni zadny prvek vlozi se
    //pokud uz na indexu existuji nejake prvku, prvek se vlozi na zacatek seznamu
    if ((*table)[index] == NULL){
        (*table)[index] = item;
        item->next = NULL;
    }
    else{
        item->next = (*table)[index];
        (*table)[index] = item;
    }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    //table musi byt inicializovany
    if(table == NULL){
        return NULL;
    }

    //hledam prvek, pokud ho najdu, vratim jeho hodnotu
    ht_item_t *item;
    if((item = ht_search(table,key)) != NULL){
       return &(item->value);
    }

    return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    //table musi byt inicializovany
    if(table == NULL){
        return;
    }
    int index = get_hash(key);
    ht_item_t *curr_item = (*table)[index];
    ht_item_t *prev_item = NULL;

    while(curr_item != NULL){
        if(strcmp(curr_item->key,key) == 0){
            //prvek nema zadne dalsi prvky v seznamu
            if(curr_item->next == NULL){
                free(curr_item);
                return;
            }
            //prvek je prvni seznamu
            if(prev_item == NULL){
                (*table)[index] = curr_item->next;
                free(curr_item);
                return;
            }
            //prvek byl nalezen
            else{
                prev_item->next = curr_item->next;
                free(curr_item);
                return;
            }
        }
        //prechod na dalsi prvek
        prev_item = curr_item;
        curr_item = curr_item->next;
    }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
    int i = 0;
    //prochazeni vsech bunek
    while(i < MAX_HT_SIZE){
        if((*table)[i] != NULL) {
            ht_item_t *del_item = (*table)[i];
            //mazu seznam prvku na stejnem indexu
            while (del_item != NULL) {
                ht_item_t *next_item = del_item->next;
                free(del_item);
                del_item = next_item;
            }
        }
        (*table)[i] = NULL;
        i++;
    }
}
