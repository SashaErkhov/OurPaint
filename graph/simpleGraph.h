#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <stack>

template <typename edge, typename vertex>
struct Edge {
    edge v_weight;
    vertex v_from;
    vertex v_to;
};

template <typename edge, typename vertex>
class Graph {
    std::unordered_set<vertex> m_vertices;
    std::vector<Edge<edge, vertex>> m_edges;
    std::unordered_map<vertex, std::vector<Edge<edge, vertex>>> m_adjacencyList;

    void DFS(const vertex& v, std::unordered_set<vertex>& visited, std::vector<vertex>& component) const {
        visited.insert(v);
        component.push_back(v);
        if (m_adjacencyList.find(v) != m_adjacencyList.end()) {
            for (const auto& e : m_adjacencyList.at(v)) {
                if (visited.find(e.v_to) == visited.end()) {
                    DFS(e.v_to, visited, component);
                }
            }
        }
    }

public:
    Graph(): m_vertices(), m_adjacencyList(), m_edges(){}
    void addVertex(const vertex& v) {
        m_vertices.insert(v);
    }

    bool addEdge(edge weight, const vertex& from, const vertex& to) {
        if (m_vertices.find(from) != m_vertices.end() && m_vertices.find(to) != m_vertices.end()) {
            m_edges.push_back({weight, from, to});
            m_adjacencyList[from].push_back({weight, from, to});
            m_adjacencyList[to].push_back({weight, to, from});
            return true;
        }
        return false;
    }

    bool hasVertex(const vertex& v) const {
        return m_vertices.find(v) != m_vertices.end();
    }

    bool hasEdge(const vertex& from, const vertex& to) const {
        if (m_adjacencyList.find(from) != m_adjacencyList.end()) {
            for (const auto& e : m_adjacencyList.at(from)) {
                if (e.v_to == to) {
                    return true;
                }
            }
        }
        return false;
    }
    std::vector<vertex> findConnectedComponent(const vertex& start) const {
        std::vector<vertex> component;
        if (hasVertex(start)) {
            std::unordered_set<vertex> visited;
            DFS(start, visited, component);
        }
        return component;
    }
};

#endif // GRAPH_H
