//======== Copyright (c) 2022, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - test suite
//
// $NoKeywords: $ivs_project_1 $white_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2023-03-07
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace testu hasovaci tabulky.
 */

#include <vector>

#include "gtest/gtest.h"

#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy hasovaci tabulky, testujte nasledujici:
// 1. Verejne rozhrani hasovaci tabulky
//     - Vsechny funkce z white_box_code.h
//     - Chovani techto metod testuje pro prazdnou i neprazdnou tabulku.
// 2. Chovani tabulky v hranicnich pripadech
//     - Otestujte chovani pri kolizich ruznych klicu se stejnym hashem 
//     - Otestujte chovani pri kolizich hashu namapovane na stejne misto v 
//       indexu
//============================================================================//

// ctor
TEST(ctor, ctor)
{
	hash_map_t* table = hash_map_ctor();
	EXPECT_TRUE(table != nullptr);
}

// put
TEST(put, put_one)
{
	hash_map_t* table = hash_map_ctor();
	EXPECT_EQ(hash_map_put(table, "key", 1), OK);
}

TEST(put, put_more_than_could_fit)
{
	hash_map_t* table = hash_map_ctor();
	std::string key;
	int allocated = table->allocated;

	for (int i = 0; i < allocated+1; i++)
	{
		key = "key" + std::to_string(i);
		EXPECT_EQ(hash_map_put(table, key.c_str(), 1), OK);
	}
}

TEST(put, put_existed_key)
{
	hash_map_t* table = hash_map_ctor();
	ASSERT_EQ(hash_map_put(table, "key", 1), OK);
	EXPECT_EQ(hash_map_put(table, "key", 2), KEY_ALREADY_EXISTS);
}

TEST(put, put_collision)
{
	hash_map_t* table = hash_map_ctor();
	ASSERT_EQ(hash_map_put(table, "abc", 1), OK);
	EXPECT_EQ(hash_map_put(table, "cba", 2), OK);
}

// get
TEST(get, get_existed)
{
	hash_map_t* table = hash_map_ctor();
	hash_map_put(table, "key", 1);
	int value;
	EXPECT_EQ(hash_map_get(table, "key", &value), OK);
	EXPECT_EQ(value, 1);
}

TEST(get, get_not_existed)
{
	hash_map_t* table = hash_map_ctor();
	int value;
	EXPECT_EQ(hash_map_get(table, "key", &value), KEY_ERROR);
}

//pop
TEST(pop, pop_existed_first)
{
	hash_map_t* table = hash_map_ctor();
	hash_map_put(table, "key", 1);
	int value;
	EXPECT_EQ(hash_map_pop(table, "key", &value), OK);
	EXPECT_EQ(value, 1);
}

TEST(pop, pop_existed_last)
{
	hash_map_t* table = hash_map_ctor();
	hash_map_put(table, "key", 1);
	hash_map_put(table, "key2", 2);
	int value;
	EXPECT_EQ(hash_map_pop(table, "key2", &value), OK);
	EXPECT_EQ(value, 2);
}

TEST(pop, pop_existed_not_first_or_last)
{
	hash_map_t* table = hash_map_ctor();
	hash_map_put(table, "key", 1);
	hash_map_put(table, "key2", 2);
	hash_map_put(table, "key3", 3);
	int value;
	EXPECT_EQ(hash_map_pop(table, "key2", &value), OK);
	EXPECT_EQ(value, 2);
}

TEST(pop, pop_not_existed)
{
	hash_map_t* table = hash_map_ctor();
	int value;
	EXPECT_EQ(hash_map_pop(table, "key", &value), KEY_ERROR);
}

// remove
TEST(remove, remove_existed)
{
	hash_map_t* table = hash_map_ctor();
	hash_map_put(table, "key", 1);
	EXPECT_EQ(hash_map_remove(table, "key"), OK);
}

TEST(remove, remove_not_existed)
{
	hash_map_t* table = hash_map_ctor();
	EXPECT_EQ(hash_map_remove(table, "key"), KEY_ERROR);
}

// contains
TEST(contains, contains_existed)
{
	hash_map_t* table = hash_map_ctor();
	hash_map_put(table, "key", 1);
	EXPECT_EQ(hash_map_contains(table, "key"), true);
}

TEST(contains, contains_not_existed)
{
	hash_map_t* table = hash_map_ctor();
	EXPECT_EQ(hash_map_contains(table, "key"), false);
}

// size
TEST(size, size)
{
	hash_map_t* table = hash_map_ctor();
	EXPECT_EQ(hash_map_size(table), table->used);
}

// capacity
TEST(capacity, capacity)
{
	hash_map_t* table = hash_map_ctor();
	EXPECT_EQ(hash_map_capacity(table), table->allocated);
}

// reserve
TEST(reserve, reserve_less_used)
{
	hash_map_t* table = hash_map_ctor();
	hash_map_put(table, "key", 1);
	hash_map_put(table, "key2", 2);
	EXPECT_EQ(hash_map_reserve(table, table->used-1), VALUE_ERROR);
}

TEST(reserve, reserve_more_allocated)
{
	hash_map_t* table = hash_map_ctor();
	hash_map_put(table, "key", 1);
	hash_map_put(table, "key2", 2);
	EXPECT_EQ(hash_map_reserve(table, table->allocated+1), OK);
}

TEST(reserve, reserve_same_allocated)
{
	hash_map_t* table = hash_map_ctor();
	hash_map_put(table, "key", 1);
	hash_map_put(table, "key2", 2);
	EXPECT_EQ(hash_map_reserve(table, table->allocated), OK);
}

// clear
TEST(clear, clear)
{
	hash_map_t* table = hash_map_ctor();
	hash_map_put(table, "key", 1);
	hash_map_clear(table);
	EXPECT_EQ(table->used, 0);
}

// dtor
TEST(dtor, dtor)
{
	hash_map_t* table = hash_map_ctor();
	hash_map_put(table, "key", 1);
	hash_map_dtor(table);
	EXPECT_EQ(table->allocated, 0);
}

/*** Konec souboru white_box_tests.cpp ***/
