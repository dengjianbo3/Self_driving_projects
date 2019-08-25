/**
 * particle_filter.cpp
 *
 * Created on: Dec 12, 2016
 * Author: Tiffany Huang
 */

#include "particle_filter.h"

#include <math.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "helper_functions.h"

using std::string;
using std::vector;

void ParticleFilter::init(double x, double y, double theta, double std[]) {
  /**
   * TODO: Set the number of particles. Initialize all particles to 
   *   first position (based on estimates of x, y, theta and their uncertainties
   *   from GPS) and all weights to 1. 
   * TODO: Add random Gaussian noise to each particle.
   * NOTE: Consult particle_filter.h for more information about this method 
   *   (and others in this file).
   */

  num_particles = 100;  // TODO: Set the number of particles

  //initialize the distributions of parameters with given GPS data
  weights.resize(num_particles);
  particles.resize(num_particles);

  std::normal_distribution<double> x_distribution(x, std[0]);
  std::normal_distribution<double> y_distribution(y, std[1]);
  std::normal_distribution<double> theta_distribution(theta, std[2]);
  
  std::default_random_engine gen;  //random data generation

  //particle generation
  for (unsigned int i=0; i<num_particles; ++i){
    //sampling samples from parameters distribution & add gaussian noise

    double x_p = x_distribution(gen);
    double y_p = y_distribution(gen);
    double theta_p = theta_distribution(gen);

    //update parameters
    particles[i].id = i;
    particles[i].x = x_p;
    particles[i].y = y_p;
    particles[i].theta = theta_p;
    particles[i].weight = 1.0/num_particles; //weight initialization
  
  }

  is_initialized=true;
}

void ParticleFilter::prediction(double delta_t, double std_pos[], 
                                double velocity, double yaw_rate) {
  /**
   * TODO: Add measurements to each particle and add random Gaussian noise.
   * NOTE: When adding noise you may find std::normal_distribution 
   *   and std::default_random_engine useful.
   *  http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
   *  http://www.cplusplus.com/reference/random/default_random_engine/
   */

  std::default_random_engine gen;

  for (unsigned int i=0; i<particles.size(); ++i){
    //statements of last time step
    double x0 = particles[i].x;
    double y0 = particles[i].y;
    double theta0 = particles[i].theta;
    double x_pred;
    double y_pred;
    double theta_pred;

    if (fabs(yaw_rate) > 0.00001){
      x_pred = x0 + (velocity/yaw_rate) * (sin(theta0+yaw_rate*delta_t) - sin(theta0));
      y_pred = y0 + (velocity/yaw_rate) * (cos(theta0) - cos(theta0+yaw_rate*delta_t));
      theta_pred = theta0 + yaw_rate*delta_t;
    }
    else{
      x_pred = x0 + velocity*delta_t*cos(theta0);
      y_pred = y0 + velocity*delta_t*sin(theta0);
      theta_pred = theta0;
    }

    //sample from predicted postion with uncertainty
    std::normal_distribution<double> x_noise(0, std_pos[0]);
    std::normal_distribution<double> y_noise(0, std_pos[1]);
    std::normal_distribution<double> theta_noise(0, std_pos[2]);

    particles[i].x = x_pred + x_noise(gen);
    particles[i].y = y_pred + y_noise(gen);
    particles[i].theta = theta_pred + theta_noise(gen);
  }

}

