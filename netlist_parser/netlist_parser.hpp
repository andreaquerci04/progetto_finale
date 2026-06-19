#pragma once

#include "circuit_components.hpp"
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace netlist {

inline std::string rimuovi_commento(const std::string& riga) {
    const std::size_t posizione_commento = riga.find('#');
    if (posizione_commento == std::string::npos) {
        return riga;
    }
    return riga.substr(0, posizione_commento);
}

inline bool riga_vuota_o_spazi(const std::string& riga) {
    for (char carattere : riga) {
        if (!std::isspace(static_cast<unsigned char>(carattere))) {
            return false;
        }
    }
    return true;
}

inline char tipo_componente(const std::string& nome_componente) {
    if (nome_componente.empty()) {
        throw std::runtime_error("Nome componente vuoto nella netlist");
    }

    const char tipo = static_cast<char>(
        std::toupper(static_cast<unsigned char>(nome_componente[0]))
    );

    if (tipo != 'R' && tipo != 'V') {
        throw std::runtime_error(
            "Tipo componente non valido: " + nome_componente +
            ". Sono ammessi solo componenti R e V."
        );
    }

    return tipo;
}

inline void controlla_nome_componente(const std::string& nome_componente, int numero_riga) {
    if (nome_componente.size() < 2) {
        throw std::runtime_error(
            "Nome componente non valido alla riga " + std::to_string(numero_riga) +
            ". Esempi validi: R1, R2, V1."
        );
    }


    for (std::size_t i = 1; i < nome_componente.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(nome_componente[i]))) {
            throw std::runtime_error(
                "Nome componente non valido alla riga " + std::to_string(numero_riga) +
                ": " + nome_componente +
                ". Dopo R o V e' atteso un numero progressivo."
            );
        }
    }
}

inline void aggiungi_componente_da_riga(
    const std::string& riga_originale,
    CircuitoNetlist& circuito,
    int numero_riga
) {
    const std::string riga = rimuovi_commento(riga_originale);

    if (riga_vuota_o_spazi(riga)) {
        return;
    }

    std::istringstream input(riga);

    std::string nome;
    double valore = 0.0;
    int nodo1 = 0;
    int nodo2 = 0;

    if (!(input >> nome >> valore >> nodo1 >> nodo2)) {
        throw std::runtime_error(
            "Formato non valido alla riga " + std::to_string(numero_riga) +
            ". Formato atteso: <nome> <valore> <nodo1> <nodo2>."
        );
    }

    std::string token_extra;
    if (input >> token_extra) {
        throw std::runtime_error(
            "Troppi campi alla riga " + std::to_string(numero_riga) +
            ". Formato atteso: <nome> <valore> <nodo1> <nodo2>."
        );
    }

    controlla_nome_componente(nome, numero_riga);
    const char tipo = tipo_componente(nome);

    if (nodo1 <= 0 || nodo2 <= 0) {
        throw std::runtime_error(
            "Nodi non validi alla riga " + std::to_string(numero_riga) +
            ". I nodi devono essere interi positivi."
        );
    }

    if (nodo1 == nodo2) {
        throw std::runtime_error(
            "Componente " + nome + " alla riga " + std::to_string(numero_riga) +
            " collegato due volte allo stesso nodo."
        );
    }

    if (tipo == 'R' && valore <= 0.0) {
        throw std::runtime_error(
            "Resistenza non valida alla riga " + std::to_string(numero_riga) +
            ". Il valore di una resistenza deve essere positivo."
        );
    }

    if (tipo == 'R') {
        circuito.resistori.push_back(Resistore{nome, valore, nodo1, nodo2});
    } else { // tipo == 'V'
        circuito.generatori.push_back(Generatore{nome, valore, nodo1, nodo2});
    }
}

inline CircuitoNetlist leggi_netlist_da_stream(std::istream& input) {
    CircuitoNetlist circuito;
    std::string riga;
    int numero_riga = 0;

    while (std::getline(input, riga)) {
        ++numero_riga;
        aggiungi_componente_da_riga(riga, circuito, numero_riga);
    }

    return circuito;
}

inline CircuitoNetlist leggi_netlist_da_file(const std::string& nome_file) {
    std::ifstream file(nome_file);
    if (!file) {
        throw std::runtime_error("Impossibile aprire il file netlist: " + nome_file);
    }

    return leggi_netlist_da_stream(file);
}

} 
