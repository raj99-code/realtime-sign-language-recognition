---
# Feel free to add content and custom Front Matter to this file.
# To modify the layout, see https://jekyllrb.com/docs/themes/#overriding-theme-defaults
layout: page
title: Hand Detection Models
permalink: /journey/handdet/

---
## 1. Real-time hand detection using neural networks on tensor flow
[This model](https://github.com/victordibia/handtracking) is trained using the Egohands Dataset which is advantageous in real world applications because these images have egoocentric view across different environment. To reduce the time of learning, this approach used Transfer  Learning with a pre-trained model from Tensorflow(ssd_mobilenet_v1_coco) as the starting point.

Then trained models are saved as '.pb' files. However, for the augmentation of the model with OpenCV required for our purpose, we would need to convert it into a more compatible format like .tflite.

But unfortunately we failed using this model as we faced several conversion issues, as the conversion is not version compliant. Furthermore, the neural network had FlexTensorArray nodes, which are not supported by TFLite in C++ and Raspberry Pi.

We considered [another TFLite implementation of this model](https://github.com/shubham0204/Google_Colab_Notebooks/blob/main/Hand_Tracking_Model_TFLite_Conversion.ipynb), however, it was not supported as it was depended on various Java libraries which was not available in the C++ scope of TFLite.

## 2. Hand detection using C++ and OpenCV
[This approach](https://github.com/PierfrancescoSoffritti/handy) tries to extract the hand from the video feed by segmentation. First, the color of skin is determined when the user moves his hand to the sample area so as to take samples and the binarization of the frame is done. Next, the user's face is removed using identification via the FrontalFace haar cascade and then replacing it with a black rectangle. For background removal, the first frame is saved as reference frame and is compared with each new frame.

After binarization, the contour with biggest area will be the hand. So we draw bounding rectangle using the convexHull function, so that the approximate centre of the hand can be determined. To determine the finger tips, the intersections between the hand contour and the convexHull are saved in an array. To determine the lowest point of the gap between the fingers, convexityDefects function is used and is saved in an array.

But with this approach, we observed that the bounding rectangle is obtained and cropping it works, but the accuracy is compromised. Also, the face and background removal did not work every time.

## 3. Hand Detection using color recognition
This is the [current approach](https://github.com/simenandresen/handDetectionCV) using OpenCV. The user places hand over the rectangles to get median color sample values, then several binary representatation are determined on each sample. They are summed up and a median blur is applied to get the final binary image. Then get the convex points in contour and filter out the convexity defects that are not relevant. From this stage we can informations such as fingertips, number of fingers, number of hands and area of hand.
![currentflow]({{site.url}}/images/currentflow.png)
