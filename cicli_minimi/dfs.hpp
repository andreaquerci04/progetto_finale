#pragma once
#include "unidirected_graph.hpp"
#include <map>
#include <vector>
#include <unordered_set>

//funziona ricorsiva per trovare il percorso tra due nodi
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



//questa parte serve solo a creare la "struttura"
std::vector<int> findpath(const unidirected_graph<int>& albero, int a, int b){
	std::unordered_set<int> visitato;
	std::vector<int> path;
	recursive_findpath(albero,a,b,visitato,path);
	
	return path;
	}



//Cicli fondamentali con DFS
std::vector<std::vector<int>> cicli_dfs(const unidirected_graph<int>& albero, const unidirected_graph<int>& coalbero){
	std::vector<std::vector<int>> cicli;
	
	for(const auto& arco : coalbero.all_edges())
	{
		
			if(arco.from()<arco.to()){	
				cicli.push_back(findpath(albero,arco.from(),arco.to()));
				}		
	}
	return cicli;
	}
	



