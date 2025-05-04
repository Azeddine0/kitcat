# kitcat

A fun ASCII-based endless runner game where you control a cat jumping over fences and obstacles while collecting power-ups.

## Table of Contents
- [Game Overview](#game-overview)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [How to Play](#how-to-play)
  - [Controls](#controls)
  - [Objective](#objective)
  - [Getting Started](#getting-started)
  - [Basic Gameplay](#basic-gameplay)
  - [Advanced Techniques](#advanced-techniques)
  - [Scoring System](#scoring-system)
- [Game Mechanics](#game-mechanics)
- [Power-ups](#power-ups)
- [Character Types](#character-types)
- [Customization](#customization)
- [Troubleshooting](#troubleshooting)
- [Development Notes](#development-notes)

## Game Overview

Fence Jumper is a C++ console-based endless runner game inspired by the Chrome T-Rex game. You control a cat that must jump over fences and obstacles while collecting power-ups to increase your score and unlock special abilities.

## Features

- ASCII art graphics
- Multiple character types (Normal Cat, Ninja Cat, Space Cat)
- Various obstacles (fences, cacti, boss enemies)
- Power-up system (milk, fish, catnip, yarn)
- Day/night cycle with visual changes
- Parallax scrolling background
- Level progression system
- Lives system with invincibility frames
- Double jump ability
- Ducking/sliding mechanic
- High score tracking
- Settings menu
- Sound effects

## Requirements

- Windows operating system
- C++ compiler (Visual Studio, MinGW, Dev-C++, etc.)
- Windows.h and related libraries

## Installation

### Method 1: Compile from Source

1. Save the game code to a file named `fence_jumper.cpp`
2. Open your preferred C++ IDE or command line
3. Compile the code:

#### Using Visual Studio:
- Open Visual Studio
- Create a new C++ Console Application
- Replace the default code with the game code
- Build and run the project (F5)

#### Using MinGW:
```bash
g++ -o fence_jumper.exe fence_jumper.cpp -lwinmm

###Using Dev-C++:
- Open Dev-C++
- Create a new C++ source file
- Paste the game code
- Save as fence_jumper.cpp
- Compile & Run (F11)

### Method 2: Download Pre-compiled Binary

1. Download the `fence_jumper.exe` file from the releases section
2. Double-click to run the game

## How to Play

### Controls
- **SPACE** - Jump (press twice for double jump when available)
- **DOWN ARROW** - Duck under obstacles
- **P** - Pause game
- **R** - Restart game (when paused or game over)
- **Q** - Quit game (when paused or game over)
- **S** - Settings menu

### Objective
- Jump over obstacles to avoid collisions
- Collect power-ups to gain special abilities
- Survive as long as possible and achieve a high score
- Progress through levels to unlock new cat characters

### Getting Started
1. Launch the game by running the executable
2. You'll see the title screen with the game name and a cat character
3. Press any key to start the game
4. The tutorial will appear at the top of the screen for your first few games
5. Your cat will automatically start running

### Basic Gameplay
1. **Obstacles**: Various fences and cacti will appear from the right side of the screen
2. **Jumping**: Press SPACE to jump over obstacles
3. **Ducking**: Press DOWN ARROW to duck under high obstacles
4. **Lives**: You start with 3 lives, shown at the top of the screen
5. **Game Over**: When you lose all lives, the game ends and shows your final score

### Advanced Techniques
1. **Double Jump**: After collecting the catnip power-up, press SPACE while in mid-air to perform a second jump
2. **Timing**: Different obstacles require different timing - small fences need a quick jump, while larger ones require jumping earlier
3. **Boss Enemies**: Occasionally, dog bosses will appear that are larger and move faster than regular obstacles
4. **Day/Night Cycle**: The game transitions between day and night, changing the visuals but not affecting gameplay
5. **Power-up Strategy**: Save your double jump ability for when you really need it, especially when multiple obstacles appear in sequence

### Scoring System
- Your score increases automatically as you run
- Collecting milk bottles adds 50 points to your score
- Every 1000 points advances you to the next level
- Higher levels increase the game speed and obstacle frequency
- Your high score is saved between game sessions

## Game Mechanics

### Jumping
Press SPACE to make your cat jump. The cat will automatically return to the ground after reaching the peak of its jump.

### Double Jumping
After collecting the catnip power-up, you can press SPACE again while in mid-air to perform a double jump.

### Ducking
Press DOWN ARROW to duck under obstacles. Your cat will have a smaller hitbox while ducking.

### Lives System
You start with 3 lives. Each collision with an obstacle costs one life. After losing all lives, the game ends.

### Invincibility
After being hit, your cat becomes temporarily invincible (flashing effect). You can also gain invincibility by collecting the yarn power-up.

### Level Progression
Every 1000 points, you'll advance to the next level. The game becomes progressively harder with each level, but you'll unlock new cat characters at levels 3 and 5.

### Day/Night Cycle
The game features a day/night cycle that changes the background visuals. During night time, stars appear in the sky.

## Power-ups

The game features four types of power-ups:

1. **Milk Bottle (M)** - Adds 50 points to your score
2. **Fish (<><)** - Increases your speed temporarily
3. **Catnip (*)** - Grants double jump ability for a limited time
4. **Yarn Ball (O)** - Provides temporary invincibility

## Character Types

As you progress through the game, you'll unlock different cat characters:

1. **Normal Cat** (Level 1) - Standard abilities
2. **Ninja Cat** (Level 3) - Darker appearance
3. **Space Cat** (Level 5) - Cyan-colored with special effects

## Customization

You can customize various aspects of the game through the Settings menu (press S):

1. **Difficulty** - Easy, Medium, or Hard (affects game speed)
2. **Cat Type** - Choose between unlocked cat characters
3. **Tutorial** - Toggle the tutorial display on/off
4. **Reset High Score** - Clear your saved high score

## Troubleshooting

### Game window is too small or text is garbled
- The game attempts to set the console size automatically, but some Windows configurations may prevent this
- Try running the command prompt as administrator
- Manually set the console window size to at least 80x25 characters

### Compilation errors
- Make sure you're using a Windows-compatible C++ compiler
- Check that you have the required libraries (windows.h, mmsystem.h)
- Add the winmm library to your linker settings if sound doesn't work

### Game runs too fast/slow
- Adjust the difficulty in the Settings menu
- Modify the `gameSpeed` variable in the code if needed

## Development Notes

### Code Structure
- The game uses a simple game loop with update and draw phases
- Double buffering technique is used to reduce flickering
- Collision detection uses rectangular hitboxes
- ASCII art is stored in string arrays

### Extending the Game
To add new features:
1. Add new ASCII art in the appropriate section
2. Implement the game logic in the update() function
3. Add rendering code in the draw() function

### Known Limitations
- The game is designed for Windows console only
- Sound effects are basic (using Beep function)
- Console rendering may cause flickering on some systems
