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

Si può testare il codice di netlist_parser usando ./parser_demo ../netlist_prova.txt dopo aver usato i comandi cmake/make nella build

## Cose da verificare prima di unire tutto


Prima di creare una versione unica del progetto, ci sono alcune cose da controllare.

La prima è che nel progetto al momento ci sono due versioni diverse di unidirected_edge e unidirected_graph: una nella parte cicli_minimi e una nella parte linear_system. Hanno lo stesso nome, quindi, se in un unico file includiamo entrambe le parti senza sistemare prima questa cosa, potremmo avere errori o conflitti tra tipi.

La seconda cosa riguarda i cicli prodotti dalla parte 5.2. La parte del sistema lineare si aspetta cicli “chiusi”, cioè scritti così:

{1, 2, 4, 1}


Quindi, prima della consegna finale, secondo me dobbiamo controllare queste cose:

decidere se usare una sola versione comune di unidirected_edge / unidirected_graph, oppure tenere gli adattatori;
verificare che la parte 5.2 restituisca cicli chiusi (ho testato il codice e l'output del file prova mi dà cicli aperti, però non ho analizzato il codice nel dettaglio).

Per ora gli adattatori adapter_cicli.hpp e adapter_sistema_lineare.hpp servono proprio a evitare di rompere le parti già fatte. Quando uniremo tutto, potremo decidere se tenerli oppure usare strutture dati comuni.

Compilando con queste opzioni -Wall -Wextra -Wshadow -Werror=vla nel cmakelists mi dà un po' di warning nella parte di ../cicli_minimi/unidirected_graph.hpp (non sono errori, funziona, però meglio buttarci un occhio)