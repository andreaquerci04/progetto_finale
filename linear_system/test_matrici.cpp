#include <vector>
#include <iostream>
#include <Eigen/Dense>
#include <algorithm>
#include "matrices.hpp"

int main(void){
    std::vector<std::vector<int>> cicli = {
    {1, 2, 4, 1},   // C1
    {2, 1, 3, 2},   // C2
    {2, 3, 5, 2}    // C3
    };

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

    auto B = Matrice_B(cicli, resistori);
    auto R = Matrice_R(resistori);
    auto v = Vettore_v(cicli, generatori);
    auto A = B.transpose() * R * B;

    std::cout << "Matrice dei versi di percorrenza: \n" << B << "\n\n";
    std::cout << "Matrice diagonale delle resistenze: \n" << R << "\n\n";
    std::cout << "Vettore dei generatori con segno: \n" << v << "\n\n";
    std::cout << "Matrice B^t * R * B: \n" << A << "\n\n";

}