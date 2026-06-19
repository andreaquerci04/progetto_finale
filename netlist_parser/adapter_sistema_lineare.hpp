#pragma once

#include "circuit_components.hpp"
#include "../linear_system/matrices.hpp"
#include <vector>

namespace netlist {

inline std::vector<Resistori> converti_resistori_sistema(const CircuitoNetlist& circuito) {
    std::vector<Resistori> risultato;
    risultato.reserve(circuito.resistori.size());

    for (const auto& resistore : circuito.resistori) {
        risultato.push_back(
            Resistori{unidirected_edge<int>(resistore.nodo1, resistore.nodo2), resistore.valore_ohm}
        );
    }

    return risultato;
}


inline std::vector<Generatori> converti_generatori_sistema(const CircuitoNetlist& circuito) {
    std::vector<Generatori> risultato;
    risultato.reserve(circuito.generatori.size());

    for (const auto& generatore : circuito.generatori) {
        risultato.push_back(
            Generatori{generatore.polo_positivo, generatore.polo_negativo, generatore.valore_volt}
        );
    }

    return risultato;
}

} 
