# PID controller WriteUp 

------------------------------------------------------------------

[image1]: ./PID_controller_result.gif  "P8 result git"
![alt text][image1]

## Description about PID controller

PID controller is short for P(Proportion), I(Integral), D(Derivative), the goal of this controller is maintain the vechicle in a right trajectory and smoother.

For each parameter in the controller, Kp is the punishment of cross track error (cte), Ki is the punishment of system bias, Kd is the punishment of oscillation. With the rightful parameters' group, the vehicle will drive safe and smooth.

The cost function (steering angle) is 

steering angle  = - Kp * cte - Ki * (sum(cte)) - Kd * (cte - cte_prev)

## Basic component

1. Parameters and errors initialization;

2. Update each error and compute the derivative of cte trough one single time step;

3. Computing the total error and assign the result to steer angle in order to control the vehicle.

## Paramters tuning

P : while it is too small, the vechicle could not reduce the cross track error efficiently, but could cause unexpected jerking when it is too large.

I : integral of all cross track error will help the vechicle back to the lane in the right direction so that it could reduce the machanic system error.

D : the parameter of derivative of cross track error control the vechicle's oscillation, the steering angle will too sensitive if it is too large but oscillating when it is too small.

The final parameters I tuned is 0.12 for P, 0.004 for I, 2.2 for D, with a fairwell simulation result.

## Avancement

Parameters tuning by hand is too intuitive, so the better solution is to use some automative function to find the best parameters like gradient descent.