<p align="center">
<img src="kNOw SIGN-logo.png">
</p>

# KnowSign

Portable sign language (ASL) recognition device that utilizes real-time and efficient programming to help mute and deaf by establishing two-way communication channel with normal people who have never studied sign language.

KnowSign compact design consists of Raspberry Pi 4B, Pi camera (V2.1) and a display.

## Stay tuned!

## Table of Contents

1. [ Project Demo. ](#Project_Demo)
2. [ Project Flowchart. ](#flow)
3. [ kNOw SIGN Features. ](#kNOw_SIGN_Features)
4. [ Social Media Channels. ](#social_media)
5. [ Installation. ](#install)
6. [ Authors. ](#cont)
7. [ Project Progress. ](#progress)






<details>
<a name="Project_Demo"></a>
 <summary>## 1. Project Demo</summary>

To be added soon!
</details>
<a name="flow"></a>
## 2. Project Flowchart 

To be added soon!

<a name="kNOw_SIGN_Features"></a>
## 3. kNOw SIGN Features

1. Real-time American sign language recognition 
2. Sign language to text display
3. Fast and efficient deep learning algorithm 
4. Portable device for the recognition using Raspberry PI

more features will be available soon!

<a name="social_media"></a>
## 4. Social Media Channels
 
KnowSign is still under development. The project progress and advancements will be shared regularly on our social media channels below:  
 
 [Facebook ](https://www.facebook.com/KnowSign)    
 [Instagram](https://www.instagram.com/know.sign)    
 [Twitter](https://twitter.com/Know__Sign)  
 [Youtube](https://www.youtube.com/channel/UCDok7kkuFErYF-Gch3gnTTw)  

<a name="install"></a>
## 5. Installation 

  ### a.Prerequisites  
  #### OpenCV 4.5 on Raspberry Pi 4
  ##### Introduction
  This guide will help you install OpenCV 4.5.x on Raspberry Pi 4 with 32-bit operating system (it can also be used without any change for Pi 3 or 2).
  ##### Operating System
  
  Use a fast SD memory card for your Raspberry Pi 4. 100MB/s and a capacity of 32 GB is perfect.
The operating system is installed by the new Raspberry Pi Image Tool from (this site)[https://www.raspberrypi.org/downloads/]. The Image Tool can write an operating system of your choice on an SD card. At the same time, it will format the card into the correct ext4 for the Raspberry Pi, even a 64, 128 or even 256 GByte card. Different image formats are supported, making this tool a better alternative to balena Etcher.
The second step is downloading the OS of your choice for your Raspberry Pi 4. The slideshow below shows how easy the whole image writing process is. After successful installing the Raspbian operating system, it is time to update and upgrade your operating system with the next commands in the terminal.

![ezgif com-gif-maker](https://user-images.githubusercontent.com/97736043/159683878-76ac1146-342a-404c-a836-6873f4b3339e.gif)

##### Bad Ideas
Some words of warning. Do not use pip to install OpenCV on your Raspberry Pi. First of all, pip installations don't support C++ due to missing header files. If you want to write code in C++, as we like to do, never use pip. The same story applies to sudo apt-get install python3-opencv. The installed version of OpenCV (3.2.0 at the moment) neither supports C++. Another possible pitfall may be the repository. As soon as this ecosystem is updated with a newer version, a simple command like sudo apt-get upgrade will automatically install version 4.0.0 or higher which is incompatible with the 3 series

##### Swap Memory
The next step is to increase your swap space. OpenCV needs a lot of memory to compile. The latest versions want to see a minimum of 6.5 GB of memory before building. Your swap space is limited to 2048 MByte by default. To exceed this 2048 MByte limit, you will need to increase this maximum in the /sbin/dphys-swapfile. Everything is demonstrated in the slideshow below
```
# edit the swap configuration
$ sudo nano /sbin/dphys-swapfile
$ sudo nano /etc/dphys-swapfile
# reboot
$ sudo reboot
```
![22](https://user-images.githubusercontent.com/97736043/159685279-affb8ce5-7f22-49df-a837-ddeab57d109b.gif)

##### Installation 
Installing OpenCV on your Raspberry Pi is not that complicated. The installation will take one and a half hour to complete. It starts with the installation of the dependencies and ends with the ldconfig.

###### OpenCV 4.5.5 installation

```
# check your memory first
$ free -m
# you need at least a total of 6.5 GB!
# if not, enlarge your swap space as explained in the guide
$ wget https://github.com/Qengineering/Install-OpenCV-Raspberry-Pi-32-bits/raw/main/OpenCV-4-5-5.sh
$ sudo chmod 755 ./OpenCV-4-5-5.sh
$ ./OpenCV-4-5-5.sh
```

##### Dependencies
The OpenCV software uses other third-party software libraries. These have to be installed first. Some come with the Raspbian operating system, others may have been gathered over time, but it's better to be safe than sorry, so here is the complete list. Only the latest packages are installed by the procedure.

```
$ sudo apt-get update
$ sudo apt-get upgrade
$ sudo apt-get install cmake gfortran
$ sudo apt-get install python3-dev python3-numpy
$ sudo apt-get install libjpeg-dev libtiff-dev libgif-dev
$ sudo apt-get install libgstreamer1.0-dev gstreamer1.0-gtk3
$ sudo apt-get install libgstreamer-plugins-base1.0-dev gstreamer1.0-gl
$ sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev
$ sudo apt-get install libgtk2.0-dev libcanberra-gtk*
$ sudo apt-get install libxvidcore-dev libx264-dev libgtk-3-dev
$ sudo apt-get install libtbb2 libtbb-dev libdc1394-22-dev libv4l-dev
$ sudo apt-get install libopenblas-dev libatlas-base-dev libblas-dev
$ sudo apt-get install libjasper-dev liblapack-dev libhdf5-dev
$ sudo apt-get install protobuf-compiler
# The latest Debian 11, Bullseye don't support python2 full
# don't try to install if you're having a Raspberry Bullseye OS
$ sudo apt-get install python-dev python-numpy
```

###### Downloading OpenCV

When all third-party software is installed, OpenCV itself can be downloaded. There are two packages needed; the basic version and the additional contributions. Check before downloading the latest version at https://opencv.org/releases/. If necessary, change the names of the zip files according to the latest version. After downloading, you can unzip the files. Please be aware of line wrapping in the text boxes. The two commands are starting with wget and ending with zip.

```
# check your memory first
$ free -m
# you need at least a total of 6.5 GB!
# if not, enlarge your swap space as explained earlier
# download the latest version
$ cd ~
$ wget -O opencv.zip https://github.com/opencv/opencv/archive/4.5.5.zip
$ wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.5.5.zip
# unpack
$ unzip opencv.zip
$ unzip opencv_contrib.zip
# some administration to make live easier later on
$ mv opencv-4.5.5 opencv
$ mv opencv_contrib-4.5.5 opencv_contrib
# clean up the zip files
$ rm opencv.zip
$ rm opencv_contrib.zip
```

##### Build Make

Before we begin with the actual build of the library, there is one small step to go. You have to make a directory where all the build files can be located.

```
$ cd ~/opencv/
$ mkdir build
$ cd build
```

Now it is time for an important step. Here you tell CMake what, where and how to make OpenCV on your Raspberry. There are many flags involved. The most you will recognize. You will probably notice the -D WITH_QT=OFF line. Here Qt5 support is disabled. Set -D WITH_QT=ON if you choose to use the Qt5 software for the GUI. We save space by excluding any (Python) examples or tests. There are only bare spaces before the -D flags, not tabs. By the way, the two last dots are no typo. It tells CMake where it can find its CMakeLists.txt (the large recipe file); one directory up.

```
$ cmake -D CMAKE_BUILD_TYPE=RELEASE \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib/modules \
-D ENABLE_NEON=ON \
-D ENABLE_VFPV3=ON \
-D WITH_OPENMP=ON \
-D WITH_OPENCL=OFF \
-D BUILD_ZLIB=ON \
-D BUILD_TIFF=ON \
-D WITH_FFMPEG=ON \
-D WITH_TBB=ON \
-D BUILD_TBB=ON \
-D BUILD_TESTS=OFF \
-D WITH_EIGEN=OFF \
-D WITH_GSTREAMER=ON \
-D WITH_V4L=ON \
-D WITH_LIBV4L=ON \
-D WITH_VTK=OFF \
-D WITH_QT=OFF \
-D OPENCV_ENABLE_NONFREE=ON \
-D INSTALL_C_EXAMPLES=OFF \
-D INSTALL_PYTHON_EXAMPLES=OFF \
-D PYTHON3_PACKAGES_PATH=/usr/lib/python3/dist-packages \
-D OPENCV_GENERATE_PKGCONFIG=ON \
-D BUILD_EXAMPLES=OFF ..
```


If everything went well, CMake generates a report that looks something like this (for readability purposes we omitted most lines). Very crucial are the Python sections. If these are missing, OpenCV will not install proper Python libraries. In that case, usually CMake could not find the Python folders, or in the case of a virtual environment, a dependency such as numpy is probably not installed within the environment. . NEON and VFPV3 support must also be enabled. Certainly if you intend to build our deep learning examples. Check if v4l/v4l2 is available if your planning to use the Raspicam.

```
-- General configuration for OpenCV 4.5.2 =====================================
--   Version control:               unknown
--
--   Extra modules:
--     Location (extra):            /home/pi/opencv_contrib/modules
--     Version control (extra):     unknown
--
--   Platform:
--     Timestamp:                   2021-05-09T10:16:42Z
--     Host:                        Linux 5.10.17-v7l+ armv7l
--     CMake:                       3.16.3
--     CMake generator:             Unix Makefiles
--     CMake build tool:            /usr/bin/make
--     Configuration:               RELEASE
--
--   CPU/HW features:
--     Baseline:                    VFPV3 NEON
--       requested:                 DETECT
--       required:                  VFPV3 NEON
--
--   C/C++:
--     Built as dynamic libs?:      YES
--     C++ Compiler:                /usr/bin/c++  (ver 8.3.0)
***********************
--     C Compiler:                  /usr/bin/cc
***********************
--     Documentation:               NO
--     Non-free algorithms:         YES
***********************
--   Media I/O:
--     ZLib:                        /usr/lib/arm-linux-gnueabihf/libz.so (ver 1.2.11)
--     JPEG:                        /usr/lib/arm-linux-gnueabihf/libjpeg.so (ver 62)
--     WEBP:                        build (ver encoder: 0x020f)
--     PNG:                         /usr/lib/arm-linux-gnueabihf/libpng.so (ver 1.6.36)
--     TIFF:                        build (ver 42 - 4.2.0)
--     JPEG 2000:                   build (ver 2.4.0)
--     OpenEXR:                     build (ver 2.3.0)
--     HDR:                         YES
--     SUNRASTER:                   YES
--     PXM:                         YES
--     PFM:                         YES
--
--   Video I/O:
--   DC1394:                      YES (2.2.5)
--   FFMPEG:                      YES
--     avcodec:                   YES (58.35.100)
--     avformat:                  YES (58.20.100)
--     avutil:                    YES (56.22.100)
--     swscale:                   YES (5.3.100)
--     avresample:                NO
--   GStreamer:                   NO
--   v4l/v4l2:                    YES (linux/videodev2.h)
--
--   Parallel framework:            TBB (ver 2020.2 interface 11102)
***********************
--   Python 2:
--     Interpreter:                 /usr/bin/python2.7 (ver 2.7.16)
--     Libraries:                   /usr/lib/arm-linux-gnueabihf/libpython2.7.so (ver 2.7.16)
--     numpy:                       /usr/lib/python2.7/dist-packages/numpy/core/include (ver 1.16.2)
--     install path:                lib/python2.7/dist-packages/cv2/python-2.7
--
--   Python 3:
--     Interpreter:                 /usr/bin/python3 (ver 3.7.3)
--     Libraries:                   /usr/lib/arm-linux-gnueabihf/libpython3.7m.so (ver 3.7.3)
--     numpy:                       /usr/lib/python3/dist-packages/numpy/core/include (ver 1.16.2)
--     install path:                lib/python3.7/dist-packages/cv2/python-3.7
--
--   Python (for build):            /usr/bin/python2.7
--
--   Java:                          
--     ant:                         /usr/bin/ant (ver 1.10.5)
--     JNI:                         NO
--     Java wrappers:               NO
--     Java tests:                  NO
--
--   Install to:                    /usr/local
-- -----------------------------------------------------------------
--
-- Configuring done
-- Generating done
-- Build files have been written to: /home/pi/opencv/build

```
##### Make
Now everything is ready for the great build. This takes a lot of time. Be very patient is the only advice here. Don't be surprised if at 99% your build seems to be crashed. That is 'normal' behaviour. Even when your CPU Usage Monitor gives very low ratings like 7%. In reality, your CPU is working so hard it has not enough time to update these usage numbers correctly.
You can speed things up with four cores working simultaneously (make -j4). On a Raspberry Pi 4, it takes just over an hour to build the whole library. Sometimes the system crashes for no apparent reason at all at 99% or even 100%. In that case, restart all over again

You can check the number of cores available by running this command and change -j flag to the output number.
```
$ nproc
``` 
So take a coffee and start building the library with:

```
$ make -j4
```
Now to complete, install all the generated packages and update the database of your system with the next commands.

```
$ sudo make install
$ sudo ldconfig
# cleaning (frees 300 KB)
$ make clean
$ sudo apt-get update
```

There is one thing left to do before the installation of OpenCV 4.5 on your Raspberry Pi 4 is completed. That is resetting the swap space back to its original 100 Mbyte. Flash memory can only write a limited number of cycles. In the end, it will wear your SD card out. It is therefore wise to keep memory swapping to a minimum. Besides, it also slows down your application. The final commands are deleting the now useless zip files and rebooting your system so that all changes are implemented.

```
$ sudo nano /etc/dphys-swapfile

set CONF_SWAPSIZE=100 with the Nano text editor

$ cd ~
$ rm opencv.zip
$ rm opencv_contrib.zip
$ sudo reboot

```

##### Checking

You can check your installation by running the following command which will output the OpenCv version if it was installed correctly:

```
$  pkg-config opencv --cflags 
```




  
  
  ### b.Build from soucre     
  
<a name="cont"></a>
## 6. Authors 
<a href="https://github.com/raj99-code/realtime-sign-language-recognition/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=raj99-code/realtime-sign-language-recognition" />
</a>

<a name="progress"></a>
## 7. Project Progress 
