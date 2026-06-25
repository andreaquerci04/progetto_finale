# Parser della netlist

Questa cartella contiene la parte relativa alla lettura della netlist.

## File principali

- `circuit_components.hpp`: definisce una rappresentazione del circuito letto dal file.
- `netlist_parser.hpp`: legge la netlist e riempie `CircuitoNetlist`.
- `adapter_cicli.hpp`: costruisce il grafo compatibile con la parte `cicli_minimi`.
- `adapter_sistema_lineare.hpp`: converte resistori e generatori nel formato usato da `linear_system/matrices.hpp`.
- `parser_demo.cpp`: demo compilabile senza Eigen.

## Come funziona

Il parser produce prima una rappresentazione neutra:

```cpp
netlist::CircuitoNetlist circuito = netlist::leggi_netlist_da_file("netlist_prova.txt");
```

Da questa struttura si possono ottenere:

```cpp
// Input per la parte 5.2
unidirected_graph<int> G = netlist::costruisci_grafo_cicli(circuito);

// Input per la parte 5.3
std::vector<Resistori> resistori = netlist::converti_resistori_sistema(circuito);
std::vector<Generatori> generatori = netlist::converti_generatori_sistema(circuito);
```

L'ordine dei nodi dei generatori non viene perso: viene conservato nei campi `polo_positivo` e `polo_negativo`.

Si può testare il codice di netlist_parser usando ./parser_demo ../netlist_prova.txt (o anche solo ./parser_demo) dopo aver usato i comandi cmake/make nella build
