#pragma once


template <typename T>
class unidirected_edge{
private:
	T nodo_from;
	T nodo_to;

public:
	
	unidirected_edge(T a, T b) {
		if (a<b) { 
			nodo_from = a;
			nodo_to = b; 
			}
		else {
			nodo_from = b;
			nodo_to = a;
			
			}
		}
	T from() const{return nodo_from; }
	T to() const{return nodo_to; }
	
	
	bool operator==(const unidirected_edge& other) const{ 
		return (nodo_from == other.nodo_from && nodo_to == other.nodo_to);
		}
	
	bool operator<(const unidirected_edge& other) const{
		if (nodo_from < other.nodo_from) {return true;}
		else if(nodo_from == other.nodo_from && nodo_to < other.nodo_to) {return true;}
		else{return false;}
		
		}
	
		
	};