# **Behavioral Cloning** 

---

**Behavioral Cloning Project**

The goals / steps of this project are the following:
* Use the simulator to collect data of good driving behavior
* Build, a convolution neural network in Keras that predicts steering angles from images
* Train and validate the model with a training and validation set
* Test that the model successfully drives around track one without leaving the road
* Summarize the results with a written report


[//]: # (Image References)

[image1]: ./model.png "Model Visualization"
[image2]: ./examples/center.jpg "center Image"
[image3]: ./examples/left.jpg "left Image"
[image4]: ./examples/right.jpg "right Image"

## Rubric Points
### Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/432/view) individually and describe how I addressed each point in my implementation.  

---
### Files Submitted & Code Quality

#### 1. Submission includes all required files and can be used to run the simulator in autonomous mode

My project includes the following files:
* CNN_LSTM_model.ipynb containing the script to create and train the model
* drive.py for driving the car in autonomous mode
* model.h5 containing a trained convolution neural network 
* writeup_report.md summarizing the results

#### 2. Submission includes functional code
Using the Udacity provided simulator and my drive.py file, the car can be driven autonomously around the track by executing 
```sh
python drive.py model.h5
```

#### 3. Submission code is usable and readable

The CNN_LSTM_model.ipynb file contains the code for training and saving the convolution & LSTM neural network. The file shows the pipeline I used for training and validating the model, and it contains comments to explain how the code works.

### Model Architecture and Training Strategy

#### 1. An appropriate model architecture has been employed

My model consists of two parts, CNN part and LSTM part.

Camera images' features extraction by using CNN architecture, and learning sequence relations in LSTM part.

#### 2. Attempts to reduce overfitting in the model

The model contains dropout layers in order to reduce overfitting (CNN_LSTM_model.ipynb block  8). 

The model was trained and validated on different data sets to ensure that the model was not overfitting (block 12). The model was tested by running it through the simulator and ensuring that the vehicle could stay on the track.

#### 3. Model parameter tuning

The model used an adam optimizer, so the learning rate was not tuned manually (CNN_LSTM_model.ipynb block 11).

#### 4. Appropriate training data

Training data was chosen to keep the vehicle driving on the road. I used a combination of center lane driving, recovering from the left and right sides of the road. 

I collect about 4 laps training images.

For details about how I created the training data, see the next section. 

### Model Architecture and Training Strategy

#### 1. Solution Design Approach

The overall strategy for deriving a model architecture was to capture features from both center, left and right cameras by using appropriate convolution architecture, and feeds the result to a lstm layer to capture the sequence relationship between frames.

The reason why I choose this strategy is, I think there is sequence relationship between what I had seen before and what I need to do after few seconds while driving a car. For example, while I staying at a stright road and about to drive into a corner, I may steer my wheel a little bit to fit the corner's curvature, in contrast, when I about to leave the corner, I won't steering to hard.

My first step was to use a convolution neural network model with 4 layers and concatenate with 1 layer lstm layer. I thought this model might be appropriate because the camera images just like  'eyes' of the driver, convlution layers provide perceptions of the evironment. Lstm layer's job is find out the sequence realtionship between 'before' and 'after' actions, so It could drive smoothly.

Then I training the model, I found a problem that sometime the mean square error of training and validation sometimes dramatically increase into a unbelievable situation like a million then start to drop again. I think this problem might cause by the lstm layer, the loss surface has something like a 'cliff', one step ahead will climb to the top of the 'cliff'. 

The way I try to solve this proplem is decrease the initail learing rate, after some tuning I found 0.0005 will make the training process more stable.

The final step was to run the simulator to see how well the car was driving around track one, the result is nice, the car driving smoothly as I expected. I try 10 rounds more in autonomous mode, this model provides a good driving behaviour and the car never leave the road during the whole time.


#### 2. Final Model Architecture

The final model architecture (CNN_LSTM_model.ipynb block 6-9) consisted of a convolution neural network with the 4 combination of convolution layer and maxpooling layer, LSTM layer with 50 units, and two dense layers.

Here is a visualization of the architecture 

![alt text][image1]

#### 3. Creation of the Training Set & Training Process

To capture good driving behavior, I first recorded two laps on track one using center lane driving and try to dry two recovery laps, here is some examples of center, left and right images captured.

![alt text][image2]
![alt text][image3]
![alt text][image4]

After the collection process, I had 22392 number of data points include both three cameras, then preprocessed this data by cropping 60 lines pixels from top and 30 lines pixels from the bottom off the input images, and normalize it by devide 255.0 then subtract 0.5.

I finally  put 20% of the data into a validation set without shuffle the data, because the model contain a lstm layer and  could not break the sequence realtion. 

I used this training data for training the model. The validation set helped determine if the model was over or under fitting. The ideal number of epochs was 20,  I used an adam optimizer with initial rate of 0.0005 .
