//======== Copyright (c) 2023, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - graph
//
// $NoKeywords: $ivs_project_1 $tdd_tests.cpp
// $Authors:    Karel Ondřej <ondrej@fit.vutbr.cz>
//              Martin Dočekal <idocekal@fit.vutbr.cz>
// $Date:       $2023-02-15
//============================================================================//
/**
 * @file tdd_tests.cpp
 * @author Martin Dočekal
 * @author Karel Ondřej
 *
 * @brief Testy implementace grafu.
 */

#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "tdd_code.h"

using namespace ::testing;

/**
 * @brief Fixture pro testy nad neprázdným grafem.
 */
class NonEmptyGraph : public Test{

    /**
     * @brief Vytvori graf s 5 uzly a 6 hranami.
     *  1 - 4
     *  |   |
     *  5 - 6
     *  |   |
     *  - 7 -
     */
    void SetUp() {
        graph.addMultipleEdges({{ 1, 4 }, { 1, 5 }, { 4, 6 }, { 5, 6 }, { 5, 7 }, { 7, 6 } });
    }

protected:
    Graph graph;
};

/**
 * @brief Fixture pro testy nad prázdným grafem.
 */
class EmptyGraph : public Test{
protected:
    Graph graph;
};

TEST_F(NonEmptyGraph, nodes){
    auto nodes = graph.nodes();

    EXPECT_THAT(nodes, UnorderedElementsAre(Field(&Node::id, 1),
                                            Field(&Node::id, 4),
                                            Field(&Node::id, 5),
                                            Field(&Node::id, 6),
                                            Field(&Node::id, 7)));
}

TEST_F(NonEmptyGraph, edges){
    auto edges = graph.edges();
    std::vector<Edge> expectedEdges({{1, 4},
                                     {1, 5},
                                     {4, 6},
                                     {5, 6},
                                     {5, 7},
                                     {7, 6}});
    EXPECT_THAT(edges, UnorderedElementsAre(Eq(Edge(1, 4)), Eq(Edge(1, 5)), Eq(Edge(4, 6)), Eq(Edge(5, 6)),
                                            Eq(Edge(5, 7)), Eq(Edge(7, 6))));
}

TEST_F(NonEmptyGraph, addNode){
    auto node = graph.addNode(8);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->id, 8);

    auto nodes = graph.nodes();

    EXPECT_THAT(nodes, UnorderedElementsAre(Field(&Node::id, 1),
                                            Field(&Node::id, 4),
                                            Field(&Node::id, 5),
                                            Field(&Node::id, 6),
                                            Field(&Node::id, 7),
                                            Field(&Node::id, 8)));

    EXPECT_EQ(graph.addNode(8), nullptr);
    nodes = graph.nodes();

    EXPECT_THAT(nodes, UnorderedElementsAre(Field(&Node::id, 1),
                                            Field(&Node::id, 4),
                                            Field(&Node::id, 5),
                                            Field(&Node::id, 6),
                                            Field(&Node::id, 7),
                                            Field(&Node::id, 8)));
}

TEST_F(NonEmptyGraph, addEdge){
    EXPECT_TRUE(graph.addEdge(Edge(1, 8)));
    auto edges = graph.edges();
    EXPECT_THAT(edges, UnorderedElementsAre(Eq(Edge(1, 4)), Eq(Edge(1, 5)), Eq(Edge(1, 8)), Eq(Edge(4, 6)),
                                            Eq(Edge(5, 6)), Eq(Edge(5, 7)), Eq(Edge(7, 6))));

    EXPECT_FALSE(graph.addEdge(Edge(8, 1)));
    edges = graph.edges();
    EXPECT_THAT(edges, UnorderedElementsAre(Eq(Edge(1, 4)), Eq(Edge(1, 5)), Eq(Edge(1, 8)), Eq(Edge(4, 6)),
                                            Eq(Edge(5, 6)), Eq(Edge(5, 7)), Eq(Edge(7, 6))));

    EXPECT_FALSE(graph.addEdge(Edge(1, 8)));
    edges = graph.edges();
    EXPECT_THAT(edges, UnorderedElementsAre(Eq(Edge(1, 4)), Eq(Edge(1, 5)), Eq(Edge(1, 8)), Eq(Edge(4, 6)),
                                            Eq(Edge(5, 6)), Eq(Edge(5, 7)), Eq(Edge(7, 6))));

    EXPECT_FALSE(graph.addEdge(Edge(1, 1)));
    edges = graph.edges();
    EXPECT_THAT(edges, UnorderedElementsAre(Eq(Edge(1, 4)), Eq(Edge(1, 5)), Eq(Edge(1, 8)), Eq(Edge(4, 6)),
                                            Eq(Edge(5, 6)), Eq(Edge(5, 7)), Eq(Edge(7, 6))));
}