void ParticleFilter::dataAssociation(vector<LandmarkObs> predicted, 
                                     vector<LandmarkObs>& observations) {
  /**
   * TODO: Find the predicted measurement that is closest to each 
   *   observed measurement and assign the observed measurement to this 
   *   particular landmark.
   * NOTE: this method will NOT be called by the grading code. But you will 
   *   probably find it useful to implement this method and use it as a helper 
   *   during the updateWeights phase.
   */
  for (unsigned int i=0; i<observations.size(); ++i){
    double x = observations[i].x;
    double y = observations[i].y;

    double min_dist = std::numeric_limits<double>::max();

    int map_id;

    for (unsigned int j=0; j<predicted.size(); j++){
      double x_p = predicted[j].x;
      double y_p = predicted[j].y;
      
      double new_dist = dist(x, y, x_p, y_p);

      if (new_dist < min_dist){
        min_dist = new_dist;
        map_id = predicted[j].id;
      }
    }
    observations[i].id = map_id;
  }
 
}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
                                   const vector<LandmarkObs> &observations, 
                                   const Map &map_landmarks) {
  /**
   * TODO: Update the weights of each particle using a mult-variate Gaussian 
   *   distribution. You can read more about this distribution here: 
   *   https://en.wikipedia.org/wiki/Multivariate_normal_distribution
   * NOTE: The observations are given in the VEHICLE'S coordinate system. 
   *   Your particles are located according to the MAP'S coordinate system. 
   *   You will need to transform between the two systems. Keep in mind that
   *   this transformation requires both rotation AND translation (but no scaling).
   *   The following is a good resource for the theory:
   *   https://www.willamette.edu/~gorr/classes/GeneralGraphics/Transforms/transforms2d.htm
   *   and the following is a good resource for the actual equation to implement
   *   (look at equation 3.33) http://planning.cs.uiuc.edu/node99.html
   */
  //transform the observation coordinate
  //landmark list
  

  for (unsigned int i=0; i<particles.size(); ++i){

    double x_p = particles[i].x;   //current particle's x
    double y_p = particles[i].y;   //current particle's y
    double theta_p = particles[i].theta;  //current particle's theta

    vector<LandmarkObs> valid_landmarks;  //valid landmarks in sensor range

    //cutting small valid range in the map
    for (unsigned int l=0; l<map_landmarks.landmark_list.size(); ++l){
      double x_l = map_landmarks.landmark_list[l].x_f;
      double y_l = map_landmarks.landmark_list[l].y_f;
      int id_l = map_landmarks.landmark_list[l].id_i;
      
      double sensor_dist = dist(x_p,y_p, x_l,y_l);

      if (sensor_dist <= sensor_range){
        valid_landmarks.push_back(LandmarkObs{id_l, x_l, y_l});
      }
    }

    vector<LandmarkObs> TOBS;  //transformed observation 

    for (unsigned int j=0; j<observations.size(); ++j){
      double x_c = observations[j].x;
      double y_c = observations[j].y;
      double id_c = observations[j].id;
      //coordinate transformations
      double x_m = x_p + x_c*cos(theta_p) - y_c*sin(theta_p);
      double y_m = y_p + x_c*sin(theta_p) + y_c*cos(theta_p);

      TOBS.push_back(LandmarkObs{id_c, x_m, y_m});
    }

    dataAssociation(valid_landmarks, TOBS); // associate observation positions to landmarks; 

    particles[i].weight = 1.0; //re-initialize particles weight
    //update weights
    for (unsigned int k=0; k<TOBS.size(); ++k){
      double x_obs = TOBS[k].x;
      double y_obs = TOBS[k].y;
      double x_valid;
      double y_valid;
      int associated_id = TOBS[k].id;

      for (int m=0; m<valid_landmarks.size(); ++m){
        if (valid_landmarks[m].id == associated_id){
          x_valid = valid_landmarks[m].x;
          y_valid = valid_landmarks[m].y;
        }
      }

      double x_sigma = std_landmark[0];
      double y_sigma = std_landmark[1];
      double w_new;

      double x_d = x_obs - x_valid;
      double y_d = y_obs - y_valid;

      w_new = (1/(2*M_PI*x_sigma*y_sigma))*(exp(-((x_d*x_d)/(2*x_sigma*x_sigma) + (y_d*y_d)/(2*y_sigma*y_sigma))));

      
      if (w_new < 0.000001){
        particles[i].weight *= 0.000001;
      }
      else{
        particles[i].weight *= w_new;
      }
    }
  }
  //normalize weight
  
  double sum_weight=0;
  for (int i=0; i<particles.size(); ++i){
    sum_weight += particles[i].weight;
  }


  for (int j=0; j<particles.size(); ++j){
    particles[j].weight /= sum_weight;
  }

}

void ParticleFilter::resample() {
  /**
   * TODO: Resample particles with replacement with probability proportional 
   *   to their weight. 
   * NOTE: You may find std::discrete_distribution helpful here.
   *   http://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
   */
  /*
  vector<Particle> new_particles(num_particles);
  vector<double> weights(num_particles);
  for (unsigned int i=0; i<num_particles; ++i){
    weights.push_back(particles[i].weight);
  }

  std::random_device rd;

  std::default_random_engine gen(rd());

  for (unsigned int i=0; i<num_particles; ++i){
    std::discrete_distribution<int> index(weights.begin(), weights.end());
    new_particles[i] = particles[index(gen)];
  }
  particles = new_particles;
  */
 vector<double> weights;
 double max_weight = std::numeric_limits<double>::min();

 for (int i=0; i<num_particles; ++i){
   weights.push_back(particles[i].weight);
   if (particles[i].weight > max_weight){
     max_weight = particles[i].weight;
   }
  }
  std::default_random_engine gen;

  std::uniform_int_distribution<int> dist_int(0, num_particles-1);
  std::uniform_real_distribution<float> dist_real(0.0, max_weight);

  int index = dist_int(gen);

  double beta = 0.0;

  vector<Particle> new_particles;

  for (int i=0; i<num_particles; ++i){
    beta += dist_real(gen)*2.0;
    while (beta>weights[index]){
      beta -= weights[index];
      index = (index+1) % num_particles;
    }
    new_particles.push_back(particles[index]);
  }
  particles = new_particles;
}

void ParticleFilter::SetAssociations(Particle& particle, 
                                     const vector<int>& associations, 
                                     const vector<double>& sense_x, 
                                     const vector<double>& sense_y) {
  // particle: the particle to which assign each listed association, 
  //   and association's (x,y) world coordinates mapping
  // associations: The landmark id that goes along with each listed association
  // sense_x: the associations x mapping already converted to world coordinates
  // sense_y: the associations y mapping already converted to world coordinates
  particle.associations= associations;
  particle.sense_x = sense_x;
  particle.sense_y = sense_y;
}

string ParticleFilter::getAssociations(Particle best) {
  vector<int> v = best.associations;
  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<int>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}

string ParticleFilter::getSenseCoord(Particle best, string coord) {
  vector<double> v;

  if (coord == "X") {
    v = best.sense_x;
  } else {
    v = best.sense_y;
  }

  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<float>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}