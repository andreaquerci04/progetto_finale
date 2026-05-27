#include <iostream>
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <string>
#include "gc.hpp"

double condA(const Eigen::MatrixXd& A)
{
  Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
  Eigen::VectorXd singularValuesA = svd.singularValues();
  return singularValuesA.maxCoeff() / singularValuesA.minCoeff();
}

int main(int argc, char **argv) 
{
    const double tol = 1.0e-15;
    unsigned int n = 3; 

    if (argc > 1) {
      n = std::stoi(argv[1]);   
    }

    Eigen::MatrixXd B = Eigen::MatrixXd::Random(n, n);  

    if (abs(B.determinant()) < tol)
      return -1;    

    Eigen::MatrixXd A = B.transpose() * B;
    Eigen::VectorXd x_ex = Eigen::VectorXd::Ones(n);    
    Eigen::VectorXd b = A * x_ex;   
    Eigen::VectorXd x = Eigen::VectorXd::Zero(n);

    Eigen::VectorXd res = b - A * x; 

    Eigen::VectorXd p = res;       

    double res_norm_0 = res.norm(); 
    const unsigned int iter_max = 10000;
    const double res_tol = 1.0e-12; 

    std::cout.precision(2);
    std::cout << std::scientific << "Matrix Cond: " << condA(A) << std::endl;   

    Eigen::VectorXd x0 = Eigen::VectorXd::Zero(n);  

    auto result = conjugate_gradient(A, b, x0, iter_max, res_tol);

    const auto err_rel = (x_ex.norm() == 0.0) ? (result.x - x_ex).norm() :  
                                                (result.x - x_ex).norm() / x_ex.norm();


    std::cout.precision(4);
    std::cout << std::scientific << "Iterazioni (it / it_max): " 
              << result.iter << " / " << iter_max << "\n";
              
    std::cout << std::scientific << "Residuo (res / res_tol):  " 
              << result.relative_residual << " / " << res_tol << "\n";
              
    std::cout << std::scientific << "Errore relativo (err_rel): " 
              << err_rel << "\n\n";
              
    std::cout << "Soluzione:\n" << result.x << std::endl;

  return 0;
}