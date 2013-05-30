#ifndef GRAPH_INCLUDED
#define GRAPH_INCLUDED

#include <type_traits>
#include <vector>
#include <limits>
#include <cassert>

namespace graph {

class Edge;

class Vertex {
 public:
  Vertex() : edges_() {}
  void AddEdge(size_t edge) {
    edges_.push_back(edge);
  }
  std::vector<size_t> GetEdges() const {
    return edges_;
  }
 private:
  std::vector<size_t> edges_;
};

enum class EdgeCompare { FALSE = 0, SAME_TARGETS = -1, TRUE = 1 };

class Edge {
 public:
  Edge(size_t first, size_t  second, bool directed = false)
      : first_(first), second_(second), directed_(directed) {
  }
  size_t first() const {
    return first_;
  }
  size_t second() const {
    return second_;
  }
  void setdirected(bool new_directed) {
    directed_ = new_directed;
  }
  bool directed() const {
    return directed_;
  }
  EdgeCompare operator==(Edge other_edge) {
    auto same = (other_edge.first_ == first_ && other_edge.second_ == second_);
    auto reverse = (other_edge.first_ == second_ && other_edge.second_ == first_);
    auto direction = directed_ == other_edge.directed_;
    if (same) {
      if (direction) return EdgeCompare::TRUE;
      return EdgeCompare::SAME_TARGETS;
    } else if (reverse) {
      if (direction && !directed_) return EdgeCompare::TRUE;
      return EdgeCompare::SAME_TARGETS;
    }
    return EdgeCompare::FALSE;
  }
  
 protected:
  bool directed_;
  size_t first_;
  size_t second_;
};

template <typename T>
class WeightedEdge : public Edge {
 public:
  WeightedEdge(decltype(first_) first,
	       decltype(second_) second,
	       T weight,
	       decltype(directed_) directed = false)
      : Edge(first,second,directed),
	weight_(weight) {
  }
  T weight() const {
    return weight_;
  }

 private:
  T weight_;
};

template <class EdgeT = Edge>
class Graph {
 public:
  Graph() : vertex_set_(), edge_set_() {
    static_assert(std::is_base_of<Edge,EdgeT>::value,
		    "Edgetype must be derived from Edge");
  }
  void AddVertex(Vertex vertex) {
    vertex_set_.push_back(vertex);
  }
  template <typename ...Args>
  void AddVertex(Args&&... args) {
    vertex_set_.emplace_back(std::forward(args)...);
  }
  template <typename ...Args>
  void AddVertices(size_t n, Args&&... args) {
    vertex_set_.insert(vertex_set_.end(), n, Vertex(std::forward(args)...));
  }
  Vertex GetVertex(size_t index) const {
    return vertex_set_.at(index); // some error checking please :)
  }
  void AddEdge(EdgeT edge) {
    edge_set_.push_back(edge);
    _SimplifyEdges();
  }
  template <typename ...Args> //FIXME: wont work for the simplest constants
  void AddEdge(Args&&... args) {
    edge_set_.emplace_back(std::forward(args)...);
    _SimplifyEdges();
  }
  EdgeT GetEdge(size_t index) const {
    return edge_set_.at(index); // some error checking please :)
  }
  void print() {
    std::cout << "Vertex set: ";
    for (auto i = 0; i < vertex_set_.size(); ++i) {
      std::cout << i << " ( ";
      for (auto j : vertex_set_[i].GetEdges()) {
	std::cout << j << ' ';
      }
      std::cout << "), ";
    }	     
    std::cout << "\nEdge set : ";
    for (auto edge : edge_set_) {
      std::cout << edge.first() << (edge.directed() ? " -> " : " <-> ")
                << edge.second() << ", ";
    }
    std::cout << '\n';
  }
  size_t Order() const {
    return vertex_set_.size();
  }
  size_t Size() const {
    return edge_set_.size();
  }
  
 private:
  void _SimplifyEdges() { // checks duplicates
    auto new_edge = edge_set_.end() - 1; // only the last one will be checked
    EdgeCompare flag = EdgeCompare::FALSE;
    auto edge = edge_set_.begin();
    for (; edge != new_edge; ++edge) {
      flag = *new_edge == *edge;
      if (flag != EdgeCompare::FALSE) break;//there should never be > 1
    }
    if (flag != EdgeCompare::FALSE) { //if identical just remove
      edge_set_.erase(new_edge);
      if (flag == EdgeCompare::SAME_TARGETS) {//if one is directed and one not
	edge->setdirected(false); //just set the old one undirected
	_AddEdgeToVertex(std::distance(edge_set_.begin(),edge));
      }
    } else {
      //update vertex information
      _AddEdgeToVertex(std::distance(edge_set_.begin(),new_edge));
    }
  }
  void _AddEdgeToVertex(size_t edge_index) {
    auto edge = edge_set_[edge_index];
    assert(edge.first() < vertex_set_.size());
    assert(edge.second() < vertex_set_.size());
    auto& first = vertex_set_[edge.first()];
    auto& second = vertex_set_[edge.second()];
    auto CheckAndAdd = 
      [&edge_index](Vertex& vertex) {
        bool flag = false;
	for (auto edge_idx : vertex.GetEdges())
	  if (edge_idx == edge_index) {
	    flag = true;
	    break;
	  }
	if (!flag) vertex.AddEdge(edge_index);
      };

    CheckAndAdd(first);
    if (!edge.directed())
      CheckAndAdd(second);
  }
  
  std::vector<Vertex> vertex_set_;
  std::vector<EdgeT> edge_set_;
};

namespace detail{
  template <typename>
  struct get_type;

  template <typename T>
  struct get_type< Graph<T> > {
    typedef T type;
  };

  template <typename T>
  struct get_type< WeightedEdge<T> > {
    typedef T type;
  };
}

namespace search{
  template <typename EdgeT> // will only work for WeightedEdge
  typename graph::detail::get_type<EdgeT>::
  type Dijkstra(Graph<EdgeT> G, size_t initial_vertex, size_t Y) {
    //static_assert(std::is_member_function_pointer<EdgeT::weight>,
    //	  "Is not weighted");
    typedef typename graph::detail::get_type<EdgeT>::type T;
    const T infinity = std::numeric_limits<T>::max();
    std::vector<T> distance(G.Order(),infinity);
    std::vector<bool> visited(G.Order(),false);
    distance[initial_vertex] = 0;
    visited[initial_vertex] = true;
    size_t current = initial_vertex;
    size_t other;
    T tmp_distance;
    std::vector<size_t> edges;
    for(auto count = 0; count < G.Order(); ++count) {
      // update distances
      edges = G.GetVertex(current).GetEdges();
      for (auto edge_index : edges) { // no need to check for direction
	EdgeT edge = G.GetEdge(edge_index);
	other = (edge.first() == current) ? edge.second() : edge.first();
	tmp_distance = distance[current] + edge.weight();
	if (tmp_distance < distance[other]) distance[other] = tmp_distance;
      }
      visited[current] = true;
      // break conditions
      if (visited[Y]) break; // are we at our destination?
      tmp_distance = infinity;

      for (size_t i = 0; i < visited.size(); ++i) {
	if (visited[i]) continue;
	if (distance[i] < tmp_distance) {
	  tmp_distance = distance[i];
	  current = i; // new current node is the unvisited node with the
	               // least distance
	}
      }
      if (tmp_distance == infinity) break; // all unvisited are never reached
    }
    return distance[Y];
  }
}

}

#endif//GRAPH_INCLUDED
