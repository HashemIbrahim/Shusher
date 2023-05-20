# Shusher

## Table of Contents

1. [Shusher Showcase Video](#shusher-showcase-video)
2. [Introduction](#introduction)
3. [Description](#description)
4. [Hardware and Software Architecture](#hardware-and-software-architecture)
5. [Visuals](#visuals)
6. [Installation](#installation)
7. [Purpose](#purpose)
8. [Benefits](#benefits)
9. [License](#license)
10. [Support](#support)
11. [Roadmap](#roadmap)
12. [Authors and acknowledgment](#authors-and-acknowledgment) 

---

## Shusher Showcase Video

[![Capture](/uploads/65ade24ef1675763dde1934d9db75aa0/Capture.PNG)](https://www.youtube.com/watch?v=n39D97m3lp4)

Click on video above ^^^^

---

## Introduction
Are you susceptible to waking up your neighbours late at night? Do you wish for an easy solution to keep your noise levels at a minimum? Well look no further! With the **Shusher**, a technical marvel at voice level detection, will you not only be able to easily visually detect when your noise levels are getting out of hand, but you are also able to say you finally made a worthwhile purchase. 

---

## Description
The **Shusher** is a device that visually shows a User when they are crossing a given noise level threshold. (Thresholds can be changed in the settings) Lights ranging from green to red (In the default theme) will display low to high audio levels respectively. The sensitivity of the device changes based on how far away you are from it ranging from less sensitive when you are close and more sensitive when you are far away.

---

## Hardware and Software Architecture
Welcome to the hardware and software architecture section of the README. In this section, we will provide an overview of the system's architecture, both in terms of hardware and software components.

### System design
The system diagram illustrates the connections and roles of the different parts of the system. It provides a visual representation of how the hardware components interact with each other and the application to support the functionality of the Shusher system.
![Image Description](https://previews.dropbox.com/p/thumb/AB5WHdfN3Rwtg4W_NEGv0gKzhxb3kdTlGBqF7AbiXunIV4R812GT5hexzhPvhruD_dGSzForxOSYbQ2GI6mK2GdN6OW6vxrgrWP0LH--4QhWPu23vOFIc7CnRWcGUTbLYG7w0TEBPheVsThALRMb6eXRpL_6hBMGhKVt5AAfsVcKRvCdzLGLMEPMaygxh5E_mxkef68yZr4S9XRLj180gxK-MFtV00b7HF_9vvWB0rPACbVHKMlLnUKk2SWajfnDNXUHFMRMMbPPvjnv3EGVvkde0Pw_HFL3y2igF5VOmq-GIJvewl952ZArTrePwqRmg6NPNk2imubWH4EYItCa9rmZORY_ZG9WmRxG3EdAVXQugvFN1czigPpK7agIR_TbZczJ17kPITRAxHhvZ-I6sst-/p.png)

### Software Architecture
The integration of the WIO Terminal and sensors into the system involves the use of the C programming language. By utilizing C, we have been able to establish seamless communication and data exchange between the WIO Terminal and the sensors, ensuring accurate and reliable readings. 

The application itself is built using JavaFX, a framework for creating desktop and mobile applications with a rich user interface. Leveraging capabilities and advantages of scene-builder within JavaFX, we have developed a user-friendly and visually appealing interface for the Shusher application. The software architecture is shown in the class diagram below. This diagram offers a detailed view of the software components, their relationships, and the overall structure of the application.
![Image Description](https://previews.dropbox.com/p/thumb/AB6X300xSMyMcBhu0Q5H7hSeeIp3kbdGUZfjEa-eQh-_ABfVwdsdEfE0JBmMQR24oLQiHcylk1eUTtrQYRaQL72_NBM56KkqNboUtTQMcMQAxLQMOpJFytyITMA8v_b2usGfcsC3YAkTTTg6YJdciSqkaPNPh2X5dzwWCEN41FVzVHud-Td_JNq4RWBbZ49ARAiy8-wDUsN_27nxjPlvWXj7Vu-VBBxpXPXgLxbZbppqSPlo6v7H9K3KSmeTYKxJ0APB4FWKqkIW472XoI7-M3V0t37rzKwvm5iMTuFBfZMbRIB1OMa6aTpE1ejM0gAjrIxf2_axWWgq_pbHdG-eMbnm-mQXs8RZaKAUKNaXSEuuzf15YIk-szxzCdN7uvG6onNzQcmU6XT3Hk4GuKT2vrje/p.png)
There were issues when exporting the application to an executable jar file, therefore a 'FakeMain' class was created. The 'FakeMain' class brings no functionality to the system design. It is only used to call the 'MainApplication' class. By having a 'FakeMain', we were able to export the application as an executable jar file and make the program customer friendly.

## Visuals

### Shusher detecting audio
![ezgif.com-video-to-gif](/uploads/edf9dde10f6efb3528ee197de6780af4/ezgif.com-video-to-gif.gif)

### Shusher changing themes
![ezgif.com-video-to-gif__1_](/uploads/6cccd9b40bd454f276159119810e86ce/ezgif.com-video-to-gif__1_.gif)

### Shusher alerting that the user is being too loud
![ezgif.com-video-to-gif__2_](/uploads/0f814d16feb1fee6de77345c28ed1049/ezgif.com-video-to-gif__2_.gif)

---

## Installation
### Prerequisites
- Java 17 [Download here](https://www.oracle.com/java/technologies/javase/jdk17-archive-downloads.html)
- Eclipse mosquitto [Download here](https://mosquitto.org/download/)
- Shusher Version 1.0 [Download here]
- Arduino IDE [Download here](https://www.arduino.cc/en/software)

### System setup

#### Step 1: Set up local Eclipse mosquitto message broker
The system uses a combination of sensors and actuators that communicate with a Java application through MQTT. This system uses the Eclipse Mosquitto message broker. If you have installed and know how to launch your own local message broker, move to [step 2](#step-2-:-edit-configuration-files).
- Locate the mosquito directory.
- Open "mosquitto.conf".
- Under "General configuration", add two new lines [1] "listener 1883 0.0.0.0", [2] "allow_anonymous true"
- Open terminal and navigate to the Mosquitto directory.
- Launch local broker by typing "mosquitto -c mosquitto.conf -v".

#### Step 2: Edit configuration files
The Shusher device and the application both need to connect to the same network that the local broker is running on. Therefore, configuring your network settings is required to use the application. In the Version1.0 folder, you will find two folders. You need to specify your network settings in both folders. Starting with the "Shusher App" folder:
- Open "config.txt".
- The first line indicates your WiFi SSID. Use the default configuration (tcp://localhost:1883) if you're running a local message broker. Otherwise, specify the IP address of the network the broker is connected to instead of "localhost".
- The second line is the client name of the application. This name will be seen in the message broker.

Moving on to the configuration file inside the "ShusherCode" folder:
- Plug in the WIO terminal to your computer.
- Open "shusherCode.ino".
- Click on the WiFiSetup.h tab.
- Follow the instructions provided in the file.
- Compile and upload the program to the WIO terminal.

#### Step 3: Launch application 
The required configurations are now complete, and the application is ready to launch. To launch the application:
- Navigate to the Shusher App folder.
- Launch the application by double-clicking the executable jar file named "ShusherApplication".

#### Common Problems
> ##### Eclipse mosquitto broker
> If you receive an error message saying "port already in use" when launching the Eclipse Mosquitto message broker. 
> - Open terminal as an administrator and type "netstat -ano | findstr "1883"".
> - Note the numbers after "Listening:"
> - Type "taskkill /F /PID XXXX". Replace XXXX with the four numbers.
> - Re-run mosquitto.

> ##### Launching Shusher Application
> If you can't run the application by double clicking the executable jar file.
> - Open terminal and navigate to the jar file directory. 
> - Run "java -jar ShusherApplication.jar" and the application should run.
> - If you're still having issues launching the application, try downloading Java 17.0.5.

---

## Purpose

The Shusher's main purpose is to be of great support to those in need of audio visualizers to be wary of how loud they are being. Whether you find yourself in a intense match of a video game, a heated debate over discord, or a book club on teams, having trouble volume control is an universal problem. 

This is where the Shusher comes in. Being composed of an ultrasonic ranger, a rgb light strip, and a microphone, the Shusher uses all these sensors to collaberatively detect and visualize how loud an user is. With addition of a GUI responding differently to your volume, the Shusher actively educates and warns the user on their current volume. This, hopefully, will keep a user's voice in check and consequentively save his/her neighbours from a loud disturbance in the area.

---

## Benefits
- Compact Device
- Easy Setup
  - The Shusher only requires a user to plug in the device and have a WiFi connection.
- Customizable
  - The Shusher can be extensively customized upon three different themes
- Open Source
  - The Shusher Code is licensed under MIT and is open source for all to manipulate and grow the product even further

---

## License

Shusher project is licensed under [MIT](https://git.chalmers.se/courses/dit113/2023/group-14/shusher/-/blob/ReadMe-updates/LICENSE)  

Copyright (c) 2023 courses / DIT113 / 2023 / Group-14

---

## Support
Any further assistance and questions with the **Shusher** can be forwarded to:

shushercustomerservice@gmail.com

---

## Roadmap

### **1.1** ShusherVibbo (2025)
- Added vibration to generate haptic feedback for the User

### **1.2** ShusherHome (2032)
- Added connectivity between **ShusherHome** and house lights (sponsered companies only)

---

## Authors and acknowledgment

A standing ovation to the following board members who all contributed in their respective ways on every sprint delivery and the final product with special mentions to:
- Mika Rannisto:
  For designing and implementing the Wio gui, setting up automated build and CI/CD. Helping with ultrasonic ranger code and refactoring. Doing crucial work on connecting sensors and merging files. User manual and ReadMe text contributions as well. 
- Hashem Ibrahim:
  For implementing the functionality of inputting data from the wio mic and recording sound, converting it into usable values to display and be used around the code and implementing ultrasonic ranger functionality. Doing minor work on a wio gui MVP. Doing major work on backend structuring, debugging and modularisation.
- Johan Engstrom:
  For implementing the displaying of the data using the LED strip and implementing mqtt into the Arduino code and making sure it publishes and subscribes values. Doing backend formatting. For working closely on connecting the Java gui to the Arduino code.
- Ashor Abraham:
  For project management such as management of the readme, sprint deliveries, code review rules, and handling merge requests and code reviewing. 
- Isaac Ternbom:
  For being project manager and keeping the team on track. Doing extremely vital work on the whole front end and developing the Java gui and setting up mqtt. Cleaning up the repository and managing issue tracking.
- Abdullahi Mahamed
  For Initial work on the loudness sensor and automated build. Working on the wiki pages and formatting the home page.
