// FILE PER DEFINIRE LE MATRICI
// si assume che i dati di input siano: 
// std::vector<std::vector<int>> cicli = [{1, 2, 3, 1}, {4, 3, 2, 4}]
// std::vector<(struct)Resistori> resistori = [ {(1, 2), 10.0}, {(2, 4), 20.0}, {(2, 3), 30.0} ]
// std::vector<(struct)Generatori> generatori = [ {1, 3, 10.0}, {4, 3, 20.0}]

#include <vector>
#include <iostream>
#include <Eigen/Dense>
#include <algorithm>

// MIO FILE .HPP PER LEGGERE GLI ARCHI (assumendo che le resistenze saranno definite come edges)
template <typename T>
class unidirected_edge {
private:
    T _from;
    T _to;

public:
    unidirected_edge(T n1, T n2) {
        _from = std::min(n1, n2);
        _to   = std::max(n1, n2);
    }

    T from() const {
        return _from;
    }

    T to() const {
        return _to;
    }

    bool operator<(const unidirected_edge<T>& other) const{
        if (_from != other._from){
            return _from < other._from;
        }
        return _to < other._to;
    }

    bool operator==(const unidirected_edge<T>& other) const{
        return (_from == other._from) && (_to == other._to);
    }
};
// ----------------------------------------------------------

// definiti per provare il codice
struct Generatori {
    int polo_positivo;
    int polo_negativo;
    double valore_volt;
};

struct Resistori {
    unidirected_edge<int> arco_R;
    double valore_ohm;
};
// saranno passati direttamente dalle altre parti di codice
//------------------------------------------------------------------------------------


// FUNZIONE che prende tutti i nodi percorsi e li trasforma in archi (pairs)
// non ho voluto usare la funzione add_edge(), che avevo creato in precedenza per le esercitazioni sui grafi poiché essa restituiva
// altri risultati (come ad esempio la lista di adiacenza) che ai fini della risoluzione del sistema lineare sarebbero solo memoria 
// occupata senza motivo
struct ArchiCiclo {
    std::vector<std::pair<int,int>> vettore_archi;
    std::vector<std::pair<int,int>> vettore_archi_inv;
};

ArchiCiclo get_archi_ciclo(const std::vector<int>& ciclo) {
    ArchiCiclo result;
    for (int i = 1; i < (int)ciclo.size(); i++) {
        result.vettore_archi.push_back({ciclo[i-1], ciclo[i]});
        result.vettore_archi_inv.push_back({ciclo[i], ciclo[i-1]});
    }
    return result;
}


//MATRICE B
//------------------------------------------------------------------------------------
Eigen::MatrixXd Matrice_B(const std::vector<std::vector<int>>& cicli, const std::vector<Resistori>& resistori){
    int m = resistori.size();
    int n = cicli.size();
    
    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(m, n); //matrice 3x2 in questo esempio

    for (int j = 0; j < n; j++) {
        const auto& ciclo = cicli[j]; // siamo nel primo ciclo // secondo ciclo

        auto [vettore_archi, vettore_archi_inv] = get_archi_ciclo(ciclo); // ho messo il codice in una funzione perché mi serve anche dopo

        for (int k = 0; k < m; k++) {
            const auto& resistenza = resistori[k];

            std::pair<int,int> R_to_find = {resistenza.arco_R.from(), resistenza.arco_R.to()};
            
            if (std::find(vettore_archi.begin(), vettore_archi.end(), R_to_find) != vettore_archi.end())
            { // !!! SAPERE BENE COSA FA QUESTO IF ESATTAMENTE
                B(k, j) = 1.0;
            }
            
            else if (std::find(vettore_archi_inv.begin(), vettore_archi_inv.end(), R_to_find) != vettore_archi_inv.end())
            {
                B(k, j) = -1.0;
            }  
        }
    }

    return B;
}
//------------------------------------------------------------------------------------


//MATRICE R
//------------------------------------------------------------------------------------
Eigen::MatrixXd Matrice_R(const std::vector<Resistori>& resistori){
    int n = resistori.size();

    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(n, n); //matrice 3x3 in questo esempio

    for (int k = 0; k < n; k++){
        const auto& R_ohm = resistori[k].valore_ohm;
        R(k,k) = R_ohm;
    }

    return R;
}
//------------------------------------------------------------------------------------


//VETTORE v
//------------------------------------------------------------------------------------
Eigen::VectorXd Vettore_v(const std::vector<std::vector<int>>& cicli, const std::vector<Generatori>& generatori){
    int n = cicli.size(); //prima l'errore era generatori.size()
    Eigen::VectorXd v = Eigen::VectorXd::Zero(n);

    for (int j = 0; j < n; j++){

        const auto& ciclo = cicli[j];
        auto [vettore_archi, vettore_archi_inv] = get_archi_ciclo(ciclo);
        
        for (int k = 0; k < (int)generatori.size(); k++){ //dentro al primo generatore
            const auto& generatore = generatori[k];

            std::pair<int,int> V_to_find = {generatore.polo_positivo, generatore.polo_negativo}; //prendiamo polo positivo e negativo

            
            if (std::find(vettore_archi.begin(), vettore_archi.end(), V_to_find) != vettore_archi.end())
            {
                v(j) -= generatore.valore_volt;  //segno positivo
            }

            else if (std::find(vettore_archi_inv.begin(), vettore_archi_inv.end(), V_to_find) != vettore_archi_inv.end())
            {
                v(j) += generatore.valore_volt;  //segno negativo
            }
        }
    }
    
    return v;    
}
//------------------------------------------------------------------------------------
//appunti: ho riscontrato un problema con il sistema per fillare il vettore v. Se ci sono più maglie che generatori il vettore v
//avrà dimensione minore rispetto alla matrice A, rendendo il sistema lineare irrisolvibile.

//si è rivelato più semplice: basta iterare sulla dimensione dei cicli invece dei generatori

//altro problema: il vettore dei termini noti veniva riempito ad ogni componente v(k). In questo modo si otteneva un vettore sbagliato
//poiché si andava a mettere un generatore in una maglia potenzialmente composta solo da resistenze. 

