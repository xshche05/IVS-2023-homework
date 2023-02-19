//======== Copyright (c) 2023, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - hash map class code
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Authors:    Karel Ondřej <ondrej@fit.vutbr.cz>
//              Martin Dočekal <idocekal@fit.vutbr.cz>
// $Date:       $2023-02-15
//============================================================================//
/**
 * @file white_box_code.cpp
 * @author Karel Ondřej
 * @author Martin Dočekal
 * 
 * @brief Definice metod tridy reprezentujici hashovácí tabulku.
 */

#include "white_box_code.h"
#include <stdio.h>

/*******************************************************************************
 * Pomocné metody.
 ******************************************************************************/
/**
 * @brief Výpočet haše pro zadaný řetězec.
 *
 * @param[in] str klíč
 * @return hash 
 */
size_t hash_function(const char* str)
{
    size_t hash = 0;

    for (size_t idx = 0; str[idx] != '\0'; idx++)
    {
        hash += HASH_FUNCTION_PARAM_A*str[idx] + HASH_FUNCTION_PARAM_B;
    }

    return hash;
}

/**
 * @brief Výpočet indexu v hašovací tabulce v závislosti na dvojici klíč-hash.
 * 
 * V případě kolize funkce nalezne další volný index v tabulce. V případě 
 * vyhledávání považujeme @c dummy objekt jako jakýkoliv jiný záznam, tedy
 * @p ignore_dummy je nastaveno na @c false . Při vkládání je @c dummy objekt 
 * ekvivalentní prázdnému místu, tedy @p ignore_dummy je nastaveno na @c true .
 * Tento jev je způsoben kvůli kolizím klíčů. Když je záznam odstraněn, je 
 * nahrazen za @c dummy objekt, aby bylo možné vyřešit případné kolize.
 *
 * @param[in] self         Ukazatel na strukturu hašovací tabulky.
 * @param[in] str          Klíč.
 * @param[in] str          Haš zadaného klíče.
 * @param[in] ignore_dummy Pokud @c true, @c dummy objekt je vždy přeskočen,
 *                         pokud @c false, @c dummy objekt je interpretován jako
 *                         @c NULL.
 * 
 * @return Index záznamu asociovaný k zadanému klíči a haši, nebo prázdné místo
 *         v tabulce.
 */
size_t hash_map_lookup_handle(hash_map_t* self, const char* key, size_t hash, 
                              bool ignore_dummy)
{
    size_t idx = hash % self->allocated;
    size_t perturb = hash;

    while ( 
        (self->index[idx] != NULL && 
        (ignore_dummy || self->index[idx] != self->dummy)) &&
        (
            self->index[idx] == self->dummy ||
            (
                (self->index[idx]->hash != hash) || 
                (strcmp(self->index[idx]->key, key) != 0)
            )
        )
    )
    {
        idx = ((idx << 2) + idx + perturb + 1) % self->allocated;
        perturb >>= HASH_MAP_PERTURB_SHIFT;
    }

    return idx;
}
/**
 * @brief Výpočet indexu v hašovací tabulce v závislosti na dvojici klíč-hash.
 *
 * @param[in] self Ukazatel na strukturu hašovací tabulky.
 * @param[in] str  Klíč.
 * @param[in] str  Haš zadaného klíče.
 * 
 * @return Index záznamu asociovaný k zadanému klíči a haši, nebo prázdné místo
 *         v tabulce.
 * 
 * @see hash_map_lookup_handle
 */
size_t hash_map_lookup(hash_map_t* self, const char* key, size_t hash)
{
    return hash_map_lookup_handle(self, key, hash, true);
}

/**
 * @brief Inicializace hašovací tabulky.
 * 
 * Metoda alokuje a inicializuje položky struktury hašovací tabulky.
 * 
 * @param self[in] Ukazatel na neinicializovanou hašovací tabulku
 * @param size[in] Počet prvků v tabulce.
 * 
 * @return @c MEMORY_ERROR v případě chyby v alokaci paměti, jinak @c OK.
 */
hash_map_state_code_t hash_map_init(hash_map_t* self, size_t size)
{
    self->dummy = (hash_map_item_t*)malloc(sizeof(hash_map_item_t));
    self->first = self->last = NULL;
    self->used = 0;
    self->allocated = 0;
    self->index = NULL;
    
    if (hash_map_reserve(self, size) == MEMORY_ERROR)
    {
        free(self->dummy);
        return MEMORY_ERROR;
    }

    return OK;
}

/*******************************************************************************
 * Definice veřejných metod.
 ******************************************************************************/

hash_map_t* hash_map_ctor()
{
    hash_map_t* map = (hash_map_t*)malloc(sizeof(hash_map_t));
    if (map != NULL && hash_map_init(map, HASH_MAP_INIT_SIZE) == MEMORY_ERROR) 
    {
        free(map);
        map = NULL;
    }
    return map;
}

void hash_map_clear(hash_map_t* self)
{
    size_t idx;
    hash_map_item_t* item = self->first;
    hash_map_item_t* curr_item;
    while (item != NULL)
    {
        curr_item = item;
        item = item->next;
        free(curr_item->key);
        free(curr_item);
    }

    for (size_t i = 0; i < self->allocated; ++i)
    {
        self->index[i] = NULL;
    }
    

    self->first = NULL;
    self->last = NULL;
    self->used = 0;
}

