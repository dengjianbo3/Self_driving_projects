#include <uWS/uWS.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include "helpers.h"
#include "json.hpp"
#include "spline.h"

// for convenience
using nlohmann::json;
using std::string;
using std::vector;

int main() {
  uWS::Hub h;

  // Load up map values for waypoint's x,y,s and d normalized normal vectors
  vector<double> map_waypoints_x;
  vector<double> map_waypoints_y;
  vector<double> map_waypoints_s;
  vector<double> map_waypoints_dx;
  vector<double> map_waypoints_dy;

  // Waypoint map to read from
  string map_file_ = "../data/highway_map.csv";
  // The max s value before wrapping around the track back to 0
  double max_s = 6945.554;

  std::ifstream in_map_(map_file_.c_str(), std::ifstream::in);

  string line;
  while (getline(in_map_, line)) {
    std::istringstream iss(line);
    double x;
    double y;
    float s;
    float d_x;
    float d_y;
    iss >> x;
    iss >> y;
    iss >> s;
    iss >> d_x;
    iss >> d_y;
    map_waypoints_x.push_back(x);
    map_waypoints_y.push_back(y);
    map_waypoints_s.push_back(s);
    map_waypoints_dx.push_back(d_x);
    map_waypoints_dy.push_back(d_y);
  }

  //initial lane number started from lane 0, lane 1 is the middle lane
  int lane = 1;

  //initialize reference velocity
  double ref_vel = 0;


  h.onMessage([&map_waypoints_x,&map_waypoints_y,&map_waypoints_s,
               &map_waypoints_dx,&map_waypoints_dy, &lane, &ref_vel]
              (uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
               uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event

    if (length && length > 2 && data[0] == '4' && data[1] == '2') {

      auto s = hasData(data);

      if (s != "") {
        auto j = json::parse(s);
        
        string event = j[0].get<string>();
        
        if (event == "telemetry") {
          // j[1] is the data JSON object
          
          // Main car's localization Data
          double car_x = j[1]["x"];
          double car_y = j[1]["y"];
          double car_s = j[1]["s"];
          double car_d = j[1]["d"];
          double car_yaw = j[1]["yaw"];
          double car_speed = j[1]["speed"];

          // Previous path data given to the Planner
          auto previous_path_x = j[1]["previous_path_x"];
          auto previous_path_y = j[1]["previous_path_y"];
          // Previous path's end s and d values 
          double end_path_s = j[1]["end_path_s"];
          double end_path_d = j[1]["end_path_d"];

          // Sensor Fusion Data, a list of all other cars on the same side 
          //   of the road.
          auto sensor_fusion = j[1]["sensor_fusion"];

          json msgJson;

          /**
           * TODO: define a path made up of (x,y) points that the car will visit
           *   sequentially every .02 seconds
           */

          //start

          int prev_size = previous_path_x.size();

          
          //sensor fusion part
          if (prev_size > 0){
            car_s = end_path_s;
          }

          bool too_close = false;   //in order to detect vehicle ahead

          //find ref_v to use
          for (unsigned int i = 0; i < sensor_fusion.size(); ++i){
            //car is in my lane
            float d = sensor_fusion[i][6];

            //the width of lane is 4 meters
            //check a little bit wider in case of the car on the other lane change suddenly
            if (d < (2+4*lane+2.5) && d > (2+4*lane-2.5)){

              double vx = sensor_fusion[i][3];
              double vy = sensor_fusion[i][4];
              double check_speed = sqrt(vx*vx+vy*vy);
              double check_car_s = sensor_fusion[i][5];

              check_car_s += ((double)prev_size*.02*check_speed);

              if((check_car_s > car_s) && ((check_car_s-car_s) < 30)){

                //ref_vel = 29.5; 
                too_close = true;
              }
            }
          }

          /*
          seperate the cars in sensor fusion in four parts
          the cars in front of ego on the right
          the cars in front of ego on the left
          the cars behind ego on the right
          the cars behind ego on the left
          */
  
          if (too_close){  

            //slow down first
            ref_vel -= .224;

            //control flag of change lane 
            bool change_left = false;
            bool change_right = false;

            //check cars in those four parts, update the closest cars in those parts

            double left_front_min_dist = std::numeric_limits<double>::max();
            double left_behind_min_dist = std::numeric_limits<double>::max();
            double right_front_min_dist = std::numeric_limits<double>::max();
            double right_behind_min_dist = std::numeric_limits<double>::max();

            //try to scan all possible vehicles on the road
            for (unsigned int i = 0; i < sensor_fusion.size(); ++i){
              //each sensor fusion result has 7 properties
              //0:id  1:x  2:y  3:vx  4:vy  5:s  6:d
              double vx = sensor_fusion[i][3];
              double vy = sensor_fusion[i][4];
              double s = sensor_fusion[i][5];
              double d = sensor_fusion[i][6];

              //position prediction
              double speed = sqrt(vx*vx + vy*vy);
              s += ((double)prev_size*.02*speed);

              double dist = abs(s - car_s);

              //left lane
              if ((d < car_d) && (d > (car_d - 6))){
                if (s < car_s){
                  if (dist < left_behind_min_dist){
                    left_behind_min_dist = dist;
                  }
                }
                else if (s > car_s){
                  if (dist < left_front_min_dist){
                    left_front_min_dist = dist;
                  }
                }
              }

              //right lane
              if ((d > car_d) && (d < (car_d + 6))){
                if (s < car_s){
                  if (dist < right_behind_min_dist){
                    right_behind_min_dist = dist;
                  }
                }
                else if (s > car_s){
                  if (dist < right_front_min_dist){
                    right_front_min_dist = dist;
                  }
                }  
              }
            }
            
            //safety checking
            if ((left_behind_min_dist > 15) && (left_front_min_dist > 35) && (lane > 0)){
              change_left = true;            
            }

            if ((right_behind_min_dist > 15) && (right_front_min_dist > 35) && (lane < 2)){
              change_right = true;       
            }

            //change lane
            if ((lane == 0) && (change_right)){
              if (ref_vel < 49.5){
                //when it is safe, change lane a little bit faster would be more safer from my experience
                ref_vel += .3;
              }
              lane = 1;         
            }

            else if (lane == 1){
              //try to find a better lane
              if ((change_left) && (change_right)){
                if (ref_vel < 49.5){
                  ref_vel += .3;
                }
                if (left_front_min_dist > right_front_min_dist){
                  lane = 0;
                }
                else {
                  lane = 2;
                }  
              }
              
              else if ((change_left)){
                if (ref_vel < 49.5){
                  ref_vel += .3;
                }        
                lane = 0;             
              }
              else if (change_right){    
                if (ref_vel < 49.5){
                  ref_vel += .3;
                }        
                lane = 2;
              }
            }

            else if ((lane == 2) && (change_left)){  
              if (ref_vel < 49.5){
                ref_vel += .3;
              }  
              lane = 1;
            }
          }

          else if (ref_vel < 49.5){
            ref_vel += .224;
          }

          //using spline to fit a smoother polynomial trajectory
          vector<double> ptsx;
          vector<double> ptsy;

          //initialize reference velocity
          double ref_x = car_x;
          double ref_y = car_y;
          double ref_yaw = deg2rad(car_yaw);

          if(prev_size < 2){
            /*if the previous path has non enough points to fit the spline, use the 
            current sensor fusion location result (x,y) as the starter point
            and plot a tangle point as the previous point
            */
            double prev_car_x = car_x - cos(car_yaw);
            double prev_car_y = car_y - sin(car_yaw);

            ptsx.push_back(prev_car_x);
            ptsx.push_back(car_x);
            
            ptsy.push_back(prev_car_y);
            ptsy.push_back(car_y);
        
          }

          else{
            /*
            use the last trajectory path's last two points as the starter points
            */
            ref_x = previous_path_x[prev_size-1];
            ref_y = previous_path_y[prev_size-1];

            double ref_x_prev = previous_path_x[prev_size-2];
            double ref_y_prev = previous_path_y[prev_size-2];

            ref_yaw = atan2(ref_y-ref_y_prev, ref_x-ref_x_prev);

            //assure those two points make the path tangent to the previous path's end point
            ptsx.push_back(ref_x_prev);
            ptsx.push_back(ref_x);

            ptsy.push_back(ref_y_prev);
            ptsy.push_back(ref_y);
          }
          
          /*
          find 4 points ahead of the ego vehicle to fit a spline (or polynomial) as the predicted trajectory 
          */
          vector<double> next_wp0 = getXY(car_s+30, (2+4*lane), map_waypoints_s, map_waypoints_x, map_waypoints_y);
          vector<double> next_wp1 = getXY(car_s+60, (2+4*lane), map_waypoints_s, map_waypoints_x, map_waypoints_y);
          vector<double> next_wp2 = getXY(car_s+90, (2+4*lane), map_waypoints_s, map_waypoints_x, map_waypoints_y);
      

          ptsx.push_back(next_wp0[0]);
          ptsx.push_back(next_wp1[0]);
          ptsx.push_back(next_wp2[0]);

          ptsy.push_back(next_wp0[1]);
          ptsy.push_back(next_wp1[1]);
          ptsy.push_back(next_wp2[1]);


          for (int i = 0; i<ptsx.size(); i++){

            double shift_x = ptsx[i] - ref_x;
            double shift_y = ptsy[i] - ref_y;

            ptsx[i] = (shift_x*cos(0-ref_yaw) - shift_y*sin(0-ref_yaw));
            ptsy[i] = (shift_x*sin(0-ref_yaw) + shift_y*cos(0-ref_yaw));
          }
          

          //create a spline instance
          tk::spline s;

          //set x,y points to the spline
          s.set_points(ptsx, ptsy);

          vector<double> next_x_vals;
          vector<double> next_y_vals;

          
          for (int i=0; i<previous_path_x.size(); i++){

            next_x_vals.push_back(previous_path_x[i]);
            next_y_vals.push_back(previous_path_y[i]);
          }
          

          double target_x = 30;
          double target_y = s(target_x);
          double target_dist = sqrt((target_x)*(target_x)+(target_y)*(target_y));

          double x_add_on = 0.0; 

          for (int i=1; i<=50-previous_path_x.size(); i++){

            double N  = (target_dist/(.02*ref_vel/2.24));
            double x_point = x_add_on+(target_x)/N;
            double y_point = s(x_point);

            x_add_on = x_point;

            double x_ref = x_point;
            double y_ref = y_point;

            //coordinate transformation from local coordinate to map coordinate
            x_point = (x_ref*cos(ref_yaw)-y_ref*sin(ref_yaw));
            y_point = (x_ref*sin(ref_yaw)+y_ref*cos(ref_yaw));

            x_point += ref_x;
            y_point += ref_y;

            next_x_vals.push_back(x_point);
            next_y_vals.push_back(y_point);
          }


          //end

      

          msgJson["next_x"] = next_x_vals;
          msgJson["next_y"] = next_y_vals;

          auto msg = "42[\"control\","+ msgJson.dump()+"]";

          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}