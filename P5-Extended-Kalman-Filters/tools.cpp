#include "tools.h"
#include <iostream>
#include <math.h>

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
  /**
   * TODO: Calculate the RMSE here.
   */

  /**
   Initailizing vector containing 4 parameters 
   */
  VectorXd rmse(4);
  rmse << 0,0,0,0;

 //Assertion
  assert(estimations.size()!=0);
  assert(ground_truth.size()!=0);
  assert(estimations.size()==ground_truth.size());

 //accumulate 
 for (int i=0; i < estimations.size(); ++i){
    VectorXd residual = estimations[i] - ground_truth[i];
    residual = residual.array()*residual.array();
    rmse += residual;
 }

//mean
rmse = rmse/estimations.size();

//sqrt
rmse = rmse.array().sqrt();

return rmse;
  
}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
  /**
   * TODO:
   * Calculate a Jacobian here.
   */
  //initialize
  MatrixXd Jacobian(3,4);
  Jacobian << 0,0,0,0,
              0,0,0,0,
              0,0,0,0;

   float px = x_state[0];
   float py = x_state[1];
   float vx = x_state[2];
   float vy = x_state[3];

   float z = px*px + py*py;

   if (std::abs(z) < 0.0001){
      return Jacobian;
   }
   else {
      Jacobian(0,0) = px/sqrt(z);
      Jacobian(0,1) = py/sqrt(z);
      Jacobian(1,0) = -py/z;
      Jacobian(1,1) = px/z;
      Jacobian(2,0) = py*(vx*py - vy*px)/pow(z, 3/2);
      Jacobian(2,1) = px*(vy*px - vx*py)/pow(z, 3/2);
      Jacobian(2,2) = px/sqrt(z);
      Jacobian(2,3) = py/sqrt(z);
   }

   return Jacobian;

}
