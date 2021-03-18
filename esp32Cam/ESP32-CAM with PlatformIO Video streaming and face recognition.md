https://www.survivingwithandroid.com/esp32-cam-platformio-video-streaming-face-recognition/

> Discover how to compile and run code for ESP32-CAM using PlatformIO. Learn how to enable video streaming and face recognition using our ESP32-CAM

# ESP32-CAM with PlatformIO: Video streaming and face recognition
This tutorial covers how to use **ESP32-CAM with PlatformIO**. This guide describes step by step how to compile and run ESP32-CAM sketch with PlatformIO. In more detail, we will cover how to compile and run CameraWebServer sketch so that you can enable:

*   video streaming
*   face recognition

and start implementing rich IoT applications. As you may already know, ESP32-CAM is an ESP32 with camera support (OV2640) with other interesting features.

Preparing PlatformIO
--------------------

Before starting with ESP32-CAM, it is necessary to install and configure the PlatformIO. In this ESP32-CAM tutorial, we will use the Visual Studio Code with PlatformIO plugin. Download [it](https://code.visualstudio.com/) and add the PlatformIO extension using the button as shown in the picture below:

![Visual Studio Code with PlatformIO](https://www.survivingwithandroid.com/wp-content/uploads/2020/05/visual-studio-code.png.webp)

Once you have clicked on the button you have to look for PlatformIO extension and install it.

Importing CameraWebServer into PlatformIO
-----------------------------------------

Let’s clone the Github repository containing the source code for CameraWebServer. Go to [Github](https://github.com/espressif/arduino-esp32/archive/master.zip) and download the repository. If you prefer you can clone it using this command:

git clone https://github.com/espressif/arduino-esp32.git

Open the PlatformIO extension and select _Import Arduino Project_:

![Import ESP-32 CAM project into PlatformIO](https://www.survivingwithandroid.com/wp-content/uploads/2020/05/import-camerawebserver-platformIO.png.webp)

When importing the project into PlatformIO select `AI Thinker ESP32-CAM` in the board box. Then move to `arduino-esp32/libraries/ESP32/examples/Camera/CameraWebServer` and import the project.

Please be sure that in the file named `platformio.ini` contains the following lines:

`[env:esp32cam]
platform = espressif32
board = esp32cam
framework = arduino`

Compile the code.

Uploading the code into ESP32-CAM from PlatformIO
-------------------------------------------------

To connect the ESP32-CAM to your PC USB port you need an adapter because ESP32-CAM doesn’t have an USB port. Therefore, you have to use FTDI232 that’s a USB serial converter. Connect the ESP32-CAM to the FTDI232 using the following schema:

![](https://www.survivingwithandroid.com/wp-content/uploads/2020/05/esp32-cam-ftdi232.png.webp)

If your FTDI232 has a jumper to set 3.3V or 5V set 5V.

Before uploading the code you have to connect the GPIO 0 to the GND.

### Customizing the CameraWebServer code

Before using the code into the ESP32-CAM, you have to apply some customization to the CameraWebServer code. Open `CameraWebServer.ino` using PlatformIO and modify the code as shown below:

1.  Uncomment this line

Code language: PHP (php)

Next:

`onst char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";`

Code language: JavaScript (javascript)

Now you are ready to compile and upload the code into your ESP32. Then you can stream your video from ESP32-CAM.

![PlatformIO run and compile with ESP32-CAM](https://www.survivingwithandroid.com/wp-content/uploads/2020/05/esp32-cam-platformIO-run.png.webp)

Once the uploading is complete, you have disconnect the GPIO 0 from the GND e restart the device.

Streaming Video from ESP32-CAM
------------------------------

Run the code and open the serial monitor in your PlatformIO. Notice to press the Reset button to start the application

![ESP32-CAM running on PlatformIO. WebServerCam URL to stream video](https://www.survivingwithandroid.com/wp-content/uploads/2020/05/esp32-cam-running.png.webp)

Now you can start streaming video from the ESP32-CAM. Open your browswer and copy the URL shown in the previous image:

![ESP32-CAM video streaming settings](https://www.survivingwithandroid.com/wp-content/uploads/2020/05/esp32-cam-ov2640-video-streaming-min-1024x536.png)

Here you can change some settings according to your need. Using Resolution settings you can change the image and video resolution. As you can see, using the browser it is possible to stream the video using the ESP32-CAM

Face detection and face recognition
-----------------------------------

To enable face recognition you have to ckeck two buttons at the end of the settings:

![ESP32-CAM face recognition settings](https://www.survivingwithandroid.com/wp-content/uploads/2020/05/face-recognition.png.webp)

If you enable the option Face detection as soon as the ESP32 detects a face, it creates a square around it. Moreover, you can press on Enroll Face to enroll a new face. Consider that it may be necessary several attempts before the device can enroll a new face. If you want to use ESP32-CAM with machine learning you can read [how to use Machine Learning with ESP32-CAM](https://www.survivingwithandroid.com/esp32-cam-image-classification-machine-learning/).

**Recommended**:  
[How to connect ESP32 to the smartphone through Node-RED](https://www.survivingwithandroid.com/connect-esp32-to-smartphone-node-red/)  
[ESP32 MQTT client: Publish and Subscribe. HiveMQ and BME280 example](https://www.survivingwithandroid.com/esp32-mqtt-client-publish-and-subscribe/)  
[Getting started with ESP32: Build a weather station (BMP280 + SSD1306)](https://www.survivingwithandroid.com/getting-started-with-esp32-build-a-weather-station/)

Wrapping up
-----------

At the end of this post, you have learning how to use ESP32-CAM with PlatformIO. Moreover, you have discovered how to stream video from ESP32-CAM and how to detect faces. ESP32-CAM is a powerful device that adds to the ESP32 the video capabilities. You can start creating interesting projects once you know how to develop a sketch for ESP32-CAM.
