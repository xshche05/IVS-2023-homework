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
#include <algorithm>


Graph::Graph()
{
	m_nodeCount = 0;
	m_edgeCount = 0;
}

Graph::~Graph()
{
	for (auto node : m_nodes) {
		delete node->edges;
		free(node);
	}
	for (auto edge : m_edges) {
		free(edge);
	}
}

std::vector<Node*> Graph::nodes() {
    std::vector<Node*> nodes = m_nodes;
    return nodes;
}

std::vector<Edge> Graph::edges() const{
    std::vector<Edge> edges;
	for (auto edge : m_edges) {
		edges.push_back(*edge);
	}
    return edges;
}

Node* Graph::addNode(size_t nodeId) {
	for (auto node : m_nodes) {
		if (node->id == nodeId) {
			return nullptr;
		}
	}
	Node* node = (Node*)malloc(sizeof(Node));
	if (!node) {
		return nullptr;
	}
	node->id = nodeId;
	node->color = 0;
	node->edges = new std::vector<Edge*>(0);
	m_nodes.push_back(node);
	m_nodeCount++;
	return node;
}

bool Graph::addEdge(const Edge& edge){
	if (edge.a == edge.b) {
		return false;
	}
	if (this->containsEdge(edge)) {
		return false;
	}
	if (!this->getNode(edge.a)) {
		if(!this->addNode(edge.a)) return false;
	}
	if (!this->getNode(edge.b)) {
		if (!this->addNode(edge.b)) return false;
	}
	Node* node_a = this->getNode(edge.a);
	Node* node_b = this->getNode(edge.b);
	Edge* edge_c = (Edge*)malloc(sizeof(Edge));
	if (!edge_c) {
		return false;
	}
	edge_c->a = edge.a;
	edge_c->b = edge.b;
	node_a->edges->push_back(edge_c);
	node_b->edges->push_back(edge_c);
	m_edges.push_back(edge_c);
	m_edgeCount++;
    return true;
}

void Graph::addMultipleEdges(const std::vector<Edge>& edges) {
	for (auto edge : edges) {
		this->addEdge(edge);
	}
}

Node* Graph::getNode(size_t nodeId){
	for (auto node : m_nodes) {
		if (node->id == nodeId) {
			return node;
		}
	}
    return nullptr;
}

bool Graph::containsEdge(const Edge& edge) {
	for (auto edge_c : m_edges) {
		if (edge_c->a == edge.a && edge_c->b == edge.b) {
			return true;
		}
		if (edge_c->a == edge.b && edge_c->b == edge.a) {
			return true;
		}
	}
	return false;
}

void Graph::removeNode(size_t nodeId){
	Node* node_c = this->getNode(nodeId);
	if (!node_c) {
		throw std::out_of_range("Node does not exist");
	}
	for (auto edge : *node_c->edges) {
		this->removeEdge(*edge);
	}
	for (auto node : m_nodes) {
		if (node->id == nodeId) {
			m_nodes.erase(std::remove(m_nodes.begin(), m_nodes.end(), node), m_nodes.end());
			delete node->edges;
			free(node);
			break;
		}
	}
	m_nodeCount--;
}

void Graph::removeEdge(const Edge& edge){
	if (!this->containsEdge(edge)) {
		throw std::out_of_range("Edge does not exist");
	}
	Node* nodeA = this->getNode(edge.a);
	Node* nodeB = this->getNode(edge.b);
	for (auto edge_c : *nodeA->edges) {
		if (edge_c->a == edge.a && edge_c->b == edge.b || edge_c->a == edge.b && edge_c->b == edge.a) {
			nodeA->edges->erase(std::remove(nodeA->edges->begin(), nodeA->edges->end(), edge_c), nodeA->edges->end());
			break;
		}
	}
	for (auto edge_c : *nodeB->edges) {
		if (edge_c->a == edge.a && edge_c->b == edge.b || edge_c->a == edge.b && edge_c->b == edge.a) {
			nodeB->edges->erase(std::remove(nodeB->edges->begin(), nodeB->edges->end(), edge_c), nodeB->edges->end());
			break;
		}
	}
	for (auto edge_c : m_edges) {
		if (edge_c->a == edge.a && edge_c->b == edge.b || edge_c->a == edge.b && edge_c->b == edge.a) {
			m_edges.erase(std::remove(m_edges.begin(), m_edges.end(), edge_c), m_edges.end());
			free(edge_c);
			break;
		}
	}
	m_edgeCount--;
}

size_t Graph::nodeCount() const{
    return m_nodeCount;
}

size_t Graph::edgeCount() const{
    return m_edgeCount;
}

size_t Graph::nodeDegree(size_t nodeId) {
	Node* node = this->getNode(nodeId);
	if (node == nullptr) {
		throw std::out_of_range("Node does not exist");
	}
    return node->edges->size();
}

size_t Graph::graphDegree() {
	size_t m_graphDegree = 0;
	for (auto node : m_nodes) {
		if (node->edges->size() > m_graphDegree) {
			m_graphDegree = node->edges->size();
		}
	}
    return m_graphDegree;
}

void Graph::coloring(){
for (auto node : m_nodes) {
		node->color = 0;
	}
	for (auto node : m_nodes) {
		std::vector<int> colors;
		for (auto edge : *node->edges) {
			Node* nodeA = this->getNode(edge->a);
			Node* nodeB = this->getNode(edge->b);
			if (nodeA->color != 0) {
				colors.push_back(nodeA->color);
			}
			if (nodeB->color != 0) {
				colors.push_back(nodeB->color);
			}
		}
		for (int i = 1; i <= m_nodes.size(); i++) {
			if (std::find(colors.begin(), colors.end(), i) == colors.end()) {
				node->color = i;
				break;
			}
		}
	}
}

void Graph::clear() {
	for (auto node : m_nodes) {
		delete node->edges;
		free(node);
	}
	for (auto edge : m_edges) {
		free(edge);
	}
	m_nodes.clear();
	m_edges.clear();
	m_nodeCount = 0;
	m_edgeCount = 0;
}

/*** Konec souboru tdd_code.cpp ***/