TEST_F(NonEmptyGraph, addMultipleEdges)
{
    graph.addMultipleEdges({{10, 40},
                            {10, 40},
                            {10, 1},
                            {70, 70},
                            {70, 60}});

    EXPECT_THAT(graph.nodes(), UnorderedElementsAre(Field(&Node::id, 1),
                                                    Field(&Node::id, 4),
                                                    Field(&Node::id, 5),
                                                    Field(&Node::id, 6),
                                                    Field(&Node::id, 7),
                                                    Field(&Node::id, 10),
                                                    Field(&Node::id, 40),
                                                    Field(&Node::id, 70),
                                                    Field(&Node::id, 60)));

    EXPECT_THAT(graph.edges(), UnorderedElementsAre(Eq(Edge(1, 4)), Eq(Edge(1, 5)), Eq(Edge(4, 6)), Eq(Edge(5, 6)),
                                                    Eq(Edge(5, 7)), Eq(Edge(7, 6)), Eq(Edge(10, 40)), Eq(Edge(10, 1)),
                                                    Eq(Edge(70, 60))));
}


TEST_F(NonEmptyGraph, getNode){
    auto node = graph.getNode(5);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->id, 5);

    EXPECT_EQ(graph.getNode(9), nullptr);
}

TEST_F(NonEmptyGraph, containsEdge){
    EXPECT_TRUE(graph.containsEdge(Edge(1, 4)));
    EXPECT_TRUE(graph.containsEdge(Edge(4, 1)));

    EXPECT_FALSE(graph.containsEdge(Edge(1, 7)));
    EXPECT_FALSE(graph.containsEdge(Edge(7, 1)));

    EXPECT_FALSE(graph.containsEdge(Edge(4, 4)));
    EXPECT_FALSE(graph.containsEdge(Edge(15, 4)));
    EXPECT_FALSE(graph.containsEdge(Edge(4, 15)));
}

TEST_F(NonEmptyGraph, removeNode){
    graph.removeNode(1);
    auto nodes = graph.nodes();
    EXPECT_THAT(nodes, UnorderedElementsAre(Field(&Node::id, 4),
                                            Field(&Node::id, 5),
                                            Field(&Node::id, 6),
                                            Field(&Node::id, 7)));

    auto edges = graph.edges();
    EXPECT_THAT(edges, UnorderedElementsAre(Eq(Edge(4, 6)), Eq(Edge(5, 6)), Eq(Edge(5, 7)), Eq(Edge(7, 6))));

    EXPECT_THROW(graph.removeNode(1), std::out_of_range);
}

TEST_F(NonEmptyGraph, removeEdge){
    graph.removeEdge(Edge(1, 4));
    auto edges = graph.edges();
    EXPECT_THAT(edges, UnorderedElementsAre(Eq(Edge(1, 5)), Eq(Edge(4, 6)), Eq(Edge(5, 6)), Eq(Edge(5, 7)),
                                            Eq(Edge(7, 6))));

    EXPECT_THROW(graph.removeEdge(Edge(1, 4)), std::out_of_range);
}

TEST_F(NonEmptyGraph, nodeCount){
    EXPECT_EQ(graph.nodeCount(), 5);
}

TEST_F(NonEmptyGraph, edgeCount){
    EXPECT_EQ(graph.edgeCount(), 6);
}

TEST_F(NonEmptyGraph, nodeDegree){
    EXPECT_EQ(graph.nodeDegree(1), 2);
    EXPECT_EQ(graph.nodeDegree(4), 2);
    EXPECT_EQ(graph.nodeDegree(5), 3);
    EXPECT_EQ(graph.nodeDegree(6), 3);
    EXPECT_EQ(graph.nodeDegree(7), 2);
    EXPECT_THROW(graph.nodeDegree(9), std::out_of_range);
}

TEST_F(NonEmptyGraph, graphDegree){
    EXPECT_EQ(graph.graphDegree(), 3);
}

TEST_F(NonEmptyGraph, coloring){
    graph.coloring();
    auto nodes = graph.nodes();
    std::set<size_t> colors;
    for (auto node : nodes){
        colors.insert(node->color);
    }

    EXPECT_TRUE(*colors.begin() != 0); // obarveno

    EXPECT_LE(colors.size(), 4); // libovolný prostý graf je obarvitelný graphDegree + 1 barvami

    auto edges = graph.edges();
    for (auto edge : edges){
        EXPECT_NE(graph.getNode(edge.a)->color, graph.getNode(edge.b)->color);
    }
}

