# StarVoyager Defender

A space-shooter arcade game built with **Qt v5.15.2** for embedded systems, designed to run on the **BeagleBone Black** with a 480×272 LCD display and hardware button/LED peripherals.

## Overview

StarVoyager Defender is a wave-based space shooter where players control a spaceship, dodge obstacles, and destroy enemies. The game features progressive difficulty, multiple enemy types, and integrates with BeagleBone GPIO for physical button controls and LED health indicators.

## Features

- Wave-based enemy spawning
- Three enemy types: Asteroids, StraightShips, and SineShips
- GPIO button input and LED health feedback
- Touchscreen pause/resume support
- High score tracking

## Controls

- GPIO buttons for Up, Down, Left, Right movements
- Touchscreen for main menu navigation and pause/resume

## Enemy Types

- Asteroid: Drifts across screen
- StraightShip: Flies straight, shoots
- SineShip: Sine-wave movement, shoots

## Building and Running

`cd StarVoyager_Defender`
`qmake SpaceDefender.pro`
`make`
`./SpaceDefender`

## Future Improvements

- Organize fields and properties between classes
- Modify enemy attacks to work on a timer rather than chance
- Increase enemy variety
- Add player tracking for new enemy types
- Multiplayer support
- Include a final boss enemy
- Better controls for movement

## Authors

- Phyliss Darko
- Tymoteusz Pardej
- Dhiraj Simhadri
