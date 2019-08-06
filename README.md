# Udacity self driving projects

## Descritions

This repositroy contains require projects of Udacity self driving car engineer nano-degree. Udacity self driving degree contain lots of useful courses and projects to help me get into self driving industry, including vision system like lane line detection, or traffic sign classifer with deep learning model, and sensor fusion like extended kalman filter, or some technology beyone like using imitation learing to control a vechicle in the simulator. Also along with further session trajectory, I could learn SLAM and other interesting and useful skills to help me dive in to self driving career.

## Projects

### P1 - Basic lane line detection

[image1]: ./P1-LaneLines/test_videos_output/gifhome_640x360_15s.gif "P1 result git"
![alt text][image1]

By using opencv, cut out the interest region frame to frame from the central camera on the vehicle, by using thresholding and hough transformation, detect and highlight lane lines on the road.

### P2 - Lane line detection with opencv
[image2]: ./P2-Advanced-Lane-Lines/output_video/gifhome_640x360_20s.gif "P2 result git"
![alt text][image2]

Base on undistorted images after camera calibration, apply some thresholding methods like gradient thresholding, warp the interest region by using perspective transform techniques, and fit left and right lane line as two second polynomial line and draw lane line region on warped image then warp back onto the original image. In the mean time calculate the curvature and center position respect to the lane line. 

### P3 - Traffic sign classifier
[image3]: ./P3-Traffic-Sign-Classifier/examples/final_test_images.png "P3 test"
[image4]: ./P3-Traffic-Sign-Classifier/examples/final_test_images_with_predicted.png "P3 classifer reults"

original test images with labels
![alt_text][image3]

predicted results wiht label
![alt_text][image4]

### P4 - Behaviour cloning with imitation learning 
[image5]: ./P4-Behavioral-Cloning/gifhome_640x320_20s.gif "imitation learing result"
![alt_text][image5]

### P5 - Extended kalman filter
[image6]: ./P5-Extended-Kalman-Filters/result.gif "extended kalman filter result"
![alt_text][image6]