TEST_F(NonEmptyGraph, clear){
    graph.clear();
    auto nodes = graph.nodes();
    EXPECT_EQ(nodes.size(), 0);

    auto edges = graph.edges();
    EXPECT_EQ(edges.size(), 0);
}

TEST_F(EmptyGraph, nodes){
    auto nodes = graph.nodes();
    EXPECT_EQ(nodes.size(), 0);
}

TEST_F(EmptyGraph, edges){
    auto edges = graph.edges();
    EXPECT_EQ(edges.size(), 0);
}

TEST_F(EmptyGraph, addNode){
    auto node = graph.addNode(1);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->id, 1);
    auto nodes = graph.nodes();
    EXPECT_THAT(nodes, UnorderedElementsAre(Field(&Node::id, 1)));
}

TEST_F(EmptyGraph, addEdge){
    EXPECT_FALSE(graph.addEdge(Edge(1, 1)));
    auto edges = graph.edges();
    EXPECT_EQ(edges.size(), 0);

    EXPECT_TRUE(graph.addEdge(Edge(1, 4)));
    edges = graph.edges();
    EXPECT_THAT(edges, UnorderedElementsAre(Eq(Edge(1, 4))));
}

TEST_F(EmptyGraph, addMultipleEdges)
{
    graph.addMultipleEdges({{1, 4},
                            {1, 4},
                            {1, 5},
                            {4, 6},
                            {6, 4},
                            {5, 6},
                            {5, 7},
                            {7, 7},
                            {7, 6}});

    EXPECT_THAT(graph.nodes(), UnorderedElementsAre(Field(&Node::id, 1),
                                                    Field(&Node::id, 4),
                                                    Field(&Node::id, 5),
                                                    Field(&Node::id, 6),
                                                    Field(&Node::id, 7)));

    EXPECT_THAT(graph.edges(), UnorderedElementsAre(Eq(Edge(1, 4)), Eq(Edge(1, 5)), Eq(Edge(4, 6)), Eq(Edge(5, 6)),
                                                    Eq(Edge(5, 7)), Eq(Edge(7, 6))));
}

TEST_F(EmptyGraph, getNode){
    EXPECT_EQ(graph.getNode(1), nullptr);
}

TEST_F(EmptyGraph, containsEdge){
    EXPECT_FALSE(graph.containsEdge(Edge(1, 4)));
}

TEST_F(EmptyGraph, removeNode){
    EXPECT_THROW(graph.removeNode(1), std::out_of_range);
}

TEST_F(EmptyGraph, removeEdge){
    EXPECT_THROW(graph.removeEdge(Edge(1, 4)), std::out_of_range);
}

TEST_F(EmptyGraph, nodeCount){
    EXPECT_EQ(graph.nodeCount(), 0);
}

TEST_F(EmptyGraph, edgeCount){
    EXPECT_EQ(graph.edgeCount(), 0);
}

TEST_F(EmptyGraph, nodeDegree){
    EXPECT_THROW(graph.nodeDegree(1), std::out_of_range);
}

TEST_F(EmptyGraph, graphDegree){
    EXPECT_EQ(graph.graphDegree(), 0);
}

TEST_F(EmptyGraph, coloring){
    graph.coloring();
    auto nodes = graph.nodes();
    EXPECT_EQ(nodes.size(), 0);
}

TEST_F(EmptyGraph, clear){
    graph.clear();
    auto nodes = graph.nodes();
    EXPECT_EQ(nodes.size(), 0);

    auto edges = graph.edges();
    EXPECT_EQ(edges.size(), 0);
}


TEST(Edges, equal){
    EXPECT_TRUE(Edge(1, 4)==Edge(1, 4));
    EXPECT_TRUE(Edge(4, 1)==Edge(1, 4));
    EXPECT_FALSE(Edge(1, 4)==Edge(1, 5));
    EXPECT_FALSE(Edge(2, 4)==Edge(1, 4));
}

TEST(Edges, nonEqual){
    EXPECT_FALSE(Edge(1, 4)!=Edge(1, 4));
    EXPECT_FALSE(Edge(4, 1)!=Edge(1, 4));
    EXPECT_TRUE(Edge(1, 4)!=Edge(1, 5));
    EXPECT_TRUE(Edge(2, 4)!=Edge(1, 4));
}

TEST(Edges, toStringStream){
    std::stringstream ss;
    ss << Edge(1, 4);
    EXPECT_EQ(ss.str(), "{1, 4}");
}

/*** Konec souboru tdd_tests.cpp ***/
