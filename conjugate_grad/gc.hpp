#include <Eigen/Dense>

template <typename vt>
struct CGResult {
    vt x;                 
    unsigned int iter;            
    double relative_residual;     
};

template <typename mt, typename vt, typename sc>
CGResult<vt> conjugate_gradient(
    const mt& A, 
    const vt& b, 
    const vt& x0,         
    unsigned int iter_max, 
    sc res_tol)
{
    vt x = x0;
    vt res = b - A * x;
    vt p = res;
    sc res_norm_0 = res.norm();
    unsigned int iter = 0;

    while (iter < iter_max && res.norm() > res_tol * res_norm_0)
    {
        vt Ap = A * p;
        sc pT_Ap = p.dot(Ap); // dot per il prodotto scalare
        const sc alpha_k = p.dot(res) / pT_Ap;
        x = x + alpha_k * p;
        vt res_new = b - A * x;
        const sc beta_k = Ap.dot(res_new) / pT_Ap;
        p = res_new - beta_k * p;

        res = res_new;
        iter++;
    }

    return {x, iter, res.norm() / res_norm_0};
}