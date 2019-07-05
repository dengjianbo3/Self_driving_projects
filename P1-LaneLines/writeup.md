# **Finding Lane Lines on the Road** 

---

**Finding Lane Lines on the Road**

The goals / steps of this project are the following:
* Make a pipeline that finds lane lines on the road
* Reflect on your work in a written report


[//]: # (Image References)

[image1]: ./examples/grayscale.jpg "Grayscale"

---

### Reflection

### 1. Describe your pipeline. As part of the description, explain how you modified the draw_lines() function.

**My pipeline consisted of 5 steps.**

First, I make a copy of the original image,  then turn it into grayscale ;

Second, blurring the gray image by using gaussian_blur;

Third, use canny method to detect edges of the image;

Fourth, identify the interest region, and tuming the parameters of vertices, try to find the most appropiate region that can detect lanes;

Last, transforming into hough space, and draw lines into the image.

**In order to draw a single line on the left and right lanes, I modified the draw_lines() function by trying three methods.**

Slope filter , to reduce the nosiy small lines, I set the slope threshold to 0.5, and calculate the absolute slope in lines, if the absolute slope is smaller than the threshold, then drop it.
 
Sperate right lanes and left lanes, if the slope of the line is negative, then I throw it into the right lines list, and if the slope of the line is positive, I throw it into the left lines list.
 
Extroplate the lines, using numpy's ployfit function to fit a first order nominal equation, and get the slope and intersect of the right lanes and the left lanes, then calculating four points to draw a concreate line with given the y-aixs parameters.


### 2. Identify potential shortcomings with your current pipeline


One potential shortcoming would be what would happen when facing curved lines, this way of detection could not fit well.

Another shortcoming could be the lanes on the road may missing sometimes, then the pipeline could not draw a sugguested lanes.


### 3. Suggest possible improvements to your pipeline

A possible improvement would be to when facing the situation of missing road lanes, by remembering the past lanes features, drawing a suggested lane.

Another potential improvement could be to find some way to detect the curved lines.
