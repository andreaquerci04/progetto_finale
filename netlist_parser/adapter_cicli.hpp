#pragma once

#include "circuit_components.hpp"
#include "../cicli_minimi/unidirected_graph.hpp"

namespace netlist {
inline unidirected_graph<int> costruisci_grafo_cicli(const CircuitoNetlist& circuito) {
    unidirected_graph<int> grafo;

    for (const auto& resistore : circuito.resistori) {
        grafo.add_edge(resistore.nodo1, resistore.nodo2);
    }

    for (const auto& generatore : circuito.generatori) {
        grafo.add_edge(generatore.polo_positivo, generatore.polo_negativo);
    }

    return grafo;
}

}
