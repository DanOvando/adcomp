#include <TMB.hpp>

template<class Type>
Type objective_function<Type>::operator() ()
{
  using namespace R_inla;
  using namespace density;
  using namespace Eigen;

  DATA_INTEGER(flag); // flag=0 => only prior
  DATA_VECTOR(time);
  DATA_IVECTOR(notcens);
  DATA_IVECTOR(meshidxloc);
  DATA_MATRIX(X);    
  DATA_STRUCT(spde,spde_aniso_t);
  PARAMETER_VECTOR(beta);      
  PARAMETER(log_tau);
  PARAMETER(log_kappa);
  PARAMETER_VECTOR(ln_H_input);
  PARAMETER(log_alpha);  
  PARAMETER_VECTOR(x);  
  
  Type tau = exp(log_tau);
  Type kappa = exp(log_kappa);
  Type alpha = exp(log_alpha);

  Type nll = 0.0;

  // Need to parameterize H matrix such that
  matrix<Type> H(2,2);
  H(0,0) = exp(ln_H_input(0));
  H(1,0) = ln_H_input(1);
  H(0,1) = ln_H_input(1);
  H(1,1) = (1+ln_H_input(1)*ln_H_input(1)) / exp(ln_H_input(0));
  SparseMatrix<Type> Q = Q_spde(spde,kappa,H);

  //nll = GMRF(Q)(x);									// Negative log likelihood
  nll = .5*(x*(Q*x.matrix()).array()).sum(); // Drop normalizing constant

  if(flag==0)return nll;

  vector<Type> Xbeta = X*beta;  
  for(int i=0; i<time.size(); i++){    
    Type eta = Xbeta(i) + x(meshidxloc(i))/tau;
    Type lambda = exp(eta);
    Type t_alpha = pow(time(i),alpha);
    Type S = exp(-lambda*t_alpha);         		// Survival funciton
    Type f = lambda*alpha*t_alpha/time(i)*S; 	// Densities

    // Likelihood contribution depends on truncation status
    if(notcens(i))
      nll -= log(f);
    else
      nll -= log(S); 
  }
    
  return nll;
}
