#include <iostream>
#include <string>
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <iomanip> // per arrotondare il risultato 
#include "../conjugate_grad/gc.hpp"
#include "../cicli_minimi/visit.hpp"
#include "../cicli_minimi/dfs.hpp"
#include "../cicli_minimi/de_pina.hpp"
#include "../cicli_minimi/unidirected_edge.hpp"
#include "../cicli_minimi/unidirected_graph.hpp"
#include "../netlist_parser/netlist_parser.hpp"
#include "../netlist_parser/adapter_cicli.hpp"
#include "../netlist_parser/circuit_components.hpp"
#include "../netlist_parser/adapter_sistema_lineare.hpp"



double condA(const Eigen::MatrixXd& A)
{
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
    Eigen::VectorXd sv = svd.singularValues();
    return sv.maxCoeff() / sv.minCoeff();
}

int main(int argc, char* argv[])
{
    const std::string nome_file = (argc > 1) ? argv[1] : "../netlist_prova.txt";

    const netlist::CircuitoNetlist circuito = netlist::leggi_netlist_da_file(nome_file);
    const auto G1 = netlist::costruisci_grafo_cicli(circuito);
    auto resistori = converti_resistori_sistema(circuito);
    auto generatori =  converti_generatori_sistema(circuito);

    unidirected_graph<int> albero = recursive_dfs(G1,1);	
	unidirected_graph<int> coalbero = G1 - albero;
	
	std::vector<std::vector<int>> cicli_1 = cicli_dfs(albero,coalbero);
	std::vector<std::vector<int>> cicli = cicli_depina(G1,coalbero);
    for (auto& singolo_ciclo : cicli) {
    singolo_ciclo.push_back(singolo_ciclo.front());
    }

    // costruzione matrici
    Eigen::MatrixXd B = Matrice_B(cicli, resistori);
    Eigen::MatrixXd R = Matrice_R(resistori);
    Eigen::VectorXd v = Vettore_v(cicli, generatori);

    // sistema lineare A * i_maglia = v
    Eigen::MatrixXd A = B.transpose() * R * B;

    // check: A deve essere invertibile
    const double tol = 1.0e-15;
    if (abs(A.determinant()) < tol) {
        return -1;
    }

    // check: A deve essere simmetrica
    if (A(0, 1) != A(1, 0)){
        return -1;
    }

    std::cout << "Numero di condizionamento di A: " << condA(A) << "\n\n";

    // risoluzione con gradiente coniugato
    const unsigned int iter_max = 10000;
    const double res_tol = 1.0e-12;

    Eigen::VectorXd x0 = Eigen::VectorXd::Zero(cicli.size());
    auto result = conjugate_gradient(A, v, x0, iter_max, res_tol);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Iterazioni (it / it_max): "
              << result.iter << " / " << iter_max << "\n";
    std::cout << "Residuo relativo (res / tol): "
              << result.relative_residual << " / " << res_tol << "\n\n";

    // result.x contiene le correnti di maglia i_1, i_2, ...
    std::cout << "Correnti di maglia:\n" << result.x << "\n\n";

    // calcolo tensioni sui resistori: v_R = R * B * i
    Eigen::VectorXd tensioni = R * B * result.x;
    std::cout << "Tensioni e correnti sui resistori:\n";
for (int k = 0; k < (int)resistori.size(); k++) {
    double V = tensioni(k);
    double I = V / resistori[k].valore_ohm;
    std::cout << "R" << k+1 
              << ": V = " << V << " volts"
              << ", I = " << I << " amps.\n";
}

    return 0;
}