#pragma once
#include "unidirected_edge.hpp"
#include <map>
#include <list>
#include <set>
#include <fstream>
#include <string>

template <typename T>
class unidirected_graph{
	private:
	std::map<T, std::set<T>> lista_adiacenza;
	std::set<unidirected_edge<T>> archi;
	std::map<int, unidirected_edge<T>> dizionario_archi;
	std::map<unidirected_edge<T>, int > dizionario_archi_inverso;
	std::set<T> nodi;
	int max_keys;
	public:
	
	//Costruttore di Defaul
	unidirected_graph() : max_keys(-1) {}
	
	
	//Costruttore di Copia
	unidirected_graph( const unidirected_graph& copia)
		: lista_adiacenza(copia.lista_adiacenza),
			archi(copia.archi),
			dizionario_archi(copia.dizionario_archi),
			dizionario_archi_inverso(copia.dizionario_archi_inverso),
			nodi(copia.nodi),

			max_keys(copia.max_keys)
	{}
	
	
	//METODO neighbor
	const std::set<T>& neighbor(const T& nodo) const
	{return lista_adiacenza.at(nodo);}//uso il metodo at per evitare di creare nodi quando non esistono



	// METODO per aggiungere archi 
	void add_edge(const T& a, const T& b)
	{	//Aggiorno la lista di adiacenza
		lista_adiacenza[a].insert(b);
		lista_adiacenza[b].insert(a);
		
		//aggiorno set dei nodi
		nodi.insert(a);
		nodi.insert(b);
		
		
		//aggiorno dizionario degli archi
		if(!archi.contains(unidirected_edge<T>(a,b))) {
    		max_keys += 1;
    		dizionario_archi.insert({max_keys, unidirected_edge<T>(a,b)});
   			dizionario_archi_inverso.insert({unidirected_edge<T>(a,b), max_keys});
		}
		
		//aggiorno set degli archi
		archi.insert(unidirected_edge<T>(a,b));
		
		
		return;}

	//METODO per avere gli archi
	const std::set<unidirected_edge<T>>& all_edges() const{
		return archi;
		}
	
	//METODO per avere i nodi
	const std::set<T>& all_nodes() const{return nodi;}
	
	//METODO per avere l'arco dato il numero
	const unidirected_edge<T>& edge_at(int indice) const{return dizionario_archi.at(indice);}
	
	
	//METODO per avere il numero dato l'arco
	const int edge_number(const unidirected_edge<T>& arco) const{return dizionario_archi_inverso.at(arco);}
	
	
	//SOTTRAZIONE
	//Metodo per rimuovere archi
	void remove_edge(const T& a, const T& b)
	{	
		unidirected_edge<T> arco = unidirected_edge<T>(a,b);
		if (!archi.contains(arco)) {return;}
		
		//Aggiorno la lista di adiacenza
		lista_adiacenza[a].erase(b);  //erase funziona anche se non esiste l'elemento
		lista_adiacenza[b].erase(a);
				
		//aggiorno dizionario degli archi
		dizionario_archi.erase(edge_number(arco));
		dizionario_archi_inverso.erase(arco);
		
		
		//aggiorno set degli archi
		archi.erase(arco);
		
		
		return;}

	//Decremento
	unidirected_graph<T>& operator-=(const unidirected_graph<T>& grafo) 
	{ for( const auto& a: grafo.archi) {remove_edge(a.from(),a.to() );}
	
	return *this;
	}
	
	
	//Sottrazione
	unidirected_graph<T> operator-(const unidirected_graph<T>& grafo) 
	const{ 
			unidirected_graph<T> gr = *this;
			gr -= grafo;
			return gr;
		}
	
// il metodo per la stampa del grafo non era parte dell'esercitazione quindi ho fatto generare il codice dall'intelligenza artificiale solo per fare un controllo grafico del mio risulttao

	void write_to_dot(const std::string& filename) const {
		std::ofstream out(filename);
		if (!out) return;
	
		out << "graph G {\n";
		out << "    node [fontname=\"Arial\"];\n";
		
		// Scriviamo i nodi isolati (se presenti) o tutti i nodi
		for (const auto& nodo : nodi) {
			out << "    \"" << nodo << "\";\n";
		}
	
		// Scriviamo gli archi
		for (const auto& arco : archi) {
			out << "    \"" << arco.from() << "\" -- \"" << arco.to() << "\";\n";
		}
	
		out << "}\n";
		out.close();
	}	


};