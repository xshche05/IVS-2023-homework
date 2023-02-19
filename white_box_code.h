//======== Copyright (c) 2023, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - hash map class code
//
// $NoKeywords: $ivs_project_1 $white_box_code.h
// $Authors:    Karel Ondřej <ondrej@fit.vutbr.cz>
//              Martin Dočekal <idocekal@fit.vutbr.cz>
// $Date:       $2023-02-15
//============================================================================//
/**
 * @file white_box_code.h
 * 
 * @brief Deklarace metod tridy reprezentujici hashovácí tabulku.
 * 
 * Hasovaci tabulka je vyhledavaci datova struktura, ktera asociuje hasovaci
 * klice s hodnotami.
 * 
 * @author Karel Ondřej
 * @author Martin Dočekal
 */

#ifndef HASH_MAP_H_
#define HASH_MAP_H_

#include <stdlib.h>
#include <string.h>     
#include <stddef.h>
#include <stdbool.h>

/** Inicializační velikost tabulky. */
#define HASH_MAP_INIT_SIZE 8                    
/** Parametr použit v hledání dalšího indexu při kolizi. */
#define HASH_MAP_PERTURB_SHIFT 5                
/** Mez zaplnění kdy se má realokovat velikost tabulky. */
#define HASH_MAP_REALLOCATION_THRESHOLD 3/5.
/** Hyperparametr v hašovácí funkci. */
#define HASH_FUNCTION_PARAM_A 1794967309        
/** Hyperparametr v hašovácí funkci. */
#define HASH_FUNCTION_PARAM_B 7                 