void hash_map_dtor(hash_map_t* self)
{
    hash_map_clear(self);
    free(self->index);
    free(self->dummy);
    self->index = NULL;
    self->allocated = 0;
    free(self);
}

hash_map_state_code_t hash_map_reserve(hash_map_t* self, size_t size)
{
    // chceme alokovat mene mista nez je vlozenych zaznamu?
    if (size < self->used)
    {
        return VALUE_ERROR;
    }

    if (size == self->allocated)
    {
        // jiz je alokovano
        return OK;
    }

    hash_map_item_t** new_index = (hash_map_item_t**)malloc(size*sizeof(hash_map_t*));
    if (new_index == NULL)
    {
        // alokace pameti selhala
        return MEMORY_ERROR;
    }
    // vycisteni indexu
    for (size_t i = 0; i < size; ++i)
    {
        new_index[i] = NULL;
    }

    if (self->index != NULL)
    {
        // prekopirovani indexu
        size_t idx;
        for (hash_map_item_t* item = self->first; item != NULL; item = item->next)
        {
            // zmenila se velikost, potrebujeme prepocitat indexy
            idx = hash_map_lookup(self, item->key, item->hash);
            new_index[idx] = item;
        }
        // uvolneni stareho indexu
        free(self->index);
    }
    // nahrazeni stareho indexu
    self->index = new_index;
    self->allocated = size;

    return OK; 
}

size_t hash_map_size(hash_map_t* self) 
{
    return self->used;
}

size_t hash_map_capacity(hash_map_t* self)
{
    return self->allocated;
}

bool hash_map_contains(hash_map_t* self, const char* key)
{
    size_t hash = hash_function(key); 
    size_t idx = hash_map_lookup(self, key, hash);
    return self->index[idx] != NULL;
}

hash_map_state_code_t hash_map_put(hash_map_t* self, const char* key, int value)
{
    // je potreba realokovat misto?
    if (((float)self->used / (float)self->allocated) >= HASH_MAP_REALLOCATION_THRESHOLD)
    {
        hash_map_reserve(self, self->allocated<<1);
    }

    size_t hash = hash_function(key);
    size_t idx = hash_map_lookup_handle(self, key, hash, false);

    // prazdne misto v indexu nebo se jedna o dummy objekt
    // Vizte hash_map_lookup_handle
    if (self->index[idx] == NULL || self->index[idx] == self->dummy) 
    {
        self->index[idx] = (hash_map_item_t*)malloc(sizeof(hash_map_item_t));
        if (self->index[idx] == NULL)
        {
            // alokace pameti selhala
            return MEMORY_ERROR;
        }

        self->index[idx]->key = (char*)malloc((strlen(key)+1)*sizeof(char));
        if (self->index[idx]->key == NULL)
        {
            // alokace pameti selhala
            free(self->index[idx]);
            self->index[idx] = NULL;
            return MEMORY_ERROR;
        }
        strcpy(self->index[idx]->key, key);
        self->index[idx]->hash = hash;
        self->index[idx]->value = value;
        self->index[idx]->next = NULL;
        self->index[idx]->prev = NULL;
        self->used++;
        // je seznam zaznamu prazdny?
        if (self->last == NULL)
        {
            self->first = self->last = self->index[idx];
        }
        else
        {
            self->last->next = self->index[idx];
            self->index[idx]->prev = self->last;
            self->last = self->index[idx];
        }
        return OK;
    }
    else 
    {
        self->index[idx]->value = value;
        return KEY_ALREADY_EXISTS;
    }
}

hash_map_state_code_t hash_map_get(hash_map_t* self, const char* key, int* dst)
{
    size_t hash = hash_function(key);
    size_t idx = hash_map_lookup(self, key, hash);

    if (self->index[idx] == NULL)
    {
        // klic neni asociovan se zadnym zaznamem
        return KEY_ERROR;
    }
    
    *dst = self->index[idx]->value;

    return OK;
}

hash_map_state_code_t hash_map_remove(hash_map_t* self, const char* key)
{
    int dst;
    return hash_map_pop(self, key, &dst);
}

hash_map_state_code_t hash_map_pop(hash_map_t* self, const char* key, int* dst)
{
    size_t hash = hash_function(key);
    size_t idx = hash_map_lookup(self, key, hash);

    if (self->index[idx] == NULL)
    {
        // klic neni asociovan se zadnym zaznamem
        return KEY_ERROR;
    }
    else 
    {
        // jedna se o prvni zaznam v seznamu?
        if (self->index[idx]->prev == NULL)
        {
            self->first = self->index[idx]->next;
        }
        else 
        {
            self->index[idx]->prev->next = self->index[idx]->next;
        }
        // jedna se o posledni zaznam v seznamu?
        if (self->index[idx]->next == NULL)
        {
            self->last = self->index[idx]->prev;
        }
        else 
        {
            self->index[idx]->next->prev = self->index[idx]->prev;
        }
        // uloz hodnotu
        *dst = self->index[idx]->value;
        // smaz zaznam
        free(self->index[idx]->key);
        free(self->index[idx]);
        // Nahrazeni zaznamu za dummy objekt.
        // V pripade kolize, odstraneni prvne vlozeneho zaznamu s kolizi,
        // a nastaveni daneho mista na NULL, algoritmus by nemel informaci, 
        // zda ke kolizi doslo.
        self->index[idx] = self->dummy;
    }

    return OK;
}

/*** Konec souboru white_box_code.cpp ***/
