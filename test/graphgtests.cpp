#include <gtest/gtest.h>
#include "../graph/simpleGraph.h"

TEST(GraphTest, AddVerticesAndEdges) {
    Graph<int, std::string> g;

    g.addVertex("A");
    g.addVertex("B");
    g.addVertex("C");
    g.addVertex("D");
    g.addVertex("E");

    ASSERT_TRUE(g.hasVertex("A"));
    ASSERT_FALSE(g.hasVertex("F"));

    ASSERT_TRUE(g.addEdge(1, "A", "B"));
    ASSERT_TRUE(g.addEdge(2, "A", "C"));
    ASSERT_TRUE(g.addEdge(3, "B", "D"));
    ASSERT_TRUE(g.addEdge(4, "C", "E"));

    ASSERT_TRUE(g.hasEdge("A", "B"));
    ASSERT_FALSE(g.hasEdge("A", "D"));
}

TEST(GraphTest, FindConnectedComponent) {
    Graph<int, std::string> g;

    g.addVertex("A");
    g.addVertex("B");
    g.addVertex("C");
    g.addVertex("D");
    g.addVertex("E");

    g.addEdge(1, "A", "B");
    g.addEdge(2, "A", "C");
    g.addEdge(3, "B", "D");
    g.addEdge(4, "C", "E");

    auto component = g.findConnectedComponent("A");
    std::unordered_set<std::string> expected_component = {"A", "B", "C", "D", "E"};
    std::unordered_set<std::string> actual_component(component.begin(), component.end());

    ASSERT_EQ(actual_component, expected_component);
}


TEST(GraphTest, FindConnectedComponentSingleVertex) {
    Graph<int, std::string> g;

    g.addVertex("A");

    auto component = g.findConnectedComponent("A");
    std::unordered_set<std::string> expected_component = {"A"};
    std::unordered_set<std::string> actual_component(component.begin(), component.end());

    ASSERT_EQ(actual_component, expected_component);
}

TEST(GraphTest, FindConnectedComponentNoEdges) {
    Graph<int, std::string> g;

    g.addVertex("A");
    g.addVertex("B");
    g.addVertex("C");

    auto component = g.findConnectedComponent("A");
    std::unordered_set<std::string> expected_component = {"A"};
    std::unordered_set<std::string> actual_component(component.begin(), component.end());

    ASSERT_EQ(actual_component, expected_component);
}

TEST(GraphTest, FindConnectedComponentDisjointSets) {
    Graph<int, std::string> g;

    g.addVertex("A");
    g.addVertex("B");
    g.addVertex("C");
    g.addVertex("D");
    g.addVertex("E");
    g.addVertex("F");

    g.addEdge(1, "A", "B");
    g.addEdge(2, "A", "C");
    g.addEdge(3, "D", "E");

    auto componentA = g.findConnectedComponent("A");
    std::unordered_set<std::string> expected_componentA = {"A", "B", "C"};
    std::unordered_set<std::string> actual_componentA(componentA.begin(), componentA.end());

    ASSERT_EQ(actual_componentA, expected_componentA);

    auto componentD = g.findConnectedComponent("D");
    std::unordered_set<std::string> expected_componentD = {"D", "E"};
    std::unordered_set<std::string> actual_componentD(componentD.begin(), componentD.end());

    ASSERT_EQ(actual_componentD, expected_componentD);
}