// Informace pro C++ překladač, aby použil "C" linker pro následující funkce.
extern "C" {

/*******************************************************************************
 * Inicializace, deinicializace & alokace paměti
 ******************************************************************************/
/**
 * @brief Výčet návratových hodnot.
 * 
 * Výčet obsahující návratové hodnoty funkcí. Pokud nedojde k chybě, funkce 
 * vrací hodnotu @c OK. V případě problémů s alokací paměti se vrátí návratová 
 * hodnota @c MEMORY_ERROR. Podrobnější sémantika ostatních hodnot je popsána u 
 * jednotlivých funkcí.
 */
typedef enum {
    OK,                     ///< Vše v pořádku.
    MEMORY_ERROR,           ///< Problém při alokaci paměti.
    VALUE_ERROR,            ///< Neplatná hodnota argumentu.
    KEY_ERROR,              ///< Přístup ke klíči který není vložen v tabulce.
    KEY_ALREADY_EXISTS      ///< Klíč již v hašovací tabulce existuje.
} hash_map_state_code_t;

/**
 * @brief Záznam v hašovací tabulce.
 * 
 * Položka v hašovací tabulce. Pro efektivnější procházení (např. při dealokaci 
 * paměti) a prací s pamětí (projeví se při velkých indexech) jsou vložené 
 * položky implementované formou obousměrně vázaného seznamu a index obsahuje 
 * pouze ukazatele do tohoto seznamu. Pořadí položek v seznamu odpovídá pořadí 
 * vložení daného klíče do tabulky. 
 * 
 * Uživatel by k položkám struktury neměl přistupovat přímo, ale pomocí 
 * definovaného rozhraní níže. Nicméně v rámci testování můžete přímo testovat, 
 * zda rozhraní pracuje s tímto datovým typem korektně.
 */
typedef struct hash_map_item
{
    char* key;                  ///< Klíč
    size_t hash;                ///< Hash
    int value;                  ///< Uložená hodnota
    struct hash_map_item* next; ///< Následující položka 
    struct hash_map_item* prev; ///< Předcházející položka
} hash_map_item_t;

/**
 * @brief Datový typ hašovací tabulky. 
 * 
 * Uživatel by k položkám struktury neměl přistupovat přímo, ale pomocí 
 * definovaného rozhraní níže. Nicméně v rámci testování můžete přímo testovat, 
 * zda rozhraní pracuje s tímto datovým typem korektně.
 */
typedef struct hash_map
{
    hash_map_item_t** index;    ///< Index hašovací tabulky
    hash_map_item_t* first;     ///< První položka v seznamu
    hash_map_item_t* last;      ///< Poslední položka v seznamu
    /** Při odstranění je položka v indexu nahrazena tímto ukazatelem. */
    hash_map_item_t* dummy;     
    size_t allocated;           ///< Alokované místo (velikost indexu)
    size_t used;                ///< Počet vložených položek (velikost seznamu)
} hash_map_t;

/*******************************************************************************
 * Inicializace, deinicializace & alokace paměti
 ******************************************************************************/
/**
 * @brief Konstruktor hašovací tabulky.
 * 
 * Vytvoří a inicializuje hašovací tabulku s velikostí indexu 8, tzn. hašovací 
 * tabulka může obsahovat 8 záznamů s různými klíči.
 * 
 * 
 * Příklad užití:
 * @code{.c}
 * hash_map_t* map = hash_map_ctor();
 * // do something
 * hash_map_dtor(map);
 * @endcode
 * 
 * @return Ukazatel na inicializovanou hašovací tabulku. V případě chyby alokace
 *         vrací hodnotu @c NULL.
 *
 * @see hash_map_init
 */
hash_map_t* hash_map_ctor();

/**
 * @brief Destruktor hašovací tabulky.
 *  
 * Uklidí a uvolní paměť alokovanou hašovací tabulkou, tzn. jednotlivé položky 
 * seznamu, index a následně samotnou tabulku @p self .
 *  
 * Příklad užití:
 * @code{.c}
 * hash_map_t* map = hash_map_ctor();
 * // do something
 * hash_map_dtor(map);
 * @endcode
 * 
 * @param[in] self Ukazatel na hašovací tabulku
 *
 * @see hash_map_clear
 */
void hash_map_dtor(hash_map_t* self);

/**
 * @brief Dealokace vytvořeních položek a vymazání indexu.
 * 
 * @param[in] self Ukazatel na strukturu hašovací tabulky.
 */
void hash_map_clear(hash_map_t* self);

/**
 * @brief Realokace rezervovaného místa pro index. 
 * 
 * Funkce je implicitně volána ve funci @c hash_map_put, když je potřeba.
 * 
 * Příklad užití:
 * @code{.c}
 * hash_map_t* map = hash_map_ctor();
 * // hash_map_capacity(map) == 8
 * hash_map_reserve(map, 16);
 * // hash_map_capacity(map) == 16
 * @endcode
 * 
 * @warning Velikost indexu nemůže být menší než počet vložených položek, v 
 * takovém případě funkce nic nevykoná a vrátí hodnotu @c VALUE_ERROR . 
 * 
 * @param[in] self Ukazatel na strukturu hašovací tabulky.
 * @param[in] size Velikost indexu.
 * 
 * @return @c VALUE_ERROR pokud bude nová rezervovaná velikost hašovací tabulky 
 *         menší jak počet doposud vložených prvků, jinak @c OK.
 * 
 * @see hash_map_put
 */
hash_map_state_code_t hash_map_reserve(hash_map_t* self, size_t size);

/*******************************************************************************
 * Metody pro přístup k hašovací tabulce
 ******************************************************************************/
/**
 * @brief Vrací počet vložených záznamů do tabulky.
 * 
 * @param[in] self Ukazatel na strukturu hašovací tabulky.
 * 
 * @return Počet vložených záznamů do tabulky.
 * 
 * @see hash_map_put, hash_map_pop, hash_map_remove
 */
size_t hash_map_size(hash_map_t* self);

/**
 * @brief Vrací alokované místa pro tabulku.
 * 
 * @warning @c hash_map_size(map) se nerovná @c hash_map_capacity(map) ! Jelikož
 *          je alokace (resp. realokace) paměti časově "drahá" operace, alokuje 
 *          se vždy více paměti než je aktuálně potřeba. K realokaci tedy 
 *          dochází jen když je potřeba a výrazně méně často.
 * 
 * @param[in] self Ukazatel na strukturu hašovací tabulky.
 * 
 * @return Velikost alokované hašovací tabulky.
 * 
 * @see hash_map_reserve, hash_map_put, hash_map_pop, hash_map_remove
 */
size_t hash_map_capacity(hash_map_t* self);

/**
 * @brief Obsahuje tabulka záznam s daným klíčem?
 * 
 * Vyhledá zadaný klíč v tabulce a pokud tabulka obsahuje pod daným klíčem 
 * záznam, vrátí nenulovou hodnotu.
 * 
 * @param[in] self Ukazatel na strukturu hašovací tabulky.
 * @param[in] key  Klíč do tabulky.
 * 
 * @return Nenulová hodnota pokud se záznam asociován se zadaným klíčem nachází 
 *         v tabulce.
 */
bool hash_map_contains(hash_map_t* self, const char* key);

/**
 * @brief Vloží klíč a hodnotu do tabulky.
 * 
 * Pokud je již index tabulky zaplněn ze 2/3, realokuje pro index 2x větší místo
 * v paměti a provede reindexaci. Pokud tabulka již obsahuje k danému klíči 
 * záznam, hodnota záznamu se přepíše a funkce vrací hodnotu 
 * @c KEY_ALREADY_EXISTS .
 * 
 * Příklad užití:
 * @code{.c}
 * hash_map_state_code_t is_value_overwritten = hash_map_put(map, "aloha", 5);
 * @endcode
 * 
 * @param[in] self  Ukazatel na strukturu hašovací tabulky.
 * @param[in] key   Klíč do tabulky.
 * @param[in] value Hodnota k uložení.
 * 
 * @return Vrací @c KEY_ALREADY_EXISTS pokud se klíč nachází v tabulce, 
 *         jinak @c OK.
 * 
 * @see hash_map_reserve
 */
hash_map_state_code_t hash_map_put(hash_map_t* self, const char* key, 
                                   int value);

/**
 * @brief Uloží hodnotu asociovanou se zadaným klíčem na určené místo v paměti.
 * 
 * Vyhledá klíč v tabulce a uloží hodnotu přiřazenou ke klíči na místo kde 
 * ukazuje argument @p value . Pokud tabulka neobsahuje k danému klíči žádný 
 * záznam, vrací hodnotu @c KEY_ERROR .
 * 
 * Příklad užití:
 * @code{.c}
 * int value;
 * hash_map_state_code_t error = hash_map_get(map, "aloha", &value);
 * @endcode
 * 
 * @param[in]  self  Ukazatel na strukturu hašovací tabulky.
 * @param[in]  key   Klíč do tabulky.
 * @param[out] value Ukazatel na místo, kde se uloží hodnota.
 * 
 * @return Vrací @c KEY_ERROR pokud se klíč nenachází v tabulce, 
 *         jinak @c OK.
 */
hash_map_state_code_t hash_map_get(hash_map_t* self, const char* key, 
                                   int* value);

/**
 * @brief Uloží hodnotu z hašovací tabulky a odstraní záznam.
 * 
 * Vyhledá klíč v tabulce, uloží hodnotu přiřazenou ke klíči na místo kde 
 * ukazuje parametr @p value a odstraní záznam z tabulky. Pokud tabulka 
 * neobsahuje k danému klíči žádný záznam, vrací hodnotu @c KEY_ERROR . 
 * 
 * Příklad užití:
 * @code{.c}
 * int value;
 * hash_map_state_code_t error = hash_map_pop(map, "aloha", &value);
 * // hash_map_contains(map, "aloha") == false
 * @endcode
 *
 * @warning Odstranění záznamu z tabulky neovlivňuje alokované místo pro 
 *          tabulku.
 * 
 * @param[in]  self  Ukazatel na strukturu hašovací tabulky.
 * @param[in]  key   Klíč do tabulky.
 * @param[out] value Ukazatel na místo, kde se uloží hodnota.
 * 
 * @return Vrací @c KEY_ERROR pokud se klíč nenachází v tabulce, 
 *         jinak @c OK.
 * 
 * @see hash_map_remove
 */
hash_map_state_code_t hash_map_pop(hash_map_t* self, const char* key, 
                                   int* value);

/**
 * @brief Odstranění položky z hašovací tabulky.
 *
 * Vyhledá klíč v tabulce a odstraní záznam z tabulky. Pokud tabulka neobsahuje 
 * k danému klíči žádný záznam, vrací hodnotu @c KEY_ERROR}. 
 *
 * Příklad užití:
 * @code{.c}
 * hash_map_state_code_t error = hash_map_remove(map, "aloha");
 * // hash_map_contains(map, "aloha") == false
 * @endcode
 * 
 * @warning Odstranění záznamu z tabulky neovlivňuje alokované místo pro 
 *          tabulku.
 * 
 * @param[in] self Ukazatel na strukturu hašovací tabulky.
 * @param[in] key  Klíč do tabulky.
 * 
 * @return Vrací @c KEY_ERROR pokud se klíč nenachází v tabulce, 
 *         jinak @c OK.
 * 
 * @see hash_map_pop
 */
hash_map_state_code_t hash_map_remove(hash_map_t* self, const char* key);

}       // extern "C" ending

#endif  // HASH_MAP_H_

/*** Konec souboru white_box_code.h ***/
