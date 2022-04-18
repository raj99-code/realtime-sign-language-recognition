---
# Feel free to add content and custom Front Matter to this file.
# To modify the layout, see https://jekyllrb.com/docs/themes/#overriding-theme-defaults
layout: page
title: ASL using DNN
permalink: /journey/asldnn/

---
## Dataset
We used a training dataset of 87,000 images which comprised of 29 classes of [ASL Alphabet hand signs from kaggle](https://www.kaggle.com/datasets/grassknoted/asl-alphabet). Each image had a dimension of 200 x 200 pixels. Since, our project employs real-time application we used 3 classes for SPACE, DELETE and NOTHING in addition to the 26 letter classes.
The test data contains 29 images.

## Training
We implemented an ASL recognition approach using Deep Neural Network employed in [this source](https://github.com/VedantMistry13/American-Sign-Language-Recognition-using-Deep-Neural-Network). The data is augmented for real-world applications and then trained using Transfer Learning.
<p>
The data augmentation was realized by shifting the brightness upto 20% and zooming out upto 120% to incorporate all possible scenarios.
Transfer Learning is a popular enhancement technique in deep learning where a model pre-trained on one task is reused as the starting point of modeling a related second task on Computer Vision. This approach reduces training time from weeks of training and gives high performing CNN model for custom use case through the transfer of knowledge to a second task that was previously learned.
</p>
There are 2 approaches of transfer learning. One is Develop Model Approach where you develop a source model as the starting point and then reuse it for the second task. The second approach is the Pre-trained Model approach where you select a pre-trained source model from available models as the starting point and then reuse it for the second task. Then, it can further tuned and refined.
<p>
The latter approach of transfer learning was taken into the Inception V3 model where all the layers of the model is not used for training. The Fully Connected layers at the top of the network and the first 248 layers out of 311 layers are locked/removed. The data is trained on the remaining last 2 inception blocks and is given into our own Full Connected layers which is added after the inception network. The Fully Connected layers consists of 2 layers, the first one has 1024 ReLu units and the next one has 29 Softmax units.
</p>
The model is trained for 24 epochs. The loss was measured using Categorical Crossentropy 

## Using the model
The model is loaded into the application. There is a green rectangle where the hand signs should be shown in order to be identified. These signs are captured as frames using OpenCV from the video feed. The frames are used by the model to predict the ASL hand signs. 

## Results
The training accuracy and training loss was approximately 98.87% and 0.1100 respectively. The validation accuracy and validation loss was estimated to 95.75% and 0.1926 respectively. And, lastly the model gave a test accuracy of 96.43%.

## Conversion of the model
This trained model was converted to ONNX (Open Neural Network Exchange Format) format. Conversion to this format makes it easier to move between frameworks and is used to represent any kind of Machine Learning/Deep Learning model. To convert into ONNX format create an example input of the same shape our model would expect and load the pre-trained weights that must be in evaluation mode into it using onnx export.
![aslonnx]({{site.url}}/images/aslonnx.png)
