#include <iostream>
#include "visit.hpp"
#include "dfs.hpp"
#include "de_pina.hpp"




template <typename T>
void print(const std::vector<std::vector<T>>& v){
	int N = v.size();
	
	for (int i = 0; i<N; i++){
		int k = v[i].size();
		
		for(int j = 0; j <k; j++)
		{
			std::cout<< v[i][j] << " ";
		}
		std::cout << "\n";
		}
	
	
	}

int main(){
	unidirected_graph<int> G1;
    G1.add_edge(1,3);
    G1.add_edge(1,2);
    G1.add_edge(1,4);
    G1.add_edge(1,6);
    G1.add_edge(3,6);
    G1.add_edge(6,8);
    G1.add_edge(2,4);
    G1.add_edge(2,5);
    G1.add_edge(2,7);
    G1.add_edge(4,6);
    G1.add_edge(4,7);
    G1.add_edge(5,7);
    G1.add_edge(7,9);
    G1.add_edge(9,8);
    G1.add_edge(6,8);
    G1.add_edge(7,6);    
	
	
	G1.write_to_dot("grafo.dot");
	

	
	
	unidirected_graph<int> albero = recursive_dfs(G1,1);	
	unidirected_graph<int> coalbero = G1 - albero;
	
	std::vector<std::vector<int>> cicli_1 = cicli_dfs(albero,coalbero);
	std::vector<std::vector<int>> cicli_2 = cicli_depina(G1,coalbero);
	

	std::cout<< "cicli con dfs" << "\n";
	print(cicli_1);
	
	
	std::cout << "\n" << "cicli con de pina" <<"\n";
	print(cicli_2);

	return 0;
	}