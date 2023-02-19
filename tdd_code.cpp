//======== Copyright (c) 2023, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - graph
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2023-03-07
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Martin Dočekal
 * @author Karel Ondřej
 *
 * @brief Implementace metod tridy reprezentujici graf.
 */

#include "tdd_code.h"


Graph::Graph(){}

Graph::~Graph(){}

std::vector<Node*> Graph::nodes() {
    std::vector<Node*> nodes;

    return nodes;
}

std::vector<Edge> Graph::edges() const{
    std::vector<Edge> edges;

    return edges;
}

Node* Graph::addNode(size_t nodeId) {
    return nullptr;
}

bool Graph::addEdge(const Edge& edge){
    return false;

}

void Graph::addMultipleEdges(const std::vector<Edge>& edges) {

}

Node* Graph::getNode(size_t nodeId){
    return nullptr;
}

bool Graph::containsEdge(const Edge& edge) const{
    return false;
}

void Graph::removeNode(size_t nodeId){
}

void Graph::removeEdge(const Edge& edge){
}

size_t Graph::nodeCount() const{
    return 42;
}

size_t Graph::edgeCount() const{
    return 42;
}

size_t Graph::nodeDegree(size_t nodeId) const{
    return 42;
}

size_t Graph::graphDegree() const{
    return 42;
}

void Graph::coloring(){
}

void Graph::clear() {
}

/*** Konec souboru tdd_code.cpp ***/
