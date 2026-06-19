#pragma once

#include <string>
#include <vector>

namespace netlist {

struct Resistore {
    std::string nome;     
    double valore_ohm;    
    int nodo1;            
    int nodo2;            
};


struct Generatore {
    std::string nome;       
    double valore_volt;     
    int polo_positivo;      
    int polo_negativo;      
};


struct CircuitoNetlist {
    std::vector<Resistore> resistori;
    std::vector<Generatore> generatori;
};

} 
