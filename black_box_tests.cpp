//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Kirill Shchetiniuk <xshche05@stud.fit.vutbr.cz>
// $Date:       $2023-02-20
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Kirill Shchetiniuk
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

TEST(EmptyTree, InsertNode_True)
{
	BinaryTree tree;
	EXPECT_EQ(tree.InsertNode(1).first, true);
}

TEST(EmptyTree, InsertNode_NotNullPtr)
{
	BinaryTree tree;
	EXPECT_NE(tree.InsertNode(1).second, nullptr);
}

TEST(EmptyTree, DeleteNode_false)
{
	BinaryTree tree;
	EXPECT_EQ(tree.DeleteNode(1), false);
}

TEST(EmptyTree, FindNode_nullptr)
{
	BinaryTree tree;
	EXPECT_EQ(tree.FindNode(1), nullptr);
}

TEST(NonEmptyTree, InsertNode_SameKey_bool)
{
	BinaryTree tree;
	tree.InsertNode(1);
	EXPECT_EQ(tree.InsertNode(1).first, false);
}

TEST(NonEmptyTree, InsertNode_SameKey_ptr)
{
	BinaryTree tree;
	tree.InsertNode(1);
	EXPECT_NE(tree.InsertNode(1).second, nullptr);
}

TEST(NonEmptyTree, InsertNode_DiffKey_bool)
{
	BinaryTree tree;
	tree.InsertNode(1);
	EXPECT_EQ(tree.InsertNode(2).first, true);
}

TEST(NonEmptyTree, InsertNode_DiffKey_ptr)
{
	BinaryTree tree;
	tree.InsertNode(1);
	EXPECT_NE(tree.InsertNode(2).second, nullptr);
}

TEST(NonEmptyTree, DeleteNode_ExistedKey)
{
	BinaryTree tree;
	tree.InsertNode(1);
	EXPECT_EQ(tree.DeleteNode(1), true);
}

TEST(NonEmptyTree, DeleteNode_NotExistedKey)
{
	BinaryTree tree;
	tree.InsertNode(1);
	EXPECT_EQ(tree.DeleteNode(2), false);
}

TEST(NonEmptyTree, FindNode_ExistedKey)
{
	BinaryTree tree;
	tree.InsertNode(1);
	EXPECT_NE(tree.FindNode(1), nullptr);
}

TEST(NonEmptyTree, FindNode_NotExistedKey)
{
	BinaryTree tree;
	tree.InsertNode(1);
	EXPECT_EQ(tree.FindNode(2), nullptr);
}

TEST(TreeAxioms, Axiom1_allLeafsAreBlack)
{
	for (int j = 0; j < 10; j++)
	{
		BinaryTree tree;
		for (int i = 0; i < random() % 100; i++)
		{
			tree.InsertNode(i);
		}
		std::vector<Node_t*> leafs;
		tree.GetLeafNodes(leafs);
		for (auto leaf: leafs)
		{
			EXPECT_EQ(leaf->color, BLACK);
		}
	}
}

TEST(TreeAxioms, Axiom2_RedsChildrenAreBlask)
{
	for (int j = 0; j < 10; j++)
	{
		BinaryTree tree;
		for (int i = 0; i < random() % 100; i++)
		{
			tree.InsertNode(i);
		}
		std::vector<Node_t*> nodes;
		tree.GetAllNodes(nodes);
		for (auto node: nodes)
		{
			if (node->color == RED)
			{
				ASSERT_EQ(node->pLeft->color, BLACK);
				ASSERT_EQ(node->pRight->color, BLACK);
			}
		}
	}
}

TEST(TreeAxioms, Axiom3_NumberOfBlackNodesOnPaths)
{
	for (int j = 0; j < 10; j++)
	{
		BinaryTree tree;
		for (int i = 0; i < random() % 100; i++)
		{
			tree.InsertNode(i);
		}
		std::vector<Node_t*> leafs;
		tree.GetLeafNodes(leafs);
		int prevBlackNodes = -1;
		for (auto leaf: leafs)
		{
			int blackNodes = 0;
			Node_t* node = leaf;
			while (node != nullptr)
			{
				if (node->color == BLACK)
				{
					blackNodes++;
				}
				node = node->pParent;
			}
			if (prevBlackNodes == -1)
			{
				prevBlackNodes = blackNodes;
			}
			else
			{
				ASSERT_EQ(prevBlackNodes, blackNodes);
			}
		}
	}
}

/*** Konec souboru black_box_tests.cpp ***/
