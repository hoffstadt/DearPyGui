#pragma once

#include <algorithm>
#include <cassert>
#include <iterator>
#include <stack>
#include <stddef.h>
#include <utility>
#include <vector>

namespace example
{
template<typename ElementType>
struct Span
{
    using iterator = ElementType*;

    template<typename Container>
    Span(Container& c) : begin_(c.data()), end_(begin_ + c.size())
    {
    }

    iterator begin() const { return begin_; }
    iterator end() const { return end_; }

private:
    iterator begin_;
    iterator end_;
};

template<typename ElementType>
class IdMap
{
public:
    using iterator = typename std::vector<ElementType>::iterator;
    using const_iterator = typename std::vector<ElementType>::const_iterator;

    // Iterators

    const_iterator begin() const { return elements_.begin(); }
    const_iterator end() const { return elements_.end(); }

    // Element access

    Span<const ElementType> elements() const { return elements_; }

    // Capacity

    bool   empty() const { return sorted_ids_.empty(); }
    size_t size() const { return sorted_ids_.size(); }

    // Modifiers

    std::pair<iterator, bool> insert(int id, const ElementType& element);
    std::pair<iterator, bool> insert(int id, ElementType&& element);
    size_t                    erase(int id);
    void                      clear();

    // Lookup

    iterator       find(int id);
    const_iterator find(int id) const;
    bool           contains(int id) const;

private:
    std::vector<ElementType> elements_;
    std::vector<int>         sorted_ids_;
};

template<typename ElementType>
std::pair<typename IdMap<ElementType>::iterator, bool> IdMap<ElementType>::insert(
    const int          id,
    const ElementType& element)
{
    auto lower_bound = std::lower_bound(sorted_ids_.begin(), sorted_ids_.end(), id);

    if (lower_bound != sorted_ids_.end() && id == *lower_bound)
    {
        return std::make_pair(
            std::next(elements_.begin(), std::distance(sorted_ids_.begin(), lower_bound)), false);
    }

    auto insert_element_at =
        std::next(elements_.begin(), std::distance(sorted_ids_.begin(), lower_bound));

    sorted_ids_.insert(lower_bound, id);
    return std::make_pair(elements_.insert(insert_element_at, element), true);
}

template<typename ElementType>
std::pair<typename IdMap<ElementType>::iterator, bool> IdMap<ElementType>::insert(
    const int     id,
    ElementType&& element)
{
    auto lower_bound = std::lower_bound(sorted_ids_.begin(), sorted_ids_.end(), id);

    if (lower_bound != sorted_ids_.end() && id == *lower_bound)
    {
        return std::make_pair(
            std::next(elements_.begin(), std::distance(sorted_ids_.begin(), lower_bound)), false);
    }

    auto insert_element_at =
        std::next(elements_.begin(), std::distance(sorted_ids_.begin(), lower_bound));

    sorted_ids_.insert(lower_bound, id);
    return std::make_pair(elements_.insert(insert_element_at, std::move(element)), true);
}

template<typename ElementType>
size_t IdMap<ElementType>::erase(const int id)
{
    auto lower_bound = std::lower_bound(sorted_ids_.begin(), sorted_ids_.end(), id);

    if (lower_bound == sorted_ids_.end() || id != *lower_bound)
    {
        return 0ull;
    }

    auto erase_element_at =
        std::next(elements_.begin(), std::distance(sorted_ids_.begin(), lower_bound));

    sorted_ids_.erase(lower_bound);
    elements_.erase(erase_element_at);

    return 1ull;
}

template<typename ElementType>
void IdMap<ElementType>::clear()
{
    elements_.clear();
    sorted_ids_.clear();
}

template<typename ElementType>
typename IdMap<ElementType>::iterator IdMap<ElementType>::find(const int id)
{
    const auto lower_bound = std::lower_bound(sorted_ids_.cbegin(), sorted_ids_.cend(), id);
    return (lower_bound == sorted_ids_.cend() || *lower_bound != id)
               ? elements_.end()
               : std::next(elements_.begin(), std::distance(sorted_ids_.cbegin(), lower_bound));
}

template<typename ElementType>
typename IdMap<ElementType>::const_iterator IdMap<ElementType>::find(const int id) const
{
    const auto lower_bound = std::lower_bound(sorted_ids_.cbegin(), sorted_ids_.cend(), id);
    return (lower_bound == sorted_ids_.cend() || *lower_bound != id)
               ? elements_.cend()
               : std::next(elements_.cbegin(), std::distance(sorted_ids_.cbegin(), lower_bound));
}

template<typename ElementType>
bool IdMap<ElementType>::contains(const int id) const
{
    const auto lower_bound = std::lower_bound(sorted_ids_.cbegin(), sorted_ids_.cend(), id);

    if (lower_bound == sorted_ids_.cend())
    {
        return false;
    }

    return *lower_bound == id;
}

// a very simple directional graph
template<typename NodeType>
class Graph
{
public:
    Graph() : current_id_(0), nodes_(), edges_from_node_(), node_neighbors_(), edges_() {}

