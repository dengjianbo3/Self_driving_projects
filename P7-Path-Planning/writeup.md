# Path planning project report

------------------------------------------------------------------

## Goals of the project

1.The car is able to drive at least 4.32 miles without incident.

2.The car drives according to the speed limit which is 50 miles/hour.

3.Max Acceleration and Jerk are not Exceeded.(upper bounds of total acceleration & jerk are 10 m/s^2 and 10 m/s^3.)

4.Car does not have collisions.

5.The car stays in its lane, except for the time between changing lanes.

6.The car is able to change lanes.

## Project result recording

https://www.youtube.com/watch?v=p_6Q_Mzp9BI&t=646s

## Decision making process

#### 1. Trajectory generation

Trajectory is the route maneuver which the ego car follow, the ego car will move to the next way point each 0.02 sencond in the simulator.

In my code, I use spline to generate a smooth trajectroy to avoid hitting a uncomfrotable jerk. First I select the last two points from the last trajectory vector, use those points as the start anchors, then use frenet transformation to transfor 3 points in the distance (in the same lane) into map coordinate as the other anchors, fit a spline by those 5 points.

After a spline has been fitted, the ego car will drive along with this spline by using some discrete points with identical distance. In order to achieve this goal, just set a target distance and split into some points using the spline function with the velocity consideration.

Also, spline function include lane change behaviour when the code tranfer frenet coordinate to map coordinate. When the ego car driving slow because of some slowing car ahead, it will changes its lane to make sure the journey won't take too long while the other lanes are safe.

#### 2. Safety detection

Safety detection contain two parts, the first task is maintain a safe distance between ego car and the one in front of it, the second one is detect cars beside when the ego car try to change lane.

The first thing to avoid collsion is checking the car ahead in the same lane, by looping the sensor fusion data and compute the distance simultaneously. When the smallest distance below the threshold (in my code it is 30 meters), slowing down the ego car by a small negative acceleration. In here the lane I try to detect is little bit wider because it can avoid some cars on the other lanes change suddenly.

Secondly, when the ego car try to change its lane, I need to make sure the other lanes are safe. I seperate the sensor fusion range into four parts, which are the car ahead on the left lane, the car behind on the left lane, the car ahead on the right lane, the car behind on the right lane. After computing the smallest relative distance in frenet coordinate between cars in those four parts, I set four theshold in case of creating a safety gap to let the ego car change its lane.

## Conclusion

#### 1.Problems

In my project, I neglect many consideration for simplicity while building the driving behaviour model, like how to avoid cars change their lane suddenly in a high velocity, and the cars behind us moving irrational. Since cars in the simulator follow some strict principles, the result seems nice but I sure that in the real world it would be much more complicated.

#### 2.Enhacement proposal

Trajectory generation is a typical markov decision process, I think reinforcement learning could be more powerful than handmade code.