#include <iostream>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <vector>

template <typename T>
class unidirected_edge {
private:
    T _from;
    T _to;

public:
    unidirected_edge(T n1, T n2) {
        _from = std::min(n1, n2);
        _to   = std::max(n1, n2);
    }

    T from() const {
        return _from;
    }

    T to() const {
        return _to;
    }

    bool operator<(const unidirected_edge<T>& other) const{
        if (_from != other._from){
            return _from < other._from;
        }
        return _to < other._to;
    }

    bool operator==(const unidirected_edge<T>& other) const{
        return (_from == other._from) && (_to == other._to);
    }
};

template <typename T>
std::ostream&
operator<<(std::ostream& os, const unidirected_edge<T>& edge)
{
    os << "(" <<edge.from()<< "," << edge.to() << ")" << "\n";
    return os;
}

template <typename T>
class unidirected_graph{
    private: 
        std::unordered_map<T, std::set<T>> ad_list;
        std::vector<unidirected_edge<T>> edges;
        std::set<T> nodes;
        std::map<unidirected_edge<T>, int> edge_to_id;

    public:
        unidirected_graph() = default;
        unidirected_graph(const unidirected_graph<T>& other) = default;

        void add_edge(T n1, T n2){

        if (n1 == n2){
            return;
        }

        unidirected_edge<T> new_edge(n1,n2);
        if (edge_to_id.count(new_edge) > 0){
            return;
        }

        ad_list[n1].insert(n2);
        ad_list[n2].insert(n1);

        int id = edges.size();
        edge_to_id[new_edge] = id;

        edges.push_back(new_edge);

        if (nodes.count(n1) == 0){
            nodes.insert(n1);
        }

        if (nodes.count(n2) == 0){
            nodes.insert(n2);
            }
        }


        std::set<T> neighbours(const T& node){
            if (ad_list.count(node) == 0){
                return std::set<T>();
            }
            else {
                return ad_list[node];
            }
        }

        std::vector<unidirected_edge<T>> all_edges() const{
            return edges;
        }

        std::set<T> all_nodes() const{
            return nodes;
        }

        int edge_number(const unidirected_edge<T>& arco){
            return edge_to_id[arco];
        }

        unidirected_edge<T> edge_at(const int& id){
            return edges[id];
        }

        unidirected_graph<T> operator-(const unidirected_graph<T>& other){
            unidirected_graph<T> G_ris;
            for (int i = 0; i < edges.size(); i++){
                if (other.edge_to_id.count(edges[i]) == 0){
                    G_ris.add_edge(edges[i].from(), edges[i].to());
                }
           }

           return G_ris;
        }
};


