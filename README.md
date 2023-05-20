# Shusher

## Table of Contents

1. [Shusher Showcase Video](#shusher-showcase-video)
2. [Introduction](#introduction)
3. [Description](#description)
4. [Hardware & Software Architecture](#hardware-&-software-architecture)
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

## Hardware & Software Architecture

---

## Visuals

### Shusher detecting audio
![ezgif.com-video-to-gif](/uploads/edf9dde10f6efb3528ee197de6780af4/ezgif.com-video-to-gif.gif)

### Shusher changing themes
![ezgif.com-video-to-gif__1_](/uploads/6cccd9b40bd454f276159119810e86ce/ezgif.com-video-to-gif__1_.gif)

### Shusher alerting that the user is being too loud
![ezgif.com-video-to-gif__2_](/uploads/0f814d16feb1fee6de77345c28ed1049/ezgif.com-video-to-gif__2_.gif)

---

## Installation
>TODO
>>Will have instructions on what programming language to have downloaded and which App to download as well. 

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
