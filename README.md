# StarVoyager Defender

A space-shooter arcade game built with **Qt 5.15.2** for embedded systems, designed to run on the **BeagleBone Black** with a 480×272 LCD display and GPIO-based hardware controls.

---

## Overview

StarVoyager Defender is a wave-based side-scrolling shooter where players control a spaceship, avoid incoming enemies, and survive progressive difficulty phases. The game integrates GPIO buttons for movement and a three-color LED module to display player health. It is implemented in **C++ using Qt 5.15.2**, rendering directly to the BBB's Linux framebuffer (`/dev/fb0`).

---

## Features

- Wave-based enemy spawning system  
- Three enemy types: **Asteroid**, **StraightShip**, **SineShip**  
- GPIO button input for player movement  
- LED health indicator (green/yellow/red)  
- Touchscreen pause/resume  
- High score saved across sessions  
- Animated title screen and credits panel  

---

## Controls

### Hardware Buttons
- **Up / Down / Left / Right** → Move player ship  
(Buttons use **internal pull‑ups** and pull the pin to GND when pressed.)

### Touchscreen
- Tap to pause/resume gameplay  
- Tap menu buttons for navigation  

---

## Enemy Types

- **Asteroid** – Straight movement, no shooting  
- **StraightShip** – Straight movement, random firing  
- **SineShip** – Sine-wave movement, random firing  

---

## Dependencies

The following must be installed on the **BeagleBone Black**:

### Qt 5.15.2  
Install using:
```bash
sudo apt-get update
sudo apt-get install qtbase5-dev qt5-qmake
```

Qt Docs: https://doc.qt.io/qt-5/

### Linux GPIO sysfs  
Used via `/sys/class/gpio`.  
Docs: https://www.kernel.org/doc/Documentation/gpio/sysfs.txt

### LCD Cape Support  
Requires BBB Debian image with framebuffer (`/dev/fb0`).  
Docs: https://elinux.org/Beagleboard:BeagleBone_LCD3

No external libraries must be downloaded manually.

---

## Hardware Connections

### Buttons (using internal pull-ups)

| Direction | GPIO Pin |
|----------|----------|
| Left     | 61 |
| Right    | 67 |
| Up       | 44 |
| Down     | 65 |

### LED Health Indicator

| LED    | GPIO Pin |
|--------|----------|
| Green  | 49 |
| Yellow | 117 |
| Red    | 115 |

---

## Repository Structure
```
StarVoyager_Defender/
├── GameScene.cpp / GameScene.h
├── Player.cpp / Player.h
├── Enemy.cpp / Enemy.h
├── Bullet.cpp / Bullet.h
├── Explosion.cpp / Explosion.h
├── hardwareio.h
├── MainWindow.cpp / MainWindow.h / MainWindow.ui
├── SpaceDefender.pro
└── assets/
```

---

## Building and Running

### 1. Clone the repository
```bash
# Run on development machine (Linux-based)
git clone https://github.com/te1p/EC535_Lab5.git
cd EC535_Lab5/StarVoyager_Defender
```

### 2. Generate Makefile with qmake
```bash
qmake
# Notice how qmake generates a Makefile from the .pro file
```

### 3. Build
```bash
make
```

### 4. Transfer to BeagleBone Black
```bash
# From your development machine (e.g., WSL)
scp SpaceDefender root@192.168.7.2:/root/

Alternate can be through sdcard image building.
```

### 5. Run on BeagleBone Black
```bash
# Run these commands on your BeagleBone
cd /root
./SpaceDefender
```

The game renders directly to `/dev/fb0` using Qt's framebuffer backend, similar to how the Qt demo applications work:
```bash
./qt-examples/widgets/touch/fingerpaint/fingerpaint
```

---

## Replication Instructions (for Instructors)

To fully reproduce this project on a new BeagleBone Black:

### 1. Prepare the BeagleBone environment

Ensure your BBB has:
- Debian image with LCD cape support
- Active framebuffer at `/dev/fb0`
- Qt 5.15 development tools installed

You can verify framebuffer access by running:
```bash
# On your BeagleBone
echo 0 > /sys/class/vtconsole/vtcon1/bind
dd if=/dev/zero of=/dev/fb0
# Screen should go black
echo 1 > /sys/class/vtconsole/vtcon1/bind
```

### 2. Wire hardware components

Connect four buttons and three LEDs according to the GPIO pin table above. Buttons should use internal pull-ups (pin pulled to GND when pressed).

### 3. Build and deploy
```bash
# On your development machine
git clone https://github.com/te1p/EC535_Lab5.git
cd EC535_Lab5/StarVoyager_Defender
qmake
make

# Transfer to BeagleBone
scp SpaceDefender root@192.168.7.2:/root/
```

### 4. Run on BeagleBone Black

Connect to your BeagleBone via serial console or SSH, then:
```bash
cd /root
./SpaceDefender
```

### 5. Interact with the game

Use the GPIO buttons to control the spaceship and observe the LED health indicator changing color as you take damage. The touchscreen can be used to pause/resume gameplay.

No kernel modules, custom drivers, or additional libraries are required. The game operates entirely in userspace, accessing GPIO through sysfs and rendering through Qt's framebuffer interface.

---

## Implementation Notes

### Framebuffer Rendering
Like the Qt demo applications provided in lab, StarVoyager Defender renders directly to `/dev/fb0` using Qt's EGLFS or LinuxFB platform backend.

### GPIO Access (Userspace)
Hardware input/output is handled through Linux's GPIO sysfs interface (`/sys/class/gpio`), enabling userspace control without kernel modules. The `hardwareio.h` module manages pin export, direction configuration, and state reading/writing.

### Touchscreen Input
The BeagleBone's touchscreen is accessed as an event device (`/dev/input/event0`), similar to the HID exploration example in lab. Qt automatically handles touch events when running on the framebuffer.

---

## Future Improvements

- More modular class organization  
- Timer-based enemy attack patterns  
- Additional enemy behaviors  
- Boss phase  
- Improved movement responsiveness  
- Experimental multiplayer mode  

---

## Authors

- **Phyliss Darko**  
- **Tymoteusz Pardej**  
- **Dhiraj Simhadri**
