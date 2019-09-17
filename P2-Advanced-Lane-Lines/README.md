## Writeup

---

**Advanced Lane Finding Project**

The goals / steps of this project are the following:

* Compute the camera calibration matrix and distortion coefficients given a set of chessboard images.
* Apply a distortion correction to raw images.
* Use color transforms, gradients, etc., to create a thresholded binary image.
* Apply a perspective transform to rectify binary image ("birds-eye view").
* Detect lane pixels and fit to find the lane boundary.
* Determine the curvature of the lane and vehicle position with respect to center.
* Warp the detected lane boundaries back onto the original image.
* Output visual display of the lane boundaries and numerical estimation of lane curvature and vehicle position.
* All codes and demonstrations are in the P2-Advanced_lane_finding.ipynb.
* All test images after processed in  './output_image'

[//]: # (Image References)

[image1]: ./writeup_source/original_image.png "Original image"
[image2]: ./writeup_source/undistort.png "Undistorted Comprison"
[image3]: ./writeup_source/undistorted_image.png "Undistorted Example"
[image4]: ./writeup_source/combine_filer.png "Combine filter Example"
[image5]: ./writeup_source/perspective_transform.png "Perspective transform"
[image6]: ./writeup_source/fill_lane.png "Fill lane"
[image7]: ./writeup_source/final_output.png "Final output"
[image8]: ./writeup_source/chessboard_calibration.png "chessboard calibration"
[video1]: ./output_video "Video"


### Camera Calibration

The original image

![alt text][image1]

In order to calibrate the camera, by using the result of chessboard images and opencv function calibrateCamera,  we can get two paramters mtx and dist after camera calibration.
Than we can undistort images with mtx and dist with cv2.undistort. Result comparison shows below.

Chessboard calibration result:

![alt text][image8]

Test image after calibrated:

![alt text][image2]

### Image pipeline

#### 1. Example of a distortion-corrected image.

To demonstrate this step, I will describe how I apply the distortion correction to one of the test images like this one:

![alt text][image3]

#### 2. Threshold filters

I used a combination of color and horizontal sobel thresholds to generate a binary image .  I convert the image into HLS channel first, and extract the 's' channel as the first layer of thresholding, the secong layer of thresholding I use horizontal sobel to generate. At last I combine these two layers as the final thresholding result.
Here's an example of my output for this step.

![alt text][image4]


#### 3. Perspective transform.

After tuning the indexs of source points and the destination points, the most appropriate indexs I found are:

| Source        | Destination   | 
|:-------------:|:-------------:| 
| 710, 460      | 1000, 100        | 
| 1100, 720      | 1000, 680      |
| 150, 720     | 150, 680      |
| 600, 460      | 150, 100        |

The transformed image show as below:

![alt text][image5]

#### 4. Detect lane line

I try to calculate the histogram of the output from the result above , use the peak points to located the left base point and the right base point. Then cutting the image into n windows, slide the two filter boxes of left side and right to calculate the activate pixels in these boxes. 

After all activate pixels were found, use these pixels' index to fit the second order polynomial function of the left line and the right line, and draw the area between them.

The result look like these:

![alt text][image6]

#### 5. Calculate radius and deviation

At this part, I use the curvature function to calculate the avervage curvature of two poly-functions. 

Then calculate the difference between middle of the image and middle of the lane as the deviation of the car.

#### 6. Final output

The final output look like this:

![alt text][image7]

---

### Pipeline (video)


Here's a [link to my video result][video1]

---

### Discussion

The problem I found in the pipeline is the lane line is unstable when the road's curvature change rapidly. I have some suggestions to fix it:

 - Use more window filters, cutting the detection more finely.
 - Fit activated pixels with higer order polynoimal function.
 - Change the fitting function to Trigonometric function, like sin or cos or their combinations.
