#include <iostream>
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <iomanip> // per arrotondare il risultato 
#include "../linear_system/gc.hpp"
#include "../linear_system/matrices.hpp"
#include "../cicli_minimi/visit.hpp"
#include "../cicli_minimi/dfs.hpp"
#include "../cicli_minimi/de_pina.hpp"
#include "../cicli_minimi/unidirected_edge.hpp"
#include "../cicli_minimi/unidirected_graph.hpp"


double condA(const Eigen::MatrixXd& A)
{
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
    Eigen::VectorXd sv = svd.singularValues();
    return sv.maxCoeff() / sv.minCoeff();
}

int main(void)
{
    unidirected_graph<int> G1;
    G1.add_edge(1,3);
    G1.add_edge(1,2);
    G1.add_edge(1,4);
    G1.add_edge(1,6);
    G1.add_edge(3,5);
    G1.add_edge(2,3);
    G1.add_edge(2,4);
    G1.add_edge(2,5);


    unidirected_graph<int> albero = recursive_dfs(G1,1);	
	unidirected_graph<int> coalbero = G1 - albero;
	
	std::vector<std::vector<int>> cicli_1 = cicli_dfs(albero,coalbero);
	std::vector<std::vector<int>> cicli = cicli_depina(G1,coalbero);
    for (auto& singolo_ciclo : cicli) {
    singolo_ciclo.push_back(singolo_ciclo.front());
    }

    // --- input (assumendo che i dati siano di questo tipo) ---
    std::vector<Resistori> resistori = {
        { unidirected_edge<int>(2, 4), 4.0  },  // R1
        { unidirected_edge<int>(1, 2), 10.0 },  // R2
        { unidirected_edge<int>(1, 3), 30.0 },  // R3
        { unidirected_edge<int>(2, 3), 10.0 },  // R4
        { unidirected_edge<int>(2, 5), 4.0  }   // R5
};

    std::vector<Generatori> generatori = {
        {1, 4, 30.0},  // V1: polo+ sul nodo 1, polo- sul nodo 4
        {3, 5, 40.0}   // V2: polo+ sul nodo 3, polo- sul nodo 5
    };
    // ---------------------------------------------------------------

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