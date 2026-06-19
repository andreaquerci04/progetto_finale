#include "netlist_parser.hpp"
#include "adapter_cicli.hpp"
#include <exception>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    const std::string nome_file = (argc > 1) ? argv[1] : "netlist_prova.txt";

    try {
        const netlist::CircuitoNetlist circuito = netlist::leggi_netlist_da_file(nome_file);
        const auto grafo = netlist::costruisci_grafo_cicli(circuito);

        std::cout << "Netlist letta correttamente\n";
        std::cout << "Resistori: " << circuito.resistori.size() << "\n";
        std::cout << "Generatori: " << circuito.generatori.size() << "\n";
        std::cout << "Nodi nel grafo: " << grafo.all_nodes().size() << "\n";
        std::cout << "Archi nel grafo: " << grafo.all_edges().size() << "\n\n";

        std::cout << "Resistori letti:\n";
        for (const auto& r : circuito.resistori) {
            std::cout << "  " << r.nome
                      << " valore=" << r.valore_ohm
                      << " nodi=(" << r.nodo1 << "," << r.nodo2 << ")\n";
        }

        std::cout << "\nGeneratori letti:\n";
        for (const auto& g : circuito.generatori) {
            std::cout << "  " << g.nome
                      << " valore=" << g.valore_volt
                      << " +=" << g.polo_positivo
                      << " -=" << g.polo_negativo << "\n";
        }

        std::cout << "\nArchi del grafo:\n";
        for (const auto& arco : grafo.all_edges()) {
            std::cout << "  (" << arco.from() << "," << arco.to() << ")\n";
        }
    } catch (const std::exception& errore) {
        std::cerr << "Errore parser netlist: " << errore.what() << "\n";
        return 1;
    }

    return 0;
}
