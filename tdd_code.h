//======== Copyright (c) 2023, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - graph
//
// $NoKeywords: $ivs_project_1 $tdd_code.h
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2023-03-07
//============================================================================//
/**
 * @file tdd_code.h
 * @author Martin Dočekal
 * @author Karel Ondřej
 *
 * @brief Definice metod tridy reprezentujici graf.
 */
#pragma once

#ifndef TDD_CODE_H_
#define TDD_CODE_H_

#include <vector>
#include <stdexcept>
#include <iostream>


/**
 * @brief reprezentace uzlu
 */
struct Node{
    size_t id;  ///< jednoznačný identifikátor uzlu
    size_t color;  ///< celé číslo reprezentující barvu uzlu, výchozí barva je 0 a značí neobarveno
    // doplňte vhodné struktury, pokud potřebujete
};

/**
 * @brief reprezentace hrany
 *
 * Jedná se o hotovou reprezentaci hrany a není nutný Váš zásah.
 * Třída umožňuje porovnání dvou hran tak, že i opačně orientované hrany mezi stejnými dvěma uzly jsou si rovny.
 */
class Edge{
public:
    size_t a;  ///< id uzlu a
    size_t b;  ///< id uzlu b

    /**
     * @brief Konstruktor hrany
     * @param[in] a	Id uzlu a
     * @param[in] b	Id uzlu b
     */
    Edge(size_t a, size_t b) : a(a), b(b) { }

    /**
     * @brief Porovnávání hran. Hrany jsou porovnávány podle id uzlů.
     * @param[in] other	Druhá hrana.
     * @return True pokud jsou hrany stejné, jinak false.
     */
    bool operator==(const Edge& other) const{
        return (a == other.a && b == other.b) || (a == other.b && b == other.a);
    }

    /**
     * @brief Porovnávání hran. Hrany jsou porovnávány podle id uzlů.
     * @param[in] other	Druhá hrana.
     * @return True pokud jsou hrany různé, jinak false.
     */
    bool operator!=(const Edge& other) const{
        return !(*this == other);
    }

    /**
     * @brief Vypíše hranu do streamu.
     * @param[in, out] os stream
     * @param[in] e hrana pro výpis
     * @return stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Edge& e) {
        return os << "{" << e.a << ", " << e.b << "}";
    }
};

/**
 * @brief Třída reprezentující neorientovaný graf bez smyček.
 *
 */
class Graph{
public:

    /**
     * @brief konstruktor prázdného grafu
     */
    Graph();

    /**
     * @brief destruktor grafu
     */
    ~Graph();

    /**
     * @return vektor ukazatelů na všechny uzly v grafu
     */
    std::vector<Node*> nodes();

    /**
     * @return vektor všech hran v grafu
     */
    std::vector<Edge> edges() const;

    /**
     * Přidá uzel s daným id do grafu a vrátí ukazatel na vytvořený uzel. Pokud uzel existuje vrátí nullptr.
     * Volající se nestárá o mazání uzlu.
     *
     * @param[in] nodeId Jednoznačný identifikátor uzlu
     * @return ukazatel na uzel nebo nullptr
     */
    Node* addNode(size_t nodeId);

    /**
     * Přidá hranu do grafu. Smyčky a duplicitní hrany jsou ignorovány.
     * Pokud uzel definovaný hranou neexistuje, tak bude vytvořen.
     *
     * @param[int] edge Hrana, která bude přidána do grafu.
     * @return True pokud byla hrana do grafu přidána, jinak false.
     */
    bool addEdge(const Edge& edge);

    /**
     * @brief Naplní graf z vektoru hran. Ignoruje duplicitní hrany a smyčk
     * Pokud uzel definovaný hranou neexistuje, tak bude vytvořen.
     *
     * @param[in] edges	Vektor obsahující hrany.
     */
    void addMultipleEdges(const std::vector<Edge>& edges);

    /**
     * @brief Vrátí ukazatel na uzel s daným id.
     * @param[in] nodeId	Id uzlu.
     * @return Ukazatel na uzel nebo nullptr, pokud uzel neexistuje.
     */
    Node* getNode(size_t nodeId);

    /**
     * @brief Zjistí, zda hrana existuje v grafu.
     * @param edge hrana, která nás zajímá
     * @return true pokud hrana existuje, jinak false
     */
    bool containsEdge(const Edge& edge) const;

    /**
     * odstraní uzel z grafu
     *
     * @param[in] nodeId id uzlu, který má být odstraněn
     * @exception out_of_range pokud uzel s daným id v grafu neexistuje
     */
    void removeNode(size_t nodeId);

    /**
     * odstraní hranu z grafu
     *
     * @param[in] edge hrana, která má být odstraněna
     * @exception out_of_range pokud hrana v grafu neexistuje
     */
    void removeEdge(const Edge& edge);

    /**
     * @return počet uzlů v grafu
     */
    size_t nodeCount() const;

    /**
     * @return počet hran v grafu
     */
    size_t edgeCount() const;

    /**
     * stupeň uzlu
     *
     * @param[in] nodeId id uzlu
     * @return počet hran, které mají tento uzel za svůj jeden koncový bod
     * @exception out_of_range pokud uzel v grafu neexistuje
     */
    size_t nodeDegree(size_t nodeId) const;

    /**
     * @return maximální stupeň uzlu v grafu
     */
    size_t graphDegree() const;

    /**
     * Provede obarvení uzlů v grafu. Obarvení je uloženo v atributu color v daném uzlu.
     * Nesmí se použít více než graphDegree + 1 barev.
     * Jaký algoritmus pro barvení zvolíte je na Vás. Může být použita heuristika nepřiřazující minimální počet barev,
     * ale musí být splněny testy.
     *
     * Barvením se rozumí, že přiřadíte každému uzlu barvu tak, že sousední uzly nemají stejnou barvu.
     */
    void coloring();

    /**
     * Smazání všech uzlů a hran v grafu.
     */
    void clear();

protected:
    // doplňte vhodné struktury

};

#endif // TDD_CODE_H_

/*** Konec souboru tdd_code.h ***/
