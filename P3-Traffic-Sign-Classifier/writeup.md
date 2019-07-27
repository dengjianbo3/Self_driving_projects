# **Traffic Sign Recognition** 

## Writeup

### You can use this file as a template for your writeup if you want to submit it as a markdown file, but feel free to use some other method and submit a pdf if you prefer.

---

**Build a Traffic Sign Recognition Project**

The goals / steps of this project are the following:
* Load the data set (see below for links to the project data set)
* Explore, summarize and visualize the data set
* Design, train and test a model architecture
* Use the model to make predictions on new images
* Analyze the softmax probabilities of the new images
* Summarize the results with a written report


[//]: # (Image References)

[image1]: ./examples/labels_distribution.png "labels_distribution"
[image2]: ./examples/training_examples.png "training examples"
[image3]: ./examples/validating_examples.png "validating examples"
[image4]: ./examples/testing_examples.png "testing examples"
[image5]: ./examples/final_test_images.png "final test images"
[image6]: ./examples/final_test_images_with_predicted.png "predicted final test images"



## Rubric Points
### Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/481/view) individually and describe how I addressed each point in my implementation.  

---
### Writeup / README

#### 1. Provide a Writeup / README that includes all the rubric points and how you addressed each one. You can submit your writeup as markdown or pdf. You can use this template as a guide for writing the report. The submission includes the project code.

You're reading it! and here is a link to my [project code](
https://github.com/dengjianbo3/Self_driving_projects/blob/master/P3-Traffic-Sign-Classifier/Traffic_Sign_Classifier.ipynb)

### Data Set Summary & Exploration

#### 1. Provide a basic summary of the data set. In the code, the analysis should be done using python, numpy and/or pandas methods rather than hardcoding results manually.

I used the pandas library to calculate summary statistics of the traffic
signs data set:

* The size of training set is 34799.
* The size of the validation set is 34799.
* The size of test set is 12630.
* The shape of a traffic sign image is 32x32x3.
* The number of unique classes/labels in the data set is 43.

#### 2. Include an exploratory visualization of the dataset.

Here is an exploratory visualization of the data set. By visualize datasets as histogram graph, the training dataset, validating dataset and testing dataset , have very similar labels distribution. The histogram graph shows below.

![alt text][image1]

### Design and Test a Model Architecture

#### 1. Describe how you preprocessed the image data. What techniques were chosen and why did you choose these techniques? Consider including images showing the output of each preprocessing technique. Pre-processing refers to techniques such as converting to grayscale, normalization, etc. (OPTIONAL: As described in the "Stand Out Suggestions" part of the rubric, if you generated additional data for training, describe why you decided to generate additional data, how you generated the data, and provide example images of the additional data. Then describe the characteristics of the augmented training set like number of images in the set, number of images for each class, etc.)

As a first step, I decided to normalize the training dataset by subtract 128.0 and than devide 128.0, making the training images more easy to train.

 * training examples 
![alt text][image2]

 * validating examples
![alt text][image3]

 * testing examples
![alt text][image4]


#### 2. Describe what your final model architecture looks like including model type, layers, layer sizes, connectivity, etc.) Consider including a diagram and/or table describing the final model.

My final model consisted of the following layers:

|Layer         		|     Description	        					| 
|:---------------------:|:---------------------------------------------:| 
|Input         		| 32x32x3 RGB image   							| 
|Convolution 1     	| kernel 3x3x16, 1x1 stride, same padding, outputs 32x32x16 	|
|Batch Normalization
|RELU					|												|
|Dropout     | keep_porp=0.6   |
|Max pooling	      	| kernel 2x2x16, 1x1 stride,  same padding, outputs 32x32x16 				|
|Convolution 2     	| kernel 3x3x16, 1x1 stride, same padding, outputs 32x32x16 	|
|Batch Normalization
|RELU					|												|
|Dropout     | keep_porp=0.6   |
|Max pooling	      	| kernel 2x2x16, 1x1 stride,  same padding, outputs 32x32x16 				|
|Convolution 3    	| kernel 3x3x32, 1x1 stride, same padding, outputs 32x32x32 	|
|Batch Normalization
|RELU					|												|
|Dropout     | keep_porp=0.6   |
|Max pooling	      	| kernel 2x2x32, 1x1 stride,  same padding, outputs 32x32x32 				|
|Convolution 4     	| kernel 5x5x32, 1x1 stride, same padding, outputs 32x32x32 	|
|Batch Normalization
|RELU					|												|
|Dropout     | keep_porp=0.6   |
|Max pooling	      	| kernel 2x2x32, 1x1 stride,  same padding, outputs 32x32x32 				|
|Convolution 5     	| kernel 5x5x64, 1x1 stride, same padding, outputs 32x32x64 	|
|Batch Normalization
|RELU					|												|
|Dropout     | keep_porp=0.6   |
|Max pooling	      	| kernel 2x2x64, 2x2 stride,  valid padding, outputs 16x16x64 				|
|Convolution 6     	| kernel 5x5x64, 1x1 stride, same padding, outputs 16x16x64 	|
|Batch Normalization
|RELU					|												|
|Dropout     | keep_porp=0.6   |
|Max pooling	      	| kernel 2x2x64, 2x2 stride,  valid padding, outputs 8x8x64 				|
| Convolution 7    	| kernel 3x3x128, 1x1 stride, same padding, outputs 8x8x128 	|
|Batch Normalization
|RELU					|												|
|Dropout     | keep_porp=0.6   |
|Max pooling	      	| kernel 2x2x128, 2x2 stride,  valid padding, outputs 4x4x128 				|
|Convolution 8     	| kernel 3x3x256, 1x1 stride, same padding, outputs 4x4x256 	|
|Batch Normalization
|RELU					|												|
|Dropout     | keep_porp=0.6   |
|Max pooling	      	| kernel 2x2x256, 2x2 stride,  valid padding, outputs 2x2x256 				|
|Flatten    |      									|
|Fully connected 1		| units 256       									|
|RELU                 |                         |
|Batch Normalization       |                       |
|Dropout       |keep_porb 0.3     |
|Fully connected 2		| units 128       									|
|RELU                 |                         |
|Batch Normalization       |                       |
|Dropout       |keep_porb 0.5     |
|Logit				| units 43     									|
|SOFTMAX						|												|


#### 3. Describe how you trained your model. The discussion can include the type of optimizer, the batch size, number of epochs and any hyperparameters such as learning rate.

  My training setup is 

batch size : 128
epochs : 1000
optimizer : Adam
learning rate : 0.001
loss function : cross entropy loss
early stopping crateria : validation accuracy >= 98%



#### 4. Describe the approach taken for finding a solution and getting the validation set accuracy to be at least 0.93. Include in the discussion the results on the training, validation and test sets and where in the code these were calculated. Your approach may have been an iterative process, in which case, outline the steps you took to get to the final solution and why you chose those steps. Perhaps your solution involved an already well known implementation or architecture. In this case, discuss why you think the architecture is suitable for the current problem.

My final model results were:

training set accuracy of 99.6% 
validation set accuracy of 98.3%
test set accuracy of 96.9%
early stop at epoch 243


* What was the first architecture that was tried and why was it chosen?

I bulit a base-line by 3 convolution layers with 2 dense layers,  without any regularization method, just try to verify is there any problem with the dataset and my preprocessing step, finally get a result of validation accuracy around 90%.

* What were some problems with the initial architecture?

The initial base line architecture is too sample and with no regularization method applied, with no suprise it shows a big over fitting problem.

* How was the architecture adjusted and why was it adjusted? Typical adjustments could include choosing a different model architecture, adding or taking away layers (pooling, dropout, convolution, etc), using an activation function or changing the activation function. One common justification for adjusting an architecture would be due to overfitting or underfitting. A high accuracy on the training set but low accuracy on the validation set indicates over fitting; a low accuracy on both sets indicates under fitting.

The first step is adding dropout layer after convolution layer and dense layers, but the validation accuracy increase unstablility.
Then I try add batch normalization layer to fix the proplem, it works well, validation accuracy is more stable but the upper bound of validation accuracy still not satisfied.
After than I try to use some augumentation method, like horizontal flip, vertical flip, shear the image and etc. , but as a result I can not tell too much difference weather use these augmentation methods or not, so I decided skip this preprocess step.
At last I modified the architecture of my model, adding more convolution layer, try to extract deeper features from the image, and It works well.

* Which parameters were tuned? How were they adjusted and why?

Dropout rate 
At first I set the dropout rate to 0.5 both in convolution layers and dense layers, then I found the model is really hard to trained, than I increase the keep prop rate in convolution layer to 0.8 and decrease keep prop rate to 0.3. The reason I did these is try to solve overfitting problem.
 
Learning rate
Learning tuning is try to figure out a reasonable update rate which could balance time consuming and result.

* What are some of the important design choices and why were they chosen? For example, why might a convolution layer work well with this problem? How might a dropout layer help with creating a successful model?

In my archituecture design, I used these methods to help my model works well.

Batch normalization layer : Batch normalization  renormalize the distribution of mini-batch' s weight, the training process will be more stable.

Dropout layers : Handle overfitting problem.

Xavier weights initialization : Initialization method which could make the training process more quickly.

Early stop : Prevent overfiting by early stop the training process once it meets some criteria.


### Test a Model on New Images

#### 1. Choose five German traffic signs found on the web and provide them in the report. For each image, discuss what quality or qualities might be difficult to classify.

I download the latest version of final test images from German traffic sign website at here [GTSRB_FINAL_TEST ](https://sid.erda.dk/public/archives/daaeac0d7ce1152aea9b61d9f1e19370/published-archive.html)

Numbers of all final test images is 12630.

I pickup 5 images randomly, and print out their labels as tiltes, shows below:

![alt text][image5] 

The fourth image might be difficult to classify because the image's backgroup light is dark , and the traffic sign's edge is blur.

#### 2. Discuss the model's predictions on these new traffic signs and compare the results to predicting on the test set. At a minimum, discuss what the predictions were, the accuracy on these new predictions, and compare the accuracy to the accuracy on the test set (OPTIONAL: Discuss the results in more detail as described in the "Stand Out Suggestions" part of the rubric).

Here are the results of the prediction:

| Image			        |     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| Yield    		| Yield									| 
| Bumpy road    			| Bumpy road   									|
| Dangerous curve to the right					| Dangerous curve to the right												|
| General caution	      		| General caution				 				|
| No vehicles			| No vehicles								|

Images with predicted labels show blow:
![alt text][image6]

The model was able to correctly guess 5 of the 5 traffic signs, which gives an accuracy of 100%. 

Maybe there is some lucky choice about my final test images, so I evaluated 1000 images choose from the directory randomly, these images have different shape,  the evaluation accuracy result is 96.4%.

#### 3. Describe how certain the model is when predicting on each of the five new images by looking at the softmax probabilities for each prediction. Provide the top 5 softmax probabilities for each image along with the sign type of each probability. (OPTIONAL: as described in the "Stand Out Suggestions" part of the rubric, visualizations can also be provided such as bar charts)

The code for making predictions on my final model is located in the 25th cell of the Ipython notebook.

For the first image, the model is relatively sure that this is a stop sign (probability of 0.6), and the image does contain a stop sign. The top five soft max probabilities were

| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| 1.0000000e+00       			| Yield  									| 
| 1.2628540e-25     				| Speed limit (50km/h)										|
| 1.0890007e-26					| Priority road											|
| 3.6696055e-28	      			| No passing for vehicles over 3.5 metric tons				 				|
|  7.8182246e-30				    | No vehicles    							|


For the second image 

| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| 9.9758089e-01        			| Bumpy road  									| 
| 9.1148354e-04   				| Vehicles over 3.5 metric tons prohibited										|
| 6.3720439e-04					| Turn right ahead											|
| 5.0419249e-04	      			| General caution					 				|
| 3.2822657e-04			    | No passing   							|

For the third image 

| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| 1.0000000e+00       			| Dangerous curve to the right								| 
| 3.4456635e-10    				| Road work										|
| 4.2837543e-12					| Right-of-way at the next intersection										|
| 4.2970782e-15	      			| Speed limit (60km/h)				 				|
| 3.4165296e-17				    | Slippery road   							|

For the fourth image 

| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| 1.0        			| General caution									| 
| 0.0     				| Speed limit (20km/h)									|
| 0.0					| Speed limit (30km/h)										|
| 0.0	      			| Speed limit (50km/h)					 				|
| 0.0				    | Speed limit (60km/h)   							|

For the fifth image 

| Probability         	|     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| 1.0000000e+00        			| No vehicles 									| 
| 4.7081784e-15    				| Stop										|
| 2.3320602e-15					| Speed limit (60km/h)										|
| 2.7734379e-16	      			| Speed limit (80km/h)					 				|
| 1.5904953e-17			    | Vehicles over 3.5 metric tons prohibited   							|



