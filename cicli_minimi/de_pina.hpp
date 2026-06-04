#pragma once
#include "unidirected_graph.hpp"
#include <vector>
#include "visit.hpp"

//cammino minimo tra due vettori (sfruttando djikstra già implementato)
std::vector<int> cam_min(int a, int b, const unidirected_graph<int>& grafo){
    std::vector<int> cammino;
    std::map<int, int> pred = dijkstra(grafo, a).second;
    int cur = b;
    cammino.push_back(cur);
    while(cur!=a)
        {
            cur = pred[cur];
            cammino.push_back(cur);
        }
    return cammino;}


//conversione nodo (la uso nel find_minimun_cycle per riconvertire i nodi da G' a G)
int conv_nodo(int nodo, int dimensione)
{
    if(nodo > dimensione){return nodo-dimensione;}
    return nodo;        
}


//find_minimu_cycle
std::vector<bool> find_minimun_cycle(const std::vector<bool>& S, const unidirected_graph<int>& grafo)
    {
        int N = grafo.all_nodes().size();
        int m = grafo.all_edges().size();
        unidirected_graph<int> grafo_primo;
        //costruzione di G'
        for (const auto& archi : grafo.all_edges()){
            int num = grafo.edge_number(archi);
            if(S[num]){
                grafo_primo.add_edge(archi.from(),archi.to()+N);
                grafo_primo.add_edge(archi.from()+N,archi.to());
                }
            else{
                grafo_primo.add_edge(archi.from(),archi.to());
                grafo_primo.add_edge(archi.from()+N,archi.to()+N);
                }
            }
            //vettori di incidenza e cammini minimi
            std::vector<std::vector<int>> C(N, std::vector<int>(m, 0));
            int i = 0;
            for(const auto& v : grafo.all_nodes())
            {
                std::vector<int> cammino = cam_min(v,v+N,grafo_primo);    
                for(int k = 0; k+1 < cammino.size(); k++)
                {
                    int nodo_a = conv_nodo(cammino[k],N);
                    int nodo_b = conv_nodo(cammino[k+1],N);
                    unidirected_edge<int> arco = unidirected_edge(nodo_a,nodo_b);
                    int indice = grafo.edge_number(arco);
                    C[i][indice] = (C[i][indice] +1 )% 2;
                }
                i +=1;
            }
            //trovo il miglior vettori di incidenza e poi lo converto in booleano
            int min_elementi = m+1;
            int miglior_indice = -1;
            for(int j=0; j<N; j++)
            {    int conteggio = 0;
                    for(int k=0; k<m; k++)
                    {
                        conteggio += C[j][k];    
                    }
                if (conteggio < min_elementi)
                {
                    min_elementi = conteggio;
                    miglior_indice = j;
                }    
            }
            std::vector<bool> risultato(C[miglior_indice].begin(), C[miglior_indice].end());
            return risultato;
}


//XOR(sto assumendo i vettori abbiano stessa dimensione ma non lo verifico nel programma)
std::vector<bool> XOR(const std::vector<bool>& v_1, const std::vector<bool>& v_2){
    int N = v_1.size();
    std::vector<bool> risultato(N);
    for(int i=0;i<N; i++){
        if(v_1[i] == v_2[i]){
            risultato[i] = false;
            }
        else risultato[i] = true;
        }
    return risultato;
    }
    
 
//dot_product
int dot_product(const std::vector<bool>& v_1, const std::vector<bool>& v_2){
    int N = v_1.size();
    int risultato = 0;
    for(int i=0;i<N; i++){    
        risultato += v_1[i]*v_2[i];    
            }
    return risultato % 2;
    }
    

//algoritmo di De Pina vero e proprio
std::vector<std::vector<bool>> cicli_depina(const unidirected_graph<int>& grafo, const unidirected_graph<int>& coalbero){
    int k = coalbero.all_edges().size();
    int m = grafo.all_edges().size();
    std::vector<std::vector<bool>> cicli(k, std::vector<bool>(m, false));
    //fase di inizializzazione dei vettori
    std::vector<std::vector<bool>> S(k, std::vector<bool>(m, false));
    int i = 0;
    for (const auto& e : coalbero.all_edges()){
        S[i][grafo.edge_number(unidirected_edge<int>(e.from(), e.to()))] = true;
        i++;
        }
    //implementazione del ciclo finale
    for(int i = 0; i <k; i++){
        std::vector<bool> C = find_minimun_cycle(S[i], grafo);
        cicli[i] = C;
        for(int j = i+1; j < k; j ++){
            if(dot_product(C,S[j])==1){
                S[j] = XOR(S[j],S[i]);
                }
            }
        }
    return cicli;
    }
 

//conversione de pina da booleano con gli archi a ciclo con i nodi

std::vector<std::vector<int>> cicli_depina_convertito(const std::vector<std::vector<bool>>& cicli, const unidirected_graph<int>& grafo){
	int N = cicli.size();
	int K = cicli[0].size();
	std::vector<std::vector<int>> risultato;
	
	for(int i = 0; i<N; i++)
	{
		std::vector<int> v;
		for(int j = 0; j < K; j++)
		{
			if(cicli[i][j])
			{
				unidirected_edge<int> arco = grafo.edge_at(j);
				v.push_back(arco.from());
				v.push_back(arco.to());	
			}	
		}
		risultato.push_back(v);	
		
	}
	
	
	return risultato;
	
	
	
	
	
	
	
	
	
	
	}