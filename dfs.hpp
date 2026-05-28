#include "unidirected_graph.hpp"
#include <map>
#include <vector>
#include <unordered_set>

//questa prima parte di codice è solo una copia della dfs ricorsiva dell'esercitazione 9
template <typename T>	
unidirected_graph<T> recursive_dfs(const unidirected_graph<T>& grafo, const T& nodo_iniziale){
	
	unidirected_graph<T> albero_visita;
	std::set<T> nodi_non_visitati = grafo.all_nodes();
	
	DFS_visit(grafo, nodo_iniziale, albero_visita, nodi_non_visitati);
	
	return albero_visita;}
	

template <typename T>
void DFS_visit(const unidirected_graph<T>& grafo, const T& nodo, unidirected_graph<T>& albero_visita, std::set<T>& nodi_non_visitati) {
	
	nodi_non_visitati.extract(nodo);
	
	for(const auto& w : grafo.neighbor(nodo))
	{
		if(nodi_non_visitati.contains(w)) {
			albero_visita.add_edge(nodo,w);
			DFS_visit(grafo,w,albero_visita,nodi_non_visitati);
			}
		}
	
	}
	

//Cicli fondamentali con DFS
std::vector<std::vector<int>> cicli_dfs(const unidirected_graph<int>& albero, const unidirected_graph<int>& coalbero){
	std::vector<std::vector<int>> cicli;
	
	for(const auto& [u,v] : coalbero.all_edges())
	{
			if(u<v){	
				cicli.push_back(findpath(albero,u,v));
				}		
	}
	return cicli;
	}
	

//funziona ricorsiva per trovare il percorso tra due nodi
//questa prima parte serve solo a creare la "struttura"
std::vector<int> findpath(const unidirected_graph<int>& albero, int a, int b){
	std::unordered_set<int> visitato;
	std::vector<int> path;
	recursive_findpath(albero,a,b,visitato,path);
	
	return path;
	}

//questa è la vera e propria parte ricorsiva
bool recursive_findpath(const unidirected_graph<int>& albero, int u, int v, std::unordered_set<int>& visited,
	std::vector<int>& path){
		
		visited.insert(u);
		path.push_back(u);
		
		if (u==v) return true;
		
		for(const auto&n : albero.neighbor(u))
			{
			if(!visited.contains(n)){
				if(recursive_findpath(albero,n,v,visited,path)) return true;
				}
			}
		path.pop_back();
		return false;}