    struct Edge
    {
        int id;
        int from, to;

        Edge() = default;
        Edge(const int id, const int f, const int t) : id(id), from(f), to(t) {}

        inline int  opposite(const int n) const { return n == from ? to : from; }
        inline bool contains(const int n) const { return n == from || n == to; }
    };

    // Element access

    NodeType&        node(int node_id);
    const NodeType&  node(int node_id) const;
    Span<const int>  neighbors(int node_id) const;
    Span<const Edge> edges() const;

    // Capacity

    size_t num_edges_from_node(int node_id) const;

    // Modifiers

    int  insert_node(const NodeType& node);
    void erase_node(int node_id);

    int  insert_edge(int from, int to);
    void erase_edge(int edge_id);

private:
    int current_id_;
    // These contains map to the node id
    IdMap<NodeType>         nodes_;
    IdMap<int>              edges_from_node_;
    IdMap<std::vector<int>> node_neighbors_;

    // This container maps to the edge id
    IdMap<Edge> edges_;
};

template<typename NodeType>
NodeType& Graph<NodeType>::node(const int id)
{
    return const_cast<NodeType&>(static_cast<const Graph*>(this)->node(id));
}

template<typename NodeType>
const NodeType& Graph<NodeType>::node(const int id) const
{
    const auto iter = nodes_.find(id);
    assert(iter != nodes_.end());
    return *iter;
}

template<typename NodeType>
Span<const int> Graph<NodeType>::neighbors(int node_id) const
{
    const auto iter = node_neighbors_.find(node_id);
    assert(iter != node_neighbors_.end());
    return *iter;
}

template<typename NodeType>
Span<const typename Graph<NodeType>::Edge> Graph<NodeType>::edges() const
{
    return edges_.elements();
}

template<typename NodeType>
size_t Graph<NodeType>::num_edges_from_node(const int id) const
{
    auto iter = edges_from_node_.find(id);
    assert(iter != edges_from_node_.end());
    return *iter;
}

template<typename NodeType>
int Graph<NodeType>::insert_node(const NodeType& node)
{
    const int id = current_id_++;
    assert(!nodes_.contains(id));
    nodes_.insert(id, node);
    edges_from_node_.insert(id, 0);
    node_neighbors_.insert(id, std::vector<int>());
    return id;
}

template<typename NodeType>
void Graph<NodeType>::erase_node(const int id)
{

    // first, remove any potential dangling edges
    {
        static std::vector<int> edges_to_erase;

        for (const Edge& edge : edges_.elements())
        {
            if (edge.contains(id))
            {
                edges_to_erase.push_back(edge.id);
            }
        }

        for (const int edge_id : edges_to_erase)
        {
            erase_edge(edge_id);
        }

        edges_to_erase.clear();
    }

    nodes_.erase(id);
    edges_from_node_.erase(id);
    node_neighbors_.erase(id);
}

template<typename NodeType>
int Graph<NodeType>::insert_edge(const int from, const int to)
{
    const int id = current_id_++;
    assert(!edges_.contains(id));
    assert(nodes_.contains(from));
    assert(nodes_.contains(to));
    edges_.insert(id, Edge(id, from, to));

    // update neighbor count
    assert(edges_from_node_.contains(from));
    *edges_from_node_.find(from) += 1;
    // update neighbor list
    assert(node_neighbors_.contains(from));
    node_neighbors_.find(from)->push_back(to);

    return id;
}

template<typename NodeType>
void Graph<NodeType>::erase_edge(const int edge_id)
{
    // This is a bit lazy, we find the pointer here, but we refind it when we erase the edge based
    // on id key.
    assert(edges_.contains(edge_id));
    const Edge& edge = *edges_.find(edge_id);

    // update neighbor count
    assert(edges_from_node_.contains(edge.from));
    int& edge_count = *edges_from_node_.find(edge.from);
    assert(edge_count > 0);
    edge_count -= 1;

    // update neighbor list
    {
        assert(node_neighbors_.contains(edge.from));
        auto neighbors = node_neighbors_.find(edge.from);
        auto iter = std::find(neighbors->begin(), neighbors->end(), edge.to);
        assert(iter != neighbors->end());
        neighbors->erase(iter);
    }

    edges_.erase(edge_id);
}

template<typename NodeType, typename Visitor>
void dfs_traverse(const Graph<NodeType>& graph, const int start_node, Visitor visitor)
{
    std::stack<int> stack;

    stack.push(start_node);

    while (!stack.empty())
    {
        const int current_node = stack.top();
        stack.pop();

        visitor(current_node);

        for (const int neighbor : graph.neighbors(current_node))
        {
            stack.push(neighbor);
        }
    }
}
} // namespace example
